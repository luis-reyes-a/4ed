
CUSTOM_COMMAND_SIG(luis_view_input_handler)
CUSTOM_DOC("Input consumption loop for default view behavior") {
    Scratch_Block scratch(app);
    default_input_handler_init(app, scratch);
    
    View_ID view = get_this_ctx_view(app, Access_Always);
    Managed_Scope scope = view_get_managed_scope(app, view);
    
    for (;;) {
        // NOTE(allen): Get input
        User_Input input = get_next_input(app, EventPropertyGroup_Any, 0);
        if (input.abort){
            break;
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
        default_pre_command(app, scope);
        ProfileCloseNow(view_input_profile);
        map_result.command(app);
        ProfileScope(app, "after view input");
        default_post_command(app, scope);
        
        if (view != get_this_ctx_view(app, Access_Always)) {
            Assert (0);
        }
    }
}

function void
luis_render_caller(Application_Links *app, Frame_Info frame_info, View_ID view_id) {
    ProfileScope(app, "default render caller");
    View_ID active_view = get_active_view(app, Access_Always);
    b32 is_active_view = (active_view == view_id);
    
    Rect_f32 region = draw_background_and_margin(app, view_id, is_active_view);
    Rect_f32 prev_clip = draw_set_clip(app, region);
    
    Buffer_ID buffer = view_get_buffer(app, view_id, Access_Always);
    Face_ID face_id = get_face_id(app, buffer);
    Face_Metrics face_metrics = get_face_metrics(app, face_id);
    f32 line_height = face_metrics.line_height;
    f32 digit_advance = face_metrics.decimal_digit_advance;
    
    // NOTE(allen): file bar
    b64 showing_file_bar = false;
    if (view_get_setting(app, view_id, ViewSetting_ShowFileBar, &showing_file_bar) && showing_file_bar){
        Rect_f32_Pair pair = layout_file_bar_on_top(region, line_height);
        draw_file_bar(app, view_id, buffer, face_id, pair.min);
        region = pair.max;
    }
    
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
    
    // NOTE(allen): draw the buffer
    default_render_buffer(app, view_id, face_id, buffer, text_layout_id, region);
    
    text_layout_free(app, text_layout_id);
    draw_set_clip(app, prev_clip);
}

function String_Const_u8
get_entire_scope_parents_at_pos(Application_Links *app, Arena *arena, Buffer_ID buffer, i64 pos) {
    
    Code_Index_File *code_index_file = code_index_get_file(buffer);
    if (!code_index_file) return {};
    
    Scratch_Block scratch(app);
    i32  outer_note_count = 0;
    i32  max_outer_note_count = 12;
    Code_Index_Note **outer_notes = push_array_zero(scratch, Code_Index_Note *, max_outer_note_count);
    
    for (Code_Index_Note *note = code_index_file->note_list.first; note; note = note->next) {
        if (outer_note_count == max_outer_note_count) break;
        
        if (pos >= note->nest_range.min && 
            pos <  note->nest_range.max) {
            outer_notes[outer_note_count++] = note;
        }
    }
    
            //insertion sort by greatest to smallest
    for (i32 i = 1; i < outer_note_count; i += 1) {
        
        auto get_size = [](Code_Index_Note *note) {
            return note->nest_range.max - note->nest_range.min;
        };
        
        i32 next = i;
        while (next > 0) {
            i32 prev = next - 1;
            if (get_size(outer_notes[next]) > get_size(outer_notes[prev])) {
                SWAP(outer_notes[next], outer_notes[prev]);
            }
            next = prev;
        }
    }
    
    String_u8 result = {};
    if (outer_note_count > 0) {
        result = string_u8_push(arena, 512);
        
        for (i32 i = 0; i < outer_note_count; i += 1) {
            Code_Index_Note *note = outer_notes[i];
            string_append(&result, note->text);
            string_append(&result, SCu8("::"));
            
        //Code_Index_Note *note = outer_notes[i];
        //push_fancy_stringf(scratch, &list, base_color, "%.*s::", string_expand(note->text)); //makes it in reverse order ezSadge
        }
    }
    
    return SCu8(result);
}


function void
luis_whole_screen_render_caller(Application_Links *app, Frame_Info frame_info) {
    
    Rect_f32 region = global_get_screen_rectangle(app);
    Face_ID face_id = get_face_id(app, 0);
    Face_Metrics face_metrics = get_face_metrics(app, face_id);
    f32 line_height = face_metrics.line_height;
    
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);

    i64 cursor_pos  = view_get_cursor_pos(app, view);
    Buffer_Cursor cursor = view_compute_cursor(app, view, seek_pos(cursor_pos));
    
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
        #if 1
        String_Const_u8 string = get_entire_scope_parents_at_pos(app, scratch, buffer, cursor_pos);
        if (string.size > 0) {
            push_fancy_stringf(scratch, &list, base_color, "%.*s", string_expand(string)); //makes it in reverse order ezSadge    
        }
        
        //if (code_index_file) {
            
            //push_fancy_stringf(scratch, &list, base_color, "%.*s::", string_expand(note->text)); //makes it in reverse order ezSadge
        //}
        #else
        if (tokens.tokens && code_index_file) {
        //if (string_match(extension, string_u8_litexpr("cpp")) || string_match(extension, string_u8_litexpr("h"))) {
            i64 pos = cursor_pos;
            
            i32  outer_note_count = 0;
            i32  max_outer_note_count = 12;
            Code_Index_Note **outer_notes = push_array_zero(scratch, Code_Index_Note *, max_outer_note_count);
            
            Range_i64 range = {};
            while (find_surrounding_nest(app, buffer, pos, FindNest_Scope, &range)) {
                
                Token_Iterator_Array it = token_iterator_pos(0, &tokens, range.min);
                Token *token = token_it_read(&it);
                if (token->kind == TokenBaseKind_ScopeOpen){
                    token_it_dec_non_whitespace(&it);
                    token = token_it_read(&it);
                    
                    if (token && token->kind == TokenBaseKind_Identifier) {
                        
                        String_Const_u8 identifier = push_token_lexeme(app, scratch, buffer, token);
                        
                        //NOTE if this is too slow we can just try to do a simple parse ourselves (look for namespace/struct/union right before the identifier)
                        Code_Index_Note *note = 0;
                        for (Code_Index_Note *node = code_index_file->note_list.first; node; node = node->next) {
                            if (string_match(node->text, identifier)) {
                                note = node;
                                break;
                            }
                        }
                        
                        if (note) {
                            //push_fancy_stringf(scratch, &list, base_color, "%.*s::", string_expand(note->text)); //makes it in reverse order ezSadge
                            if (outer_note_count < max_outer_note_count) {
                                outer_notes[outer_note_count++] = note;
                            }
                        }
                        
                    	//token_it_dec_non_whitespace(&it);
                    	//token = token_it_read(&it);
                    }
                }
                
                
                
                pos = range.min;
            }
            
            for (i32 i = outer_note_count-1; i >= 0; i -= 1) {
                Code_Index_Note *note = outer_notes[i];
                push_fancy_stringf(scratch, &list, base_color, "%.*s::", string_expand(note->text)); //makes it in reverse order ezSadge
            }
        }
        #endif
        
        
        
        String_Const_u8 unique_name = push_buffer_unique_name(app, scratch, buffer);
        push_fancy_string(scratch, &list, base_color, unique_name);
        push_fancy_stringf(scratch, &list, base_color, " - Row: %3.lld Col: %3.lld -", cursor.line, cursor.col);
        
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