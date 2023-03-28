
//stolen from BYP, pretty cool!
CUSTOM_COMMAND_SIG(show_file_in_explorer)
CUSTOM_DOC("Opens file explorer in hot directory") {
    Scratch_Block scratch(app);
    #if 0 //BYP version
	String_Const_u8 hot = push_hot_directory(app, scratch);
    #else //mine uses file hot directory
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer_id = view_get_buffer(app, view, Access_Always);
    String_Const_u8 hot = get_directory_for_buffer(app, scratch, buffer_id);
    if (hot.size == 0) {
        hot = push_hot_directory(app, scratch); //use current hot directory if looking at *scratch* for example
    }
    
    #endif
    exec_system_command(app, 0, buffer_identifier(0), hot, string_u8_litexpr("explorer ."), 0); /////////////////////////////////////////////////////////////////////////////////////////
    
}

CUSTOM_COMMAND_SIG(open_file_in_visual_studio)
CUSTOM_DOC("Open current file in visual studio") {
    Scratch_Block scratch(app);
    
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer_id = view_get_buffer(app, view, Access_Always);
    i64 linenum = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), view_get_cursor_pos(app, view));
    
    String_Const_u8 hot = get_directory_for_buffer(app, scratch, buffer_id);
    if (hot.size == 0) {
        hot = push_hot_directory(app, scratch); //use current hot directory if looking at *scratch* for example
    }
    
    String_Const_u8 file_name = push_buffer_file_name(app, scratch, buffer_id);
    String_Const_u8 command = push_stringf(scratch, "devenv /edit %.*s", string_expand(file_name));
    
    if (exec_system_command(app, 0, buffer_identifier(0), hot, command, 0)) {
        String_Const_u8 linenum_string = string_from_integer(scratch, linenum, 10);
        if (linenum_string.size > 0) {
            clipboard_post(0, linenum_string);
        }
    }
}

CUSTOM_COMMAND_SIG(luis_escape)
CUSTOM_DOC("escape key")
{
    //does nothing, hanlded by view_input_handler
}

CUSTOM_COMMAND_SIG(luis_toggle_modal_mode)
CUSTOM_DOC("Toggles modal mode") {	
	IN_MODAL_MODE = !IN_MODAL_MODE;
    
    View_ID view        = get_active_view(app, Access_Always);
    Buffer_ID buffer_id = view_get_buffer(app, view, Access_Always);
    update_buffer_bindings_for_modal_toggling(app, buffer_id);
}


CUSTOM_COMMAND_SIG(luis_interactive_open_or_new)  
CUSTOM_DOC("open in new in same tab") {	
    //my_interactive_open_or_new_internal(app, false);
	View_ID view = get_active_view(app, Access_Always);
    
    Scratch_Block scratch(app);
    
    Buffer_ID buffer_id = view_get_buffer(app, view, Access_Always);
    String_Const_u8 directory = get_directory_for_buffer(app, scratch, buffer_id);
    if(directory.size) set_hot_directory(app, directory);
    //tab_state_dont_peek_new_buffer(); 
    interactive_open_or_new(app);
}

CUSTOM_COMMAND_SIG(luis_home)
CUSTOM_DOC("go start of visual line") {
    View_ID view = get_active_view(app, Access_Always);
    i64 linenum = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), view_get_cursor_pos(app, view));
    Range_i64 range = get_visual_line_start_end_pos(app, view, linenum);
    view_set_cursor_and_preferred_x(app, view, seek_pos(range.min));
}

CUSTOM_COMMAND_SIG(luis_left_word)
CUSTOM_DOC("move left")
{
    Scratch_Block scratch(app);
    current_view_scan_move(app, Scan_Backward, push_boundary_list(scratch, boundary_alpha_numeric_underscore));
}

CUSTOM_COMMAND_SIG(luis_right_word)
CUSTOM_DOC("move right")
{
    Scratch_Block scratch(app);
    current_view_scan_move(app, Scan_Forward, push_boundary_list(scratch, boundary_alpha_numeric_underscore));
}


internal void
luis_set_mark(Application_Links *app, View_ID view, i64 pos) {
    luis_view_set_flags(app, view, VIEW_NOTEPAD_MODE_MARK_SET);
    view_set_mark(app, view, seek_pos(pos));
}

CUSTOM_COMMAND_SIG(luis_set_mark)
CUSTOM_DOC("set mark")
{
    View_ID view = get_active_view(app, Access_Always);
    luis_set_mark(app, view, view_get_cursor_pos(app, view));
}

internal b32
strmatch_so_far(String_Const_u8 a, String_Const_u8 b, i32 count)
{
    if(a.size >= count && b.size >= count)
    {
        for(i32 i = 0; i < count; i += 1)
        {
            if(a.str[i] != b.str[i])	return false;
        }
        return true;
    }
    else return false;  
}


CUSTOM_COMMAND_SIG(luis_scope_braces)
CUSTOM_DOC("writes {}")
{
    #if 1
    //write_text(app, SCu8("\n{\n\n}"));
    View_ID view = get_active_view(app, Access_ReadWriteVisible);
    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadWriteVisible);
    Scratch_Block scratch(app);
    i64 pos = view_get_cursor_pos(app, view);
    i64 linenum = get_line_number_from_pos(app, buffer, pos);
    String_Const_u8 line = push_buffer_line(app, scratch, buffer, linenum);
    line = string_skip_whitespace(line);
    
    
    History_Group hgroup = history_group_begin(app, buffer);
    //global_history_edit_group_begin(app);
    //NOTE I check for space after the keyword to ensure it's not a substring of a bigger word
    //this will miss if there's a newline right after it. The more correct way of doing this is with tokens
    String_Const_u8 string = {};; 
    if(strmatch_so_far(SCu8("struct "), line, 7) ||
        strmatch_so_far(SCu8("enum "),   line, 5) ||
        strmatch_so_far(SCu8("union "),  line, 6))
    {
        string.str = (u8 *)"{\n\n};";
        string.size = sizeof("{\n\n};") - 1;
        move_up(app);
    }
    else if(line.size == 0)
    {
        string.str = (u8 *)"{\n\n}";
        string.size = sizeof("{\n\n}") - 1;
        move_up(app);
    }
    else
    {
        string.str = (u8 *)"{\n\n}";
        string.size = sizeof("{\n\n}") - 1;
        move_up(app);
    }
    
    buffer_replace_range(app, buffer, Ii64(pos), string);
    auto_indent_buffer(app, buffer, Ii64_size(pos, string.size));
    move_vertical_lines(app, 2);
    auto_indent_line_at_cursor(app);
    history_group_end(hgroup);
    //global_history_edit_group_end(app);
    #endif  
}

CUSTOM_COMMAND_SIG(luis_indent_range)
CUSTOM_DOC("indent_range") {
    if(PREV_PASTE_INIT_CURSOR_POS > -1) {
        View_ID view = get_active_view(app, Access_ReadWriteVisible);
        Buffer_ID buffer = view_get_buffer(app, view, Access_ReadWriteVisible);
        Range_i64 range = Ii64(PREV_PASTE_INIT_CURSOR_POS, view_get_cursor_pos(app, view));
        if(range.min != range.max)
        {
            auto_indent_buffer(app, buffer, range);
            move_past_lead_whitespace(app, view, buffer);   
        }
    }
    else auto_indent_range(app);
}

CUSTOM_COMMAND_SIG(luis_add_tab)
CUSTOM_DOC("add tab to line start") {
    View_ID view = get_active_view(app, Access_ReadWriteVisible);
    if (!view) return;

    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 mark_pos   = view_get_mark_pos(app, view);
    
    i64 min = cursor_pos;
    i64 max = mark_pos;
    if (min > max) SWAP(min, max);
    
    i64 line_start = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), min);
    i64 line_end   = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), max);
    
    b32 indent_with_tabs = def_get_config_b32(vars_save_string_lit("indent_with_tabs"));
    i32 indent_width     = (i32)def_get_config_u64(app, vars_save_string_lit("indent_width"));
    
    u8 spaces[] = "                                                                         ";    
    String_Const_u8 insert;
    insert.str = spaces;
    insert.size = Min(indent_width, ArrayCount(spaces)-1);;
    if (indent_with_tabs) insert = SCu8("\t");
    
    for (i64 linenum = line_start; linenum <= line_end; linenum += 1) {
        i64 start_pos = get_line_start_pos(app, buffer, linenum);
        //write_text_at(app, insert, start_pos);
        buffer_replace_range(app, buffer, Ii64(start_pos), insert);
    }
}

CUSTOM_COMMAND_SIG(luis_add_space)
CUSTOM_DOC("add space to line start") {
    View_ID view = get_active_view(app, Access_ReadWriteVisible);
    if (!view) return;

    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 mark_pos   = view_get_mark_pos(app, view);
    
    i64 min = cursor_pos;
    i64 max = mark_pos;
    if (min > max) SWAP(min, max);
    
    i64 line_start = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), min);
    i64 line_end   = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), max);
    String_Const_u8 insert = SCu8(" ");
    
    for (i64 linenum = line_start; linenum <= line_end; linenum += 1) {
        i64 start_pos = get_line_start_pos(app, buffer, linenum);
        //write_text_at(app, insert, start_pos);
        buffer_replace_range(app, buffer, Ii64(start_pos), insert);
    }
}

CUSTOM_COMMAND_SIG(luis_remove_tab)
CUSTOM_DOC("remove tab from line start") {
    View_ID view = get_active_view(app, Access_ReadWriteVisible);
    if (!view) return;

    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 mark_pos   = view_get_mark_pos(app, view);
    
    i64 min = cursor_pos;
    i64 max = mark_pos;
    if (min > max) SWAP(min, max);
    
    i64 line_start = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), min);
    i64 line_end   = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), max);
    
    i32 indent_width     = (i32)def_get_config_u64(app, vars_save_string_lit("indent_width"));
    
    Scratch_Block scratch(app);
    for (i64 linenum = line_start; linenum <= line_end; linenum += 1) {
        String_Const_u8 line  = push_buffer_line(app, scratch, buffer, linenum);
        u64 at_non_whitespace = string_find_first_non_whitespace(line);
        
        i32 remove_type = 0;
        if ((at_non_whitespace > 0) && (at_non_whitespace < line.size)) {
            u8 ch = line.str[at_non_whitespace-1];
            if (ch == '\t') remove_type = -1;
            else if (ch == ' ') { //TODO make this remove other type of whitespace (tab) and add correct amount of spaces to even it out...
                for (i32 at = (i32)(at_non_whitespace-1); at >= 0; at -= 1) {
                    if (line.str[at] == ' ') remove_type += 1;
                    else break;
                    
                    if (remove_type == indent_width) break; //ate enough spaces to constitute a 'tab'
                }
            }
        }
        
        if (remove_type) {
            i64 line_start_pos = get_line_start_pos(app, buffer, linenum);
            i64 end = line_start_pos + at_non_whitespace - 1;
            Range_i64 range = {end, end + 1};
            if (remove_type > 0) { //remove spaces by count
                range.min -= (remove_type - 1);
            }
            buffer_replace_range(app, buffer, range, string_u8_empty);
        }
    }
}

CUSTOM_COMMAND_SIG(luis_remove_space)
CUSTOM_DOC("remove space from line start") {
    View_ID view = get_active_view(app, Access_ReadWriteVisible);
    if (!view) return;

    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 mark_pos   = view_get_mark_pos(app, view);
    
    i64 min = cursor_pos;
    i64 max = mark_pos;
    if (min > max) SWAP(min, max);
    
    i64 line_start = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), min);
    i64 line_end   = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), max);
    
    i32 indent_width     = (i32)def_get_config_u64(app, vars_save_string_lit("indent_width"));
    
    Scratch_Block scratch(app);
    for (i64 linenum = line_start; linenum <= line_end; linenum += 1) {
        String_Const_u8 line  = push_buffer_line(app, scratch, buffer, linenum);
        u64 at_non_whitespace = string_find_first_non_whitespace(line);
        
        i32 num_chars_to_remove = 0;
        i32 num_spaces_to_add = 0;
        if ((at_non_whitespace > 0) && (at_non_whitespace < line.size)) {
            u8 ch = line.str[at_non_whitespace-1];
            if (ch == '\t') {
                num_chars_to_remove = 1;
                num_spaces_to_add = Max(0, indent_width-1);
            } else if (ch == ' ') { 
                num_chars_to_remove = 1;
            }
        }
        
        if (num_chars_to_remove > 0) {
            i64 line_start_pos = get_line_start_pos(app, buffer, linenum);
            i64 end = line_start_pos + at_non_whitespace - 1;
            Range_i64 range;
            range.min = end - (num_chars_to_remove - 1);
            range.max = end + 1;
            
            String_Const_u8 insert = string_u8_empty;
            if (num_spaces_to_add > 0) {
                static u8 spaces[] = "                                                                         ";
                insert.str = spaces;
                insert.size = Min(num_spaces_to_add, ArrayCount(spaces)-1);    
            }
            
            buffer_replace_range(app, buffer, range, insert);
        }
    }
}

CUSTOM_COMMAND_SIG(luis_select_line)
CUSTOM_DOC("go end of visual line") {
    View_ID view = get_active_view(app, Access_Always);
    i64 linenum = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), view_get_cursor_pos(app, view));
    Range_i64 range = get_visual_line_start_end_pos(app, view, linenum);
    luis_set_mark(app, view, range.min);
    view_set_cursor_and_preferred_x(app, view, seek_pos(range.max));
}

CUSTOM_COMMAND_SIG(luis_cut_line) 
CUSTOM_DOC("Emacs-style kill line") {
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer_id = view_get_buffer(app, view, Access_Always);
    i64 pos     = view_get_cursor_pos(app, view);
    i64 linenum = get_line_number_from_pos(app, buffer_id, pos);
    i64 end     = get_line_end_pos(app, buffer_id, linenum);
    
    if (end > pos) {
        view_set_mark(app, view, seek_pos(end));
        cut(app);
    }
}

function void
luis_select_scope(Application_Links *app, View_ID view, Range_i64 range){
    view_set_cursor_and_preferred_x(app, view, seek_pos(range.first));
    luis_set_mark(app, view, range.end);
    view_look_at_region(app, view, range.first, range.end);
    no_mark_snap_to_cursor(app, view);
}

#if 0
CUSTOM_COMMAND_SIG(luis_select_surrounding_scope)
CUSTOM_DOC("select surrounding scope") {
    View_ID view = get_active_view(app, Access_ReadVisible);
    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 pos = view_get_cursor_pos(app, view);
    Range_i64 range = {};
    if (find_surrounding_nest(app, buffer, pos, FindNest_Scope, &range)){
        luis_select_scope(app, view, range);
    }
}
#else
//NOTE this version joins preexisting selection with surrounding scope
CUSTOM_COMMAND_SIG(luis_select_surrounding_scope)
CUSTOM_DOC("select surrounding scope") {
    View_ID view = get_active_view(app, Access_ReadVisible);
    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 mark_pos   = view_get_mark_pos(app, view);
    Range_i64 range;
    if (find_surrounding_nest(app, buffer, cursor_pos, FindNest_Scope, &range)) {
        i64 min = Min(range.min, Min(cursor_pos, mark_pos));
        i64 max = Max(range.max, Max(cursor_pos, mark_pos));
        
        i64 next_cursor_pos = min;
        i64 next_mark_pos   = max;
        if (cursor_pos > mark_pos) {
            SWAP(next_cursor_pos, next_mark_pos);
        }
        
        view_set_cursor_and_preferred_x(app, view, seek_pos(next_cursor_pos));
        luis_set_mark(app, view, next_mark_pos);
        view_look_at_region(app, view, next_cursor_pos, next_mark_pos);
        no_mark_snap_to_cursor(app, view);
    }
}
#endif

CUSTOM_COMMAND_SIG(luis_view_peek_as_split_window)
CUSTOM_DOC("view peek buffer in split window") {
    View_ID view = get_active_view(app, Access_Always);
    View_ID peek = 0;
    if (luis_view_has_flags(app, view, VIEW_IS_PEEK_WINDOW)) {
        peek = view;  
        view = luis_get_other_child_view(app, peek);
    } else {
        View_ID bro_view = luis_get_other_child_view(app, view);
        if(bro_view && luis_view_has_flags(app, bro_view, VIEW_IS_PEEK_WINDOW)) {
            peek = bro_view;    
        }
    }
    
    if (view && peek) {
        Assert (view != peek);
        i64 peek_cursor_pos = view_get_cursor_pos(app, peek);
        Buffer_Scroll peek_scroll = view_get_buffer_scroll(app, peek);
        Buffer_ID peek_buffer = view_get_buffer(app, peek, Access_Always);
        
        
        Panel_ID view_panel   = view_get_panel(app, view);
        Panel_ID parent_panel = panel_get_parent(app, view_panel);
        Panel_ID uncle_panel  = get_sibling_panel(app, parent_panel);
        
        View_ID target_view = 0;
        if (uncle_panel) {
            if (panel_is_leaf(app, uncle_panel)) {
                target_view = panel_get_view(app, uncle_panel, Access_Always);    
            } else {
                Panel_ID panel_id = panel_get_child(app, uncle_panel, Side_Min);
                target_view = panel_get_view(app, panel_id, Access_Always);
            }
        } else {
            //target_view = open_view(app, view, ViewSplit_Right); //causes crash
            if (panel_split(app, parent_panel, Dimension_X)) {
                Panel_ID new_panel_id = panel_get_child(app, parent_panel, Side_Max);
                target_view = panel_get_view(app, new_panel_id, Access_Always);
            }
            
        }
        
        if (target_view) {
            view_close(app, peek); //doing this before causes assert to fail in os_thread_wrapper() call when trying to execute a coroutine
            
            view_set_buffer(app, target_view, peek_buffer, 0);
            view_set_active(app, target_view);
            view_set_cursor_and_preferred_x(app, target_view, seek_pos(peek_cursor_pos));
            view_set_buffer_scroll(app, target_view, peek_scroll, SetBufferScroll_NoCursorChange);    
        }
    }
}

CUSTOM_COMMAND_SIG(luis_view_peek_in_parent_window)
CUSTOM_DOC("view peek buffer in split window") {
    View_ID view = get_active_view(app, Access_Always);
    View_ID peek = 0;
    if (luis_view_has_flags(app, view, VIEW_IS_PEEK_WINDOW)) {
        peek = view;  
        view = luis_get_other_child_view(app, peek);
    } else {
        View_ID bro_view = luis_get_other_child_view(app, view);
        if(bro_view && luis_view_has_flags(app, bro_view, VIEW_IS_PEEK_WINDOW)) {
            peek = bro_view;    
        }
    }
    
    if (view && peek) {
        i64 peek_cursor_pos = view_get_cursor_pos(app, peek);
        Buffer_Scroll peek_scroll = view_get_buffer_scroll(app, peek);
        Buffer_ID peek_buffer = view_get_buffer(app, peek, Access_Always);
        view_close(app, peek); //doing this before causes assert to fail in os_thread_wrapper() call when trying to execute a coroutine
        
        view_set_buffer(app, view, peek_buffer, 0);
        view_set_active(app, view);
        view_set_cursor_and_preferred_x(app, view, seek_pos(peek_cursor_pos));
        view_set_buffer_scroll(app, view, peek_scroll, SetBufferScroll_NoCursorChange);
    }
}



CUSTOM_COMMAND_SIG(luis_select_surrounding_scope_maximal)
CUSTOM_DOC("select surrounding scope")
{
    View_ID view = get_active_view(app, Access_ReadVisible);
    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 pos = view_get_cursor_pos(app, view);
    Range_i64 range = {};
    if (find_surrounding_nest(app, buffer, pos, FindNest_Scope, &range)){
        for (;;){
            pos = range.min;
            if (!find_surrounding_nest(app, buffer, pos, FindNest_Scope, &range)){
                break;
            }
        }
        luis_select_scope(app, view, range);
    }
}


CUSTOM_COMMAND_SIG(luis_write_underscore)
CUSTOM_DOC("")
{	write_text(app, SCu8("_"));	}

CUSTOM_COMMAND_SIG(luis_write_pointer_arrow)
CUSTOM_DOC("")
{	write_text(app, SCu8("->"));	}

CUSTOM_COMMAND_SIG(luis_write_newline)
CUSTOM_DOC("")
{	write_text(app, SCu8("\n"));	}

CUSTOM_COMMAND_SIG(luis_write_tab)
CUSTOM_DOC("")
{	write_text(app, SCu8("\t"));	}



//this one doesn't work as well as hoped
CUSTOM_COMMAND_SIG(luis_adjust_horizontal_view_toggle)
CUSTOM_DOC("Moves view horizontally to cursor x pos or back to leftmost of screen")
{
    View_ID view = get_active_view(app, Access_ReadVisible);
    Buffer_Scroll scroll = view_get_buffer_scroll(app, view);
    if (scroll.position.pixel_shift.x < 100) { //on left side of screen go to cursor
        i64 pos = view_get_cursor_pos(app, view);
        
         
        Buffer_Cursor cursor = view_compute_cursor(app, view, seek_pos(pos));
        i64 left_target_pos = pos - clamp(0, (cursor.col-1), 6);
        Vec2_f32 p = view_relative_xy_of_pos(app, view, cursor.line, left_target_pos);
        //scroll.target.pixel_shift.x = clamp_bot(0.f, p.x - 8*30.f);
        scroll.target.pixel_shift.x = p.x;
    }
    else //go back to leftmost of screen
    {
        scroll.target.pixel_shift.x = 0;
    }
    view_set_buffer_scroll(app, view, scroll, SetBufferScroll_SnapCursorIntoView);
    no_mark_snap_to_cursor(app, view);
    
}

CUSTOM_COMMAND_SIG(luis_horizontal_mouse_wheel_scroll)
CUSTOM_DOC("Reads the scroll wheel value from the mouse state and scrolls accordingly.") {
    View_ID view = get_active_view(app, Access_ReadVisible);
    Mouse_State mouse = get_mouse_state(app);
    if (mouse.wheel != 0){
        Buffer_Scroll scroll = view_get_buffer_scroll(app, view);
        scroll.target = view_move_buffer_point(app, view, scroll.target, V2f32((f32)mouse.wheel, 0.f));
        view_set_buffer_scroll(app, view, scroll, SetBufferScroll_SnapCursorIntoView);
    }
    if (mouse.l){
        no_mark_snap_to_cursor(app, view);
    }
}


CUSTOM_COMMAND_SIG(luis_multiline_comment_toggle)
CUSTOM_DOC("Deletes all whitespace at cursor, going backwards")
{
    View_ID view = get_active_view(app, Access_ReadWriteVisible);
    Buffer_ID buffer_id = view_get_buffer(app, view, Access_ReadWriteVisible);
    i64 cursor_pos = view_get_cursor_pos(app, view);
    i64 mark_pos = view_get_mark_pos(app, view);
    b32 add_comments;
    {
        Range_i64 line_range = get_visual_line_start_end_pos(app, view, get_line_number_from_pos(app, buffer_id, cursor_pos));
        add_comments = !c_line_comment_starts_at_position(app, buffer_id, line_range.min);
    }
    ///   
    Range_i64 range = {};
    range.start = Min(cursor_pos, mark_pos);
    range.end   = Max(cursor_pos, mark_pos);
    Range_i64 lines = get_line_range_from_pos_range(app, buffer_id, range);
    if(!is_valid_line_range(app, buffer_id, lines))	return;
    
    History_Group new_history_group = history_group_begin(app, buffer_id);
    for(i64 line = lines.start; line <= lines.end; line += 1)
    {
        if(!line_is_blank(app, buffer_id, line))
        {
            //i64 pos = get_line_start_pos(app, buffer_id, line);
            //i64 pos = get_visual_line_start(app, view, buffer_id, line);
            i64 pos = get_visual_line_start_end_pos(app, view, line).min;
            
            u8  test[256];
            buffer_read_range(app, buffer_id, Ii64(pos, pos + 256), test);
            if(add_comments)
            {
                if(!c_line_comment_starts_at_position(app, buffer_id, pos))
                {
                    buffer_replace_range(app, buffer_id, Ii64(pos), SCu8("//"));
                }
            }
            else
            {
                if(c_line_comment_starts_at_position(app, buffer_id, pos))
                {
                    buffer_replace_range(app, buffer_id, Ii64(pos, pos + 2), string_u8_empty);
                }
            }
        }
    }
    history_group_end(new_history_group);
}

CUSTOM_COMMAND_SIG(luis_end)
CUSTOM_DOC("go end of visual line")
{
    View_ID view = get_active_view(app, Access_Always);
    i64 linenum = get_line_number_from_pos(app, view_get_buffer(app, view, Access_Always), view_get_cursor_pos(app, view));
    Range_i64 range = get_visual_line_start_end_pos(app, view, linenum);
    view_set_cursor_and_preferred_x(app, view, seek_pos(range.max));
}

internal View_ID
luis_find_build_view(Application_Links *app) {
    View_ID build_view = 0;
    Buffer_ID comp_buffer = get_comp_buffer(app);
    if(comp_buffer) {
        for(View_ID v = get_view_next(app, 0, Access_Always); v; v = get_view_next(app, v, Access_Always))
        {
            #if 0 //old tab group way
            Buffer_Tab_Group *group = view_get_tab_group(app, v);
            if(group) foreach_index_inc(i, group->tab_count)
            {
                if(group->tabs[i] == comp_buffer)
                {
                    build_view = v;
                    break;
                }
            }
            else if( view_get_buffer(app, v, Access_Always) == comp_buffer )
                build_view = v;
            #else
            if( view_get_buffer(app, v, Access_Always) == comp_buffer )
                build_view = v;
            #endif
            
            if(build_view)	break;
        }   
    }
    return build_view;
}

CUSTOM_COMMAND_SIG(luis_close_panel)
CUSTOM_DOC("Close panel. Peek first.") {
    
    View_ID active_view = get_active_view(app, Access_Always);
    if (View_ID peek = luis_get_peek_window(app, active_view)) {
        view_close(app, peek);
    }
    else  {
        for(View_ID v = get_view_next(app, 0, Access_Always); v; v = get_view_next(app, v, Access_Always)) {
            if (luis_view_has_flags(app, v, VIEW_IS_PEEK_WINDOW)) {
                view_close(app, v); 
                return;
            }
        }
        view_close(app, active_view);
    }
}

function b32
get_cpp_matching_file_dont_make(Application_Links *app, Buffer_ID buffer, Buffer_ID *buffer_out){
    b32 result = false;
    Scratch_Block scratch(app);
    String_Const_u8 file_name = push_buffer_file_name(app, scratch, buffer);
    if (file_name.size > 0){
        String_Const_u8 extension = string_file_extension(file_name);
        String_Const_u8 new_extensions[2] = {};
        i32 new_extensions_count = 0;
        if (string_match(extension, string_u8_litexpr("cpp")) || string_match(extension, string_u8_litexpr("cc"))){
            new_extensions[0] = string_u8_litexpr("h");
            new_extensions[1] = string_u8_litexpr("hpp");
            new_extensions_count = 2;
        }
        else if (string_match(extension, string_u8_litexpr("c"))){
            new_extensions[0] = string_u8_litexpr("h");
            new_extensions_count = 1;
        }
        else if (string_match(extension, string_u8_litexpr("h"))){
            new_extensions[0] = string_u8_litexpr("cpp");
            new_extensions[1] = string_u8_litexpr("c");
            new_extensions_count = 2;
        }
        else if (string_match(extension, string_u8_litexpr("hpp"))){
            new_extensions[0] = string_u8_litexpr("cpp");
            new_extensions_count = 1;
        }
        
        String_Const_u8 file_without_extension = string_file_without_extension(file_name);
        for (i32 i = 0; i < new_extensions_count; i += 1){
            Temp_Memory temp = begin_temp(scratch);
            String_Const_u8 new_extension = new_extensions[i];
            String_Const_u8 new_file_name = push_u8_stringf(scratch, "%.*s.%.*s", string_expand(file_without_extension), string_expand(new_extension));
            if (open_file(app, buffer_out, new_file_name, false, true)){
                result = true;
                break;
            }
            end_temp(temp);
        }
    }
    
    return(result);
}

CUSTOM_COMMAND_SIG(luis_matching_file_cpp_same_buffer)
CUSTOM_DOC("If the current file is a *.cpp or *.h, attempts to open the corresponding *.h or *.cpp file in the other view.") {
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    Buffer_ID new_buffer = 0;
    if (get_cpp_matching_file_dont_make(app, buffer, &new_buffer)){
        view_set_buffer(app, view, new_buffer, 0);
    }
}

CUSTOM_COMMAND_SIG(luis_matching_file_cpp)
CUSTOM_DOC("If the current file is a *.cpp or *.h, attempts to open the corresponding *.h or *.cpp file in the other view.") {
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    Buffer_ID new_buffer = 0;
    if (get_cpp_matching_file_dont_make(app, buffer, &new_buffer)){
        view = get_next_view_looped_primary_panels(app, view, Access_Always);
        view_set_buffer(app, view, new_buffer, 0);
        view_set_active(app, view);
    }
}



CUSTOM_COMMAND_SIG(luis_return)
CUSTOM_DOC("If the buffer in the active view is writable, inserts a character, otherwise performs goto_jump_at_cursor.")
{
    View_ID view = get_active_view(app, Access_ReadVisible);
    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadWriteVisible);
    if(buffer) //can read and write
    {
        write_text(app, SCu8("\n"));
        auto_indent_line_at_cursor(app);
    }
    else
    {
        goto_jump_at_cursor_same_panel(app);                                   
        //buffer = view_get_buffer(app, view, Access_ReadVisible);
        //if(buffer) //got buffer back as readonly
        //{
        //goto_jump_at_cursor(app);
        //lock_jump_buffer(app, buffer);
        //}
        //else leave_current_input_unhandled(app);
    }
}

internal View_ID
luis_find_first_view_with_buffer(Application_Links *app, Buffer_ID buffer) {
    if (buffer) {
        for (View_ID view = get_view_next(app, 0, Access_Always); 
            view; 
            view = get_view_next(app, view, Access_Always)) {
            
            if (view_get_buffer(app, view, Access_Always) == buffer) {
                return view;
            }
        }
    }
    return 0;
}

static Locked_Jump_State
luis_get_or_make_locked_jump_state_default_to_comp_or_search_buffer(Application_Links *app, View_ID active_view, Heap *heap) {
    Locked_Jump_State jump_state = get_locked_jump_state(app, heap);
    //try to reopen *compilation* buffer to jump around in
    if (!jump_state.view) {
        Buffer_ID comp_buffer = get_comp_buffer(app);
        if (comp_buffer) {
            View_ID build_view = luis_find_first_view_with_buffer(app, comp_buffer);
            
            if (!build_view) {
                build_view = luis_get_or_split_peek_window(app, active_view, ViewSplit_Bottom);
                if (build_view) {
                    if (!view_set_buffer(app, build_view, comp_buffer, 0)) build_view = 0;
                }
            }
            
            if (build_view) {
                lock_jump_buffer(app, comp_buffer);    
                jump_state = get_locked_jump_state(app, heap);
                
                if (jump_state.list && jump_state.list->jump_count == 0) {
                    view_close(app, build_view);
                    
                    jump_state.view = 0; 
                }
            }
        }
    }
    
    //try to reopen *search* buffer to jump around in
    if (!jump_state.view) {
        Buffer_ID search_buffer = get_buffer_by_name(app, string_u8_litexpr("*search*"), Access_Always);
        if (search_buffer) {
            View_ID search_view = luis_find_first_view_with_buffer(app, search_buffer);
            
            if (!search_view) {
                search_view = luis_get_or_split_peek_window(app, active_view, ViewSplit_Bottom);
                if (search_view) {
                    if (!view_set_buffer(app, search_view, search_buffer, 0)) search_view = 0;
                }
            }
            
            if (search_view) {
                lock_jump_buffer(app, search_buffer);    
                jump_state = get_locked_jump_state(app, heap);
                
                if (jump_state.list && jump_state.list->jump_count == 0) {
                    view_close(app, search_view);
                    jump_state.view = 0; 
                }
            }
        }
    }
    
    return jump_state;
}

CUSTOM_COMMAND_SIG(luis_goto_next_jump)
CUSTOM_DOC("If a buffer containing jump locations has been locked in, goes to the next jump in the buffer, skipping sub jump locations.") {
    View_ID active_view = get_active_view(app, Access_Always);
    Locked_Jump_State jump_state = luis_get_or_make_locked_jump_state_default_to_comp_or_search_buffer(app, active_view, &global_heap);
    
    if (jump_state.view) {
        i64 cursor_position = view_get_cursor_pos(app, jump_state.view);
        Buffer_Cursor cursor = view_compute_cursor(app, jump_state.view, seek_pos(cursor_position));
        i64 line = get_line_from_list(app, jump_state.list, jump_state.list_index);
        if (line <= cursor.line){
            jump_state.list_index += 1;
        }
        goto_next_filtered_jump(app, jump_state.list, jump_state.view, jump_state.list_index, 1, true, true);
    }
}

CUSTOM_COMMAND_SIG(luis_goto_prev_jump)
CUSTOM_DOC("If a buffer containing jump locations has been locked in, goes to the previous jump in the buffer, skipping sub jump locations."){
    View_ID active_view = get_active_view(app, Access_Always);
    Locked_Jump_State jump_state = luis_get_or_make_locked_jump_state_default_to_comp_or_search_buffer(app, active_view, &global_heap);
    
    if (jump_state.view) {
        if (jump_state.list_index > 0) {
            --jump_state.list_index;
        }
        goto_next_filtered_jump(app, jump_state.list, jump_state.view, jump_state.list_index, -1, true, true);
    }
}


CUSTOM_COMMAND_SIG(luis_build)
CUSTOM_DOC("build") {
    //logprintf(app, "\nluis_build printing (%d groups init)...\n", BUFFER_TAB_GROUP_COUNT);
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    
    View_ID build_view = luis_find_build_view(app);
    if (!build_view)
        build_view = luis_get_or_split_peek_window(app, view, ViewSplit_Bottom);
    
    if (build_view) {
        standard_search_and_build(app, build_view, buffer);
        set_fancy_compilation_buffer_font(app);
        
        block_zero_struct(&prev_location);
        lock_jump_buffer(app, string_u8_litexpr("*compilation*"));
        
        //logprintf(app, "Built and now have %d groups open\n", BUFFER_TAB_GROUP_COUNT);
        view_set_active(app, view);
        
        
        //NOTE it's weird we have to do this but basically standard_search_and_build
        //calls some begin/end buffer calls the throws off the tab group thing
        //so we just manually set the current tab to comp buffer otherwise
        //sometimes we won't get it....
        #if 0
        Buffer_ID comp_buffer = get_comp_buffer(app);
        if(comp_buffer)
        {
            Buffer_Tab_Group *group = view_get_tab_group(app, build_view);
            if(group)
            {
                group->tabs[group->current_tab] = comp_buffer;
            }
            //view_set_buffer(app, build_view, comp_buffer, 0);
        }
        #endif
    }
}

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

internal void
vim_navigate_and_peek_code_index_entry(Application_Links *app, View_ID view, Lister *lister, i32 delta) {
    //lister__navigate__default
    i32 new_index = lister->item_index + delta;
    if (new_index < 0 && lister->item_index == 0){
        lister->item_index = lister->filtered.count - 1;
    } else if (new_index >= lister->filtered.count &&
             lister->item_index == lister->filtered.count - 1) {
        lister->item_index = 0;
    } else {
        lister->item_index = clamp(0, new_index, lister->filtered.count - 1);
    }
    lister->set_vertical_focus_to_item = true;
    lister_update_selection_values(lister);
    
    
    if (in_range(0, lister->raw_item_index, lister->options.count)) {
        Code_Index_Note *note = (Code_Index_Note *)lister_get_user_data(lister, lister->raw_item_index);
        Set_Buffer_Flag flags = SetBuffer_KeepOriginalGUI;
        view_set_buffer(app, view, note->file->buffer, flags);
        
        Buffer_Seek seek = seek_pos(note->pos.first);
        view_set_cursor_and_preferred_x(app, view, seek);
        view_set_mark(app, view, seek);
        center_view(app, view, 0.1f);
    }
    
}

CUSTOM_COMMAND_SIG(luis_list_notes_this_buffer)
CUSTOM_DOC("Show code indexes for buffer") {
    Scratch_Block scratch(app);
    Lister_Block lister(app, scratch);
    lister_set_query(lister, "Code Index:");
    //lister_set_default_handlers(lister);
    vim_lister_set_default_handlers(lister);
    lister.lister.current->handlers.navigate = &vim_navigate_and_peek_code_index_entry;
    
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
            if (note->note_kind == CodeIndexNote_Function_Definition ||
                note->note_kind == CodeIndexNote_Type_Definition     ||
                note->note_kind == CodeIndexNote_Macro)
            {
                Scratch_Block temp(app, scratch);
                String_u8 string = string_u8_push(temp, 512);
                
                if (note->note_kind == CodeIndexNote_Function_Definition) {
                    string_append(&string, SCu8("() "));    
                } else if(note->note_kind == CodeIndexNote_Macro) {
                    string_append(&string, SCu8("# "));    
                } else {
                    string_append(&string, SCu8("\"\" "));
                }
                
                i32  parent_count = 0;
                i32  max_parent_count = 64;
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
                
                //Lister_Prealloced_String status = {};
                string_append(&string, note->text);
                Lister_Prealloced_String search_string = lister_prealloced(push_string_copy(lister.lister.current->arena, SCu8(string)));
                lister_add_item(lister, search_string, status, (void*)note, 0);
            }
            
            //if (note->nest) {
                
            //}
        }
    }
    code_index_unlock();
    
    
    //Tiny_Jump result = {};
    View_ID init_view = get_active_view(app, Access_ReadVisible);
    luis_view_clear_flags(app, init_view, VIEW_NOTEPAD_MODE_MARK_SET);
    Buffer_ID init_buffer = view_get_buffer(app, init_view, Access_ReadVisible);
    i64 init_cursor = view_get_cursor_pos(app, init_view);
    Buffer_Scroll init_view_scroll = view_get_buffer_scroll (app, init_view);
    
    
    Lister_Result l_result = vim_run_lister(app, lister);
    if (l_result.canceled) {
        //view_set_active(app, active); //should never have changed
        view_set_buffer(app, init_view, init_buffer, 0);
        view_set_cursor_and_preferred_x(app, init_view, seek_pos(init_cursor));
        view_set_buffer_scroll(app, init_view, init_view_scroll, SetBufferScroll_SnapCursorIntoView);
    } else if (l_result.user_data) {
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
}

function String_Const_u8
print_messagef(Application_Links *app, Arena *arena, char *format, ...){
    va_list args;
    va_start(args, format);
    String_Const_u8 result = push_stringfv(arena, format, args);
    print_message(app, result);
    va_end(args);
    return(result);
}

#if 0
internal bool
is_buffer_included_in_project(Application_Links *app, Buffer_ID buffer) {
    Scratch_Block scratch(app);
    Variable_Handle prj_var = vars_read_key(vars_get_root(), vars_save_string_lit("prj_config"));
    Variable_Handle load_paths_all_oses_var = vars_read_key(prj_var, vars_save_string_lit("load_paths"));
    Variable_Handle load_paths_var = vars_read_key(load_paths_all_oses_var, vars_save_string_lit(OS_NAME));
    
    String_ID path_id = vars_save_string_lit("path");
    String_ID recursive_id = vars_save_string_lit("recursive");
    String_ID relative_id = vars_save_string_lit("relative");
    
    Variable_Handle whitelist_var = vars_read_key(prj_var, vars_save_string_lit("patterns"));
    Variable_Handle blacklist_var = vars_read_key(prj_var, vars_save_string_lit("blacklist_patterns"));
    Prj_Pattern_List whitelist = prj_pattern_list_from_var(scratch, whitelist_var);
    Prj_Pattern_List blacklist = prj_pattern_list_from_var(scratch, blacklist_var);
    
    for (Variable_Handle load_path_var = vars_first_child(load_paths_var);
         !vars_is_nil(load_path_var);
         load_path_var = vars_next_sibling(load_path_var)) {
        
        Variable_Handle path_var = vars_read_key(load_path_var, path_id);
        Variable_Handle recursive_var = vars_read_key(load_path_var, recursive_id);
        Variable_Handle relative_var = vars_read_key(load_path_var, relative_id);
        
        String8 path  = vars_string_from_var(scratch, path_var);
        //b32 recursive = vars_b32_from_var(recursive_var);
        b32 relative  = vars_b32_from_var(relative_var);
        
        String8 file_dir = path;
        if (relative){
            String8 prj_dir = prj_path_from_project(scratch, prj_var);
            
            String8List file_dir_list = {};
            string_list_push(scratch, &file_dir_list, prj_dir);
            string_list_push_overlap(scratch, &file_dir_list, '/', path);
            string_list_push_overlap(scratch, &file_dir_list, '/', SCu8());
            file_dir = string_list_flatten(scratch, file_dir_list, StringFill_NullTerminate);
        }
        
        String_Const_u8 buffer_filepath = push_buffer_file_name(app, scratch, buffer); //NOTE this is canonical
        
        
        /*
        Editing_File *file = imp_get_file(models, buffer_id);
        String_Const_u8 result = {};
        if (api_check_buffer(file)){
            result = push_string_copy(arena, string_from_file_name(&file->canon));
        }
        String_Const_u8 system_get_path(Arena* arena, System_Path_Code path_code)
        String_Const_u8 system_get_canonical(Arena* arena, String_Const_u8 name)
        */
    }
    return false;
}

CUSTOM_COMMAND_SIG(is_current_buffer_in_project)
CUSTOM_DOC("Check if current buffer is in project") {
    View_ID view     = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    
    Scratch_Block scratch(app);
    String_Const_u8 buffer_path = push_buffer_file_name(app, scratch, buffer);
    if (is_buffer_included_in_project(app, buffer)) {
        print_messagef(app, scratch, "Buffer is project: %.*s", string_expand(buffer_path));
    } else {
        print_messagef(app, scratch, "Buffer NOT in project: %.*s", string_expand(buffer_path));
    }
}
#endif


CUSTOM_COMMAND_SIG(reload_project)
CUSTOM_DOC("Reload project.4coder") {
    Variable_Handle prj_var = vars_read_key(vars_get_root(), vars_save_string_lit("prj_config"));
    close_all_code(app);
    if (vars_is_nil(prj_var)){    
        load_project(app);
    } else {
        Scratch_Block scratch(app);
        //NOTE (copied and pasted this from load_project() (just so we don't have to find and reparse the project file
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
            
            
            u32 flags = 0;
            if (recursive){
                flags |= PrjOpenFileFlag_Recursive;
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
            
            prj_open_files_pattern_filter(app, file_dir, whitelist, blacklist, flags);
        }
        
        // NOTE(allen): Set Window Title
        Variable_Handle proj_name_var = vars_read_key(prj_var, vars_save_string_lit("project_name"));
        String_ID proj_name_id = vars_string_id_from_var(proj_name_var);
        if (proj_name_id != 0){
            String8 proj_name = vars_read_string(scratch, proj_name_id);
            String8 title = push_u8_stringf(scratch, "4coder project: %.*s", string_expand(proj_name));
            set_window_title(app, title);
        }
    }
    
}



CUSTOM_COMMAND_SIG(luis_list_notes_all_buffers)
CUSTOM_DOC("Show code indexes for all buffer") {
    Scratch_Block scratch(app);
    Lister_Block lister(app, scratch);
    lister_set_query(lister, "Code Index:");
    //lister_set_default_handlers(lister);
    vim_lister_set_default_handlers(lister);
    lister.lister.current->handlers.navigate = &vim_navigate_and_peek_code_index_entry;
    
    code_index_lock();
    for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always); buffer; buffer = get_buffer_next(app, buffer, Access_Always)) {  
        Code_Index_File *file = code_index_get_file(buffer);
        if(!file) continue;
        
        Lister_Prealloced_String status  = lister_prealloced(push_buffer_base_name(app, lister.lister.current->arena, buffer));
        for(i32 note_index = 0; note_index < file->note_array.count; note_index += 1) {
            Code_Index_Note *note = file->note_array.ptrs[note_index];
            
            if (note->note_kind == CodeIndexNote_Function_Definition ||
                note->note_kind == CodeIndexNote_Type_Definition     ||
                note->note_kind == CodeIndexNote_Macro)
            {
                Scratch_Block temp(app, scratch);
                String_u8 string = string_u8_push(temp, 512);
                
                if (note->note_kind == CodeIndexNote_Function_Definition) {
                    string_append(&string, SCu8("() "));    
                } else if(note->note_kind == CodeIndexNote_Macro) {
                    string_append(&string, SCu8("## "));    
                } else {
                    string_append(&string, SCu8("<> "));
                }
                
                i32  parent_count = 0;
                i32  max_parent_count = 64;
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
                
                //Lister_Prealloced_String status = {};
                string_append(&string, note->text);
                Lister_Prealloced_String search_string = lister_prealloced(push_string_copy(lister.lister.current->arena, SCu8(string)));
                lister_add_item(lister, search_string, status, (void*)note, 0);
            }
            
            //if (note->nest) {
            
            //}
        }
    }
    code_index_unlock();
    
    
    View_ID init_view = get_active_view(app, Access_ReadVisible);
    luis_view_clear_flags(app, init_view, VIEW_NOTEPAD_MODE_MARK_SET);
    Buffer_ID init_buffer = view_get_buffer(app, init_view, Access_ReadVisible);
    i64 init_cursor = view_get_cursor_pos(app, init_view);
    Buffer_Scroll init_view_scroll = view_get_buffer_scroll (app, init_view);
    
    
    Lister_Result l_result = vim_run_lister(app, lister);
    if (l_result.canceled) {
        //view_set_active(app, active); //should never have changed
        view_set_buffer(app, init_view, init_buffer, 0);
        view_set_cursor_and_preferred_x(app, init_view, seek_pos(init_cursor));
        view_set_buffer_scroll(app, init_view, init_view_scroll, SetBufferScroll_SnapCursorIntoView);
    } else if (l_result.user_data) {
        //block_copy_struct(&result, (Tiny_Jump*)l_result.user_data);
        
        //View_ID view = get_active_view(app, Access_Always);
        //Buffer_ID init_buffer = view_get_buffer(app, view, Access_Always);
        //i64 init_pos = view_get_cursor_pos(app, view);
        
        //view_set_current_buffer_location(app, view, init_buffer, init_pos);
        View_ID view = get_active_view(app, Access_Always);
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



CUSTOM_COMMAND_SIG(luis_list_functions_all_buffers)
CUSTOM_DOC("Show code indexes for all buffer") {
    Scratch_Block scratch(app);
    Lister_Block lister(app, scratch);
    lister_set_query(lister, "Procedure:");
    //lister_set_default_handlers(lister);
    vim_lister_set_default_handlers(lister);
    lister.lister.current->handlers.navigate = &vim_navigate_and_peek_code_index_entry;
    
    code_index_lock();
    for (Buffer_ID buffer = get_buffer_next(app, 0, Access_Always); buffer; buffer = get_buffer_next(app, buffer, Access_Always)) {  
        Code_Index_File *file = code_index_get_file(buffer);
        if (!file) continue; 
        
        Lister_Prealloced_String status  = lister_prealloced(push_buffer_base_name(app, lister.lister.current->arena, buffer));
        for(i32 note_index = 0; note_index < file->note_array.count; note_index += 1) {
            Code_Index_Note *note = file->note_array.ptrs[note_index];
            if (note->note_kind != CodeIndexNote_Function_Definition) continue;
            
            Scratch_Block temp(app, scratch);
            String_u8 string = string_u8_push(temp, 512);
            
            i32  parent_count = 0;
            i32  max_parent_count = 64;
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
            
                //Lister_Prealloced_String status = {};
            string_append(&string, note->text);
            string_append(&string, SCu8("("));
            if (note->func_arg_string.size > 0) {
                string_append(&string, note->func_arg_string);
            }
            string_append(&string, SCu8(")"));
            Lister_Prealloced_String search_string = lister_prealloced(push_string_copy(lister.lister.current->arena, SCu8(string)));
            lister_add_item(lister, search_string, status, (void*)note, 0);
        }
    }
    code_index_unlock();
    
    View_ID init_view = get_active_view(app, Access_ReadVisible);
    luis_view_clear_flags(app, init_view, VIEW_NOTEPAD_MODE_MARK_SET);
    Buffer_ID init_buffer = view_get_buffer(app, init_view, Access_ReadVisible);
    i64 init_cursor = view_get_cursor_pos(app, init_view);
    Buffer_Scroll init_view_scroll = view_get_buffer_scroll (app, init_view);
    
    Lister_Result l_result = vim_run_lister(app, lister);
    if (l_result.canceled) {
        //view_set_active(app, active); //should never have changed
        view_set_buffer(app, init_view, init_buffer, 0);
        view_set_cursor_and_preferred_x(app, init_view, seek_pos(init_cursor));
        view_set_buffer_scroll(app, init_view, init_view_scroll, SetBufferScroll_SnapCursorIntoView);
    } else if (l_result.user_data) {
        //block_copy_struct(&result, (Tiny_Jump*)l_result.user_data);
        
        //View_ID view = get_active_view(app, Access_Always);
        //Buffer_ID init_buffer = view_get_buffer(app, view, Access_Always);
        //i64 init_pos = view_get_cursor_pos(app, view);
        
        //view_set_current_buffer_location(app, view, init_buffer, init_pos);
        View_ID view = get_active_view(app, Access_Always);
        Code_Index_Note *note = (Code_Index_Note *)l_result.user_data;
        view_set_buffer(app, view, note->file->buffer, 0);
        view_set_cursor_and_preferred_x(app, view, seek_pos(note->pos.first));
        //luis_center_view_top(app);
    }
}

struct Code_Variable {
    Range_i64 scope_range;
    Code_Index_Note *type;
    Token name_token;
};



static String_Const_u8
get_type_keyword_string(Token *token) {
    if (token && token->kind == TokenBaseKind_Keyword) {
        switch (token->sub_kind) {
        case TokenCppKind_Void:     return string_u8_litexpr("void");
        case TokenCppKind_Bool:     return string_u8_litexpr("bool");
        case TokenCppKind_Char:     return string_u8_litexpr("char");
        case TokenCppKind_Int:      return string_u8_litexpr("int");
        case TokenCppKind_Float:    return string_u8_litexpr("float");
        case TokenCppKind_Double:   return string_u8_litexpr("double");
        }
    }
    return {};
}

static void token_it_skip_pass_parenthetical(Token_Iterator_Array *it) {
    Token *token = token_it_read(it);
    if (!token || token->kind != TokenBaseKind_ParentheticalOpen) return;
    
    //TokenCppKind_BraceOp = 44, //{
    //TokenCppKind_BraceCl = 45, //}
    //TokenCppKind_ParenOp = 46, //(
    //TokenCppKind_ParenCl = 48, //)
    //TokenCppKind_BrackOp = 47, //<
    //TokenCppKind_BrackCl = 49, //>
    
    i16 open_kind  = token->sub_kind;
    i16 close_kind = 0;
    switch (open_kind) {
    case TokenCppKind_BraceOp: close_kind = TokenCppKind_BraceCl; break;
    case TokenCppKind_ParenOp: close_kind = TokenCppKind_ParenCl; break;
    case TokenCppKind_BrackOp: close_kind = TokenCppKind_BrackCl; break;
    default: return;
    }
    
    i32 balance = -1;
    while (balance) {
        token_it_inc_non_whitespace(it);
        token = token_it_read(it);
        if (!token) return;
        
        //hmm seem that sub_kind is always unique so we don't really have to check base_kind...
        //but just cause i'm a paranoid...
        if (token->kind == TokenBaseKind_ParentheticalOpen || 
            token->kind == TokenBaseKind_ParentheticalClose) {
            if      (token->sub_kind == open_kind)  balance -= 1;
            else if (token->sub_kind == close_kind) balance += 1;
        }
    }
    
}

CUSTOM_COMMAND_SIG(luis_print_scope_variables)
CUSTOM_DOC("print scope variables") {
    View_ID view     = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    Code_Index_File *code_index_file = code_index_get_file(buffer);
    if (!code_index_file) return;
    
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer);
    if (!buffer_scope) return;
    
    i64 cursor_pos = view_get_cursor_pos(app, view);
    
    Code_Index_Nest *outermost_nest = 0;
    for (i32 nest_index = 0; nest_index < code_index_file->nest_array.count; nest_index += 1) {
        Code_Index_Nest *nest = code_index_file->nest_array.ptrs[nest_index];
        if (!nest) continue; //should never occur right?
        if (nest->kind != CodeIndexNest_Scope) continue;
        if (!(nest->flags & CODE_INDEX_NEST_IS_CLOSED)) continue;
        
        
        i64 start = nest->open.min;
        i64 end   = nest->close.max;
        
        if (cursor_pos >= start && cursor_pos < end) {
            outermost_nest = nest;
            break;
        }
    }
    
    if (outermost_nest) {
        Scratch_Block scratch(app);
        i32  outer_nest_count = 0;
        i32  max_outer_nest_count = 64;
        Code_Index_Nest **outer_nests = push_array_zero(scratch, Code_Index_Nest *, max_outer_nest_count);
        
        Code_Index_Nest *current = outermost_nest;
        while (current) {
            outer_nests[outer_nest_count++] = outermost_nest;
            if (outer_nest_count == max_outer_nest_count) break;
            
            Code_Index_Nest *next_current = 0;
            for (i32 nest_index = 0; nest_index < current->nest_array.count; nest_index += 1) {
                Code_Index_Nest *nest = current->nest_array.ptrs[nest_index];
                if (!nest) continue; //should never occur right?
                if (nest->kind != CodeIndexNest_Scope) continue;
                if (!(nest->flags & CODE_INDEX_NEST_IS_CLOSED)) break;
                
                if (cursor_pos >= nest->open.min && cursor_pos < nest->close.max) {
                    next_current = nest;
                }
            }
            current = next_current;
        }
        
        Token_Array token_array = get_token_array_from_buffer(app, buffer);
        if (!token_array.tokens) return;
        
        Token_Iterator_Array it = token_iterator_pos(0, &token_array, outermost_nest->open.min);
        while (Token *token = token_it_read(&it)) {
            if (token->pos > cursor_pos) break; //we're done
            
            b32 is_type_keyword = false;
            String_Const_u8 typestr = {}; //if we think token is at type identifier, we set this
            if ((token->kind == TokenBaseKind_ScopeOpen) || (token->kind == TokenBaseKind_ScopeClose) || token->kind == TokenBaseKind_StatementClose) {
                token_it_inc_non_whitespace(&it);
                token = token_it_read(&it);
                if (!token || token->pos > cursor_pos) break;
                if (token->kind == TokenBaseKind_Identifier) {
                    typestr = push_token_lexeme(app, scratch, buffer, token);
                } else if (token->kind == TokenBaseKind_Keyword) {
                    is_type_keyword = true;
                    typestr = get_type_keyword_string(token);
                }
            }
            
            token_it_inc_non_whitespace(&it);
            token = token_it_read(&it);
            
            Code_Index_Note *typenote = 0;
            if (!is_type_keyword) {
                typenote = code_index_note_from_string(typestr);
                if (!typenote) continue;
                
                if (token->kind == TokenBaseKind_ParentheticalOpen) {
                    token_it_skip_pass_parenthetical(&it);
                    token = token_it_read(&it);
                    if (!token || token->pos > cursor_pos) break;
                }
                //NOTE here we have to skip angle brackets
            }
            
            
            while (token->kind == TokenBaseKind_Operator && token->sub_kind == TokenCppKind_Star) {
                token_it_inc_non_whitespace(&it);
                token = token_it_read(&it);
                if (!token || token->pos > cursor_pos) break;
            }
            if (!token || token->pos > cursor_pos) break;
            
            
            if (token->kind == TokenBaseKind_Identifier) {
                String_Const_u8 varname = push_token_lexeme(app, scratch, buffer, token);
                i32 scope_level = outer_nest_count;
                for (i32 i = outer_nest_count-1; i >= 0; i -= 1) {
                    Code_Index_Nest *nest = outer_nests[i];
                    if (token->pos >= nest->open.min && token->pos < nest->close.max) {
                        break;
                    } else {
                        scope_level -= 1;
                    }
                }
                print_messagef(app, scratch, "Found local var: %.*s %.*s at %d scope\n", string_expand(typestr), string_expand(varname), scope_level);
            }
        }
    }
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
string_remove_whitespace(String_Const_u8 string) {
    if (string.size == 0) return {};
    
    b32 currently_inside_whitespace = false;
    u64 max_whitespace_pos = 0;
    
    //only benefit of doing this in reverse order is that we do less copying potientally because we wouldn't copy around whitespace that we'd removed
    u64 index = string.size;
    while (index--) {
        b32 is_whitespace = character_is_whitespace(string.str[index]); 
        if (!currently_inside_whitespace) {
            if (is_whitespace) {
                currently_inside_whitespace = true;
                max_whitespace_pos = index + 1;
            }
        }
        else {
            if (!is_whitespace) {
                currently_inside_whitespace = false;
                u64 min_whitespace_pos = index + 1;
                if (min_whitespace_pos < max_whitespace_pos) {
                    u64 copy_length = string.size - max_whitespace_pos;
                    if (copy_length > 0) {
                        block_copy(string.str + min_whitespace_pos, string.str + max_whitespace_pos, copy_length);
                    }
                    string.size -= (max_whitespace_pos - min_whitespace_pos);
                }
            }
        }
    }
    
    if (currently_inside_whitespace) {
        u64 min_whitespace_pos = 0;
        if (min_whitespace_pos < max_whitespace_pos) {
            u64 copy_length = string.size - max_whitespace_pos;
            if (copy_length > 0) {
                block_copy(string.str + min_whitespace_pos, string.str + max_whitespace_pos, copy_length);
            }
            string.size -= (max_whitespace_pos - min_whitespace_pos);
        }
    }
    
    return string;
}

#if 0
function Peek_Code_Index_State *
get_code_peek_state(Application_Links *app, View_ID view, i64 pos, View_ID *peek) {
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    *peek = luis_get_or_split_peek_window(app, view, ViewSplit_Bottom);
    
    Managed_Scope peek_scope = view_get_managed_scope(app, *peek);
    Peek_Code_Index_State *state = scope_attachment(app, peek_scope, view_code_peek_state, Peek_Code_Index_State);
    if (!state) {
        view_close(app, *peek);
        *peek = 0;
        return 0;   
    }
    
    
    Scratch_Block scratch(app);
    
    Scope_Prefix manual_scope_prefix = {};
    String_Const_u8 identifier = {};
    
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    if (token_array.tokens) {
        Token *identifier_token = get_token_from_pos(app, &token_array, pos);
        //size check is probably redundant here
        if (identifier_token && identifier_token->kind == TokenBaseKind_Identifier) {
            Range_i64 range = Ii64(identifier_token);
            identifier = push_buffer_range(app, scratch, buffer, range);
            
            Token *first_identifier = 0;
            Token_Iterator_Array it = token_iterator_pos(0, &token_array, identifier_token->pos - 1);
            Token *at = token_it_read(&it);
            
            auto is_scope_operator = [](Token *token) -> bool {
                if (token && token->kind == TokenBaseKind_Operator && token->sub_kind == TokenCppKind_ColonColon) {
                    return true;
                }    
                else return false;
            };
            
            if (is_scope_operator(at)) {
                b32 began_with_scope_operator = true;
                manual_scope_prefix.scope_count = 1;
                token_it_dec_non_whitespace(&it);
                at = token_it_read(&it);
                
                while (at) {
                    if (at->kind == TokenBaseKind_Identifier) {
                        first_identifier = at;
                        began_with_scope_operator = false;
                    }
                    else if (is_scope_operator(at)) {
                        manual_scope_prefix.scope_count += 1;
                        began_with_scope_operator = true;
                        /*keep scanning*/
                    }
                    else break;
                    
                    token_it_dec_non_whitespace(&it);
                    at = token_it_read(&it);
                }
                
                if (first_identifier) {
                    range.min = first_identifier->pos;
                    range.max = identifier_token->pos;
                    manual_scope_prefix.string = push_buffer_range(app, scratch, buffer, range);;
                    manual_scope_prefix.string = string_remove_whitespace(manual_scope_prefix.string);
                    if (!began_with_scope_operator) {
                        manual_scope_prefix.string = push_stringf(scratch, "::%.*s", string_expand(manual_scope_prefix.string));
                        manual_scope_prefix.scope_count += 1;
                    }
                }
            }
            
            
        }
    }
    else {
        view_close(app, *peek);
        *peek = 0;
        return 0;
    }
    
    
    //String_Const_u8 identifier = push_token_or_word_under_pos(app, scratch, buffer, pos);
    if (identifier.size == 0) {
        view_close(app, *peek);
        *peek = 0;
        return 0;
    }
    
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
        
        Scope_Prefix scope_prefix = manual_scope_prefix;
        if (scope_prefix.scope_count == 0) {
            scope_prefix = get_entire_scope_prefix(app, scratch, buffer, pos);
        }
        
        
        auto get_match_metric = [&scope_prefix] (Scope_Prefix prefix) -> i32{
            i32 search_length = (i32)Min(prefix.string.size, scope_prefix.string.size);
            i32 match_count = 0;
            for (i32 i = 0; i < search_length; i += 1) {
                if (prefix.string.str[i] == scope_prefix.string.str[i]) {
                    if (i > 0 && prefix.string.str[i-1] == ':' && prefix.string.str[i] == ':') {
                        match_count += 1;
                    }
                }
                else break;
            }
            
            i32 non_match_count = prefix.scope_count - match_count;
            //i32 non_match_count = scope_prefix_scope_count - match_count;
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
                    Scope_Prefix note_prefix = get_entire_scope_prefix(app, scratch, note->file->buffer, note->pos.min);
                    i32 note_metric = get_match_metric(note_prefix);
                    
                	//add to array already sorted
                    for (i32 i = state->note_count - 2; i >= 0; i -= 1) {
                        Code_Index_Note *prev = state->notes[i];
                        Assert (state->notes[i+1] == note); //NOTE (i+1) should always be our note
                        
                        Scope_Prefix prev_prefix = get_entire_scope_prefix(app, scratch, prev->file->buffer, prev->pos.min);
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
        
        if (state->note_count == 0) {
            view_close(app, *peek);
            *peek = 0;
            state = 0;
        }
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
#endif

struct Code_Index_Peek_State {
    i32 current_note_index;
    b32 goto_first_note;
    
    u32 identifier_length;
    u8  identifier[128];
    
    i32 scope_prefix_scope_count;
    u32 scope_prefix_length;
    u8  scope_prefix[128];
};

function Token *
expect_token_from_pos(Application_Links *app, Token_Array *array, u64 pos, Token_Base_Kind kind) {
    Token *token = get_token_from_pos(app, array, pos);
    if (token && token->kind == kind) return token;
    else return 0;
}

function Code_Index_Peek_State *
get_code_peek_state(Application_Links *app, View_ID view, i64 pos, View_ID *peek) {
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    *peek = luis_get_or_split_peek_window(app, view, ViewSplit_Bottom);
    
    Managed_Scope peek_scope = view_get_managed_scope(app, *peek);
    Code_Index_Peek_State *state = scope_attachment(app, peek_scope, view_code_peek_state, Code_Index_Peek_State);
    #define FAILED_RETURN_NULL do { view_close(app, *peek); *peek = 0; return 0; } while (0)
        
    if (!state) FAILED_RETURN_NULL;
    
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    if (!token_array.tokens) FAILED_RETURN_NULL;
    
    Scope_Prefix scope_prefix = {}; //scope prefix for current identifier
    String_Const_u8 identifier = {}; //current identifier cursor is at
    
    Scratch_Block scratch(app);
    if (Token *identifier_token = expect_token_from_pos(app, &token_array, pos, TokenBaseKind_Identifier)) {
        identifier = push_token_lexeme(app, scratch, buffer, identifier_token);
        
        Token_Iterator_Array it = token_iterator_pos(0, &token_array, identifier_token->pos - 1);
        Token *at = token_it_read(&it);
        
        auto is_scope_operator = [](Token *token) -> bool {
            return (token && token->kind == TokenBaseKind_Operator && token->sub_kind == TokenCppKind_ColonColon);
        };

        Token *first_token = identifier_token; //either another identifier or :: operator
        while (is_scope_operator(at)) {
            first_token = at;
            scope_prefix.scope_count += 1;
            token_it_dec_non_whitespace(&it);
            at = token_it_read(&it);
            
            if (at->kind == TokenBaseKind_Identifier) {
                first_token = at;
                token_it_dec_non_whitespace(&it);
                at = token_it_read(&it);
            } else {
                break; //token that shouldn't be part of the scoped identifier
            }
        }
        
        if (first_token != identifier_token) { //means we have a scope_prefix
            Range_i64 range = {first_token->pos, identifier_token->pos};
            scope_prefix.string = push_buffer_range(app, scratch, buffer, range);
            scope_prefix.string = string_remove_whitespace(scope_prefix.string);
            if (!is_scope_operator(first_token)) {
                //NOTE our helper functions that use Scope_Prefix should be doing this for us...
                scope_prefix.string = push_stringf(scratch, "::%.*s", string_expand(scope_prefix.string));
                scope_prefix.scope_count += 1;
            }
        } else {
            scope_prefix = get_entire_scope_prefix(app, scratch, buffer, pos);
        }
    }
    
    if (identifier.size == 0)                         FAILED_RETURN_NULL;
    if (identifier.size > countof(state->identifier)) FAILED_RETURN_NULL;
    
    if (scope_prefix.string.size > countof(state->scope_prefix)) FAILED_RETURN_NULL;
    
    state->goto_first_note = false;
    if (!string_match(SCu8(state->identifier, state->identifier_length), identifier)) {
        state->goto_first_note    = true;
        state->current_note_index = 0;
        state->identifier_length = (u32)identifier.size;
        block_copy(state->identifier, identifier.str, state->identifier_length);
        
        state->scope_prefix_length = (u32)scope_prefix.string.size;
        block_copy(state->scope_prefix, scope_prefix.string.str, state->scope_prefix_length);
        state->scope_prefix_scope_count = scope_prefix.scope_count;
    }
    return state;
}

function b32
luis_goto_code_index_note(Application_Links *app, View_ID peek, Code_Index_Peek_State *state, i32 target_note_index) {
    if (target_note_index < 0)         return false;
    if (state->identifier_length == 0) return false;
    
    String_Const_u8 identifier = {state->identifier, state->identifier_length};
    Scope_Prefix scope_prefix = {};
    scope_prefix.string = {state->scope_prefix, state->scope_prefix_length};
    scope_prefix.scope_count = state->scope_prefix_scope_count;
    
    
    auto get_match_metric = [&scope_prefix] (Scope_Prefix prefix) -> i32{
        i32 search_length = (i32)Min(prefix.string.size, scope_prefix.string.size);
        i32 match_count = 0;
        for (i32 i = 0; i < search_length; i += 1) {
            if (prefix.string.str[i] == scope_prefix.string.str[i]) {
                if (i > 0 && prefix.string.str[i-1] == ':' && prefix.string.str[i] == ':') {
                    match_count += 1;
                }
            }
            else break;
        }
        
        i32 non_match_count = prefix.scope_count - match_count;
            //i32 non_match_count = scope_prefix_scope_count - match_count;
        return match_count - 2*non_match_count;
    };
    
    i32 notes_found_count = 0;
    Code_Index_Note *notes_found[32];
    
    Scratch_Block scratch(app); //need this for get_entire_scope_prefix()
    Code_Index_Note *first_note = code_index_note_from_string(identifier);
    for (Code_Index_Note *note = first_note; note; note = note->next_in_hash) {
        if (notes_found_count == countof(notes_found)) break;
            //ignore type and function decls
        if (note->note_kind == CodeIndexNote_Type_Definition     ||
            note->note_kind == CodeIndexNote_Function_Definition ||
            note->note_kind == CodeIndexNote_Macro               ||
            note->note_kind == CodeIndexNote_Namespace) {    
            if (!string_match(note->text, identifier)) continue;
            
            notes_found[notes_found_count++] = note;
            Scope_Prefix note_prefix = get_entire_scope_prefix(app, scratch, note->file->buffer, note->pos.min);
            i32 note_metric = get_match_metric(note_prefix);
            
            //the higher the number, the better the result will be
            //add to array already sorted
            //insertion sort based on scope match metric
            for (i32 i = notes_found_count - 2; i >= 0; i -= 1) {
                Code_Index_Note *prev = notes_found[i];
                Assert (notes_found[i+1] == note); //NOTE (i+1) should always be our note
                
                Scope_Prefix prev_prefix = get_entire_scope_prefix(app, scratch, prev->file->buffer, prev->pos.min);
                i32 prev_metric = get_match_metric(prev_prefix);
                
                if (prev_metric < note_metric) {
                    SWAP(notes_found[i], notes_found[i+1]); 
                } else {
                    break; //we are ordered nothing else to do    
                }
            }    
        }
        
    }
    
    if (target_note_index < notes_found_count) {
        Code_Index_Note *note = notes_found[target_note_index];
        view_set_active(app, peek);
        view_set_buffer(app, peek, note->file->buffer, 0);
        view_set_cursor_and_preferred_x(app, peek, seek_pos(note->pos.first));
        view_set_mark(app, peek, seek_pos(note->pos.first));
        luis_center_view_top(app);
        return true;
    } else {
        return false;
    }
}

CUSTOM_COMMAND_SIG(luis_code_index_next)
CUSTOM_DOC("next code index") {
    View_ID view = get_active_view(app, Access_Always);
    View_ID peek;
    Code_Index_Peek_State *state = get_code_peek_state(app, view, view_get_cursor_pos(app, view), &peek);
    if (!state) return;
    
    i32 target_note_index = state->current_note_index + 1;
    if (state->goto_first_note) target_note_index = 0; 
        
    if (luis_goto_code_index_note(app, peek, state, target_note_index)) {
        state->current_note_index = target_note_index;
    } else if (state->goto_first_note) {
        //if we failed the first time, then close the view...
        view_close(app, peek);
    }
    
}

CUSTOM_COMMAND_SIG(luis_code_index_prev)
CUSTOM_DOC("prev code index") {
    View_ID view = get_active_view(app, Access_Always);
    View_ID peek;
    Code_Index_Peek_State *state = get_code_peek_state(app, view, view_get_cursor_pos(app, view), &peek);
    if (!state) return;
    
    i32 target_note_index = Max(0, state->current_note_index - 1);
    if (state->goto_first_note) target_note_index = 0;
        
    if (luis_goto_code_index_note(app, peek, state, target_note_index)) {
        state->current_note_index = target_note_index;
    } else if (state->goto_first_note) {
        //if we failed the first time, then close the view...
        view_close(app, peek);
    }
}


function void
luis_vim__fill_string_match_commands(Arena *arena, Lister *lister, String_Const_u8 selection_word, String_Const_u8 cursor_word) {
    
    struct Command_And_Desc {
        Custom_Command_Function *cmd;
        String_Const_u8 desc;
    };
    
    Command_And_Desc commands[] = {
        { list_all_locations,                            SCu8("Exact string matches for...")},
        { list_all_locations_case_insensitive,           SCu8("Ignore case string matches...")},
        { list_all_substring_locations,                  SCu8("Exact substring matches for...")},
        { list_all_substring_locations_case_insensitive, SCu8("Ignore case substring matches for...")},
    };
    
    for (i32 i = 0; i < ArrayCount(commands); i += 1) {
        Command_And_Desc *command = commands + i;
        Lister_Prealloced_String status = {};
        lister_add_item(lister, command->desc, status, (void *)command->cmd, 0);
    }
    
    if (selection_word.str) {
        Lister_Prealloced_String status = {};
        String_Const_u8 exact_string  = push_u8_stringf(lister->arena, "Exact matches for \"%.*s\"", string_expand(selection_word));
        String_Const_u8 ignore_string = push_u8_stringf(lister->arena, "Ignore case matches for \"%.*s\"", string_expand(selection_word));
        
        lister_add_item(lister, lister_prealloced(exact_string),  status, (void *)list_all_locations_of_selection, 0);
        lister_add_item(lister, lister_prealloced(ignore_string), status, (void *)list_all_locations_of_selection_case_insensitive, 0);
    }
    
    if (cursor_word.str) {
        Lister_Prealloced_String status = {};
        String_Const_u8 exact_string  = push_u8_stringf(lister->arena, "Exact matches for \"%.*s\"", string_expand(cursor_word));
        String_Const_u8 ignore_string = push_u8_stringf(lister->arena, "Ignore case matches for \"%.*s\"", string_expand(cursor_word));
        
        lister_add_item(lister, lister_prealloced(exact_string),  status, (void *)list_all_locations_of_identifier, 0);
        lister_add_item(lister, lister_prealloced(ignore_string), status, (void *)list_all_locations_of_identifier_case_insensitive, 0);
    }
}



CUSTOM_UI_COMMAND_SIG(luis_vim_string_matches)
CUSTOM_DOC("Enter Command Mode") {
    defer { minibar_string.size = 0; };
	View_ID view = get_this_ctx_view(app, Access_Always);
	if (view == 0){ return; }
    
    
	Scratch_Block scratch(app);
    String_Const_u8 selection_word = push_view_range_string(app, scratch);
    String_Const_u8 cursor_word = push_token_or_word_under_active_cursor(app, scratch);
    //print_message(app, SCu8("Selection and cursor match results are...\n"));
    //print_message(app, selection_word);
    //print_message(app, SCu8("\n"));
    //print_message(app, cursor_word);
    //print_message(app, SCu8("\n")); 
    
	Lister_Block lister(app, scratch);
	vim_lister_set_default_handlers(lister);
	lister_set_query(lister, string_u8_litexpr("Command:"));
	luis_vim__fill_string_match_commands(scratch, lister, selection_word, cursor_word);
    
    //minibar_string.size = 0;
    //vim_reset_bottom_text();
    //string_append(&vim_bot_text, string_u8_litexpr(":"));
    lister.lister.current->vim_max_col_count = 2;
	Lister_Result l_result = vim_run_lister(app, lister);
    Custom_Command_Function *cmd = l_result.canceled ? 0 : (Custom_Command_Function *)l_result.user_data;
    if (cmd) {
        view_enqueue_command_function(app, view, cmd);
    }
}

CUSTOM_COMMAND_SIG(luis_show_search_panel)
CUSTOM_DOC("Enter Command Mode") {
    Buffer_ID search_buffer = get_buffer_by_name(app, string_u8_litexpr("*search*"), Access_Always);
    if (!search_buffer) return;
    
    View_ID search_view = luis_find_first_view_with_buffer(app, search_buffer);
    if (!search_view) {
        View_ID active_view = get_active_view(app, Access_ReadVisible);
        search_view = luis_get_or_split_peek_window(app, active_view, ViewSplit_Bottom);
        if (search_view) {
            if (!view_set_buffer(app, search_view, search_buffer, 0)) search_view = 0;
        }
    }
    
    if (search_view) {
        lock_jump_buffer(app, search_buffer);
    }
}


internal void
luis_isearch(Application_Links *app, Scan_Direction start_scan, i64 first_pos, String_Const_u8 query_init) {
    View_ID view = get_active_view(app, Access_ReadVisible);
    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 buffer_size = buffer_get_size(app, buffer);
    if(buffer_size == 0)	return;
    
    
    
    Query_Bar_Group group(app);
    Query_Bar bar = {};
    if(!start_query_bar(app, &bar, 0))	return;
    
    Vec2_f32 old_margin = {};
    Vec2_f32 old_push_in = {};
    view_get_camera_bounds(app, view, &old_margin, &old_push_in);
    
    Vec2_f32 margin = old_margin;
    margin.y = clamp_bot(200.f, margin.y);
    view_set_camera_bounds(app, view, margin, old_push_in);
    
    Scan_Direction scan = start_scan;
    i64 pos = first_pos;
    
    u8 bar_string_space[256];
    bar.string = SCu8(bar_string_space, query_init.size);
    block_copy(bar.string.str, query_init.str, query_init.size);
    u64 match_size = bar.string.size;
    
    #define BAR_APPEND_STRING(string__) \
    do \
    { \
        String_Const_u8 string = (string__); \
        String_u8 bar_string = Su8(bar.string, sizeof(bar_string_space)); \
        string_append(&bar_string, string); \
        bar.string = bar_string.string; \
        string_change = true; \
    } while(0)
    
    b32 save_search_string = true;
    b32 move_to_new_pos = false;
    User_Input in = {};
    for (;;)
    {
        bar.prompt = (scan == Scan_Forward) ? string_u8_litexpr("I-Search: ") : string_u8_litexpr("Reverse-I-Search: ");
        isearch__update_highlight(app, view, Ii64_size(pos, match_size));
        
        in = get_next_input(app, EventPropertyGroup_Any, EventProperty_Escape);
        if (in.abort)	break;
        
        b32 string_change = false;
        b32 do_scan_action = false;
        b32 do_scroll_wheel = false;
        Scan_Direction change_scan = scan;
        if(in.event.kind == InputEventKind_KeyStroke)
        {
            Key_Code code = in.event.key.code;
            b32 ctrl_down = has_modifier(&in.event.key.modifiers, KeyCode_Control);
            
            if(code == KeyCode_Return || code == KeyCode_Tab)
            {
                if(ctrl_down) //append previous search string
                {
                    bar.string.size = cstring_length(previous_isearch_query);
                    block_copy(bar.string.str, previous_isearch_query, bar.string.size);
                }
                else
                {
                    //u64 size = bar.string.size;
                    //size = clamp_top(size, sizeof(previous_isearch_query) - 1);
                    //block_copy(previous_isearch_query, bar.string.str, size);
                    //previous_isearch_query[size] = 0;
                    move_to_new_pos = true;
                    break;
                }
            }
            else if(code == KeyCode_Backspace)
            {
                if (ctrl_down) {
                    if (bar.string.size > 0){
                        string_change = true;
                        bar.string.size = 0;
                    }
                } else {
                    u64 old_bar_string_size = bar.string.size;
                    bar.string = backspace_utf8(bar.string);
                    string_change = (bar.string.size < old_bar_string_size);
                }
            }
            else
            {
                View_Context ctx = view_current_context(app, view);
                Mapping *mapping = ctx.mapping;
                Command_Map *map = mapping_get_map(mapping, ctx.map_id);
                Command_Binding binding = map_get_binding_recursive(mapping, map, &in.event);
                if (binding.custom != 0)
                {
                    if(binding.custom == luis_fsearch || binding.custom == luis_rsearch)
                    {
                        if(binding.custom == luis_fsearch)
                        {
                            change_scan = Scan_Forward;
                            do_scan_action = true;
                            if(bar.string.size == 0)
                            {
                                bar.string.size = cstring_length(previous_isearch_query);
                                block_copy(bar.string.str, previous_isearch_query, bar.string.size);
                            }
                        }
                        else if(binding.custom == luis_rsearch)
                        {
                            change_scan = Scan_Backward;
                            do_scan_action = true;
                            if(bar.string.size == 0)
                            {
                                bar.string.size = cstring_length(previous_isearch_query);
                                block_copy(bar.string.str, previous_isearch_query, bar.string.size);
                            }
                        }
                    }
                    else if (binding.custom == luis_write_underscore)
                        BAR_APPEND_STRING(SCu8("_"));
                    else if (binding.custom == luis_write_pointer_arrow)
                        BAR_APPEND_STRING(SCu8("->"));
                    else if(binding.custom == word_complete) {
                        if(bar.string.size == 0) {
                            change_scan = Scan_Forward;
                            do_scan_action = true;
                            
                            Token *token = get_token_from_pos(app, buffer, pos);
                            if(token && (token->size > 0)) {
                                Scratch_Block scratch(app);
                                String_Const_u8 word = push_buffer_range(app, scratch, buffer, Ii64(token));
                                bar.string.size = word.size;
                                block_copy(bar.string.str, word.str, bar.string.size);
                                
                                pos = token->pos - 1;
                                if(pos < 0) pos = 0;
                            }
                        }
                    }
                    else
                    {
                        Command_Metadata *metadata = get_command_metadata(binding.custom);
                        if (metadata != 0){
                            if (metadata->is_ui){
                                view_enqueue_command_function(app, view, binding.custom);
                                break;
                            }
                        }
                        binding.custom(app);
                    }
                }
                else	leave_current_input_unhandled(app);
            }
            
        }
        else if(in.event.kind == InputEventKind_TextInsert)
            BAR_APPEND_STRING(to_writable(&in));
        
        if (string_change){
            switch (scan){
            case Scan_Forward:
            {
                i64 new_pos = 0;
                seek_string_insensitive_forward(app, buffer, pos - 1, 0, bar.string, &new_pos);
                if (new_pos < buffer_size){
                    pos = new_pos;
                    match_size = bar.string.size;
                }
            }break;
            
            case Scan_Backward:
            {
                i64 new_pos = 0;
                seek_string_insensitive_backward(app, buffer, pos + 1, 0, bar.string, &new_pos);
                if (new_pos >= 0){
                    pos = new_pos;
                    match_size = bar.string.size;
                }
            }break;
            }
        }
        else if (do_scan_action){
            scan = change_scan;
            switch (scan){
            case Scan_Forward:
            {
                i64 new_pos = 0;
                seek_string_insensitive_forward(app, buffer, pos, 0, bar.string, &new_pos);
                if (new_pos < buffer_size){
                    pos = new_pos;
                    match_size = bar.string.size;
                }
            }break;
            
            case Scan_Backward:
            {
                i64 new_pos = 0;
                seek_string_insensitive_backward(app, buffer, pos, 0, bar.string, &new_pos);
                if (new_pos >= 0){
                    pos = new_pos;
                    match_size = bar.string.size;
                }
            }break;
            }
        }
        else if (do_scroll_wheel){
            mouse_wheel_scroll(app);
        }
    }
    
    view_disable_highlight_range(app, view);
    
    if (move_to_new_pos)
    {
        view_set_cursor_and_preferred_x(app, view, seek_pos(pos));
        View_Buffer_Location *loc = view_get_prev_buffer_location(app, view);
        if(loc)
        {
            loc->buffer = buffer;
            loc->cursor = first_pos;
        }
    }
    else 
        view_set_cursor_and_preferred_x(app, view, seek_pos(first_pos));
    
    if(save_search_string)
    {
        u64 size = bar.string.size;
        size = clamp_top(size, sizeof(previous_isearch_query) - 1);
        block_copy(previous_isearch_query, bar.string.str, size);
        previous_isearch_query[size] = 0;
    }
    
    view_set_camera_bounds(app, view, old_margin, old_push_in);
}

CUSTOM_COMMAND_SIG(luis_fsearch)
CUSTOM_DOC("search forwards")
{
    View_ID view = get_active_view(app, Access_Always);
    luis_isearch(app, Scan_Forward, view_get_cursor_pos(app, view), SCu8());
}

CUSTOM_COMMAND_SIG(luis_rsearch)
CUSTOM_DOC("search backwards")
{
    View_ID view = get_active_view(app, Access_Always);
    luis_isearch(app, Scan_Backward, view_get_cursor_pos(app, view), SCu8());
}