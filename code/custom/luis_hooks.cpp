

CUSTOM_COMMAND_SIG(luis_startup)
CUSTOM_DOC("Default command for responding to a startup event")
{
    ProfileScope(app, "default startup");
    User_Input input = get_current_input(app);
    if (match_core_code(&input, CoreCode_Startup)){
        String_Const_u8_Array file_names = input.event.core.file_names;
        load_themes_default_folder(app);
        Buffer_ID command_line_file_buffer = default_4coder_initialize(app, file_names);
        //default_4coder_side_by_side_panels(app, file_names);
        {
            Buffer_Identifier identifier = buffer_identifier(string_u8_litexpr("*messages*"));
            Buffer_ID id = buffer_identifier_to_id(app, identifier);
            View_ID view = get_active_view(app, Access_Always);
            new_view_settings(app, view);
            
            if (command_line_file_buffer) { //prioritize command line file first
                view_set_buffer(app, view, command_line_file_buffer, 0);
            } else {
                view_set_buffer(app, view, id, 0);    
            }
                
        }
        
        b32 auto_load = def_get_config_b32(vars_save_string_lit("automatically_load_project"));
        if (auto_load){
            load_project(app);
        }

        #if 0
        Scratch_Block scratch(app);
        FILE *file = def_search_normal_fopen(scratch, "recently_opened_files.txt", "rb");
        if (file){
            String_Const_u8 data = dump_file_handle(scratch, file);
            fclose(file);

            char *end_plus_one = data.str + data.size;
            char *line_start = data.str;
            g_recently_opened_files_count = 0;
            for (char *at = data.str; at <= end_plus_one; at += 1) {
                if ((at == end_plus_one) || (*at == '\n')) {
                    String_Const_u8 filepath = SCu8(line_start, at);
                    filepath = string_skip_chop_whitespace(filepath);
                    File_Attributes attribs = system_quick_file_attributes(scratch, filepath);
                    if (attribgs.size > 0) {
                        String *str = &g_recently_opened_files[g_recently_opened_files_count];
                        *str = push_string_copy(&global_permanent_arena, filepath);
                        if (str->size == filepath.size) {
                            g_recently_opened_files_count += 1;
                            if (g_recently_opened_files_count == ArrayCount(g_recently_opened_files)) {
                                break;
                            } 
                        } 
                        
                    } 
                    line_start = at + 1;
                } 
            }

            
        }
        #endif
    }
    
    {
        def_audio_init();
    }
    
    {
        def_enable_virtual_whitespace = def_get_config_b32(vars_save_string_lit("enable_virtual_whitespace"));
        clear_all_layouts(app);
    }
    
    set_window_title(app, SCu8("milo editor"));
}



CUSTOM_COMMAND_SIG(luis_view_input_handler)
CUSTOM_DOC("Input consumption loop for default view behavior") {
    Scratch_Block scratch(app);
    default_input_handler_init(app, scratch);
    
    View_ID ctx_view = get_this_ctx_view(app, Access_Always);
    View_ID prev_active_view = get_active_view(app, Access_Always);
    Managed_Scope scope = view_get_managed_scope(app, ctx_view);
    //View_ID active_view = get_active_view(app, Access_Always);
    
    
    for (;;) {
        // NOTE(allen): Get input
        User_Input input = get_next_input(app, EventPropertyGroup_Any, 0);
        if (input.abort){
            break;
        }
        
        if (input.event.kind == InputEventKind_TextInsert ||
            input.event.kind == InputEventKind_KeyStroke) {
            system_show_mouse_cursor(MouseCursorShow_Never);
            //suppress_mouse(app);
        } else if (input.event.kind == InputEventKind_MouseMove ||
                   input.event.kind == InputEventKind_MouseWheel || 
                   input.event.kind == InputEventKind_MouseButton ||
                   input.event.kind == InputEventKind_MouseButtonRelease) {
            //allow_mouse(app);
            system_show_mouse_cursor(MouseCursorShow_Always);
        }
        
        ProfileScopeNamed(app, "before view input", view_input_profile);
        
        // NOTE(allen): Mouse Suppression
        Event_Property event_properties = get_event_properties(&input.event);
        if (suppressing_mouse && (event_properties & EventPropertyGroup_AnyMouseEvent) != 0){
            continue;
        }
        
        // NOTE(allen): Get binding
        if (implicit_map_function == 0){
            implicit_map_function = default_implicit_map;
        }
        Implicit_Map_Result map_result = implicit_map_function(app, 0, 0, &input.event);
        if (map_result.command == 0){
            leave_current_input_unhandled(app);
            continue;
        }
        
        // NOTE(allen): Run the command and pre/post command stuff
        b32 actually_do_command = true;
        //b32 try_to_recover_peek_view_after_command = false;
        { //NOTE(luis) pre command
            Rewrite_Type *next_rewrite = scope_attachment(app, scope, view_next_rewrite_loc, Rewrite_Type);
            *next_rewrite = Rewrite_None;
            
            Custom_Command_Function *cmd = map_result.command;
            if (cmd == open_panel_vsplit || cmd == open_panel_hsplit)
            {
                if (luis_view_has_flags(app, ctx_view, VIEW_IS_PEEK_WINDOW)) {
                    actually_do_command = false;
                } else { //if it has a peek view, close it and split anyways
                    View_ID peek = luis_get_peek_window(app, ctx_view);
                    if (peek) {
                        view_close(app, peek);
                        //try_to_recover_peek_view_after_command = true;   
                    }
                }
            }
        }
        
        ProfileCloseNow(view_input_profile);

        Buffer_ID buffer_id_before_command     = view_get_buffer(app, ctx_view, Access_Always);
        i64 cursor_pos_before_executed_command = view_get_cursor_pos(app, ctx_view);
        if (actually_do_command) {
            //b32 do_kill_tab_group = luis_view_has_flags(app, view, VIEW_KILL_TAB_GROUP_ON_VIEW_CLOSE);
            //i32 tab_group_index = view_get_tab_group_index(app, view);
            
            map_result.command(app);
            g_last_executed_command = map_result.command;
            
            if (map_result.command == paste) {
                PREV_PASTE_INIT_CURSOR_POS = cursor_pos_before_executed_command;
            } else {
                PREV_PASTE_INIT_CURSOR_POS = -1;
            }
                
            
            //if (map_result.command != luis_peek_code_index_up && map_result.command != luis_peek_code_index_down)
                //CURSOR_PEEK_CODE_INDEX_RELATIVE_LINE_OFFSET = -1;
            
            //NOTE we do this here instead of view_change_buffer because there we don't know
            //what the previous cursor pos and I can't find a way to get a view's buffer's cursor pos
            if (view_get_buffer(app, ctx_view, Access_Always) != buffer_id_before_command) {
                View_Buffer_Location *loc = view_get_prev_buffer_location(app, ctx_view);
                if (loc)
                {
                    loc->buffer = buffer_id_before_command;
                    loc->cursor = cursor_pos_before_executed_command;
                }
            }
            
            View_ID new_active_view = get_active_view(app, Access_Always); 
            if (prev_active_view != new_active_view) //we changed active view here, update bindings as well
            {
                update_buffer_bindings_for_modal_toggling(app, view_get_buffer(app, new_active_view, Access_Always));
            }
            //if (!view_exists(app, view) && do_kill_tab_group) //pseudo-hook for when a view is being destroyed
            //{
            //kill_tab_group(app, tab_group_index);
            //}
        }
        
        #if 0
        if (try_to_recover_peek_view_after_command)
        {
            Peek_Code_Index_State *state = scope_attachment(app, scope, view_code_peek_state, Peek_Code_Index_State);
            if (state)
            {
                peek_next_code_index(app, view, state, state->index);
                view_set_active(app, view); //peek_next_code_index sets peek active, so we just undo it... yeah it's messy
                //luis_change_active_panel_ignore_peek_windows(app) 
            }
        }
        #endif
        
        ProfileScope(app, "after view input");
        
        { //NOTE(luis) post command
            Rewrite_Type *next_rewrite = scope_attachment(app, scope, view_next_rewrite_loc, Rewrite_Type);
            if (next_rewrite != 0){
                if (*next_rewrite != Rewrite_NoChange){
                    Rewrite_Type *rewrite =
                        scope_attachment(app, scope, view_rewrite_loc, Rewrite_Type);
                    *rewrite = *next_rewrite;
                }
            }
            
            View_ID current_active_view = get_active_view(app, Access_Always);

            if (fcoder_mode == FCoderMode_Original) { 
                
                View_ID new_active_view = get_active_view(app, Access_Always);
                if ((new_active_view == prev_active_view) && (cursor_pos_before_executed_command != view_get_cursor_pos(app, ctx_view))) {
                    
                    Custom_Command_Function *cmd = map_result.command;
                    if (cmd != write_text_input) {
                        // cursor pos changed and active view hasn't changed
                        bool holding_down_shift_key = false;
                        Input_Modifier_Set mods = system_get_keyboard_modifiers(scratch);
                        if (has_modifier(&mods, KeyCode_Shift)) {
                            holding_down_shift_key = true;
                        }
                        
                        if (holding_down_shift_key) {
                            if (!g_mark_is_active) {
                                luis_set_mark(app, ctx_view, cursor_pos_before_executed_command);    
                            }   
                        }
                    }
                     
                }

                
                
                
            } else if (fcoder_mode == FCoderMode_NotepadLike && (ctx_view == current_active_view)) {
                b32 should_check_snap_mark_to_cursor = luis_view_has_flags(app, ctx_view, VIEW_NOTEPAD_MODE_MARK_SET);
                bool holding_down_shift_key = false;
                if (!should_check_snap_mark_to_cursor) {
                    Input_Modifier_Set mods = system_get_keyboard_modifiers(scratch);
                    if (has_modifier(&mods, KeyCode_Shift)) {
                        holding_down_shift_key = true;
                        should_check_snap_mark_to_cursor = true;
                    }
                }
                
                b32 snap_mark_to_cursor = true;
                if (should_check_snap_mark_to_cursor) {
                    snap_mark_to_cursor = false;
                    Custom_Command_Function *cmd = map_result.command;
                    //TODO it would be really neat if we could supply meta-data to the custom command functions to say stuff
                    //like this shouldn't snap mark to cursor (instead of doing this wierd thing of setting globals and reading it back here)
                    //this way the user could just a prepass on all the default commands and change the meta-data to their liking
                    if (cmd == cut || cmd == copy || cmd == paste || cmd == paste_and_indent || cmd == backspace_char || 
                        cmd == write_text_input || cmd == write_space || cmd == luis_write_underscore || cmd == luis_write_pointer_arrow ||
                        cmd == luis_write_tab || cmd == luis_write_newline || cmd == write_text_and_auto_indent ||
                        cmd == auto_indent_line_at_cursor || cmd == auto_indent_whole_file || cmd == auto_indent_range ||
                        cmd == delete_range || cmd == luis_multiline_comment_toggle || cmd == place_in_scope || //cmd == luis_surround_in_parens ||
                        cmd == view_buffer_other_panel || cmd == if_read_only_goto_position || cmd == if_read_only_goto_position_same_panel || cmd == luis_escape ||
                        cmd == goto_line || cmd == goto_prev_jump || cmd == goto_next_jump || cmd == goto_first_jump || cmd == reopen) {
                        luis_view_clear_flags(app, ctx_view, VIEW_NOTEPAD_MODE_MARK_SET);
                        snap_mark_to_cursor = true;
                    }   
                } 
                
                if (snap_mark_to_cursor) {
                    i64 pos = view_get_cursor_pos(app, ctx_view);
                    view_set_mark(app, ctx_view, seek_pos(pos));
                } else if (holding_down_shift_key) {
                    // NOTE I'm not entirely sure if this will work well...
                    // basically when we hold down shift and begin selecting like in notepad
                    // I want to record the mark pos. I set it to mark_pos
                    if ((map_result.command != emacs_swap_cursor_mark) && 
                        (map_result.command != cursor_mark_swap)) {
                        // we check if last command wasn't luis_swap_cursor_and_mark because
                        // that will move the mark to where cursor currently is...
                        // other commands that do that will have this problem....
                        i64 *prev_mark_pos = scope_attachment(app, scope, view_prev_mark_pos_before_snap_to_cursor, i64);
                        if (prev_mark_pos) {
                            *prev_mark_pos = view_get_mark_pos(app, ctx_view);
                        }    
                    }
                    
                }
            }
            
            Buffer_ID buffer = view_get_buffer(app, ctx_view, Access_Always);
            Dirty_State dirty = buffer_get_dirty_state(app, buffer);
            if (HasFlag(dirty, DirtyState_UnloadedChanges)) {
                bool snap_mark_to_cursor = false;
                
                if (HasFlag(dirty, DirtyState_UnsavedChanges)) {
                    // query user to maybe save changes
                    
                    u8 textbuffer[16];
                    Query_Bar bar = {};
                    bar.prompt = SCu8("File modified externally... Reload file and lose changes (y/n)? ");
                    bar.string = SCu8(textbuffer, (u64)0);
                    bar.string_capacity = sizeof(textbuffer);
                    if (query_user_string(app, &bar)) {
                        String_Const_u8 result = string_skip_whitespace(bar.string);
                        if (result.size > 0) {
                            if (result.str[0] == 'y' || result.str[0] == 'Y') {
                                Buffer_Reopen_Flag flags = {};
                                buffer_reopen(app, buffer, flags);
                                
                                luis_view_clear_flags(app, ctx_view, VIEW_NOTEPAD_MODE_MARK_SET);
                                snap_mark_to_cursor = true;
                            } else if (result.str[0] == 'n' || result.str[0] == 'N') {
                                // do nothing
                            } else {
                                // TODO flash message...
                                print_message(app, SCu8("Incorrect response. Please answer y(es) or n(o) when asked to reload the file with unsaved changes...\n"));
                            }
                            
                            // query bar doesn't go away for  some after if we input answer 
                            end_query_bar(app, &bar, 0);
                        }
                    }
                } else {
                    // just reload file no questions asked
                    Buffer_Reopen_Flag flags = {};
                    buffer_reopen(app, buffer, flags);
                    
                    luis_view_clear_flags(app, ctx_view, VIEW_NOTEPAD_MODE_MARK_SET);
                    snap_mark_to_cursor = true;
                }
                
                if (snap_mark_to_cursor) {
                    i64 pos = view_get_cursor_pos(app, ctx_view);
                    view_set_mark(app, ctx_view, seek_pos(pos));
                }
            }
        }
    }
}


#if 0
function void
vim_animate_filebar(Application_Links *app, Frame_Info frame_info){
#if 1
	f32 diff = vim_nxt_filebar_offset - vim_cur_filebar_offset;
	if (fabs(diff) > 1.0f){
		vim_cur_filebar_offset += diff*frame_info.animation_dt*25.0f;
		animate_in_n_milliseconds(app, 0);
	}else{
		vim_cur_filebar_offset = vim_nxt_filebar_offset;
	}
#else
	vim_cur_filebar_offset = vim_nxt_filebar_offset;
#endif
}
#endif

function void
luis_tick(Application_Links *app, Frame_Info frame_info){
	code_index_update_tick(app);
	if (tick_all_fade_ranges(app, frame_info.animation_dt)){
		animate_in_n_milliseconds(app, 0);
	}

    
	// vim_animate_filebar(app, frame_info);
	//vim_animate_cursor(app, frame_info);
	//vim_cursor_blink++;
	//fold_tick(app, frame_info);

	b32 enable_virtual_whitespace = def_get_config_b32(vars_save_string_lit("enable_virtual_whitespace"));
	if (enable_virtual_whitespace != def_enable_virtual_whitespace){
		def_enable_virtual_whitespace = enable_virtual_whitespace;
		clear_all_layouts(app);
	}
}

internal b32
maybe_draw_string_with_printf_specifier_highlight(Application_Links *app, Text_Layout_ID text_layout_id, Buffer_ID buffer_id, Token *token) {
    b32 drew_special_comment_token = false;
    if (HIGHLIGHT_PRINTF_SPECIFIER && 
        token->kind == TokenBaseKind_LiteralString && 
        token->sub_kind != TokenCppKind_LiteralCharacter)
    {
        drew_special_comment_token = true;
        Scratch_Block scratch(app);
        String_Const_u8 string = push_token_lexeme(app, scratch, buffer_id, token);
        ARGB_Color alt_argb = fcolor_resolve(fcolor_id(defcolor_comment));
        ARGB_Color str_argb = fcolor_resolve(fcolor_id(defcolor_str_constant));
        u64 normal_at = 0; //represents the substring before the specifier that we color normally
        u64 normal_length = 0; //we iterate until we reach a specifier, color normal substring, specifier, and continue
        for(u64 i = 0; i < string.size; /*we increment i ourselves*/ ) 
        {
            if (string.str[i] == '\\' || string.str[i] == '%')
            {
                if (normal_length) //draw normal substring
                    paint_text_color(app, text_layout_id, Ii64_size(token->pos + normal_at, normal_length), str_argb);
                
                //decides whether to actually color in the case where user hasn't finished typing the specifier
                b32 actually_found_specifier = false;
                i32 specifier_length = 0;
                
                if (string.str[i] == '\\')  
                {
                    //NOTE 4coder seems to not lex a token as a string token if nothing comes after the first '\'
                    //meaning it seems safe to just assume the specifier is complete and of length 2
                    //NOTE I don't actually check if the escape character is valid because I don't care :P
                    specifier_length = 2; 
                    actually_found_specifier = true;
                    goto JUST_DRAW_THE_FREAKING_STRING; //we use this extensively below to 'break' out if we pass the end of str
                }
                //else{} we have a printf specifier like so %[flags][width][.precision][length]specifier
                
                //increments length but if we reach the ending " we just 'break' out and draw
                #define INCREMENT_SPECIFIER_LENGTH do {specifier_length += 1; \
                    if ((i + specifier_length) >= (string.size - 1)) goto JUST_DRAW_THE_FREAKING_STRING; } while(0)
                
                //next character to see if it's part of the specifier
                #define NEXT string.str[i + specifier_length]
                
                INCREMENT_SPECIFIER_LENGTH; //for first %
                
                //[flags] 
                if (NEXT == '+' || NEXT == '-' || NEXT == '#' || NEXT == '0' /*|| NEXT == ' '*/)	
                    INCREMENT_SPECIFIER_LENGTH;
                
                //[width] is either '*' or a sequence of numbers denoting the width
                if (NEXT == '*')	INCREMENT_SPECIFIER_LENGTH;
                else if (character_is_base10(NEXT))
                {
                    while(character_is_base10(NEXT)) INCREMENT_SPECIFIER_LENGTH;
                }
                
                //[precision] begins with '.' and followed by either '*' or a sequence of numbers
                if (NEXT == '.')
                {
                    INCREMENT_SPECIFIER_LENGTH;
                    if (NEXT == '*') INCREMENT_SPECIFIER_LENGTH;
                    else if (character_is_base10(NEXT))
                    {
                        while(character_is_base10(NEXT)) INCREMENT_SPECIFIER_LENGTH;
                    }
                }
                
                //[length] length can either be h, l, j, z, t, hh, ll
                if (NEXT == 'j' || NEXT == 'z' || NEXT == 't') INCREMENT_SPECIFIER_LENGTH; 
                else if (NEXT == 'h')  //hh
                {
                    INCREMENT_SPECIFIER_LENGTH;
                    if (NEXT == 'h') INCREMENT_SPECIFIER_LENGTH; //second h 
                }
                else if (NEXT == 'l') //ll
                {
                    INCREMENT_SPECIFIER_LENGTH;
                    if (NEXT == 'l') INCREMENT_SPECIFIER_LENGTH; //second l
                }
                
                //the actual specifier where we decide if to actually highlight
                if (NEXT == 'd' || NEXT == 'i' || NEXT == 'u' || NEXT == 'o' || NEXT == 'x' || NEXT == 'X' ||
                    NEXT == 'f' || NEXT == 'e' || NEXT == 'g' || NEXT == '%' || NEXT == 'c' || NEXT == 's' || 
                    NEXT == 'p' || NEXT == 'n')
                /*NEXT == 'F' || NEXT == 'E' || NEXT == 'G' || NEXT == 'a' || NEXT == 'A' ||*/
                {
                    actually_found_specifier = true;
                    INCREMENT_SPECIFIER_LENGTH;
                }
                
                JUST_DRAW_THE_FREAKING_STRING:
                paint_text_color(app, text_layout_id, Ii64_size(token->pos + i, specifier_length), 
                    actually_found_specifier ? alt_argb : str_argb);
                i += specifier_length;
                normal_at = i; 
                normal_length = 0; //to begin search for a new normal substring
                #undef NEXT
                #undef INCREMENT_SPECIFIER_LENGTH
            }
            else
            {
                normal_length += 1;
                i += 1;
            }
        }
        //we do this one more time bc we only draw the normnal substring *before* specifiers 
        //so the normal substring after the last specifier won't come up in the loop
        if (normal_length) paint_text_color(app, text_layout_id, Ii64_size(token->pos + normal_at, normal_length), str_argb);
    }
    return drew_special_comment_token;
}

struct Token_Visual_Properties
{
    ARGB_Color color;
    b32 underline;
    Face_ID special_face_id;
};

internal b32
does_token_look_like_var_identifier(Application_Links *app, Buffer_ID buffer, Token_Iterator_Array *it, Token *identifier)
{
    b32 looks_like_var = false;
    if (identifier->kind == TokenBaseKind_Identifier)
    {
        //NOTE(luis) this won't handle macros even markup one's that expand to nothing
        for(Token *token = identifier - 1; token >= it->tokens; token -= 1)
        {
            if (token->kind == TokenBaseKind_Whitespace) ; //keep going
            else if (token->kind == TokenBaseKind_Operator && token->sub_kind == TokenCppKind_Star) ; //keep going
            else if (token->kind == TokenBaseKind_Identifier) //stop, see if it looks like a note type
            {
                Scratch_Block scratch(app);
                String_Const_u8 string = push_token_lexeme(app, scratch, buffer, token);
                Code_Index_Note *note = code_index_note_from_string(string);
                looks_like_var = note && (note->note_kind == CodeIndexNote_Type);
                break;
            }
            else if (token->kind == TokenBaseKind_Keyword) //stop, see if it's a built-in keyword type
            {
                looks_like_var = (token->sub_kind == TokenCppKind_Void ||
                    token->sub_kind == TokenCppKind_Bool ||
                    token->sub_kind == TokenCppKind_Char ||
                    token->sub_kind == TokenCppKind_Int ||
                    token->sub_kind == TokenCppKind_Float ||
                    token->sub_kind == TokenCppKind_Double ||
                    token->sub_kind == TokenCppKind_Long ||
                    token->sub_kind == TokenCppKind_Short);
                break;
            }
            else break; //didn't expect that token type in a var decl token sequence, just end
            
        }   
    }
    return looks_like_var;
}

internal Token_Visual_Properties
get_token_visual_properties(Application_Links *app, Buffer_ID buffer, Token_Iterator_Array *it, Token *token, String_Const_u8 token_string)
{
    Token_Visual_Properties prop = {};
    prop.color = fcolor_resolve(get_token_color_cpp(*token));   
    
    Code_Index_Note *note = code_index_note_from_string(token_string);
    if (note) {
        if (note->note_kind == CodeIndexNote_Function || note->note_kind == CodeIndexNote_Function_Definition) {
            Token *next = token + 1;
            if (next >= (it->tokens + it->count)) next = 0;
            
            if (next && next->kind == TokenBaseKind_Whitespace) {
                next += 1;
                if (next >= (it->tokens + it->count)) next = 0;
            }
            
            if (next) {
                if ((next->kind == TokenBaseKind_ParentheticalOpen && next->sub_kind == TokenCppKind_ParenOp) ||
                     next->kind == TokenBaseKind_Operator          && next->sub_kind == TokenCppKind_Less) {
                    prop.color = fcolor_resolve(fcolor_id(luiscolor_function));
                }   
            }
        }   
        else if (note->note_kind == CodeIndexNote_Type || note->note_kind == CodeIndexNote_Type_Definition){
            prop.color = fcolor_resolve(fcolor_id(luiscolor_type));//= 0xffE89393; //pinkish 
        }
        else if (note->note_kind == CodeIndexNote_Namespace){
            prop.color = fcolor_resolve(fcolor_id(luiscolor_namespace));//= 0xffE89393; //pinkish 
        }
        else if (note->note_kind == CodeIndexNote_Macro) 
        {
            if (!string_match(token_string, str8_lit("assert"))       && 
                !string_match(token_string, str8_lit("if_assert"))    &&
                !string_match(token_string, str8_lit("defer_assert"))) { //use normal color for assert
                prop.color = fcolor_resolve(fcolor_id(luiscolor_macro));//= 0xffDFAF8F; //orange
            }  
        }    
    }
    else {
        if (does_token_look_like_var_identifier(app, buffer, it, token)) prop.color = fcolor_resolve(fcolor_id(luiscolor_variable_decl));
        else if (token->kind == TokenBaseKind_Operator && token->sub_kind == TokenCppKind_Not)
            prop.color = fcolor_resolve(fcolor_id(luiscolor_logical_not));    
    }
    
    if (token->kind == TokenBaseKind_Keyword)           prop.special_face_id = BOLD_CODE_FACE;
    //else if (token->kind == TokenBaseKind_Comment)      prop.special_face_id = ITALICS_CODE_FACE; //ignoring for now
    else if (token->kind == TokenBaseKind_Preprocessor) prop.underline = true;
    else if (token->kind == TokenBaseKind_Identifier) { //some words not considered keywords for some reason
        if (string_match(token_string, str8_lit("constexpr")) || string_match(token_string, str8_lit("auto")))
        {
            prop.color = fcolor_resolve(fcolor_id(defcolor_keyword));
            prop.special_face_id = BOLD_CODE_FACE;
        }        
    }
    
    
    return prop;
}

internal void
draw_buffer_range(Application_Links *app, Buffer_ID buffer, Text_Layout_ID layout_id, Face_ID face_id, Range_i64 range, ARGB_Color color)
{
    //stupid easy way I could get this to work.... you should probably scan for words to reduce draw_string calls, but whatevs
    Scratch_Block scratch(app);
    String_Const_u8 string = push_buffer_range(app, scratch, buffer, range);
    for(i64 byte_pos = range.min; byte_pos < range.max; byte_pos += 1) 
    {
        Vec2_f32 pos = text_layout_character_on_screen(app, layout_id, byte_pos).p0;
        String_Const_u8 c;
        c.str = string.str + (byte_pos - range.min);
        c.size = 1;
        draw_string(app, face_id, c, pos, color);
    }
}

function void //adpated from similar function in 4coder_draw.cpp
luis_draw_character_block_outline(Application_Links *app, Text_Layout_ID layout, Range_i64 range, f32 roundness, FColor fcolor){
    if (range.first < range.one_past_last){
        i64 i = range.first;
        Rect_f32 first_rect = text_layout_character_on_screen(app, layout, i);
        i += 1;
        Range_f32 y = rect_range_y(first_rect);
        Range_f32 x = rect_range_x(first_rect);
        for (;i < range.one_past_last; i += 1){
            Rect_f32 rect = text_layout_character_on_screen(app, layout, i);
            if (rect.x0 < rect.x1 && rect.y0 < rect.y1){
                Range_f32 new_y = rect_range_y(rect);
                Range_f32 new_x = rect_range_x(rect);
                b32 joinable = false;
                if (new_y == y && (range_overlap(x, new_x) || x.max == new_x.min || new_x.max == x.min)){
                    joinable = true;
                }
                
                if (!joinable){
                    //draw_rectangle(app, Rf32(x, y), roundness, fcolor_resolve(fcolor));
                    draw_rectangle_outline_fcolor(app, Rf32(x, y), roundness, 4.0f, fcolor);
                    y = new_y;
                    x = new_x;
                }
                else{
                    x = range_union(x, new_x);
                }
            }
        }
        draw_rectangle_outline_fcolor(app, Rf32(x, y), roundness, 4.0f, fcolor);
    }
}

internal b32
draw_paren_range_and_its_sub_ranges(Application_Links *app, Buffer_ID buffer, Text_Layout_ID text_layout_id, Range_i64 max_range, i32 color_index) {
    b32 drew_range_and_subranges = false;
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    if (range_overlap(max_range, visible_range)) {
        Color_Array color_array = finalize_color_array(defcolor_text_cycle);
        ARGB_Color color = color_array.vals[color_index % color_array.count];
        paint_text_color_pos(app, text_layout_id, max_range.min,   color);
        paint_text_color_pos(app, text_layout_id, max_range.max-1, color);
        
        i32 range_count = 0;
        Range_i64 ranges[16];
        if(find_next_parens_absolute(app, buffer, max_range.min, ranges + 0) &&
           range_overlap(ranges[0], max_range) && range_overlap(ranges[0], visible_range)) //find first sub range
        {
            drew_range_and_subranges = true;
            range_count = 1;
            while(range_count < ArrayCount(ranges) && 
                  find_next_parens_absolute(app, buffer, ranges[range_count-1].max-1, ranges + range_count))
            {
                if(range_overlap(ranges[range_count], max_range) && range_overlap(ranges[range_count], visible_range))
                range_count += 1;
                else break;
            }
            
            for(i32 i = 0; i < range_count; i += 1)
            draw_paren_range_and_its_sub_ranges(app, buffer, text_layout_id, ranges[i], color_index + 1);
        }
        
        #if 1 //draw commas to match the max_range color we chose
        Token_Array token_array = get_token_array_from_buffer(app, buffer);
        Token_Iterator_Array it = token_iterator_pos(0, &token_array, max_range.min + 1);
        for(;;)
        {
            Token *token = token_it_read(&it);
            if(!token) break;
            if(token->pos >= (max_range.max-1)) break;
            if(token->pos >= visible_range.max) break;
            
            
            if((token->kind == TokenBaseKind_StatementClose && token->sub_kind == TokenCppKind_Comma))
                //token->kind == TokenBaseKind_Operator)
            {
                b32 in_another_subrange = false;
                for(i32 i = 0; i < range_count; i += 1)
                {
                    if(token->pos > ranges[i].min && token->pos < ranges[i].max)
                    {
                        in_another_subrange = true;
                        break;
                    }
                }
                
                if(!in_another_subrange)
                paint_text_color(app, text_layout_id, Ii64_size(token->pos, token->size), color);
            }
            if(!token_it_inc_all(&it))	break;
        }
        #endif
    }
    return drew_range_and_subranges;
}

function void
luis_draw_original_mode_cursor_and_mark(Application_Links *app, View_ID view_id, b32 is_active_view,
                                                 Buffer_ID buffer, Text_Layout_ID text_layout_id,
                                                 f32 roundness, f32 outline_thickness){
    b32 has_highlight_range = draw_highlight_range(app, view_id, buffer, text_layout_id, roundness);
    if (!has_highlight_range){
        i32 cursor_sub_id = default_cursor_sub_id();
        
        i64 cursor_pos = view_get_cursor_pos(app, view_id);
        i64 mark_pos = view_get_mark_pos(app, view_id);
        if (is_active_view){
            draw_character_block(app, text_layout_id, cursor_pos, roundness,
                                 fcolor_id(defcolor_cursor, cursor_sub_id));
            paint_text_color_pos(app, text_layout_id, cursor_pos,
                                 fcolor_id(defcolor_at_cursor));
        } else {
            // draw_character_wire_frame(app, text_layout_id, cursor_pos,
                                      // roundness, outline_thickness,
                                      // fcolor_id(defcolor_cursor, cursor_sub_id));
            draw_character_wire_frame(app, text_layout_id, mark_pos,
                                      roundness, outline_thickness,
                                      fcolor_id(defcolor_mark));
        }
    }
}

function void
luis_render_buffer(Application_Links *app, View_ID view_id, Face_ID face_id,
    Buffer_ID buffer, Text_Layout_ID text_layout_id,
    Rect_f32 rect) {
    ProfileScope(app, "luis render buffer");
    
    View_ID active_view = get_active_view(app, Access_Always);
    b32 is_active_view = (active_view == view_id);
    Rect_f32 prev_clip = draw_set_clip(app, rect);
    
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    
    // NOTE(allen): Cursor shape
    Face_Metrics metrics = get_face_metrics(app, face_id);
    u64 cursor_roundness_100 = def_get_config_u64(app, vars_save_string_lit("cursor_roundness"));
    f32 cursor_roundness = metrics.normal_advance*cursor_roundness_100*0.01f;
    f32 mark_thickness = (f32)def_get_config_u64(app, vars_save_string_lit("mark_thickness"));
    
    i64 cursor_pos = view_correct_cursor(app, view_id);
    view_correct_mark(app, view_id);
    
    //Mouse_State mouse = get_mouse_state(app);
    //i64 mouse_pos = view_pos_from_xy(app, view_id, V2f32(mouse.p));
    
    // NOTE(allen): Line highlight
    b32 highlight_line_at_cursor = def_get_config_b32(vars_save_string_lit("highlight_line_at_cursor"));
    if (highlight_line_at_cursor && is_active_view){
        i64 line_number = get_line_number_from_pos(app, buffer, cursor_pos);
        draw_line_highlight(app, text_layout_id, line_number, fcolor_id(defcolor_highlight_cursor_line));
    }
    
    // NOTE(allen): Token colorizing
    Token *token_cursor_over = 0;
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    if (token_array.tokens != 0) {
        //Scratch_Block scratch(app);
        
        b32 ate_assert_keyword = false;
        i32 assert_balance_to_end_on = 0;
        i32 paren_balance = 0;
        {
            Range_i64 range = {visible_range.min};
            while (find_surrounding_nest(app, buffer, range.min, FindNest_Paren, &range)) {
                if (range.min < visible_range.min) { //this only needs to be checked the first time, but whatevs
                    paren_balance += 1;
                }
            }
            
            if (paren_balance < 0) { //see if it's from an assert
                Token *before = token_from_pos(&token_array, range.min - 1);
                if (before->kind == TokenBaseKind_Whitespace) {
                    before -= 1;
                    if (before < token_array.tokens) before = 0;
                }
                
                if (before && before->kind == TokenBaseKind_Identifier) {
                    Scratch_Block scratch(app);
                    String_Const_u8 token_string = push_token_lexeme(app, scratch, buffer, before);
                    if (string_match(token_string, str8_lit("assert")) ||
                        string_match(token_string, str8_lit("if_assert")) ||
                        string_match(token_string, str8_lit("defer_assert"))) {
                        ate_assert_keyword = true;
                        assert_balance_to_end_on = 0;
                    }
                } 
            }
        }
        
        Token_Iterator_Array it = token_iterator_pos(0, &token_array, visible_range.first);
        Token *token = token_it_read(&it);
        while (token && token->pos < visible_range.one_past_last) {
            if ((cursor_pos >= token->pos) && (cursor_pos < (token->pos + token->size)))
                token_cursor_over = token;
            
            if (!maybe_draw_string_with_printf_specifier_highlight(app, text_layout_id, buffer, token)) {
                Scratch_Block scratch(app);
                String_Const_u8 token_string = push_token_lexeme(app, scratch, buffer, token);
                Token_Visual_Properties prop = get_token_visual_properties(app, buffer, &it, token, token_string);
                
                #if 1 //make words inside parents fade out a bit (make ones inside assert macros even more faded out)
                b32 fade_tokens_inside_parens  = def_get_config_b32(vars_save_string_lit("luis_fade_tokens_inside_parens"));
                if (fade_tokens_inside_parens) {
                    if (token->kind == TokenBaseKind_Identifier) {
                        if (!ate_assert_keyword &&
                            string_match(token_string, str8_lit("assert")) ||
                            string_match(token_string, str8_lit("if_assert")) ||
                            string_match(token_string, str8_lit("defer_assert"))) {
                            ate_assert_keyword = true;
                            assert_balance_to_end_on = paren_balance;
                        }
                    } else if (token->kind == TokenBaseKind_ParentheticalOpen) {
                        paren_balance += 1;    
                    } else if (token->kind == TokenBaseKind_ParentheticalClose) {
                        paren_balance -= 1;
                        
                        if (ate_assert_keyword) {
                            if (paren_balance == assert_balance_to_end_on) {
                                ate_assert_keyword = false;
                            }    
                        }
                    //if (paren_balance == 0) ate_assert_keyword = false;
                    }
                    
                    if (paren_balance > 0) {
                    //NOTE we do this check to avoid fading away the first matching paren
                        if (!(paren_balance == 1 && token->kind == TokenBaseKind_ParentheticalOpen)) {
                            Vec4_f32 color_v4 = unpack_color(prop.color);
                            f32 scale = 0.6f;
                            if (ate_assert_keyword && (paren_balance > assert_balance_to_end_on)) scale = 0.4f;
                        //f32 scale = ate_assert_keyword ? 0.4f : 0.6f;
                            color_v4.a *= scale;
                            prop.color = pack_color(color_v4);    
                            
                        }
                        
                    }    
                }
                #endif //end assert test 
                    
                
                //TODO changing font size doesn't scale the other fonts well
                Rect_f32 first_rect = text_layout_character_on_screen(app, text_layout_id, token->pos);
                if (prop.special_face_id)
                    draw_buffer_range(app, buffer, text_layout_id, prop.special_face_id, Ii64_size(token->pos, token->size), prop.color);
                else	
                    paint_text_color(app, text_layout_id, Ii64_size(token->pos, token->size), prop.color);
                
                //TODO maybe just render a small rectangle
                String_Const_u8 underline_str = SCu8("_______________________________________________________________________________________________________________________________________________________________________________________________");
                if (prop.underline && (u64)token->size <= underline_str.size)
                {
                    underline_str.size = (u64)token->size;
                    draw_string(app, face_id, underline_str, first_rect.p0, prop.color);
                }   
            }
            
            if (!token_it_inc_all(&it))	break;
            token = token_it_read(&it);
        }
        
        // NOTE(allen): Scan for TODOs and NOTEs
        b32 use_comment_keyword = def_get_config_b32(vars_save_string_lit("use_comment_keywords"));
        if (use_comment_keyword){
            Comment_Highlight_Pair pairs[] = {
                {string_u8_litexpr("NOTE"), finalize_color(defcolor_comment_pop, 0)},
                {string_u8_litexpr("TODO"), finalize_color(defcolor_comment_pop, 1)},
            };
            draw_comment_highlights(app, buffer, text_layout_id, &token_array, pairs, ArrayCount(pairs));
        }
        
        #if 0 //ignoring this for now
        b32 do_draw_brace_lines = true;
        if (do_draw_brace_lines)
        {
            Range_i64 range;
            b32 found_next_scope = find_maximal_scope(app, buffer, visible_range.min, &range);
            if (!found_next_scope && find_next_scope_absolute(app, buffer, visible_range.min, &range))
                found_next_scope = range_overlap(range, visible_range);
            
            while(found_next_scope)
            {
                if (range_overlap(range, visible_range)) 
                    draw_brace_lines(app, view_id, buffer, text_layout_id, range, rect);
                found_next_scope = find_next_scope_absolute(app, buffer, range.min+1, &range);
                if (found_next_scope && range.min > visible_range.max) break;
            }
        }
        #endif
        
        #if 1
        b32 do_draw_colored_parens = true;
        if (do_draw_colored_parens) {
            //get the first max_parens
            Range_i64 max_range;
            if (find_maximal_parens(app,       buffer, visible_range.min, &max_range) ||
                find_next_parens_absolute(app, buffer, visible_range.min, &max_range)) {
                while (range_overlap(max_range, visible_range)) {
                    draw_paren_range_and_its_sub_ranges(app, buffer, text_layout_id, max_range, 0);
                    b32 found_next = find_next_parens_absolute(app, buffer, max_range.max-1, &max_range);
                    if (!found_next) break;
                }
            }
        }
        #endif
    }
    else{
        paint_text_color_fcolor(app, text_layout_id, visible_range, fcolor_id(defcolor_text_default));
    }
    
    
    
    
    
    // NOTE(allen): Scope highlight
    b32 use_scope_highlight = def_get_config_b32(vars_save_string_lit("use_scope_highlight"));
    if (use_scope_highlight){
        Color_Array colors = finalize_color_array(defcolor_back_cycle);
        draw_scope_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
    }
    
    b32 use_error_highlight = def_get_config_b32(vars_save_string_lit("use_error_highlight"));
    b32 use_jump_highlight = def_get_config_b32(vars_save_string_lit("use_jump_highlight"));
    if (use_error_highlight || use_jump_highlight){
        // NOTE(allen): Error highlight
        String_Const_u8 name = string_u8_litexpr("*compilation*");
        Buffer_ID compilation_buffer = get_buffer_by_name(app, name, Access_Always);
        if (use_error_highlight){
            draw_jump_highlights(app, buffer, text_layout_id, compilation_buffer,
                fcolor_id(defcolor_highlight_junk));
        }
        
        // NOTE(allen): Search highlight
        if (use_jump_highlight){
            Buffer_ID jump_buffer = get_locked_jump_buffer(app);
            if (jump_buffer != compilation_buffer){
                draw_jump_highlights(app, buffer, text_layout_id, jump_buffer,
                    fcolor_id(defcolor_highlight_white));
            }
        }
    }
    
    // NOTE(allen): Color parens
    b32 use_paren_helper = def_get_config_b32(vars_save_string_lit("use_paren_helper"));
    if (use_paren_helper){
        Color_Array colors = finalize_color_array(defcolor_text_cycle);
        draw_paren_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
    }
    
    // NOTE(allen): Whitespace highlight
    b64 show_whitespace = false;
    view_get_setting(app, view_id, ViewSetting_ShowWhitespace, &show_whitespace);
    if (show_whitespace){
        if (token_array.tokens == 0){
            draw_whitespace_highlight(app, buffer, text_layout_id, cursor_roundness);
        }
        else{
            draw_whitespace_highlight(app, text_layout_id, &token_array, cursor_roundness);
        }
    }
    
    // NOTE(luis): draw cursor
    if (fcoder_mode == FCoderMode_NotepadLike) {
        b32 has_highlight_range = draw_highlight_range(app, view_id, buffer, text_layout_id, cursor_roundness);
        if (!has_highlight_range)
        {
            i32 cursor_sub_id = default_cursor_sub_id();
            
            //i64 cursor_pos = view_get_cursor_pos(app, view_id);
            i64 mark_pos = view_get_mark_pos(app, view_id);
            if (is_active_view){
                //draw selection range if any...
                if (fcoder_mode == FCoderMode_NotepadLike && cursor_pos != mark_pos)
                {
                    Range_i64 range = Ii64(cursor_pos, mark_pos);
                    luis_draw_character_block_outline(app, text_layout_id, range, metrics.normal_advance*50*0.01f, fcolor_id(defcolor_highlight));
                }
                //mark block and character over
                FColor cursor_color = fcolor_id(defcolor_cursor, cursor_sub_id);
                FColor mark_color   = fcolor_id(defcolor_mark);
                if (IN_MODAL_MODE) cursor_color = fcolor_id(luiscolor_modal_cursor);//fcolor_id(defcolor_highlight);
                draw_character_block(app, text_layout_id, mark_pos, cursor_roundness, mark_color);
                draw_character_block(app, text_layout_id, cursor_pos, cursor_roundness, cursor_color);
                paint_text_color_pos(app, text_layout_id, cursor_pos,
                    fcolor_id(defcolor_at_cursor));
                paint_text_color_pos(app, text_layout_id, mark_pos,
                    fcolor_id(defcolor_at_cursor));
                
                
            }
            else{
                draw_character_wire_frame(app, text_layout_id, mark_pos,
                    cursor_roundness, mark_thickness,
                    fcolor_id(defcolor_mark));
                draw_character_wire_frame(app, text_layout_id, cursor_pos,
                    cursor_roundness, mark_thickness,
                    fcolor_id(defcolor_cursor, cursor_sub_id));
            }
        }
    } else if (fcoder_mode == FCoderMode_Original) {
        luis_draw_original_mode_cursor_and_mark(app, view_id, is_active_view, buffer, text_layout_id, cursor_roundness, mark_thickness);
        i64 mark_pos = view_get_mark_pos(app, view_id);
        if (g_mark_is_active && is_active_view && (cursor_pos != mark_pos)) {
            Range_i64 range = Ii64(cursor_pos, mark_pos);
            luis_draw_character_block_outline(app, text_layout_id, range, metrics.normal_advance*50*0.01f, fcolor_id(defcolor_highlight));
        } 
    }
    
    // NOTE(allen): Fade ranges
    paint_fade_ranges(app, text_layout_id, buffer);
    
    // NOTE(allen): put the actual text on the actual screen
    draw_text_layout_default(app, text_layout_id);
    
    #if 0 //I think this was suppoed to be a tooltip popup but I never used it
    if (CURSOR_PEEK_CODE_INDEX_RELATIVE_LINE_OFFSET >= 0 &&
        token_cursor_over && token_cursor_over->kind == TokenBaseKind_Identifier)
    {
        FColor highlight_fcolor = fcolor_id(defcolor_pop2);
        Scratch_Block scratch(app);
        String_Const_u8 token_string = push_token_lexeme(app, scratch, buffer, token_cursor_over);
        Code_Index_Note *note = code_index_note_from_string(token_string);
        if (note)
        {
            i64 line_start = get_line_number_from_pos(app, note->file->buffer, note->pos.start);
            line_start += CURSOR_PEEK_CODE_INDEX_RELATIVE_LINE_OFFSET;
            i32 num_lines_to_peek = 8;
            //if (note->kind == Code_Index_Note_Kind
            
            Fancy_Block block = {};
            {
                Fancy_Line *line = push_fancy_line(scratch, &block, face_id);
                String_Const_u8 buffer_name = push_buffer_unique_name(app, scratch, note->file->buffer);
                push_fancy_string(scratch, line, highlight_fcolor, buffer_name);
                push_fancy_stringf(scratch, line, highlight_fcolor, " :%lld", line_start);
            }
            
            for_index_inc(i32, i, num_lines_to_peek)
            {
                Fancy_Line *line = push_fancy_line(scratch, &block, face_id);
                //push_fancy_stringf(scratch, line, fcolor_id(defcolor_pop1), "F%d:", i + 1);
                String_Const_u8 textline = push_buffer_line(app, scratch, note->file->buffer, line_start + i);
                push_fancy_string(scratch, line, fcolor_id(defcolor_text_default), textline);
                
                
                //i64 cursor_position = view_get_cursor_pos(app, view_id);
                //Buffer_Cursor cursor = view_compute_cursor(app, view_id, seek_pos(cursor_position));
                //push_fancy_stringf(scratch, &line, base_color, "%lld-", cursor.line);
            }
            
            Rect_f32 region = view_get_buffer_region(app, view_id);
            
            Buffer_Scroll scroll = view_get_buffer_scroll(app, view_id);
            Buffer_Point buffer_point = scroll.position;
            Vec2_f32 cursor_p = view_relative_xy_of_pos(app, view_id, buffer_point.line_number, cursor_pos);
            //Vec2_f32 cursor_p;
            //cursor_p.x = (f32)mouse.p.x;
            //cursor_p.y = (f32)mouse.p.y;
            cursor_p -= buffer_point.pixel_shift;
            cursor_p += region.p0;
            
            //Face_Metrics metrics = get_face_metrics(app, face);
            f32 x_padding = metrics.normal_advance;
            f32 x_half_padding = x_padding*0.5f;
            
            draw_drop_down(app, face_id, &block, cursor_p, region, x_padding, x_half_padding,
                highlight_fcolor, fcolor_id(defcolor_back));
            
            
            //Rect_f32 popup_rect = text_layout_character_on_screen(app, text_layout_id, token_mouse_over->pos);
            //popup_rect.x1 += 64;
            //popup_rect.y1 += 64;
            //f32 popup_roundness = 6.0f;
            //draw_rectangle_fcolor(app, popup_rect, popup_roundness, fcolor_id(defcolor_pop1));   
        }
    }
    #endif
    
    draw_set_clip(app, prev_clip);
}

CUSTOM_COMMAND_SIG(luis_try_exit)
CUSTOM_DOC("Default command for responding to a try-exit event") {
    User_Input input = get_current_input(app);
    if (match_core_code(&input, CoreCode_TryExit)){
        b32 do_exit = true;
        if (!allow_immediate_close_without_checking_for_changes){
            b32 has_unsaved_changes = false;
            for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
                 buffer != 0;
                 buffer = get_buffer_next(app, buffer, Access_Always)){
                Dirty_State dirty = buffer_get_dirty_state(app, buffer);
                if (HasFlag(dirty, DirtyState_UnsavedChanges)){
                    has_unsaved_changes = true;
                    break;
                }
            }
            if (has_unsaved_changes) {
                //View_ID view = get_active_view(app, Access_Always);
                
                Scratch_Block scratch(app);
                Lister_Choice_List list = {};
                
                lister_choice(scratch, &list, "(Y)es - Save all and quit",       "", KeyCode_Y, SureToKill_Save);
                lister_choice(scratch, &list, "(N)o  - Don't save, just quit" , "",  KeyCode_N, SureToKill_Yes);
                lister_choice(scratch, &list, "(C)ancel"  ,                    "",   KeyCode_C, SureToKill_No);
                
                Lister_Choice *choice = get_choice_from_user(app, "There are buffer(s) with unsaved changes. Do you want to save changes before quitting?", list);
                do_exit = false;
                if (choice) {
                    switch (choice->user_data){
                    case SureToKill_No: break;
                    
                    case SureToKill_Yes: {
                        allow_immediate_close_without_checking_for_changes = true;
                        do_exit = true;
                    } break;
                    
                    case SureToKill_Save: {
                        save_all_dirty_buffers(app);
                        allow_immediate_close_without_checking_for_changes = true;
                        do_exit = true;
                    }break;
                    }    
                }
                
            }
        }
        if (do_exit){
            hard_exit(app);
        }
    }
}

function void
luis_render_caller(Application_Links *app, Frame_Info frame_info, View_ID view_id) {
    ProfileScope(app, "default render caller");
    View_ID active_view = get_active_view(app, Access_Always);
    b32 is_active_view = (active_view == view_id);
    
    Buffer_ID buffer = view_get_buffer(app, view_id, Access_Always);
    Face_ID face_id = get_face_id(app, buffer);
    Face_Metrics face_metrics = get_face_metrics(app, face_id);
    f32 line_height = face_metrics.line_height;
    f32 digit_advance = face_metrics.decimal_digit_advance;
    
    #if 0 // original way of laying out view and clip rect
    Rect_f32 region = draw_background_and_margin(app, view_id, is_active_view);
    Rect_f32 prev_clip = draw_set_clip(app, region);
    #else // this takes into account the custom lister rendering stuff
    
    Rect_f32 view_rect = view_get_screen_rect(app, view_id);
    Rect_f32 prev_clip = draw_set_clip(app, view_rect); //this is what BYP does
    Rect_f32 global_rect = global_get_screen_rectangle(app);


    if (luis_custom_lister_is_being_drawn) {
        view_rect.y1 = Min(view_rect.y1, luis_lister_rect_top_y);
    } 
    
    f32 width = 3.0f;
    Rect_f32 region = rect_inner(view_rect, width);
    draw_rectangle_fcolor(app, region, 0.f, fcolor_id(defcolor_back));
    
    if (width > 0.f) {
        FColor margin_color = get_panel_margin_color(is_active_view?UIHighlight_Active:UIHighlight_None);
        draw_margin(app, view_rect, region, margin_color);
    }
    
    
    //Rect_f32 prev_clip = draw_set_clip(app, view_rect);
    #endif
    
    
    // NOTE(allen): file bar
    b64 showing_file_bar = false;
    if (view_get_setting(app, view_id, ViewSetting_ShowFileBar, &showing_file_bar) && showing_file_bar){
        Rect_f32_Pair pair = layout_file_bar_on_top(region, line_height);
        draw_file_bar(app, view_id, buffer, face_id, pair.min);
        region = pair.max;
        draw_set_clip(app, region); //otherwise drop shadow clips to filebar
    }
    
    #if 0
    draw_set_clip(app, region);

	// Draw borders
	if (region.x0 > global_rect.x0){
		Rect_f32_Pair border_pair = rect_split_left_right(region, 2.f);
		draw_rectangle_fcolor(app, border_pair.a, 0.f, fcolor_id(defcolor_margin));
		region = border_pair.b;
	}
	if (region.x1 < global_rect.x1){
		Rect_f32_Pair border_pair = rect_split_left_right_neg(region, 2.f);
		draw_rectangle_fcolor(app, border_pair.b, 0.f, fcolor_id(defcolor_margin));
		region = border_pair.a;
	}
	region.y0 += 3.f;
    #endif
    
    Buffer_Scroll scroll = view_get_buffer_scroll(app, view_id);
    
    Buffer_Point_Delta_Result delta = delta_apply(app, view_id,
                                                  frame_info.animation_dt, scroll);
    if (!block_match_struct(&scroll.position, &delta.point)){
        block_copy_struct(&scroll.position, &delta.point);
        view_set_buffer_scroll(app, view_id, scroll, SetBufferScroll_NoCursorChange);
    }
    if (delta.still_animating){
        animate_in_n_milliseconds(app, 0);
    }
    
    // NOTE(allen): query bars
    region = default_draw_query_bars(app, region, view_id, face_id);
    
    // NOTE(allen): FPS hud
    if (show_fps_hud){
        Rect_f32_Pair pair = layout_fps_hud_on_bottom(region, line_height);
        draw_fps_hud(app, frame_info, face_id, pair.max);
        region = pair.min;
        animate_in_n_milliseconds(app, 1000);
    }
    
    // NOTE(allen): layout line numbers
    b32 show_line_number_margins = def_get_config_b32(vars_save_string_lit("show_line_number_margins"));
    Rect_f32 line_number_rect = {};
    if (show_line_number_margins){
        Rect_f32_Pair pair = layout_line_number_margin(app, buffer, region, digit_advance);
        line_number_rect = pair.min;
        region = pair.max;
    }
    
    // NOTE(allen): begin buffer render
    Buffer_Point buffer_point = scroll.position;
    Text_Layout_ID text_layout_id = text_layout_create(app, buffer, region, buffer_point);
    
    // NOTE(allen): draw line numbers
    if (show_line_number_margins){
        draw_line_number_margin(app, view_id, buffer, face_id, text_layout_id, line_number_rect);
    }
    
    if (g_byp_drop_shadow) {
		Buffer_Point shadow_point = buffer_point;
		Face_Description desc = get_face_description(app, face_id);
		shadow_point.pixel_shift -= Max((f32(desc.parameters.pt_size) / 8), 1.f)*V2f32(1.f, 1.f);
		Text_Layout_ID shadow_layout_id = text_layout_create(app, buffer, region, shadow_point);
		paint_text_color(app, shadow_layout_id, text_layout_get_visible_range(app, text_layout_id), 0xBB000000);
		draw_text_layout_default(app, shadow_layout_id);
		text_layout_free(app, shadow_layout_id);
    }
    
    // NOTE(allen): draw the buffer
    luis_render_buffer(app, view_id, face_id, buffer, text_layout_id, region);
    
    text_layout_free(app, text_layout_id);
    draw_set_clip(app, prev_clip);
}




function void
luis_whole_screen_render_caller(Application_Links *app, Frame_Info frame_info) {
    
    Rect_f32 region = global_get_screen_rectangle(app);
    Face_ID face_id = get_face_id(app, 0);
    Face_Metrics face_metrics = get_face_metrics(app, face_id);
    // f32 line_height = face_metrics.line_height;
    
    View_ID view = get_active_view(app, Access_Always);
    // Buffer_ID buffer = view_get_buffer(app, view, Access_Always);

    i64 cursor_pos  = view_get_cursor_pos(app, view);
    Buffer_Cursor cursor = view_compute_cursor(app, view, seek_pos(cursor_pos));

    #if 0 // this draws a file bar at the bottom
    {
        Scratch_Block scratch(app);
        //draw_file_bar(app, view, buffer, face_id, pair.min);
        
        Rect_f32_Pair pair = layout_file_bar_on_bot(region, line_height);
        draw_rectangle_fcolor(app, pair.max, 0.5f, fcolor_id(defcolor_bar));
        
        FColor base_color = fcolor_id(defcolor_base);
        FColor pop2_color = fcolor_id(defcolor_pop2);
        
        Fancy_Line list = {};
        
        String_Const_u8 file_name = push_buffer_file_name(app, scratch, buffer);
        String_Const_u8 extension = string_file_extension(file_name);
        
        //Token_Array tokens = get_token_array_from_buffer(app, buffer);
        //Code_Index_File *code_index_file = code_index_get_file(buffer);
        
        if (minibar_string.size > 0) {
            push_fancy_stringf(scratch, &list, base_color, "%.*s  ", string_expand(minibar_string)); //makes it in reverse order ezSadge
        }
        else {
            Scope_Prefix prefix = get_entire_scope_prefix(app, scratch, buffer, cursor_pos);
            if (prefix.string.size > 0) {
                push_fancy_stringf(scratch, &list, base_color, "%.*s  ", string_expand(prefix.string)); //makes it in reverse order ezSadge    
            }    
        }
        
        
                
        
        
        
        String_Const_u8 unique_name = push_buffer_unique_name(app, scratch, buffer);
        push_fancy_string(scratch, &list, base_color, unique_name);
        push_fancy_stringf(scratch, &list, base_color, " (%lld,%lld)", cursor.col, cursor.line);
        
        //extra stuff
        {
            Managed_Scope scope = buffer_get_managed_scope(app, buffer);
            Line_Ending_Kind *eol = scope_attachment(app, scope, buffer_eol_setting,
                                                     Line_Ending_Kind);
            if      (*eol == LineEndingKind_Binary) push_fancy_string(scratch, &list, base_color, string_u8_litexpr(" bin"));
            else if (*eol == LineEndingKind_LF)     push_fancy_string(scratch, &list, base_color, string_u8_litexpr(" lf"));
            else if (*eol == LineEndingKind_CRLF)   push_fancy_string(scratch, &list, base_color, string_u8_litexpr(" crlf"));
            
            {
                u8 space[3];
                String_u8 str = Su8(space, 0, 3);
                
                Dirty_State dirty = buffer_get_dirty_state(app, buffer);
                
                if (dirty != 0) string_append(&str, string_u8_litexpr(" "));
                if (HasFlag(dirty, DirtyState_UnsavedChanges))
                    string_append(&str, string_u8_litexpr("*"));
                if (HasFlag(dirty, DirtyState_UnloadedChanges))
                    string_append(&str, string_u8_litexpr("!"));
                push_fancy_string(scratch, &list, pop2_color, str.string);
            }
        }
        
        {
            Vec2_f32 p = pair.max.p0 + V2f32(2.f, 2.f);
            draw_fancy_line(app, face_id, fcolor_zero(), &list, p);    
        }
        //region = pair.max;
    }
    #endif


    
#if 0
    Rect_f32 region = global_get_screen_rectangle(app);
    Vec2_f32 center = rect_center(region);
    
    Face_ID face_id = get_face_id(app, 0);
    Scratch_Block scratch(app);
    draw_string_oriented(app, face_id, finalize_color(defcolor_text_default, 0),
                         SCu8("Hello, World!"), center - V2f32(200.f, 300.f),
                         0, V2f32(0.f, -1.f));
    draw_string_oriented(app, face_id, finalize_color(defcolor_text_default, 0),
                         SCu8("Hello, World!"), center - V2f32(240.f, 300.f),
                         0, V2f32(0.f, 1.f));
    draw_string_oriented(app, face_id, finalize_color(defcolor_text_default, 0),
                         SCu8("Hello, World!"), center - V2f32(400.f, 400.f),
                         0, V2f32(-1.f, 0.f));
    draw_string_oriented(app, face_id, finalize_color(defcolor_text_default, 0),
                         SCu8("Hello, World!"), center - V2f32(400.f, -100.f),
                         0, V2f32(cos_f32(pi_f32*.333f), sin_f32(pi_f32*.333f)));
#endif
}

BUFFER_HOOK_SIG(luis_begin_buffer){
    ProfileScope(app, "begin buffer");
    
    Scratch_Block scratch(app);
    
    b32 treat_as_code = false;
    String_Const_u8 file_name = push_buffer_file_name(app, scratch, buffer_id);
    if (file_name.size > 0){
        String_Const_u8 treat_as_code_string = def_get_config_string(scratch, vars_save_string_lit("treat_as_code"));
        String_Const_u8_Array extensions = parse_extension_line_to_extension_list(app, scratch, treat_as_code_string);
        String_Const_u8 ext = string_file_extension(file_name);
        for (i32 i = 0; i < extensions.count; ++i){
            if (string_match(ext, extensions.strings[i])){
                
                if (string_match(ext, string_u8_litexpr("cpp")) ||
                    string_match(ext, string_u8_litexpr("h")) ||
                    string_match(ext, string_u8_litexpr("c")) ||
                    string_match(ext, string_u8_litexpr("hpp")) ||
                    string_match(ext, string_u8_litexpr("cc"))){
                    treat_as_code = true;
                }
                break;
            }
        }
    }
    
    String_ID file_map_id = vars_save_string_lit("keys_file");
    String_ID code_map_id = vars_save_string_lit("keys_code");
    
    Command_Map_ID map_id = (treat_as_code)?(code_map_id):(file_map_id);
    Managed_Scope scope = buffer_get_managed_scope(app, buffer_id);
    Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID);
    *map_id_ptr = map_id;
    
    Line_Ending_Kind setting = guess_line_ending_kind_from_buffer(app, buffer_id);
    Line_Ending_Kind *eol_setting = scope_attachment(app, scope, buffer_eol_setting, Line_Ending_Kind);
    *eol_setting = setting;
    
    // NOTE(allen): Decide buffer settings
    b32 wrap_lines = true;
    b32 use_lexer = false;
    if (treat_as_code){
        wrap_lines = def_get_config_b32(vars_save_string_lit("enable_code_wrapping"));
        use_lexer = true;
    }
    
    String_Const_u8 buffer_name = push_buffer_base_name(app, scratch, buffer_id);
    if (buffer_name.size > 0 && buffer_name.str[0] == '*' && buffer_name.str[buffer_name.size - 1] == '*'){
        wrap_lines = def_get_config_b32(vars_save_string_lit("enable_output_wrapping"));
    }
    
    if (use_lexer){
        ProfileBlock(app, "begin buffer kick off lexer");
        Async_Task *lex_task_ptr = scope_attachment(app, scope, buffer_lex_task, Async_Task);
        *lex_task_ptr = async_task_no_dep(&global_async_system, do_full_lex_async, make_data_struct(&buffer_id));
    }
    
    {
        b32 *wrap_lines_ptr = scope_attachment(app, scope, buffer_wrap_lines, b32);
        *wrap_lines_ptr = wrap_lines;
    }
    
    if (use_lexer){
        buffer_set_layout(app, buffer_id, layout_virt_indent_index_generic);
    }
    else{
        if (treat_as_code){
            buffer_set_layout(app, buffer_id, layout_virt_indent_literal_generic);
        }
        else{
            buffer_set_layout(app, buffer_id, layout_generic);
        }
    }
    
    // no meaning for return
    
    //NOTE(luis) added this
    if (string_match(buffer_name, string_u8_litexpr("project.4coder"))) {
        load_project(app);
    }
    return(0);
}

BUFFER_HOOK_SIG(luis_new_file) {
    Scratch_Block scratch(app);
    String_Const_u8 file_name = push_buffer_base_name(app, scratch, buffer_id);
    if (!string_match(string_postfix(file_name, 2), string_u8_litexpr(".h"))) {
        return(0);
    }
    
    List_String_Const_u8 guard_list = {};
    for (u64 i = 0; i < file_name.size; ++i){
        u8 c[2] = {};
        u64 c_size = 1;
        u8 ch = file_name.str[i];
        if ('A' <= ch && ch <= 'Z'){
            c_size = 2;
            c[0] = '_';
            c[1] = ch;
        }
        else if ('0' <= ch && ch <= '9'){
            c[0] = ch;
        }
        else if ('a' <= ch && ch <= 'z'){
            c[0] = ch - ('a' - 'A');
        }
        else{
            c[0] = '_';
        }
        String_Const_u8 part = push_string_copy(scratch, SCu8(c, c_size));
        string_list_push(scratch, &guard_list, part);
    }
    String_Const_u8 guard = string_list_flatten(scratch, guard_list);
    
    Date_Time date_time = system_now_date_time_universal();
    date_time = system_local_date_time_from_universal(&date_time);
    //String_Const_u8 date_string = date_time_format(scratch, "month day yyyy h:mimi ampm", &date_time);
    String_Const_u8 date_string = date_time_format(scratch, "month day yyyy", &date_time);
    
    Buffer_Insertion insert = begin_buffer_insertion_at_buffered(app, buffer_id, 0, scratch, KB(16));
    insertf(&insert,
            "#ifndef %.*s // Created on %.*s \n"
            "#define %.*s\n"
            "\n"
            "#endif //%.*s\n",
            string_expand(guard),
            string_expand(date_string),
            string_expand(guard),
            string_expand(guard));
    end_buffer_insertion(&insert);
    
    return(0);
}

function Rect_f32
luis_buffer_region(Application_Links *app, View_ID view_id, Rect_f32 region){
    Buffer_ID buffer = view_get_buffer(app, view_id, Access_Always);
    Face_ID face_id = get_face_id(app, buffer);
    Face_Metrics metrics = get_face_metrics(app, face_id);
    f32 line_height = metrics.line_height;
    f32 digit_advance = metrics.decimal_digit_advance;
    
    // NOTE(allen): margins
    region = rect_inner(region, 3.f);
    
    // NOTE(allen): file bar
    b64 showing_file_bar = false;
    if (view_get_setting(app, view_id, ViewSetting_ShowFileBar, &showing_file_bar) &&
        showing_file_bar){
        Rect_f32_Pair pair = layout_file_bar_on_top(region, line_height);
        region = pair.max;
    }
    
    // NOTE(allen): query bars
    {
        Query_Bar *space[32];
        Query_Bar_Ptr_Array query_bars = {};
        query_bars.ptrs = space;
        if (get_active_query_bars(app, view_id, ArrayCount(space), &query_bars)){
            Rect_f32_Pair pair = layout_query_bar_on_top(region, line_height, query_bars.count);
            region = pair.max;
        }
    }
    
    // NOTE(allen): FPS hud
    if (show_fps_hud){
        Rect_f32_Pair pair = layout_fps_hud_on_bottom(region, line_height);
        region = pair.min;
    }
    
    // NOTE(allen): line numbers
    b32 show_line_number_margins = def_get_config_b32(vars_save_string_lit("show_line_number_margins"));
    if (show_line_number_margins){
        Rect_f32_Pair pair = layout_line_number_margin(app, buffer, region, digit_advance);
        region = pair.max;
    }


    #if 0
    if (1) { //NOTE this is the VIM style status bar on bottom of screen
        Rect_f32_Pair pair = layout_file_bar_on_bot(region, line_height);
        region = pair.min;
    }
    #endif
    
    return(region);
}
