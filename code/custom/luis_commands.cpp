
internal void
add_nest_to_lister(Application_Links *app, Arena *temp, Lister_Block *lister, Lister_Prealloced_String status, Code_Index_Nest *nest) {
    if (nest->text.size == 0) return;
    
    /*View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    i64 linenum = get_line_number_from_pos(app, buffer, nest->open.start);
    String_Const_u8 line = push_buffer_line(app, temp, buffer, linenum); */
    
    String_u8 string = string_u8_push(temp, 512);
    
    i32  parent_count = 0;
    i32  max_parent_count = 12;
    Code_Index_Nest **parents = push_array_zero(temp, Code_Index_Nest *, max_parent_count);
    
    if (parents) for (Code_Index_Nest *parent = nest->parent; parent && parent->text.size > 0; parent = parent->parent) {
        if (parent_count < max_parent_count) {
            parents[parent_count++] = parent;
        }
        else break;
    }
    
    for (i32 i = parent_count - 1; i >= 0; i -= 1) {
        string_append(&string, parents[i]->text);
        string_append(&string, SCu8("::"));
    }
    
    Lister_Prealloced_String search_string = {};
    if (nest->flags & CODE_INDEX_NEST_IS_STRUCT) {
        search_string = lister_prealloced(push_stringf(lister->lister.current->arena, "struct %.*s%.*s", string_expand(string), string_expand(nest->text)));
    } 
    else if (nest->flags & CODE_INDEX_NEST_IS_UNION) {
        search_string = lister_prealloced(push_stringf(lister->lister.current->arena, "union %.*s%.*s", string_expand(string), string_expand(nest->text)));
    }
    else if (nest->flags & CODE_INDEX_NEST_IS_NAMESPACE) {
        search_string = lister_prealloced(push_stringf(lister->lister.current->arena, "namespace %.*s%.*s", string_expand(string), string_expand(nest->text)));
    }
    else if (nest->flags & CODE_INDEX_NEST_IS_FUNCTION) {
        search_string = lister_prealloced(push_stringf(lister->lister.current->arena, "proc %.*s%.*s", string_expand(string), string_expand(nest->text)));    
    }
    
    lister_add_item(lister->lister.current, search_string, status, (void*)nest, 0);
    
    
        //don't like how this is recursive but we shouldn't have too many levels of nests in principle
    for (i32 i = 0; i < nest->nest_array.count; i += 1) {
        Code_Index_Nest *subnest = nest->nest_array.ptrs[i];
        //NOTE(luis) I don't understand why we get a null pointer in this array sometimes... it's not a good sign honestly
        if (subnest && subnest->text.size > 0) {
            add_nest_to_lister(app, temp, lister, status, subnest);
        }
    }
}

CUSTOM_COMMAND_SIG(luis_list_nests_this_buffer) 
CUSTOM_DOC("Show code nests for this buffer") {
    Scratch_Block scratch(app);
    Lister_Block lister(app, scratch);
    lister_set_query(lister, "Code Index:");
    lister_set_default_handlers(lister);
    
    code_index_lock();
    
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    Code_Index_File *file = code_index_get_file(buffer);
    if(!file) return; //continue;
    
    Lister_Prealloced_String status  = lister_prealloced(push_buffer_base_name(app, lister.lister.current->arena, buffer));
    
    
    
    for (i32 nest_index = 0; nest_index < file->nest_array.count; nest_index += 1) {
        Code_Index_Nest *nest = file->nest_array.ptrs[nest_index];
        
        
        
        if (nest->text.size == 0) continue; //only nests with names can be printed
        
        Scratch_Block temp(app, scratch);
        add_nest_to_lister(app, temp, &lister, status, nest);
    }
    
    code_index_unlock();
    
    Lister_Result l_result = run_lister(app, lister);
    if (!l_result.canceled && l_result.user_data != 0) {
        Code_Index_Nest *nest = (Code_Index_Nest *)l_result.user_data;
        view_set_buffer(app, view, nest->file->buffer, 0);
        view_set_cursor_and_preferred_x(app, view, seek_pos(nest->open.first));
    }
}



CUSTOM_COMMAND_SIG(luis_list_notes_this_buffer)
CUSTOM_DOC("Show code indexes for buffer")
{
    Scratch_Block scratch(app);
    Lister_Block lister(app, scratch);
    lister_set_query(lister, "Code Index:");
    lister_set_default_handlers(lister);
    
    code_index_lock();
    /*for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always);
         buffer != 0;
         buffer = get_buffer_next(app, buffer, Access_Always)) */
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    {
        Code_Index_File *file = code_index_get_file(buffer);
        if(!file) return; //continue;
        
        Lister_Prealloced_String status  = lister_prealloced(push_buffer_base_name(app, lister.lister.current->arena, buffer));
        for(i32 note_index = 0; note_index < file->note_array.count; note_index += 1)
        {
            Code_Index_Note *note = file->note_array.ptrs[note_index];
            if (note->note_kind == CodeIndexNote_Function ||
                note->note_kind == CodeIndexNote_Type     ||
                note->note_kind == CodeIndexNote_Macro)
            {
                //Lister_Prealloced_String status = {};
                Lister_Prealloced_String search_string;
                if(note->note_kind == CodeIndexNote_Function)
                    search_string = lister_prealloced(push_stringf(lister.lister.current->arena, "%.*s()", string_expand(note->text)));
                else if(note->note_kind == CodeIndexNote_Macro)
                    search_string = lister_prealloced(push_stringf(lister.lister.current->arena, "# %.*s", string_expand(note->text)));
                else {
                    if (note->parent && note->parent->text.size > 0) { //big list of parent::child::child2::child3 stuff
                        Scratch_Block temp(app, scratch);
                        
                        String_u8 string = string_u8_push(temp, 512);
                        
                        i32  parent_count = 0;
                        i32  max_parent_count = 12;
                        Code_Index_Nest **parents = push_array_zero(temp, Code_Index_Nest *, max_parent_count);
                        
                        if (parents) for (Code_Index_Nest *parent = note->parent; parent && parent->text.size > 0; parent = parent->parent) {
                            if (parent_count < max_parent_count) {
                                parents[parent_count++] = parent;
                            }
                            else break;
                        }
                        
                        for (i32 i = parent_count - 1; i >= 0; i -= 1) {
                            Code_Index_Nest *nest = parents[i];
                            string_append(&string, nest->text);
                            string_append(&string, SCu8("::"));
                        }
                        string_append(&string, note->text);
                        
                        
                        search_string = lister_prealloced(push_string_copy(lister.lister.current->arena, SCu8(string)));
                        /*
                        search_string = lister_prealloced(push_stringf(lister.lister.current->arena, "%.*s::%.*s",
                                                                       string_expand(note->parent->text),
                                                                       string_expand(note->text))); */
                    } 
                    else { //fast path for top level stuff
                        search_string = lister_prealloced(push_string_copy(lister.lister.current->arena, note->text));    
                    }
                }
                
                
                lister_add_item(lister, search_string, status, (void*)note, 0);
            }
            
            //if (note->nest) {
                
            //}
        }
    }
    code_index_unlock();
    
    
    //Tiny_Jump result = {};
    Lister_Result l_result = run_lister(app, lister);
    if (!l_result.canceled && l_result.user_data != 0){
        //block_copy_struct(&result, (Tiny_Jump*)l_result.user_data);
        
        //View_ID view = get_active_view(app, Access_Always);
        //Buffer_ID init_buffer = view_get_buffer(app, view, Access_Always);
        //i64 init_pos = view_get_cursor_pos(app, view);
        
        //view_set_current_buffer_location(app, view, init_buffer, init_pos);
        Code_Index_Note *note = (Code_Index_Note *)l_result.user_data;
        view_set_buffer(app, view, note->file->buffer, 0);
        view_set_cursor_and_preferred_x(app, view, seek_pos(note->pos.first));
        //luis_center_view_top(app);
    }
    
    //if (result.buffer != 0){
        //View_ID view = get_this_ctx_view(app, Access_Always);
        //point_stack_push_view_cursor(app, view);
        //jump_to_location(app, view, result.buffer, result.pos);
    //}
}

#if 0

internal void
luis_offset_code_index(Application_Links *app, i32 offset) {
    View_ID view = get_active_view(app, Access_Always);
    //Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    //i64 cursor_pos = view_get_cursor_pos(app, view);
    
    Scratch_Block scratch(app);
    String_Const_u8 identifier = push_token_or_word_under_active_cursor(app, scratch);
    //String_Const_u8 scope_string = get_entire_scope_parents_at_pos(app, scratch, buffer, cursor_pos);
    
    Peek_Code_Index_State *state = luis_get_code_peek_state(app, view, identifier);
    if (!state) return;
    
    i32 new_index = state->index + offset;
    if (new_index < 0)	new_index = 0;
    
    i32 max_note_index = 0;
    Code_Index_Note *note = 0;
    Code_Index_Note *last_note = 0;
    for (Code_Index_Note *n = state->first_note; n; n = n->next_in_hash) {
        if (string_match(n->text, identifier)) {
            
            //String_Const_u8 note_scope_string = get_entire_scope_parents_at_pos(app, scratch, note->file->buffer, note->pos.min);
            //if (string_match(note_scope_string, scope_string)) 
            {
                if(max_note_index == new_index)
                    note = n;
                
                max_note_index += 1;
                last_note = n;    
            }
        }
    }
    
    if(new_index >= max_note_index)
        new_index = max_note_index - 1;
    
    if(!note)
        note = last_note;
    
    if(note) {
        View_ID peek = luis_get_or_split_peek_window(app, view, ViewSplit_Bottom);
        if(peek) {
            view_set_active(app, peek);
            view_set_buffer(app, peek, note->file->buffer, 0);
            view_set_cursor_and_preferred_x(app, peek, seek_pos(note->pos.first));
            view_set_mark(app, peek, seek_pos(note->pos.first));
            luis_center_view_top(app);
            
            state->index = new_index;
        }   
    }
}

CUSTOM_COMMAND_SIG(luis_code_index_prev)
CUSTOM_DOC("prev code index") {
    luis_offset_code_index(app, -1);
}

CUSTOM_COMMAND_SIG(luis_code_index_next)
CUSTOM_DOC("prev code index") {
    luis_offset_code_index(app, 1);
}
#endif

function String_Const_u8
get_entire_scope_parents_at_pos(Application_Links *app, Arena *arena, Buffer_ID buffer, i64 pos);

function Peek_Code_Index_State *
get_code_peek_state(Application_Links *app, View_ID view, i64 pos, View_ID *peek) {
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    *peek = luis_get_or_split_peek_window(app, view, ViewSplit_Bottom);
    
    Managed_Scope peek_scope = view_get_managed_scope(app, *peek);
    Peek_Code_Index_State *state = scope_attachment(app, peek_scope, view_code_peek_state, Peek_Code_Index_State);
    if (!state) return 0;
    
    
    Scratch_Block scratch(app);
    String_Const_u8 identifier = push_token_or_word_under_pos(app, scratch, buffer, pos);
    if (identifier.size == 0) return 0;
    
    state->is_first_time_getting = false;
    
    if (state->initialized) { //check if user moved onto another word. If so just mark uninitialized and check next if statement
        if (!string_match(identifier, SCu8(state->identifier))) {
            state->initialized = false;
            
            state->index = 0;
            state->note_count = 0;
        }
    }
    
    if (!state->initialized) {
        state->initialized = true;
        state->is_first_time_getting = true;
        
        state->identifier.str  = state->identifier_buffer;
        state->identifier.size = 0;
        state->identifier.cap  = ArrayCount(state->identifier_buffer);
        string_append(&state->identifier, identifier);
        
        
        String_Const_u8 scope_prefix = get_entire_scope_parents_at_pos(app, scratch, buffer, pos);
        i32 scope_prefix_scope_count = 0;
        for (i32 i = 0; i < scope_prefix.size; i += 1) {
            if (i > 0 && scope_prefix.str[i-1] == ':' && scope_prefix.str[i] == ':') {
                scope_prefix_scope_count += 1;
            } 
        }
        
        auto get_match_metric = [&scope_prefix, scope_prefix_scope_count](String_Const_u8 prefix) {
            i32 search_length = (i32)Min(prefix.size, scope_prefix.size);
            i32 match_count = 0;
            for (i32 i = 0; i < search_length; i += 1) {
                if (prefix.str[i] == scope_prefix.str[i]) {
                    if (i > 0 && prefix.str[i-1] == ':' && prefix.str[i] == ':') {
                        match_count += 1;
                    }
                }
                else break;
            }
            
            i32 non_match_count = scope_prefix_scope_count - match_count;
            return match_count - 2*non_match_count;
        };
        
        Code_Index_Note *first_note = code_index_note_from_string(identifier);
        for (Code_Index_Note *note = first_note; note; note = note->next_in_hash) {
            if (state->note_count == countof(state->notes)) break;
            
            //ignore type and function decls
            if (note->note_kind == CodeIndexNote_Type_Definition     ||
                note->note_kind == CodeIndexNote_Function_Definition ||
                note->note_kind == CodeIndexNote_Macro               ||
                note->note_kind == CodeIndexNote_Namespace) 
            {    
                if (string_match(note->text, identifier)) 
                {
                    //the higher the number, the better the result will be
                    
                    
                    state->notes[state->note_count++] = note;
                    String_Const_u8 note_prefix = get_entire_scope_parents_at_pos(app, scratch, note->file->buffer, note->pos.min);
                    i32 note_metric = get_match_metric(note_prefix);
                    
                	//add to array already sorted
                    for (i32 i = state->note_count - 2; i >= 0; i -= 1) {
                        Code_Index_Note *prev = state->notes[i];
                        Assert (state->notes[i+1] == note); //NOTE (i+1) should always be our note
                        
                        String_Const_u8 prev_prefix = get_entire_scope_parents_at_pos(app, scratch, prev->file->buffer, prev->pos.min);
                        i32 prev_metric = get_match_metric(prev_prefix);
                        
                        if (prev_metric < note_metric) {
                            SWAP(state->notes[i], state->notes[i+1]); 
                        }
                        else break; //we are ordered nothing else to do
                    }
                }    
            }
            
        }
        
        #if 0
        int k = 0;
        
        for (i32 i = 0; i < state->note_count; i += 1) {
            Code_Index_Note *note = state->notes[i];
            Buffer_ID note_buffer = note->file->buffer;
            
            String_Const_u8 note_prefix = get_entire_scope_parents_at_pos(app, scratch, note->file->buffer, note->pos.min);
            print_message(app, note_prefix);
            print_message(app, note->text);
            print_message(app, SCu8(" @ "));
            
            String_Const_u8 filepath = push_buffer_file_name(app, scratch, note_buffer);
            print_message(app, filepath);
            print_message(app, SCu8("\n\n"));
            
            k += 1;
        }
        
         
        k -= 1;
        #endif
    }
    return state;
}


CUSTOM_COMMAND_SIG(luis_code_index_prev)
CUSTOM_DOC("prev code index") {
    View_ID view = get_active_view(app, Access_Always);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    View_ID peek;
    Peek_Code_Index_State *state = get_code_peek_state(app, view, cursor_pos, &peek);
    if (state && state->note_count > 0) {
        if (!state->is_first_time_getting) {
            state->index = Max(state->index - 1, 0);    
        }
        Code_Index_Note *note = state->notes[state->index];
        
        
        view_set_active(app, peek);
        view_set_buffer(app, peek, note->file->buffer, 0);
        view_set_cursor_and_preferred_x(app, peek, seek_pos(note->pos.first));
        view_set_mark(app, peek, seek_pos(note->pos.first));
        luis_center_view_top(app);
    }
}

CUSTOM_COMMAND_SIG(luis_code_index_next)
CUSTOM_DOC("prev code index") {
    View_ID view = get_active_view(app, Access_Always);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    View_ID peek;
    Peek_Code_Index_State *state = get_code_peek_state(app, view, cursor_pos, &peek);
    if (state && state->note_count > 0) {
        
        if (!state->is_first_time_getting) {
            state->index = Min(state->index + 1, state->note_count - 1);    
        }
        Code_Index_Note *note = state->notes[state->index];
        
        
        view_set_active(app, peek);
        view_set_buffer(app, peek, note->file->buffer, 0);
        view_set_cursor_and_preferred_x(app, peek, seek_pos(note->pos.first));
        view_set_mark(app, peek, seek_pos(note->pos.first));
        luis_center_view_top(app);
    }
} 

