
function void
vim__fill_command_lister(Arena *arena, Lister *lister, i32 *command_ids, i32 command_id_count, Command_Lister_Status_Rule *status_rule){
	if(command_ids == 0){ command_id_count = command_one_past_last_id; }

	for(i32 i=0; i<command_id_count; i++){
		i32 j = (command_ids ? command_ids[i] : i);
		j = clamp(0, j, command_one_past_last_id);

		Custom_Command_Function *proc = fcoder_metacmd_table[j].proc;

		Command_Trigger_List triggers = map_get_triggers_recursive(arena, status_rule->mapping, status_rule->map_id, proc);

		List_String_Const_u8 list = {};
		if(triggers.first == 0){
			string_list_push(arena, &list, string_u8_litexpr(""));
		}
		for(Command_Trigger *node=triggers.first; node; node=node->next){
			command_trigger_stringize(arena, &list, node);
			if(node->next){
				string_list_push(arena, &list, string_u8_litexpr(" ")); 
			}
		}

		String_Const_u8 key_bind = string_list_flatten(arena, list);
		String_Const_u8 description = SCu8(fcoder_metacmd_table[j].description);
		String_Const_u8 status = push_stringf(arena, "%.*s\n%.*s", string_expand(key_bind), string_expand(description));

		lister_add_item(lister, SCu8(fcoder_metacmd_table[j].name), status, (void*)proc, 0);
	}
}

function Custom_Command_Function*
vim_get_command_from_user(Application_Links *app, i32 *command_ids, i32 command_id_count, Command_Lister_Status_Rule *status_rule){

	Scratch_Block scratch(app);
	Lister_Block lister(app, scratch);
	vim_lister_set_default_handlers(lister);
	lister_set_query(lister, string_u8_litexpr("Command:"));
	vim__fill_command_lister(scratch, lister, command_ids, command_id_count, status_rule);

    //minibar_string.size = 0;
	//vim_reset_bottom_text();
	//string_append(&vim_bot_text, string_u8_litexpr(":"));
	Lister_Result l_result = vim_run_lister(app, lister);

	return (l_result.canceled ? 0 : (Custom_Command_Function *)l_result.user_data);
}

CUSTOM_UI_COMMAND_SIG(vim_command_mode)
CUSTOM_DOC("Enter Command Mode") {
    defer { minibar_string.size = 0; };
	View_ID view = get_this_ctx_view(app, Access_Always);
	if(view == 0){ return; }
	Command_Lister_Status_Rule rule = {};
	Buffer_ID buffer = view_get_buffer(app, view, Access_Visible);
	Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer);
	Command_Map_ID *map_id_ptr = scope_attachment(app, buffer_scope, buffer_map_id, Command_Map_ID);
	if(map_id_ptr){
		rule = command_lister_status_bindings(&framework_mapping, *map_id_ptr);
	}else{
		rule = command_lister_status_descriptions();
	}

	Custom_Command_Function *func = vim_get_command_from_user(app, 0, 0, &rule);
	if(func != 0){
		view_enqueue_command_function(app, view, func);
	}
}

function void
vim_generate_hot_directory_file_list(Application_Links *app, Lister *lister){
	Scratch_Block scratch(app, lister->arena);

	Temp_Memory temp = begin_temp(lister->arena);
	String_Const_u8 hot = push_hot_directory(app, lister->arena);
	if (!character_is_slash(string_get_character(hot, hot.size - 1))){
		hot = push_u8_stringf(lister->arena, "%.*s/", string_expand(hot));
	}
	lister_set_text_field(lister, hot);
	lister_set_key(lister, string_front_of_path(hot));

	File_List file_list = system_get_file_list(scratch, hot);
	end_temp(temp);

	File_Info **one_past_last = file_list.infos + file_list.count;

	lister_begin_new_item_set(app, lister);

	hot = push_hot_directory(app, lister->arena);
	push_align(lister->arena, 8);
	if (hot.str != 0){
		String_Const_u8 empty_string = string_u8_litexpr("");
		Lister_Prealloced_String empty_string_prealloced = lister_prealloced(empty_string);
		for (File_Info **info = file_list.infos;
			 info < one_past_last;
			 info += 1){
			if (!HasFlag((**info).attributes.flags, FileAttribute_IsDirectory)) continue;
			String_Const_u8 file_name = push_u8_stringf(lister->arena, "%.*s/",
														string_expand((**info).file_name));
			lister_add_item(lister, lister_prealloced(file_name), empty_string_prealloced, file_name.str, 0);
			;
		}

		for (File_Info **info = file_list.infos;
			 info < one_past_last;
			 info += 1){
			if (HasFlag((**info).attributes.flags, FileAttribute_IsDirectory)) continue;
			String_Const_u8 file_name = push_string_copy(lister->arena, (**info).file_name);
			char *is_loaded = "";
			char *status_flag = "";

			Buffer_ID buffer = {};

			{
				Temp_Memory path_temp = begin_temp(lister->arena);
				List_String_Const_u8 list = {};
				string_list_push(lister->arena, &list, hot);
				string_list_push_overlap(lister->arena, &list, '/', (**info).file_name);
				String_Const_u8 full_file_path = string_list_flatten(lister->arena, list);
				buffer = get_buffer_by_file_name(app, full_file_path, Access_Always);
				end_temp(path_temp);
			}

			if (buffer != 0){
				is_loaded = "LOADED";
				Dirty_State dirty = buffer_get_dirty_state(app, buffer);
				switch (dirty){
					case DirtyState_UnsavedChanges:  status_flag = " *"; break;
					case DirtyState_UnloadedChanges: status_flag = " !"; break;
					case DirtyState_UnsavedChangesAndUnloadedChanges: status_flag = " *!"; break;
				}
			}
			String_Const_u8 status = push_u8_stringf(lister->arena, "%s%s", is_loaded, status_flag);
			lister_add_item(lister, lister_prealloced(file_name), lister_prealloced(status), file_name.str, 0);
		}
	}
}

function Lister_Choice*
vim_get_choice_from_user(Application_Links *app, String_Const_u8 query, Lister_Choice_List list){
    Scratch_Block scratch(app);
    Lister_Block lister(app, scratch);
    for(Lister_Choice *choice = list.first; choice; choice = choice->next){
        u64 code_size = sizeof(choice->key_code);
        void *extra = lister_add_item(lister, choice->string, choice->status, choice, code_size);
        block_copy(extra, &choice->key_code, code_size);
    }
    lister_set_query(lister, query);
    Lister_Handlers handlers = {};
    handlers.navigate        = lister__navigate__default;
    handlers.key_stroke      = lister__key_stroke__choice_list;
    lister_set_handlers(lister, &handlers);

    Lister_Result l_result = vim_run_lister(app, lister);
    Lister_Choice *result = 0;
    if(!l_result.canceled){
        result = (Lister_Choice*)l_result.user_data;
    }
    return result;
}

function b32
vim_query_create_folder(Application_Links *app, String_Const_u8 folder_name){
	Scratch_Block scratch(app);
    Lister_Choice_List list = {};
    lister_choice(scratch, &list, "(N)o"  , "", KeyCode_N, SureToKill_No);
    lister_choice(scratch, &list, "(Y)es" , "", KeyCode_Y, SureToKill_Yes);

    String_Const_u8 message = push_u8_stringf(scratch, "Create the folder %.*s?", string_expand(folder_name));
    Lister_Choice *choice = vim_get_choice_from_user(app, message, list);

	b32 did_create_folder = false;
    if(choice != 0){
        switch(choice->user_data){
            case SureToCreateFolder_No:{} break;

            case SureToCreateFolder_Yes:{
				String_Const_u8 hot = push_hot_directory(app, scratch);
                String_Const_u8 fixed_folder_name = push_string_copy(scratch, folder_name);
                foreach(i, fixed_folder_name.size){
					if(fixed_folder_name.str[i] == '/'){ fixed_folder_name.str[i] = '\\'; }
				}
                if(fixed_folder_name.size > 0){
                    String_Const_u8 cmd = push_u8_stringf(scratch, "mkdir %.*s", string_expand(fixed_folder_name));
                    exec_system_command(app, 0, buffer_identifier(0), hot, cmd, 0);
                    did_create_folder = true;
				}
            } break;
        }
    }

    return(did_create_folder);
}

internal String_Const_u8 //NOTE luis added this
get_directory_for_buffer(Application_Links *app, Arena *arena, Buffer_ID buffer_id);


CUSTOM_UI_COMMAND_SIG(vim_interactive_open_or_new)
CUSTOM_DOC("Interactively open a file out of the file system.") {
    defer { minibar_string.size = 0; };
    #if 1 //NOTE(luis) added this so that we always open file in the file's directory
    {
        View_ID view = get_active_view(app, Access_Always);
        
        Scratch_Block scratch(app);
        
        Buffer_ID buffer_id = view_get_buffer(app, view, Access_Always);
        String_Const_u8 directory = get_directory_for_buffer(app, scratch, buffer_id);
        if(directory.size) {
            set_hot_directory(app, directory);
            
            minibar_string.size = Min(minibar_string.cap, directory.size);
            block_copy(minibar_string.str, directory.str, minibar_string.size); 
        }   
        else {
            directory = push_hot_directory(app, scratch);
            if (directory.size) {
                minibar_string.size = Min(minibar_string.cap, directory.size);
                block_copy(minibar_string.str, directory.str, minibar_string.size);
            }
        }
    }
    #endif
    
	for(;;) {
		Scratch_Block scratch(app);
		View_ID view = get_this_ctx_view(app, Access_Always);
		File_Name_Result result = vim_get_file_name_from_user(app, scratch, SCu8("Open/New:"), view);
		if(result.canceled || result.path_in_text_field.str == 0){ break; }

		String_Const_u8 file_name = result.file_name_activated;
		if(file_name.size == 0){ file_name = result.file_name_in_text_field; }

		String_Const_u8 path = result.path_in_text_field;
		String_Const_u8 full_file_name =
			push_u8_stringf(scratch, "%.*s/%.*s", string_expand(path), string_expand(file_name));

		if(result.is_folder){
			set_hot_directory(app, full_file_name);
            
            minibar_string.size = Min(minibar_string.cap, full_file_name.size);
            block_copy(minibar_string.str, full_file_name.str, minibar_string.size);
			continue;
		}

		if(character_is_slash(file_name.str[file_name.size - 1])){
			File_Attributes attribs = system_quick_file_attributes(scratch, full_file_name);
			if(HasFlag(attribs.flags, FileAttribute_IsDirectory)){
				set_hot_directory(app, full_file_name);
				continue;
			}
			if(string_looks_like_drive_letter(file_name)){
				set_hot_directory(app, file_name);
				continue;
			}
			if(vim_query_create_folder(app, full_file_name)){
				set_hot_directory(app, full_file_name);
				continue;
			}else{
				set_hot_directory(app, result.path_in_text_field);
				continue;
			}
			break;
		}

		Buffer_ID buffer = create_buffer(app, full_file_name, 0);
		if(buffer != 0){ view_set_buffer(app, view, buffer, 0); }
		break;
	}
}



function void 
add_files_in_directory_to_path(Application_Links *app, Lister *lister, String8 directory, 
                               Prj_Pattern_List whitelist, Prj_Pattern_List blacklist, u32 prj_flags) {
    Scratch_Block scratch(app);
    
    if (!character_is_slash(string_get_character(directory, directory.size - 1))){
        directory = push_u8_stringf(scratch, "%.*s/", string_expand(directory));
    }
    
    //prj_open_files_pattern_filter__rec(app, directory, whitelist, blacklist, flags);
    File_List list = system_get_file_list(scratch, directory);
    //ProfileCloseNow(profile_get_file_list);
    
    File_Info **info = list.infos;
    for (u32 i = 0; i < list.count; ++i, ++info) {
        if (HasFlag((**info).attributes.flags, FileAttribute_IsDirectory)){
            String8 file_name = (**info).file_name;
            if ((prj_flags & PrjOpenFileFlag_Recursive) == 0){
                continue;
            }
            if (prj_match_in_pattern_list(file_name, blacklist)){
                continue;
            }
            
            String8 new_path = push_u8_stringf(scratch, "%.*s%.*s/", string_expand(directory), string_expand(file_name));
            add_files_in_directory_to_path(app, lister, new_path, whitelist, blacklist, prj_flags);
                    //prj_open_files_pattern_filter__rec(app, new_path, whitelist, blacklist, flags);
        }
        else{
            if (!prj_match_in_pattern_list((**info).file_name, whitelist)){
                continue;
            }
            if (prj_match_in_pattern_list((**info).file_name, blacklist)){
                continue;
            }
                    //String8 full_path = push_u8_stringf(scratch, "%.*s%.*s", string_expand(path), string_expand(file_name));
                    //create_buffer(app, full_path, 0);
            
            if (HasFlag((**info).attributes.flags, FileAttribute_IsDirectory)) continue;
            String_Const_u8 file_name = push_string_copy(lister->arena, (**info).file_name);
            char *is_loaded = "";
            char *status_flag = "";
            
            Buffer_ID buffer = {};
            
            {
                Temp_Memory path_temp = begin_temp(lister->arena);
                List_String_Const_u8 string_list = {};
                //string_list_push(lister->arena, &string_list, hot);
                string_list_push(lister->arena, &string_list, directory);
                string_list_push_overlap(lister->arena, &string_list, '/', (**info).file_name);
                String_Const_u8 full_file_path = string_list_flatten(lister->arena, string_list);
                buffer = get_buffer_by_file_name(app, full_file_path, Access_Always);
                end_temp(path_temp);
            }
            
            if (buffer != 0){
                is_loaded = "LOADED";
                Dirty_State dirty = buffer_get_dirty_state(app, buffer);
                switch (dirty){
                case DirtyState_UnsavedChanges:  status_flag = " *"; break;
                case DirtyState_UnloadedChanges: status_flag = " !"; break;
                case DirtyState_UnsavedChangesAndUnloadedChanges: status_flag = " *!"; break;
                }
            }
            String_Const_u8 status = push_u8_stringf(lister->arena, "%s%s", is_loaded, status_flag);
            lister_add_item(lister, lister_prealloced(file_name), lister_prealloced(status), file_name.str, 0);
        }
    }
}

function void
generate_project_directory_file_list(Application_Links *app, Lister *lister) {
    //generate_hot_directory_file_list
    
    Scratch_Block scratch(app, lister->arena);
    
    //Temp_Memory temp = begin_temp(lister->arena);
    String_Const_u8 hot = push_hot_directory(app, lister->arena);
    if (!character_is_slash(string_get_character(hot, hot.size - 1))){
        hot = push_u8_stringf(lister->arena, "%.*s/", string_expand(hot));
    }
    lister_set_text_field(lister, hot);
    lister_set_key(lister, string_front_of_path(hot));
    
    //File_List file_list = system_get_file_list(scratch, hot);
    //end_temp(temp);
    
    //File_Info **one_past_last = file_list.infos + file_list.count;
    
    lister_begin_new_item_set(app, lister);
    
    hot = push_hot_directory(app, lister->arena);
    push_align(lister->arena, 8);
    if (hot.str == 0) {
        return;
    }
    
    
    String_Const_u8 empty_string = string_u8_litexpr("");
    Lister_Prealloced_String empty_string_prealloced = lister_prealloced(empty_string);
    
    #if 0
    //add directory entries first
    for (File_Info **info = file_list.infos; info < one_past_last; info += 1) {
        if (!HasFlag((**info).attributes.flags, FileAttribute_IsDirectory)) continue;
        String_Const_u8 file_name = push_u8_stringf(lister->arena, "%.*s/", string_expand((**info).file_name));
        lister_add_item(lister, lister_prealloced(file_name), empty_string_prealloced, file_name.str, 0);
    }
    
        //add actual files to load here
    for (File_Info **info = file_list.infos; info < one_past_last; info += 1) {
        if (HasFlag((**info).attributes.flags, FileAttribute_IsDirectory)) continue;
        String_Const_u8 file_name = push_string_copy(lister->arena, (**info).file_name);
        char *is_loaded = "";
        char *status_flag = "";
        
        Buffer_ID buffer = {};
        
        {
            Temp_Memory path_temp = begin_temp(lister->arena);
            List_String_Const_u8 list = {};
            string_list_push(lister->arena, &list, hot);
            string_list_push_overlap(lister->arena, &list, '/', (**info).file_name);
            String_Const_u8 full_file_path = string_list_flatten(lister->arena, list);
            buffer = get_buffer_by_file_name(app, full_file_path, Access_Always);
            end_temp(path_temp);
        }
        
        if (buffer != 0){
            is_loaded = "LOADED";
            Dirty_State dirty = buffer_get_dirty_state(app, buffer);
            switch (dirty){
            case DirtyState_UnsavedChanges:  status_flag = " *"; break;
            case DirtyState_UnloadedChanges: status_flag = " !"; break;
            case DirtyState_UnsavedChangesAndUnloadedChanges: status_flag = " *!"; break;
            }
        }
        String_Const_u8 status = push_u8_stringf(lister->arena, "%s%s", is_loaded, status_flag);
        lister_add_item(lister, lister_prealloced(file_name), lister_prealloced(status), file_name.str, 0);
    }
    #endif
    
    
    //load project command thing
    // NOTE(allen): Load the project file from the hot directory
    String8 project_path = push_hot_directory(app, scratch);
    File_Name_Data dump = dump_file_search_up_path(app, scratch, project_path, string_u8_litexpr("project.4coder"));
    String8 project_root = string_remove_last_folder(dump.file_name);
    
    if (dump.data.str == 0){
        print_message(app, string_u8_litexpr("Did not find project.4coder.\n"));
    }
    
    // NOTE(allen): Parse config data out of project file
    Config *config_parse = 0;
    Variable_Handle prj_var = vars_get_nil();
    if (dump.data.str != 0){
        Token_Array array = token_array_from_text(app, scratch, dump.data);
        if (array.tokens != 0){
            config_parse = def_config_parse(app, scratch, dump.file_name, dump.data, array);
            if (config_parse != 0){
                i32 version = 0;
                if (config_parse->version != 0){
                    version = *config_parse->version;
                }
                
                switch (version){
                    case 0:
                    case 1:
                    {
                        prj_var = prj_v1_to_v2(app, project_root, config_parse);
                    }break;
                    default:
                    {
                        prj_var = def_fill_var_from_config(app, vars_get_root(), vars_save_string_lit("prj_config"), config_parse);
                    }break;
                }
                
            }
        }
    }
    
    // NOTE(allen): Print Project
    if (!vars_is_nil(prj_var)){
        vars_print(app, prj_var);
        print_message(app, string_u8_litexpr("\n"));
    }
    
    // NOTE(allen): Print Errors
    if (config_parse != 0){
        String8 error_text = config_stringize_errors(app, scratch, config_parse);
        if (error_text.size > 0){
            print_message(app, string_u8_litexpr("Project errors:\n"));
            print_message(app, error_text);
            print_message(app, string_u8_litexpr("\n"));
        }
    }
    
    // NOTE(allen): Open All Project Files
    Variable_Handle load_paths_var = vars_read_key(prj_var, vars_save_string_lit("load_paths"));
    Variable_Handle load_paths_os_var = vars_read_key(load_paths_var, vars_save_string_lit(OS_NAME));
    
    String_ID path_id = vars_save_string_lit("path");
    String_ID recursive_id = vars_save_string_lit("recursive");
    String_ID relative_id = vars_save_string_lit("relative");
    
    Variable_Handle whitelist_var = vars_read_key(prj_var, vars_save_string_lit("patterns"));
    Variable_Handle blacklist_var = vars_read_key(prj_var, vars_save_string_lit("blacklist_patterns"));
    
    Prj_Pattern_List whitelist = prj_pattern_list_from_var(scratch, whitelist_var);
    Prj_Pattern_List blacklist = prj_pattern_list_from_var(scratch, blacklist_var);
    
    for (Variable_Handle load_path_var = vars_first_child(load_paths_os_var);
         !vars_is_nil(load_path_var);
         load_path_var = vars_next_sibling(load_path_var)){
        Variable_Handle path_var = vars_read_key(load_path_var, path_id);
        Variable_Handle recursive_var = vars_read_key(load_path_var, recursive_id);
        Variable_Handle relative_var = vars_read_key(load_path_var, relative_id);
        
        String8 path = vars_string_from_var(scratch, path_var);
        b32 recursive = vars_b32_from_var(recursive_var);
        b32 relative = vars_b32_from_var(relative_var);
        
        
        u32 prj_flags = 0;
        if (recursive){
            prj_flags |= PrjOpenFileFlag_Recursive;
        }
        
        String8 file_dir = path;
        if (relative){
            String8 prj_dir = prj_path_from_project(scratch, prj_var);
            
            String8List file_dir_list = {};
            string_list_push(scratch, &file_dir_list, prj_dir);
            string_list_push_overlap(scratch, &file_dir_list, '/', path);
            string_list_push_overlap(scratch, &file_dir_list, '/', SCu8());
            file_dir = string_list_flatten(scratch, file_dir_list, StringFill_NullTerminate);
        }
        
        //prj_open_files_pattern_filter(app, file_dir, whitelist, blacklist, prj_flags);
        
        add_files_in_directory_to_path(app, lister, file_dir, whitelist, blacklist, prj_flags);
    }
}

#if 0
function File_Name_Result
vim_get_file_name_from_user(Application_Links *app, Arena *arena, String_Const_u8 query, View_ID view){
	Lister_Handlers handlers = lister_get_default_handlers();
	//handlers.refresh = generate_hot_directory_file_list;
    handlers.refresh = generate_project_directory_file_list;
	handlers.write_character = vim_lister__write_character__file_path;
	handlers.backspace = vim_lister_file__backspace;
    
    //minibar_string.size = 0;
	//vim_reset_bottom_text(); //TODO make string zero basically
	Lister_Result l_result = vim_run_lister_with_refresh_handler(app, arena, query, handlers);
	return vim_convert_lister_result_to_file_name_result(l_result);
}

CUSTOM_COMMAND_SIG(vim_open_file_in_project)
CUSTOM_DOC("Lists files in the project directory list") {
    ProfileScope(app, "vim open project file");
    save_all_dirty_buffers(app);
    Scratch_Block scratch(app);
    
    
}
#endif

CUSTOM_UI_COMMAND_SIG(vim_theme_lister)
CUSTOM_DOC("Opens an interactive list of all registered themes.")
{
    defer { minibar_string.size = 0; };
	Color_Table_List *color_table_list = &global_theme_list;

	Scratch_Block scratch(app);
	Lister_Block lister(app, scratch);
	vim_lister_set_default_handlers(lister);
    minibar_string.size = 0;
	//vim_reset_bottom_text();

	lister_add_item(lister, string_u8_litexpr("4coder"), string_u8_litexpr(""),
					(void*)&default_color_table, 0);

	for(Color_Table_Node *node = color_table_list->first; node; node = node->next){
		lister_add_item(lister, node->name, string_u8_litexpr(""), (void*)&node->table, 0);
	}

#if VIM_USE_BOTTOM_LISTER
	string_append(&vim_bot_text, string_u8_litexpr("Theme:"));
#endif
	Lister_Result l_result = vim_run_lister(app, lister);

	Color_Table *result = 0;
	if(!l_result.canceled){ result = (Color_Table*)l_result.user_data; }

	if(result != 0){ active_color_table = *result; }
}

CUSTOM_UI_COMMAND_SIG(vim_switch_lister)
CUSTOM_DOC("Opens an interactive list of all loaded buffers.")
{
    defer { minibar_string.size = 0; };
	Lister_Handlers handlers = lister_get_default_handlers();
    handlers.write_character = vim_lister__write_character;
	handlers.refresh = generate_all_buffers_list;
	handlers.backspace = vim_lister__backspace;
	Scratch_Block scratch(app);
    minibar_string.size = 0;
	//vim_reset_bottom_text();
	//string_append(&vim_bot_text, string_u8_litexpr("Switch:"));
	Lister_Result l_result = vim_run_lister_with_refresh_handler(app, scratch, string_u8_litexpr("Switch:"), handlers);
	Buffer_ID buffer = 0;
	if (!l_result.canceled){
		buffer = (Buffer_ID)(PtrAsInt(l_result.user_data));
	}
	if (buffer != 0){
		View_ID view = get_this_ctx_view(app, Access_Always);
		view_set_buffer(app, view, buffer, 0);
	}
}

function void
luis_generate_all_files_and_buffers_list(Application_Links *app, Lister *lister) {
    
    //NOTE this code here was addapted from generate_hot_directory_file_list
    Scratch_Block scratch(app, lister->arena);
    
    String_Const_u8 hot = push_hot_directory(app, scratch);
    if (!character_is_slash(string_get_character(hot, hot.size - 1))){
        hot = push_u8_stringf(lister->arena, "%.*s/", string_expand(hot));
    }
    lister_set_text_field(lister, hot);
    lister_set_key(lister, string_front_of_path(hot));
    
    File_List file_list = system_get_file_list(scratch, hot);
    
    File_Info **one_past_last = file_list.infos + file_list.count;
    
    lister_begin_new_item_set(app, lister);
    hot = push_hot_directory(app, lister->arena); //NOTE(luis) not sure why Allen calls this again
	push_align(lister->arena, 8);
    if (hot.str != 0) {
        String_Const_u8 empty_string = string_u8_litexpr("");
		Lister_Prealloced_String empty_string_prealloced = lister_prealloced(empty_string);
        //add directories first, then files
		for (File_Info **info = file_list.infos; info < one_past_last; info += 1) {
			if (!HasFlag((**info).attributes.flags, FileAttribute_IsDirectory)) continue;
			String_Const_u8 file_name = push_u8_stringf(lister->arena, "%.*s/",
														string_expand((**info).file_name));
			lister_add_item(lister, lister_prealloced(file_name), empty_string_prealloced, file_name.str, 0);
		}
        
        //add files
		for (File_Info **info = file_list.infos; info < one_past_last; info += 1){
			if (HasFlag((**info).attributes.flags, FileAttribute_IsDirectory)) continue;
			String_Const_u8 file_name = push_string_copy(lister->arena, (**info).file_name);
			char *is_loaded = "";
			char *status_flag = "";
            
			Buffer_ID buffer = {}; //buffer is just to check it's dirty/modified state
			{
				Temp_Memory path_temp = begin_temp(scratch);
				List_String_Const_u8 list = {};
				string_list_push(scratch, &list, hot);
				string_list_push_overlap(scratch, &list, '/', (**info).file_name);
				String_Const_u8 full_file_path = string_list_flatten(scratch, list);
				buffer = get_buffer_by_file_name(app, full_file_path, Access_Always);
				end_temp(path_temp);
			}
            
			if (buffer != 0) {
				is_loaded = "LOADED";
				Dirty_State dirty = buffer_get_dirty_state(app, buffer);
				switch (dirty){
                case DirtyState_UnsavedChanges:  status_flag = " *"; break;
                case DirtyState_UnloadedChanges: status_flag = " !"; break;
                case DirtyState_UnsavedChangesAndUnloadedChanges: status_flag = " *!"; break;
				}
			}
			String_Const_u8 status = push_u8_stringf(lister->arena, "%s%s", is_loaded, status_flag);
			lister_add_item(lister, lister_prealloced(file_name), lister_prealloced(status), file_name.str, 0);
		}   
    }
    
    
    //NOTE all code below here was adapted from generate_all_buffers_list (Allen's code)
    Buffer_ID viewed_buffers[16];
    i32 viewed_buffer_count = 0;
    auto is_in_viewed_buffers = [&viewed_buffers, viewed_buffer_count](Buffer_ID buffer) -> b32 {
        for (i32 i = 0; i < viewed_buffer_count; i += 1) {
            if (buffer == viewed_buffers[i]) {
                return true;
            }
        }
        return false;
    };
    
    // List currently viewed buffers
    for (View_ID view = get_view_next(app, 0, Access_Always); view; view = get_view_next(app, view, Access_Always)) {
        Buffer_ID new_buffer_id = view_get_buffer(app, view, Access_Always);
        if (!is_in_viewed_buffers(new_buffer_id)) {
            viewed_buffers[viewed_buffer_count++] = new_buffer_id;    
        } 
    }
    
    auto in_hot_directory = [&hot](String_Const_u8 filepath) {
        String_Const_u8 directory = string_remove_front_of_path(filepath);
        return string_match(hot, directory);
    };
    
    //not buffers with stars
    auto add_buffer_to_list = [](Application_Links *app, Lister *lister, Buffer_ID buffer) {
        Dirty_State dirty = buffer_get_dirty_state(app, buffer);
        String_Const_u8 status = {};
        switch (dirty) {
        case DirtyState_UnsavedChanges:  status = string_u8_litexpr("*"); break;
        case DirtyState_UnloadedChanges: status = string_u8_litexpr("!"); break;
        case DirtyState_UnsavedChangesAndUnloadedChanges: status = string_u8_litexpr("*!"); break;
        }
        String_Const_u8 file_name = push_buffer_base_name(app, lister->arena, buffer);
        lister_add_item(lister, lister_prealloced(file_name), status, file_name.str, 0);
    };
    
    // Regular Buffers
    for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always); buffer; buffer = get_buffer_next(app, buffer, Access_Always)) {
        if (!is_in_viewed_buffers(buffer) && !buffer_has_name_with_star(app, buffer)) {
            Temp_Memory _temp = begin_temp(scratch);
            String_Const_u8 filename = push_buffer_file_name(app, scratch, buffer);
            b32 in_dir = in_hot_directory(filename); 
            end_temp(_temp);
            
            if (!in_dir) {
                add_buffer_to_list(app, lister, buffer);    
            }
        }
    }
    
    // Buffers Starting with *
    for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always); buffer; buffer = get_buffer_next(app, buffer, Access_Always)) {
        if (!is_in_viewed_buffers(buffer) && buffer_has_name_with_star(app, buffer)) {
            add_buffer_to_list(app, lister, buffer);
            //generate_all_buffers_list__output_buffer(app, lister, buffer);
        }
    }
    
    // Buffers That Are Open in Views
    for (i32 i = 0; i < viewed_buffer_count; i += 1) {
        Buffer_ID buffer = viewed_buffers[i];
        Temp_Memory _temp = begin_temp(scratch);
        String_Const_u8 filename = push_buffer_file_name(app, scratch, buffer);
        b32 in_dir = in_hot_directory(filename); 
        end_temp(_temp);
        
        if (!in_dir) {
            add_buffer_to_list(app, lister, buffer);    
        }
    }
}

//NOTE this is a combo of interactive_open_or_new and switch buffer...
//we list all files in the current hot directory, then we also add all the buffers,
//NOTE not tested yet
CUSTOM_UI_COMMAND_SIG(vim_switch_file_or_buffer)
CUSTOM_DOC("Interactively open a file out of the file system.") {
    defer { minibar_string.size = 0; };
    #if 1 //NOTE(luis) added this so that we always open file in the file's directory
    {
        View_ID view = get_active_view(app, Access_Always);
        
        Scratch_Block scratch(app);
        
        Buffer_ID buffer_id = view_get_buffer(app, view, Access_Always);
        String_Const_u8 directory = get_directory_for_buffer(app, scratch, buffer_id);
        if(directory.size) {
            set_hot_directory(app, directory);
            
            minibar_string.size = Min(minibar_string.cap, directory.size);
            block_copy(minibar_string.str, directory.str, minibar_string.size); 
        }   
        else {
            directory = push_hot_directory(app, scratch);
            if (directory.size) {
                minibar_string.size = Min(minibar_string.cap, directory.size);
                block_copy(minibar_string.str, directory.str, minibar_string.size);
            }
        }
    }
    #endif
    
	for(;;) {
		Scratch_Block scratch(app);
		View_ID view = get_this_ctx_view(app, Access_Always);
        
        Lister_Handlers handlers = lister_get_default_handlers();
        handlers.refresh = luis_generate_all_files_and_buffers_list;
        handlers.write_character = vim_lister__write_character__file_path;
        handlers.backspace = vim_lister_file__backspace;
        
        //minibar_string.size = 0;
        //vim_reset_bottom_text(); //TODO make string zero basically
        Lister_Result l_result  = vim_run_lister_with_refresh_handler(app, scratch, SCu8("Open/New:"), handlers);
        File_Name_Result result = vim_convert_lister_result_to_file_name_result(l_result);
        
		if(result.canceled || result.path_in_text_field.str == 0){ break; }

		String_Const_u8 file_name = result.file_name_activated;
		if(file_name.size == 0){ file_name = result.file_name_in_text_field; }

		String_Const_u8 path = result.path_in_text_field;
		String_Const_u8 full_file_name =
			push_u8_stringf(scratch, "%.*s/%.*s", string_expand(path), string_expand(file_name));

		if(result.is_folder){
			set_hot_directory(app, full_file_name);
            
            minibar_string.size = Min(minibar_string.cap, full_file_name.size);
            block_copy(minibar_string.str, full_file_name.str, minibar_string.size);
			continue;
		}

		if(character_is_slash(file_name.str[file_name.size - 1])){
			File_Attributes attribs = system_quick_file_attributes(scratch, full_file_name);
			if(HasFlag(attribs.flags, FileAttribute_IsDirectory)){
				set_hot_directory(app, full_file_name);
				continue;
			}
			if(string_looks_like_drive_letter(file_name)){
				set_hot_directory(app, file_name);
				continue;
			}
			if(vim_query_create_folder(app, full_file_name)){
				set_hot_directory(app, full_file_name);
				continue;
			}else{
				set_hot_directory(app, result.path_in_text_field);
				continue;
			}
			break;
		}

		Buffer_ID buffer = create_buffer(app, full_file_name, 0);
		if(buffer != 0){ view_set_buffer(app, view, buffer, 0); }
		break;
	}
}


function Jump_Lister_Result
vim_get_jump_index_from_user(Application_Links *app, Marker_List *list,
                             String_Const_u8 query){
	Jump_Lister_Result result = {};
	if (list != 0){
		Scratch_Block scratch(app);
		Lister_Block lister(app, scratch);
		lister_set_query(lister, query);
		lister_set_default_handlers(lister);

		Buffer_ID list_buffer = list->buffer_id;

		i32 option_count = list->jump_count;
		Managed_Object stored_jumps = list->jump_array;
		for (i32 i = 0; i < option_count; i += 1){
			Sticky_Jump_Stored stored = {};
			managed_object_load_data(app, stored_jumps, i, 1, &stored);
			String_Const_u8 line = push_buffer_line(app, scratch, list_buffer,
													stored.list_line);
			lister_add_item(lister, line, SCu8(), IntAsPtr(i), 0);
		}

		Lister_Result l_result = vim_run_lister(app, lister);
		if (!l_result.canceled){
			result.success = true;
			result.index = (i32)PtrAsInt(l_result.user_data);
		}
	}

	return(result);
}

function Jump_Lister_Result
vim_get_jump_index_from_user(Application_Links *app, Marker_List *list, char *query){
	return(vim_get_jump_index_from_user(app, list, SCu8(query)));
}


CUSTOM_UI_COMMAND_SIG(vim_list_all_functions_current_buffer_lister)
CUSTOM_DOC("Creates a lister of locations that look like function definitions and declarations in the buffer.")
{
    defer { minibar_string.size = 0; };
	Heap *heap = &global_heap;
	View_ID view = get_active_view(app, Access_ReadVisible);
	Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
	if (buffer != 0){
		list_all_functions(app, buffer);
		view = get_active_view(app, Access_Always);
		buffer = view_get_buffer(app, view, Access_Always);
		Marker_List *list = get_or_make_list_for_buffer(app, heap, buffer);
		if (list != 0){
			Jump_Lister_Result jump = vim_get_jump_index_from_user(app, list, "Function:");
			jump_to_jump_lister_result(app, view, list, &jump);
		}
	}
}

CUSTOM_UI_COMMAND_SIG(vim_proj_cmd_lister)
CUSTOM_DOC("Opens an interactive list of all project commands.")
{
    defer { minibar_string.size = 0; };
	Variable_Handle prj_var = vars_read_key(vars_get_root(), vars_save_string_lit("prj_config"));

	Scratch_Block scratch(app);
	Lister_Block lister(app, scratch);

	Lister_Handlers handlers = lister_get_default_handlers();
	handlers.backspace = vim_lister__backspace;
	lister_set_handlers(lister, &handlers);

    minibar_string.size = 0;
	//vim_reset_bottom_text();
	//string_append(&vim_bot_text, string_u8_litexpr("Command:"));

	Variable_Handle cmd_list_var = vars_read_key(prj_var, vars_save_string_lit("commands"));
	String_ID os_id = vars_save_string_lit(OS_NAME);

	i32 i=1;
	for(Variable_Handle cmd=vars_first_child(cmd_list_var); !vars_is_nil(cmd); cmd=vars_next_sibling(cmd), i++){
		Variable_Handle os_cmd = vars_read_key(cmd, os_id);
		if(!vars_is_nil(os_cmd)){
			String8 cmd_name = vars_key_from_var(scratch, cmd);
			String8 os_cmd_str = vars_string_from_var(scratch, os_cmd);
			u8 *f_str = push_array(scratch, u8, 3);
			f_str[0] = 'F';
			f_str[1] = '0' + u8(i%10);
			f_str[2] = '0' + u8(i/10);
			//lister_add_item(lister, cmd_name, os_cmd_str, cmd.ptr, 0);
			lister_add_item(lister, cmd_name, SCu8(f_str, 2+(i>9)), cmd.ptr, 0);
		}
	}

	Variable_Handle prj_cmd = vars_get_nil();
	Lister_Result l_result = vim_run_lister(app, lister);
	if(!l_result.canceled && l_result.user_data){
		prj_cmd.ptr = (Variable*)l_result.user_data;
	}
	if(!vars_is_nil(prj_cmd)){
		prj_exec_command(app, prj_cmd);
	}
}





function b32
vim_do_buffer_close_user_check(Application_Links *app, Buffer_ID buffer, View_ID view){
    Scratch_Block scratch(app);
    Lister_Choice_List list = {};
    lister_choice(scratch, &list, "(N)o"  , "", KeyCode_N, SureToKill_No);
    lister_choice(scratch, &list, "(Y)es" , "", KeyCode_Y, SureToKill_Yes);
    lister_choice(scratch, &list, "(S)ave", "", KeyCode_S, SureToKill_Save);

    Lister_Choice *choice = vim_get_choice_from_user(app, string_u8_litexpr("There are unsaved changes, close anyway?"), list);

    b32 do_kill = false;
    if (choice != 0){
        switch (choice->user_data){
            case SureToKill_No:{} break;

            case SureToKill_Yes:{ do_kill = true; } break;

            case SureToKill_Save:{
                String_Const_u8 file_name = push_buffer_file_name(app, scratch, buffer);
                if(buffer_save(app, buffer, file_name, BufferSave_IgnoreDirtyFlag)){
                    do_kill = true;
                }else{
#define M "Did not close '%.*s' because it did not successfully save."
                    String_Const_u8 str = push_u8_stringf(scratch, M, string_expand(file_name));
#undef M
                    print_message(app, str);
                }
            } break;
        }
    }

    return do_kill;
}

function Buffer_Kill_Result
vim_try_buffer_kill(Application_Links *app){
	View_ID view = get_active_view(app, Access_ReadVisible);
	Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
	Buffer_Kill_Flag flags = 0;
	Buffer_Kill_Result result = buffer_kill(app, buffer, flags);
	if(result == BufferKillResult_Dirty){
		if(vim_do_buffer_close_user_check(app, buffer, view)){
			result = buffer_kill(app, buffer, BufferKill_AlwaysKill);
		}
	}
	return result;
}

function b32
vim_do_4coder_close_user_check(Application_Links *app, View_ID view){
	Scratch_Block scratch(app);
	Lister_Choice_List list = {};
	lister_choice(scratch, &list, "(N)o"  , "", KeyCode_N, SureToKill_No);
	lister_choice(scratch, &list, "(Y)es" , "", KeyCode_Y, SureToKill_Yes);
	lister_choice(scratch, &list, "(S)ave all and close", "", KeyCode_S, SureToKill_Save);

#define M "There are one or more buffers with unsave changes, close anyway?"
	Lister_Choice *choice = vim_get_choice_from_user(app, string_u8_litexpr(M), list);
#undef M

	b32 do_exit = false;
	if(choice != 0){
		switch(choice->user_data){
			case SureToKill_No:{} break;

			case SureToKill_Yes:{
				allow_immediate_close_without_checking_for_changes = true;
				do_exit = true;
			} break;

			case SureToKill_Save:{
				save_all_dirty_buffers(app);
				allow_immediate_close_without_checking_for_changes = true;
				do_exit = true;
			} break;
		}
	}

	return do_exit;
}

function void
vim_reload_external_changes_lister(Application_Links *app, Buffer_ID buffer){
	Scratch_Block scratch(app);
	Lister_Choice_List list = {};
	String_Const_u8 buffer_name = push_buffer_base_name(app, scratch, buffer);

	if(buffer_name.size == 0){ return; }
	lister_choice(scratch, &list, buffer_name, "", KeyCode_Q, u64(0));
	lister_choice(scratch, &list, "(L)oad external changes"  , "", KeyCode_L, u64(1));
	lister_choice(scratch, &list, "(K)eep current buffer" , "", KeyCode_K, u64(0));

#define M "External changes have been detected. Reload buffer from file?"
	Lister_Choice *choice = vim_get_choice_from_user(app, string_u8_litexpr(M), list);
#undef M

	if(choice != 0 && choice->user_data){
		buffer_reopen(app, buffer, 0);
	}
}

// NOTE(BYP): This hook isn't officially supported by core (it will false positive) it was just fun to write
CUSTOM_COMMAND_SIG(vim_file_externally_modified){
	User_Input input = get_current_input(app);
	if(match_core_code(&input, CoreCode_FileExternallyModified)){
		print_message(app, input.event.core.string);
		vim_reload_external_changes_lister(app, input.event.core.id);
	}
}

