#if !defined(META_PASS)
#define command_id(c) (fcoder_metacmd_ID_##c)
#define command_metadata(c) (&fcoder_metacmd_table[command_id(c)])
#define command_metadata_by_id(id) (&fcoder_metacmd_table[id])
#define command_one_past_last_id 342
#if defined(CUSTOM_COMMAND_SIG)
#define PROC_LINKS(x,y) x
#else
#define PROC_LINKS(x,y) y
#endif
#if defined(CUSTOM_COMMAND_SIG)
CUSTOM_COMMAND_SIG(__default_try_exit);
CUSTOM_COMMAND_SIG(allow_mouse);
CUSTOM_COMMAND_SIG(auto_indent_line_at_cursor);
CUSTOM_COMMAND_SIG(auto_indent_range);
CUSTOM_COMMAND_SIG(auto_indent_whole_file);
CUSTOM_COMMAND_SIG(backspace_alpha_numeric_boundary);
CUSTOM_COMMAND_SIG(backspace_char);
CUSTOM_COMMAND_SIG(basic_change_active_panel);
CUSTOM_COMMAND_SIG(begin_clipboard_collection_mode);
CUSTOM_COMMAND_SIG(build_in_build_panel);
CUSTOM_COMMAND_SIG(build_search);
CUSTOM_COMMAND_SIG(center_view);
CUSTOM_COMMAND_SIG(change_active_panel);
CUSTOM_COMMAND_SIG(change_active_panel_backwards);
CUSTOM_COMMAND_SIG(change_to_build_panel);
CUSTOM_COMMAND_SIG(clean_all_lines);
CUSTOM_COMMAND_SIG(clean_trailing_whitespace);
CUSTOM_COMMAND_SIG(clear_all_themes);
CUSTOM_COMMAND_SIG(clear_clipboard);
CUSTOM_COMMAND_SIG(click_set_cursor);
CUSTOM_COMMAND_SIG(click_set_cursor_and_mark);
CUSTOM_COMMAND_SIG(click_set_cursor_if_lbutton);
CUSTOM_COMMAND_SIG(click_set_mark);
CUSTOM_COMMAND_SIG(clipboard_record_clip);
CUSTOM_COMMAND_SIG(close_all_code);
CUSTOM_COMMAND_SIG(close_build_panel);
CUSTOM_COMMAND_SIG(close_panel);
CUSTOM_COMMAND_SIG(command_documentation);
CUSTOM_COMMAND_SIG(command_lister);
CUSTOM_COMMAND_SIG(comment_line);
CUSTOM_COMMAND_SIG(comment_line_toggle);
CUSTOM_COMMAND_SIG(copy);
CUSTOM_COMMAND_SIG(ctrl_tab_switch_buffer);
CUSTOM_COMMAND_SIG(cursor_mark_swap);
CUSTOM_COMMAND_SIG(custom_api_documentation);
CUSTOM_COMMAND_SIG(cut);
CUSTOM_COMMAND_SIG(decrease_face_size);
CUSTOM_COMMAND_SIG(default_file_externally_modified);
CUSTOM_COMMAND_SIG(default_startup);
CUSTOM_COMMAND_SIG(default_view_input_handler);
CUSTOM_COMMAND_SIG(delete_alpha_numeric_boundary);
CUSTOM_COMMAND_SIG(delete_char);
CUSTOM_COMMAND_SIG(delete_current_scope);
CUSTOM_COMMAND_SIG(delete_file_query);
CUSTOM_COMMAND_SIG(delete_line);
CUSTOM_COMMAND_SIG(delete_range);
CUSTOM_COMMAND_SIG(display_key_codes);
CUSTOM_COMMAND_SIG(display_text_input);
CUSTOM_COMMAND_SIG(double_backspace);
CUSTOM_COMMAND_SIG(duplicate_line);
CUSTOM_COMMAND_SIG(emacs_copy);
CUSTOM_COMMAND_SIG(emacs_cut);
CUSTOM_COMMAND_SIG(emacs_kill_line);
CUSTOM_COMMAND_SIG(emacs_swap_cursor_mark);
CUSTOM_COMMAND_SIG(execute_any_cli);
CUSTOM_COMMAND_SIG(execute_previous_cli);
CUSTOM_COMMAND_SIG(exit_4coder);
CUSTOM_COMMAND_SIG(go_to_user_directory);
CUSTOM_COMMAND_SIG(goto_beginning_of_file);
CUSTOM_COMMAND_SIG(goto_end_of_file);
CUSTOM_COMMAND_SIG(goto_first_jump);
CUSTOM_COMMAND_SIG(goto_first_jump_same_panel_sticky);
CUSTOM_COMMAND_SIG(goto_jump_at_cursor);
CUSTOM_COMMAND_SIG(goto_jump_at_cursor_same_panel);
CUSTOM_COMMAND_SIG(goto_line);
CUSTOM_COMMAND_SIG(goto_next_jump);
CUSTOM_COMMAND_SIG(goto_next_jump_no_skips);
CUSTOM_COMMAND_SIG(goto_prev_jump);
CUSTOM_COMMAND_SIG(goto_prev_jump_no_skips);
CUSTOM_COMMAND_SIG(grep);
CUSTOM_COMMAND_SIG(hide_filebar);
CUSTOM_COMMAND_SIG(hide_scrollbar);
CUSTOM_COMMAND_SIG(hit_sfx);
CUSTOM_COMMAND_SIG(hms_demo_tutorial);
CUSTOM_COMMAND_SIG(if0_off);
CUSTOM_COMMAND_SIG(if_read_only_goto_position);
CUSTOM_COMMAND_SIG(if_read_only_goto_position_same_panel);
CUSTOM_COMMAND_SIG(increase_face_size);
CUSTOM_COMMAND_SIG(interactive_kill_buffer);
CUSTOM_COMMAND_SIG(interactive_new);
CUSTOM_COMMAND_SIG(interactive_open);
CUSTOM_COMMAND_SIG(interactive_open_or_new);
CUSTOM_COMMAND_SIG(interactive_switch_buffer);
CUSTOM_COMMAND_SIG(jump_to_definition);
CUSTOM_COMMAND_SIG(jump_to_definition_at_cursor);
CUSTOM_COMMAND_SIG(jump_to_last_point);
CUSTOM_COMMAND_SIG(keyboard_macro_finish_recording);
CUSTOM_COMMAND_SIG(keyboard_macro_replay);
CUSTOM_COMMAND_SIG(keyboard_macro_start_recording);
CUSTOM_COMMAND_SIG(kill_buffer);
CUSTOM_COMMAND_SIG(kill_tutorial);
CUSTOM_COMMAND_SIG(left_adjust_view);
CUSTOM_COMMAND_SIG(list_all_functions_all_buffers);
CUSTOM_COMMAND_SIG(list_all_functions_all_buffers_lister);
CUSTOM_COMMAND_SIG(list_all_functions_current_buffer);
CUSTOM_COMMAND_SIG(list_all_functions_current_buffer_lister);
CUSTOM_COMMAND_SIG(list_all_locations);
CUSTOM_COMMAND_SIG(list_all_locations_case_insensitive);
CUSTOM_COMMAND_SIG(list_all_locations_of_identifier);
CUSTOM_COMMAND_SIG(list_all_locations_of_identifier_case_insensitive);
CUSTOM_COMMAND_SIG(list_all_locations_of_selection);
CUSTOM_COMMAND_SIG(list_all_locations_of_selection_case_insensitive);
CUSTOM_COMMAND_SIG(list_all_locations_of_type_definition);
CUSTOM_COMMAND_SIG(list_all_locations_of_type_definition_of_identifier);
CUSTOM_COMMAND_SIG(list_all_substring_locations);
CUSTOM_COMMAND_SIG(list_all_substring_locations_case_insensitive);
CUSTOM_COMMAND_SIG(load_project);
CUSTOM_COMMAND_SIG(load_theme_current_buffer);
CUSTOM_COMMAND_SIG(load_themes_default_folder);
CUSTOM_COMMAND_SIG(load_themes_hot_directory);
CUSTOM_COMMAND_SIG(luis_add_space);
CUSTOM_COMMAND_SIG(luis_add_tab);
CUSTOM_COMMAND_SIG(luis_adjust_horizontal_view_toggle);
CUSTOM_COMMAND_SIG(luis_backspace_char);
CUSTOM_COMMAND_SIG(luis_build);
CUSTOM_COMMAND_SIG(luis_center_view_top);
CUSTOM_COMMAND_SIG(luis_close_all_other_panels);
CUSTOM_COMMAND_SIG(luis_close_current_panel);
CUSTOM_COMMAND_SIG(luis_code_index_next);
CUSTOM_COMMAND_SIG(luis_code_index_prev);
CUSTOM_COMMAND_SIG(luis_delete_char);
CUSTOM_COMMAND_SIG(luis_eat_whitespace_backwards);
CUSTOM_COMMAND_SIG(luis_end);
CUSTOM_COMMAND_SIG(luis_escape);
CUSTOM_COMMAND_SIG(luis_fsearch);
CUSTOM_COMMAND_SIG(luis_goto_next_jump);
CUSTOM_COMMAND_SIG(luis_goto_prev_jump);
CUSTOM_COMMAND_SIG(luis_home);
CUSTOM_COMMAND_SIG(luis_horizontal_mouse_wheel_scroll);
CUSTOM_COMMAND_SIG(luis_indent_range);
CUSTOM_COMMAND_SIG(luis_interactive_open_or_new);
CUSTOM_COMMAND_SIG(luis_left_word);
CUSTOM_COMMAND_SIG(luis_list_functions_all_buffers);
CUSTOM_COMMAND_SIG(luis_list_nests_this_buffer);
CUSTOM_COMMAND_SIG(luis_list_notes_all_buffers);
CUSTOM_COMMAND_SIG(luis_list_notes_no_duplicates);
CUSTOM_COMMAND_SIG(luis_list_notes_this_buffer);
CUSTOM_COMMAND_SIG(luis_matching_file_cpp);
CUSTOM_COMMAND_SIG(luis_matching_file_cpp_same_buffer);
CUSTOM_COMMAND_SIG(luis_multiline_comment_toggle);
CUSTOM_COMMAND_SIG(luis_open_build_panel);
CUSTOM_COMMAND_SIG(luis_open_up_braces);
CUSTOM_COMMAND_SIG(luis_print_scope_variables);
CUSTOM_COMMAND_SIG(luis_remove_space);
CUSTOM_COMMAND_SIG(luis_remove_tab);
CUSTOM_COMMAND_SIG(luis_reselect_selection);
CUSTOM_COMMAND_SIG(luis_return);
CUSTOM_COMMAND_SIG(luis_right_word);
CUSTOM_COMMAND_SIG(luis_rsearch);
CUSTOM_COMMAND_SIG(luis_scope_braces);
CUSTOM_COMMAND_SIG(luis_select_line);
CUSTOM_COMMAND_SIG(luis_select_scope_contents);
CUSTOM_COMMAND_SIG(luis_select_surrounding_scope);
CUSTOM_COMMAND_SIG(luis_select_surrounding_scope_maximal);
CUSTOM_COMMAND_SIG(luis_select_word_under_cursor);
CUSTOM_COMMAND_SIG(luis_set_mark);
CUSTOM_COMMAND_SIG(luis_show_search_panel);
CUSTOM_COMMAND_SIG(luis_startup);
CUSTOM_COMMAND_SIG(luis_toggle_modal_mode);
CUSTOM_COMMAND_SIG(luis_try_exit);
CUSTOM_COMMAND_SIG(luis_view_input_handler);
CUSTOM_COMMAND_SIG(luis_view_peek_as_split_window);
CUSTOM_COMMAND_SIG(luis_view_peek_in_parent_window);
CUSTOM_COMMAND_SIG(luis_vim_string_matches);
CUSTOM_COMMAND_SIG(luis_write_newline);
CUSTOM_COMMAND_SIG(luis_write_pointer_arrow);
CUSTOM_COMMAND_SIG(luis_write_tab);
CUSTOM_COMMAND_SIG(luis_write_underscore);
CUSTOM_COMMAND_SIG(make_directory_query);
CUSTOM_COMMAND_SIG(miblo_decrement_basic);
CUSTOM_COMMAND_SIG(miblo_decrement_time_stamp);
CUSTOM_COMMAND_SIG(miblo_decrement_time_stamp_minute);
CUSTOM_COMMAND_SIG(miblo_increment_basic);
CUSTOM_COMMAND_SIG(miblo_increment_time_stamp);
CUSTOM_COMMAND_SIG(miblo_increment_time_stamp_minute);
CUSTOM_COMMAND_SIG(mouse_wheel_change_face_size);
CUSTOM_COMMAND_SIG(mouse_wheel_scroll);
CUSTOM_COMMAND_SIG(move_down);
CUSTOM_COMMAND_SIG(move_down_10);
CUSTOM_COMMAND_SIG(move_down_textual);
CUSTOM_COMMAND_SIG(move_down_to_blank_line);
CUSTOM_COMMAND_SIG(move_down_to_blank_line_end);
CUSTOM_COMMAND_SIG(move_down_to_blank_line_skip_whitespace);
CUSTOM_COMMAND_SIG(move_left);
CUSTOM_COMMAND_SIG(move_left_alpha_numeric_boundary);
CUSTOM_COMMAND_SIG(move_left_alpha_numeric_or_camel_boundary);
CUSTOM_COMMAND_SIG(move_left_token_boundary);
CUSTOM_COMMAND_SIG(move_left_whitespace_boundary);
CUSTOM_COMMAND_SIG(move_left_whitespace_or_token_boundary);
CUSTOM_COMMAND_SIG(move_line_down);
CUSTOM_COMMAND_SIG(move_line_up);
CUSTOM_COMMAND_SIG(move_right);
CUSTOM_COMMAND_SIG(move_right_alpha_numeric_boundary);
CUSTOM_COMMAND_SIG(move_right_alpha_numeric_or_camel_boundary);
CUSTOM_COMMAND_SIG(move_right_token_boundary);
CUSTOM_COMMAND_SIG(move_right_whitespace_boundary);
CUSTOM_COMMAND_SIG(move_right_whitespace_or_token_boundary);
CUSTOM_COMMAND_SIG(move_up);
CUSTOM_COMMAND_SIG(move_up_10);
CUSTOM_COMMAND_SIG(move_up_to_blank_line);
CUSTOM_COMMAND_SIG(move_up_to_blank_line_end);
CUSTOM_COMMAND_SIG(move_up_to_blank_line_skip_whitespace);
CUSTOM_COMMAND_SIG(multi_paste);
CUSTOM_COMMAND_SIG(multi_paste_interactive);
CUSTOM_COMMAND_SIG(multi_paste_interactive_quick);
CUSTOM_COMMAND_SIG(music_start);
CUSTOM_COMMAND_SIG(music_stop);
CUSTOM_COMMAND_SIG(navigate_back);
CUSTOM_COMMAND_SIG(navigate_forward);
CUSTOM_COMMAND_SIG(open_all_code);
CUSTOM_COMMAND_SIG(open_all_code_recursive);
CUSTOM_COMMAND_SIG(open_file_in_10x);
CUSTOM_COMMAND_SIG(open_file_in_quotes);
CUSTOM_COMMAND_SIG(open_file_in_visual_studio);
CUSTOM_COMMAND_SIG(open_in_other);
CUSTOM_COMMAND_SIG(open_long_braces);
CUSTOM_COMMAND_SIG(open_long_braces_break);
CUSTOM_COMMAND_SIG(open_long_braces_semicolon);
CUSTOM_COMMAND_SIG(open_matching_file_cpp);
CUSTOM_COMMAND_SIG(open_panel_hsplit);
CUSTOM_COMMAND_SIG(open_panel_vsplit);
CUSTOM_COMMAND_SIG(page_down);
CUSTOM_COMMAND_SIG(page_up);
CUSTOM_COMMAND_SIG(paste);
CUSTOM_COMMAND_SIG(paste_and_indent);
CUSTOM_COMMAND_SIG(paste_next);
CUSTOM_COMMAND_SIG(paste_next_and_indent);
CUSTOM_COMMAND_SIG(place_in_scope);
CUSTOM_COMMAND_SIG(play_with_a_counter);
CUSTOM_COMMAND_SIG(profile_clear);
CUSTOM_COMMAND_SIG(profile_disable);
CUSTOM_COMMAND_SIG(profile_enable);
CUSTOM_COMMAND_SIG(profile_inspect);
CUSTOM_COMMAND_SIG(project_command_F1);
CUSTOM_COMMAND_SIG(project_command_F10);
CUSTOM_COMMAND_SIG(project_command_F11);
CUSTOM_COMMAND_SIG(project_command_F12);
CUSTOM_COMMAND_SIG(project_command_F13);
CUSTOM_COMMAND_SIG(project_command_F14);
CUSTOM_COMMAND_SIG(project_command_F15);
CUSTOM_COMMAND_SIG(project_command_F16);
CUSTOM_COMMAND_SIG(project_command_F2);
CUSTOM_COMMAND_SIG(project_command_F3);
CUSTOM_COMMAND_SIG(project_command_F4);
CUSTOM_COMMAND_SIG(project_command_F5);
CUSTOM_COMMAND_SIG(project_command_F6);
CUSTOM_COMMAND_SIG(project_command_F7);
CUSTOM_COMMAND_SIG(project_command_F8);
CUSTOM_COMMAND_SIG(project_command_F9);
CUSTOM_COMMAND_SIG(project_command_lister);
CUSTOM_COMMAND_SIG(project_fkey_command);
CUSTOM_COMMAND_SIG(project_go_to_root_directory);
CUSTOM_COMMAND_SIG(project_reprint);
CUSTOM_COMMAND_SIG(query_replace);
CUSTOM_COMMAND_SIG(query_replace_identifier);
CUSTOM_COMMAND_SIG(query_replace_selection);
CUSTOM_COMMAND_SIG(quick_swap_buffer);
CUSTOM_COMMAND_SIG(redo);
CUSTOM_COMMAND_SIG(redo_all_buffers);
CUSTOM_COMMAND_SIG(reload_project);
CUSTOM_COMMAND_SIG(rename_file_query);
CUSTOM_COMMAND_SIG(reopen);
CUSTOM_COMMAND_SIG(replace_in_all_buffers);
CUSTOM_COMMAND_SIG(replace_in_buffer);
CUSTOM_COMMAND_SIG(replace_in_range);
CUSTOM_COMMAND_SIG(reverse_search);
CUSTOM_COMMAND_SIG(reverse_search_identifier);
CUSTOM_COMMAND_SIG(revert_buffer);
CUSTOM_COMMAND_SIG(save);
CUSTOM_COMMAND_SIG(save_all_dirty_buffers);
CUSTOM_COMMAND_SIG(save_to_query);
CUSTOM_COMMAND_SIG(search);
CUSTOM_COMMAND_SIG(search_identifier);
CUSTOM_COMMAND_SIG(seek_beginning_of_line);
CUSTOM_COMMAND_SIG(seek_beginning_of_textual_line);
CUSTOM_COMMAND_SIG(seek_end_of_line);
CUSTOM_COMMAND_SIG(seek_end_of_textual_line);
CUSTOM_COMMAND_SIG(select_all);
CUSTOM_COMMAND_SIG(select_next_scope_absolute);
CUSTOM_COMMAND_SIG(select_next_scope_after_current);
CUSTOM_COMMAND_SIG(select_prev_scope_absolute);
CUSTOM_COMMAND_SIG(select_prev_top_most_scope);
CUSTOM_COMMAND_SIG(select_surrounding_scope);
CUSTOM_COMMAND_SIG(select_surrounding_scope_maximal);
CUSTOM_COMMAND_SIG(set_eol_mode_from_contents);
CUSTOM_COMMAND_SIG(set_eol_mode_to_binary);
CUSTOM_COMMAND_SIG(set_eol_mode_to_crlf);
CUSTOM_COMMAND_SIG(set_eol_mode_to_lf);
CUSTOM_COMMAND_SIG(set_face_size);
CUSTOM_COMMAND_SIG(set_face_size_this_buffer);
CUSTOM_COMMAND_SIG(set_mark);
CUSTOM_COMMAND_SIG(set_mode_to_notepad_like);
CUSTOM_COMMAND_SIG(set_mode_to_original);
CUSTOM_COMMAND_SIG(setup_build_bat);
CUSTOM_COMMAND_SIG(setup_build_bat_and_sh);
CUSTOM_COMMAND_SIG(setup_build_sh);
CUSTOM_COMMAND_SIG(setup_new_project);
CUSTOM_COMMAND_SIG(show_file_in_explorer);
CUSTOM_COMMAND_SIG(show_filebar);
CUSTOM_COMMAND_SIG(show_scrollbar);
CUSTOM_COMMAND_SIG(show_the_log_graph);
CUSTOM_COMMAND_SIG(snipe_backward_whitespace_or_token_boundary);
CUSTOM_COMMAND_SIG(snipe_forward_whitespace_or_token_boundary);
CUSTOM_COMMAND_SIG(snippet_lister);
CUSTOM_COMMAND_SIG(string_repeat);
CUSTOM_COMMAND_SIG(suppress_mouse);
CUSTOM_COMMAND_SIG(swap_panels);
CUSTOM_COMMAND_SIG(theme_lister);
CUSTOM_COMMAND_SIG(to_lowercase);
CUSTOM_COMMAND_SIG(to_uppercase);
CUSTOM_COMMAND_SIG(toggle_filebar);
CUSTOM_COMMAND_SIG(toggle_fps_meter);
CUSTOM_COMMAND_SIG(toggle_fullscreen);
CUSTOM_COMMAND_SIG(toggle_highlight_enclosing_scopes);
CUSTOM_COMMAND_SIG(toggle_highlight_line_at_cursor);
CUSTOM_COMMAND_SIG(toggle_line_numbers);
CUSTOM_COMMAND_SIG(toggle_line_wrap);
CUSTOM_COMMAND_SIG(toggle_mouse);
CUSTOM_COMMAND_SIG(toggle_paren_matching_helper);
CUSTOM_COMMAND_SIG(toggle_show_whitespace);
CUSTOM_COMMAND_SIG(toggle_text_drop_shadow);
CUSTOM_COMMAND_SIG(toggle_virtual_whitespace);
CUSTOM_COMMAND_SIG(tutorial_maximize);
CUSTOM_COMMAND_SIG(tutorial_minimize);
CUSTOM_COMMAND_SIG(uncomment_line);
CUSTOM_COMMAND_SIG(undo);
CUSTOM_COMMAND_SIG(undo_all_buffers);
CUSTOM_COMMAND_SIG(view_buffer_other_panel);
CUSTOM_COMMAND_SIG(view_jump_list_with_lister);
CUSTOM_COMMAND_SIG(word_complete);
CUSTOM_COMMAND_SIG(word_complete_drop_down);
CUSTOM_COMMAND_SIG(write_block);
CUSTOM_COMMAND_SIG(write_filename);
CUSTOM_COMMAND_SIG(write_filepath);
CUSTOM_COMMAND_SIG(write_hack);
CUSTOM_COMMAND_SIG(write_note);
CUSTOM_COMMAND_SIG(write_space);
CUSTOM_COMMAND_SIG(write_text_and_auto_indent);
CUSTOM_COMMAND_SIG(write_text_input);
CUSTOM_COMMAND_SIG(write_todo);
CUSTOM_COMMAND_SIG(write_underscore);
CUSTOM_COMMAND_SIG(write_zero_struct);
#endif
struct Command_Metadata{
PROC_LINKS(Custom_Command_Function, void) *proc;
b32 is_ui;
char *name;
i32 name_len;
char *description;
i32 description_len;
char *source_name;
i32 source_name_len;
i32 line_number;
};
static Command_Metadata fcoder_metacmd_table[342] = {
{ PROC_LINKS(__default_try_exit, 0), false, "__default_try_exit", 18, "Default command for responding to a try-exit event", 50, "D:\\work\\4ed\\code\\custom\\4coder_default_hooks.cpp", 48, 33 },
{ PROC_LINKS(allow_mouse, 0), false, "allow_mouse", 11, "Shows the mouse and causes all mouse input to be processed normally.", 68, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 481 },
{ PROC_LINKS(auto_indent_line_at_cursor, 0), false, "auto_indent_line_at_cursor", 26, "Auto-indents the line on which the cursor sits.", 47, "D:\\work\\4ed\\code\\custom\\4coder_auto_indent.cpp", 46, 466 },
{ PROC_LINKS(auto_indent_range, 0), false, "auto_indent_range", 17, "Auto-indents the range between the cursor and the mark.", 55, "D:\\work\\4ed\\code\\custom\\4coder_auto_indent.cpp", 46, 476 },
{ PROC_LINKS(auto_indent_whole_file, 0), false, "auto_indent_whole_file", 22, "Audo-indents the entire current buffer.", 39, "D:\\work\\4ed\\code\\custom\\4coder_auto_indent.cpp", 46, 457 },
{ PROC_LINKS(backspace_alpha_numeric_boundary, 0), false, "backspace_alpha_numeric_boundary", 32, "Delete characters between the cursor position and the first alphanumeric boundary to the left.", 94, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 220 },
{ PROC_LINKS(backspace_char, 0), false, "backspace_char", 14, "Deletes the character to the left of the cursor.", 48, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 162 },
{ PROC_LINKS(basic_change_active_panel, 0), false, "basic_change_active_panel", 25, "Change the currently active panel, moving to the panel with the next highest view_id.  Will not skipe the build panel if it is open.", 132, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 734 },
{ PROC_LINKS(begin_clipboard_collection_mode, 0), true, "begin_clipboard_collection_mode", 31, "Allows the user to copy multiple strings from other applications before switching to 4coder and pasting them all.", 113, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 71 },
{ PROC_LINKS(build_in_build_panel, 0), false, "build_in_build_panel", 20, "Looks for a build.bat, build.sh, or makefile in the current and parent directories.  Runs the first that it finds and prints the output to *compilation*.  Puts the *compilation* buffer in a panel at the footer of the current view.", 230, "D:\\work\\4ed\\code\\custom\\4coder_build_commands.cpp", 49, 160 },
{ PROC_LINKS(build_search, 0), false, "build_search", 12, "Looks for a build.bat, build.sh, or makefile in the current and parent directories.  Runs the first that it finds and prints the output to *compilation*.", 153, "D:\\work\\4ed\\code\\custom\\4coder_build_commands.cpp", 49, 123 },
{ PROC_LINKS(center_view, 0), false, "center_view", 11, "Centers the view vertically on the line on which the cursor sits.", 65, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 263 },
{ PROC_LINKS(change_active_panel, 0), false, "change_active_panel", 19, "Change the currently active panel, moving to the panel with the next highest view_id.", 85, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 356 },
{ PROC_LINKS(change_active_panel_backwards, 0), false, "change_active_panel_backwards", 29, "Change the currently active panel, moving to the panel with the next lowest view_id.", 84, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 362 },
{ PROC_LINKS(change_to_build_panel, 0), false, "change_to_build_panel", 21, "If the special build panel is open, makes the build panel the active panel.", 75, "D:\\work\\4ed\\code\\custom\\4coder_build_commands.cpp", 49, 181 },
{ PROC_LINKS(clean_all_lines, 0), false, "clean_all_lines", 15, "Removes trailing whitespace from all lines and removes all blank lines in the current buffer.", 93, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 714 },
{ PROC_LINKS(clean_trailing_whitespace, 0), false, "clean_trailing_whitespace", 25, "Removes trailing whitespace from all lines in the current buffer.", 65, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 723 },
{ PROC_LINKS(clear_all_themes, 0), false, "clear_all_themes", 16, "Clear the theme list", 20, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 565 },
{ PROC_LINKS(clear_clipboard, 0), false, "clear_clipboard", 15, "Clears the history of the clipboard", 35, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 222 },
{ PROC_LINKS(click_set_cursor, 0), false, "click_set_cursor", 16, "Sets the cursor position to the mouse position.", 47, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 301 },
{ PROC_LINKS(click_set_cursor_and_mark, 0), false, "click_set_cursor_and_mark", 25, "Sets the cursor position and mark to the mouse position.", 56, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 291 },
{ PROC_LINKS(click_set_cursor_if_lbutton, 0), false, "click_set_cursor_if_lbutton", 27, "If the mouse left button is pressed, sets the cursor position to the mouse position.", 84, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 311 },
{ PROC_LINKS(click_set_mark, 0), false, "click_set_mark", 14, "Sets the mark position to the mouse position.", 45, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 324 },
{ PROC_LINKS(clipboard_record_clip, 0), false, "clipboard_record_clip", 21, "In response to a new clipboard contents events, saves the new clip onto the clipboard history", 93, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 7 },
{ PROC_LINKS(close_all_code, 0), false, "close_all_code", 14, "Closes any buffer with a filename ending with an extension configured to be recognized as a code file type.", 107, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 829 },
{ PROC_LINKS(close_build_panel, 0), false, "close_build_panel", 17, "If the special build panel is open, closes it.", 46, "D:\\work\\4ed\\code\\custom\\4coder_build_commands.cpp", 49, 175 },
{ PROC_LINKS(close_panel, 0), false, "close_panel", 11, "Closes the currently active panel if it is not the only panel open.", 67, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 742 },
{ PROC_LINKS(command_documentation, 0), true, "command_documentation", 21, "Prompts the user to select a command then loads a doc buffer for that item", 74, "D:\\work\\4ed\\code\\custom\\4coder_docs.cpp", 39, 190 },
{ PROC_LINKS(command_lister, 0), true, "command_lister", 14, "Opens an interactive list of all registered commands.", 53, "D:\\work\\4ed\\code\\custom\\4coder_lists.cpp", 40, 780 },
{ PROC_LINKS(comment_line, 0), false, "comment_line", 12, "Insert '//' at the beginning of the line after leading whitespace.", 66, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 125 },
{ PROC_LINKS(comment_line_toggle, 0), false, "comment_line_toggle", 19, "Turns uncommented lines into commented lines and vice versa for comments starting with '//'.", 92, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 149 },
{ PROC_LINKS(copy, 0), false, "copy", 4, "Copy the text in the range from the cursor to the mark onto the clipboard.", 74, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 110 },
{ PROC_LINKS(ctrl_tab_switch_buffer, 0), true, "ctrl_tab_switch_buffer", 22, "Alt-tab behaviour way of switching buffer", 41, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 2255 },
{ PROC_LINKS(cursor_mark_swap, 0), false, "cursor_mark_swap", 16, "Swaps the position of the cursor and the mark.", 46, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 190 },
{ PROC_LINKS(custom_api_documentation, 0), true, "custom_api_documentation", 24, "Prompts the user to select a Custom API item then loads a doc buffer for that item", 82, "D:\\work\\4ed\\code\\custom\\4coder_docs.cpp", 39, 175 },
{ PROC_LINKS(cut, 0), false, "cut", 3, "Cut the text in the range from the cursor to the mark onto the clipboard.", 73, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 119 },
{ PROC_LINKS(decrease_face_size, 0), false, "decrease_face_size", 18, "Decrease the size of the face used by the current buffer.", 57, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 825 },
{ PROC_LINKS(default_file_externally_modified, 0), false, "default_file_externally_modified", 32, "Notes the external modification of attached files by printing a message.", 72, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 2129 },
{ PROC_LINKS(default_startup, 0), false, "default_startup", 15, "Default command for responding to a startup event", 49, "D:\\work\\4ed\\code\\custom\\4coder_default_hooks.cpp", 48, 7 },
{ PROC_LINKS(default_view_input_handler, 0), false, "default_view_input_handler", 26, "Input consumption loop for default view behavior", 48, "D:\\work\\4ed\\code\\custom\\4coder_default_hooks.cpp", 48, 77 },
{ PROC_LINKS(delete_alpha_numeric_boundary, 0), false, "delete_alpha_numeric_boundary", 29, "Delete characters between the cursor position and the first alphanumeric boundary to the right.", 95, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 228 },
{ PROC_LINKS(delete_char, 0), false, "delete_char", 11, "Deletes the character to the right of the cursor.", 49, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 145 },
{ PROC_LINKS(delete_current_scope, 0), false, "delete_current_scope", 20, "Deletes the braces surrounding the currently selected scope.  Leaves the contents within the scope.", 99, "D:\\work\\4ed\\code\\custom\\4coder_scope_commands.cpp", 49, 112 },
{ PROC_LINKS(delete_file_query, 0), false, "delete_file_query", 17, "Deletes the file of the current buffer if 4coder has the appropriate access rights. Will ask the user for confirmation first.", 125, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1447 },
{ PROC_LINKS(delete_line, 0), false, "delete_line", 11, "Delete the line the on which the cursor sits.", 45, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1619 },
{ PROC_LINKS(delete_range, 0), false, "delete_range", 12, "Deletes the text in the range between the cursor and the mark.", 62, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 200 },
{ PROC_LINKS(display_key_codes, 0), false, "display_key_codes", 17, "Example of input handling loop", 30, "D:\\work\\4ed\\code\\custom\\4coder_examples.cpp", 43, 90 },
{ PROC_LINKS(display_text_input, 0), false, "display_text_input", 18, "Example of to_writable and leave_current_input_unhandled", 56, "D:\\work\\4ed\\code\\custom\\4coder_examples.cpp", 43, 137 },
{ PROC_LINKS(double_backspace, 0), false, "double_backspace", 16, "Example of history group helpers", 32, "D:\\work\\4ed\\code\\custom\\4coder_examples.cpp", 43, 10 },
{ PROC_LINKS(duplicate_line, 0), false, "duplicate_line", 14, "Create a copy of the line on which the cursor sits.", 51, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1605 },
{ PROC_LINKS(emacs_copy, 0), false, "emacs_copy", 10, "emacs cut or 'kill'", 19, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 248 },
{ PROC_LINKS(emacs_cut, 0), false, "emacs_cut", 9, "emacs cut or 'kill'", 19, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 242 },
{ PROC_LINKS(emacs_kill_line, 0), false, "emacs_kill_line", 15, "Emacs-style kill line", 21, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 547 },
{ PROC_LINKS(emacs_swap_cursor_mark, 0), false, "emacs_swap_cursor_mark", 22, "swap cursor and mark (works with notepad mode)", 46, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 589 },
{ PROC_LINKS(execute_any_cli, 0), false, "execute_any_cli", 15, "Queries for an output buffer name and system command, runs the system command as a CLI and prints the output to the specified buffer.", 133, "D:\\work\\4ed\\code\\custom\\4coder_cli_command.cpp", 46, 32 },
{ PROC_LINKS(execute_previous_cli, 0), false, "execute_previous_cli", 20, "If the command execute_any_cli has already been used, this will execute a CLI reusing the most recent buffer name and command.", 126, "D:\\work\\4ed\\code\\custom\\4coder_cli_command.cpp", 46, 7 },
{ PROC_LINKS(exit_4coder, 0), false, "exit_4coder", 11, "Attempts to close 4coder.", 25, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 914 },
{ PROC_LINKS(go_to_user_directory, 0), false, "go_to_user_directory", 20, "Go to the 4coder user directory", 31, "D:\\work\\4ed\\code\\custom\\4coder_config.cpp", 41, 1655 },
{ PROC_LINKS(goto_beginning_of_file, 0), false, "goto_beginning_of_file", 22, "Sets the cursor to the beginning of the file.", 45, "D:\\work\\4ed\\code\\custom\\4coder_helper.cpp", 41, 2269 },
{ PROC_LINKS(goto_end_of_file, 0), false, "goto_end_of_file", 16, "Sets the cursor to the end of the file.", 39, "D:\\work\\4ed\\code\\custom\\4coder_helper.cpp", 41, 2277 },
{ PROC_LINKS(goto_first_jump, 0), false, "goto_first_jump", 15, "If a buffer containing jump locations has been locked in, goes to the first jump in the buffer.", 95, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 529 },
{ PROC_LINKS(goto_first_jump_same_panel_sticky, 0), false, "goto_first_jump_same_panel_sticky", 33, "If a buffer containing jump locations has been locked in, goes to the first jump in the buffer and views the buffer in the panel where the jump list was.", 153, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 546 },
{ PROC_LINKS(goto_jump_at_cursor, 0), false, "goto_jump_at_cursor", 19, "If the cursor is found to be on a jump location, parses the jump location and brings up the file and position in another view and changes the active panel to the view containing the jump.", 187, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 348 },
{ PROC_LINKS(goto_jump_at_cursor_same_panel, 0), false, "goto_jump_at_cursor_same_panel", 30, "If the cursor is found to be on a jump location, parses the jump location and brings up the file and position in this view, losing the compilation output or jump list.", 167, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 375 },
{ PROC_LINKS(goto_line, 0), false, "goto_line", 9, "Queries the user for a number, and jumps the cursor to the corresponding line.", 78, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 922 },
{ PROC_LINKS(goto_next_jump, 0), false, "goto_next_jump", 14, "If a buffer containing jump locations has been locked in, goes to the next jump in the buffer, skipping sub jump locations.", 123, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 468 },
{ PROC_LINKS(goto_next_jump_no_skips, 0), false, "goto_next_jump_no_skips", 23, "If a buffer containing jump locations has been locked in, goes to the next jump in the buffer, and does not skip sub jump locations.", 132, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 498 },
{ PROC_LINKS(goto_prev_jump, 0), false, "goto_prev_jump", 14, "If a buffer containing jump locations has been locked in, goes to the previous jump in the buffer, skipping sub jump locations.", 127, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 485 },
{ PROC_LINKS(goto_prev_jump_no_skips, 0), false, "goto_prev_jump_no_skips", 23, "If a buffer containing jump locations has been locked in, goes to the previous jump in the buffer, and does not skip sub jump locations.", 136, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 515 },
{ PROC_LINKS(grep, 0), false, "grep", 4, "Runs grep and puts contents into *grep* buffer", 46, "D:\\work\\4ed\\code\\custom\\4coder_cli_command.cpp", 46, 65 },
{ PROC_LINKS(hide_filebar, 0), false, "hide_filebar", 12, "Sets the current view to hide it's filebar.", 43, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 772 },
{ PROC_LINKS(hide_scrollbar, 0), false, "hide_scrollbar", 14, "Sets the current view to hide it's scrollbar.", 45, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 758 },
{ PROC_LINKS(hit_sfx, 0), false, "hit_sfx", 7, "Play the hit sound effect", 25, "D:\\work\\4ed\\code\\custom\\4coder_examples.cpp", 43, 240 },
{ PROC_LINKS(hms_demo_tutorial, 0), false, "hms_demo_tutorial", 17, "Tutorial for built in 4coder bindings and features.", 51, "D:\\work\\4ed\\code\\custom\\4coder_tutorial.cpp", 43, 869 },
{ PROC_LINKS(if0_off, 0), false, "if0_off", 7, "Surround the range between the cursor and mark with an '#if 0' and an '#endif'", 78, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 70 },
{ PROC_LINKS(if_read_only_goto_position, 0), false, "if_read_only_goto_position", 26, "If the buffer in the active view is writable, inserts a character, otherwise performs goto_jump_at_cursor.", 106, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 568 },
{ PROC_LINKS(if_read_only_goto_position_same_panel, 0), false, "if_read_only_goto_position_same_panel", 37, "If the buffer in the active view is writable, inserts a character, otherwise performs goto_jump_at_cursor_same_panel.", 117, "D:\\work\\4ed\\code\\custom\\4coder_jump_sticky.cpp", 46, 585 },
{ PROC_LINKS(increase_face_size, 0), false, "increase_face_size", 18, "Increase the size of the face used by the current buffer.", 57, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 814 },
{ PROC_LINKS(interactive_kill_buffer, 0), true, "interactive_kill_buffer", 23, "Interactively kill an open buffer.", 34, "D:\\work\\4ed\\code\\custom\\4coder_lists.cpp", 40, 540 },
{ PROC_LINKS(interactive_new, 0), true, "interactive_new", 15, "Interactively creates a new file.", 33, "D:\\work\\4ed\\code\\custom\\4coder_lists.cpp", 40, 680 },
{ PROC_LINKS(interactive_open, 0), true, "interactive_open", 16, "Interactively opens a file.", 27, "D:\\work\\4ed\\code\\custom\\4coder_lists.cpp", 40, 734 },
{ PROC_LINKS(interactive_open_or_new, 0), true, "interactive_open_or_new", 23, "Interactively open a file out of the file system.", 49, "D:\\work\\4ed\\code\\custom\\4coder_lists.cpp", 40, 631 },
{ PROC_LINKS(interactive_switch_buffer, 0), true, "interactive_switch_buffer", 25, "Interactively switch to an open buffer.", 39, "D:\\work\\4ed\\code\\custom\\4coder_lists.cpp", 40, 530 },
{ PROC_LINKS(jump_to_definition, 0), true, "jump_to_definition", 18, "List all definitions in the code index and jump to one chosen by the user.", 74, "D:\\work\\4ed\\code\\custom\\4coder_code_index_listers.cpp", 53, 12 },
{ PROC_LINKS(jump_to_definition_at_cursor, 0), true, "jump_to_definition_at_cursor", 28, "Jump to the first definition in the code index matching an identifier at the cursor", 83, "D:\\work\\4ed\\code\\custom\\4coder_code_index_listers.cpp", 53, 68 },
{ PROC_LINKS(jump_to_last_point, 0), false, "jump_to_last_point", 18, "Read from the top of the point stack and jump there; if already there pop the top and go to the next option", 107, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1400 },
{ PROC_LINKS(keyboard_macro_finish_recording, 0), false, "keyboard_macro_finish_recording", 31, "Stop macro recording, do nothing if macro recording is not already started", 74, "D:\\work\\4ed\\code\\custom\\4coder_keyboard_macro.cpp", 49, 54 },
{ PROC_LINKS(keyboard_macro_replay, 0), false, "keyboard_macro_replay", 21, "Replay the most recently recorded keyboard macro", 48, "D:\\work\\4ed\\code\\custom\\4coder_keyboard_macro.cpp", 49, 77 },
{ PROC_LINKS(keyboard_macro_start_recording, 0), false, "keyboard_macro_start_recording", 30, "Start macro recording, do nothing if macro recording is already started", 71, "D:\\work\\4ed\\code\\custom\\4coder_keyboard_macro.cpp", 49, 41 },
{ PROC_LINKS(kill_buffer, 0), false, "kill_buffer", 11, "Kills the current buffer.", 25, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1790 },
{ PROC_LINKS(kill_tutorial, 0), false, "kill_tutorial", 13, "If there is an active tutorial, kill it.", 40, "D:\\work\\4ed\\code\\custom\\4coder_tutorial.cpp", 43, 9 },
{ PROC_LINKS(left_adjust_view, 0), false, "left_adjust_view", 16, "Sets the left size of the view near the x position of the cursor.", 65, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 278 },
{ PROC_LINKS(list_all_functions_all_buffers, 0), false, "list_all_functions_all_buffers", 30, "Creates a jump list of lines from all buffers that appear to define or declare functions.", 89, "D:\\work\\4ed\\code\\custom\\4coder_function_list.cpp", 48, 296 },
{ PROC_LINKS(list_all_functions_all_buffers_lister, 0), true, "list_all_functions_all_buffers_lister", 37, "Creates a lister of locations that look like function definitions and declarations all buffers.", 95, "D:\\work\\4ed\\code\\custom\\4coder_function_list.cpp", 48, 302 },
{ PROC_LINKS(list_all_functions_current_buffer, 0), false, "list_all_functions_current_buffer", 33, "Creates a jump list of lines of the current buffer that appear to define or declare functions.", 94, "D:\\work\\4ed\\code\\custom\\4coder_function_list.cpp", 48, 268 },
{ PROC_LINKS(list_all_functions_current_buffer_lister, 0), true, "list_all_functions_current_buffer_lister", 40, "Creates a lister of locations that look like function definitions and declarations in the buffer.", 97, "D:\\work\\4ed\\code\\custom\\4coder_function_list.cpp", 48, 278 },
{ PROC_LINKS(list_all_locations, 0), false, "list_all_locations", 18, "Queries the user for a string and lists all exact case-sensitive matches found in all open buffers.", 99, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 199 },
{ PROC_LINKS(list_all_locations_case_insensitive, 0), false, "list_all_locations_case_insensitive", 35, "Queries the user for a string and lists all exact case-insensitive matches found in all open buffers.", 101, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 211 },
{ PROC_LINKS(list_all_locations_of_identifier, 0), false, "list_all_locations_of_identifier", 32, "Reads a token or word under the cursor and lists all exact case-sensitive mathces in all open buffers.", 102, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 223 },
{ PROC_LINKS(list_all_locations_of_identifier_case_insensitive, 0), false, "list_all_locations_of_identifier_case_insensitive", 49, "Reads a token or word under the cursor and lists all exact case-insensitive mathces in all open buffers.", 104, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 229 },
{ PROC_LINKS(list_all_locations_of_selection, 0), false, "list_all_locations_of_selection", 31, "Reads the string in the selected range and lists all exact case-sensitive mathces in all open buffers.", 102, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 235 },
{ PROC_LINKS(list_all_locations_of_selection_case_insensitive, 0), false, "list_all_locations_of_selection_case_insensitive", 48, "Reads the string in the selected range and lists all exact case-insensitive mathces in all open buffers.", 104, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 241 },
{ PROC_LINKS(list_all_locations_of_type_definition, 0), false, "list_all_locations_of_type_definition", 37, "Queries user for string, lists all locations of strings that appear to define a type whose name matches the input string.", 121, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 247 },
{ PROC_LINKS(list_all_locations_of_type_definition_of_identifier, 0), false, "list_all_locations_of_type_definition_of_identifier", 51, "Reads a token or word under the cursor and lists all locations of strings that appear to define a type whose name matches it.", 125, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 255 },
{ PROC_LINKS(list_all_substring_locations, 0), false, "list_all_substring_locations", 28, "Queries the user for a string and lists all case-sensitive substring matches found in all open buffers.", 103, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 205 },
{ PROC_LINKS(list_all_substring_locations_case_insensitive, 0), false, "list_all_substring_locations_case_insensitive", 45, "Queries the user for a string and lists all case-insensitive substring matches found in all open buffers.", 105, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 217 },
{ PROC_LINKS(load_project, 0), false, "load_project", 12, "Looks for a project.4coder file in the current directory and tries to load it.  Looks in parent directories until a project file is found or there are no more parents.", 167, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 856 },
{ PROC_LINKS(load_theme_current_buffer, 0), false, "load_theme_current_buffer", 25, "Parse the current buffer as a theme file and add the theme to the theme list. If the buffer has a .4coder postfix in it's name, it is removed when the name is saved.", 165, "D:\\work\\4ed\\code\\custom\\4coder_config.cpp", 41, 1611 },
{ PROC_LINKS(load_themes_default_folder, 0), false, "load_themes_default_folder", 26, "Loads all the theme files in the default theme folder.", 54, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 535 },
{ PROC_LINKS(load_themes_hot_directory, 0), false, "load_themes_hot_directory", 25, "Loads all the theme files in the current hot directory.", 55, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 554 },
{ PROC_LINKS(luis_add_space, 0), false, "luis_add_space", 14, "add space to line start", 23, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 393 },
{ PROC_LINKS(luis_add_tab, 0), false, "luis_add_tab", 12, "add tab to line start", 21, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 361 },
{ PROC_LINKS(luis_adjust_horizontal_view_toggle, 0), false, "luis_adjust_horizontal_view_toggle", 34, "Moves view horizontally to cursor x pos or back to leftmost of screen", 69, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 755 },
{ PROC_LINKS(luis_backspace_char, 0), false, "luis_backspace_char", 19, "delete char", 11, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 232 },
{ PROC_LINKS(luis_build, 0), false, "luis_build", 10, "build", 5, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1264 },
{ PROC_LINKS(luis_center_view_top, 0), false, "luis_center_view_top", 20, "Centers the view vertically on the line on which the cursor sits.", 65, "D:\\work\\4ed\\code\\custom\\luis_custom_layer.h", 43, 357 },
{ PROC_LINKS(luis_close_all_other_panels, 0), false, "luis_close_all_other_panels", 27, "Close all panels except active one", 34, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 939 },
{ PROC_LINKS(luis_close_current_panel, 0), false, "luis_close_current_panel", 24, "Close current panel", 19, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 965 },
{ PROC_LINKS(luis_code_index_next, 0), false, "luis_code_index_next", 20, "next code index", 15, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 3282 },
{ PROC_LINKS(luis_code_index_prev, 0), false, "luis_code_index_prev", 20, "prev code index", 15, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 3301 },
{ PROC_LINKS(luis_delete_char, 0), false, "luis_delete_char", 16, "delete char", 11, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 222 },
{ PROC_LINKS(luis_eat_whitespace_backwards, 0), false, "luis_eat_whitespace_backwards", 29, "eat whitespace forward from cursor", 34, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 12 },
{ PROC_LINKS(luis_end, 0), false, "luis_end", 8, "go end of visual line", 21, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 852 },
{ PROC_LINKS(luis_escape, 0), false, "luis_escape", 11, "escape key", 10, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 126 },
{ PROC_LINKS(luis_fsearch, 0), false, "luis_fsearch", 12, "search forwards", 15, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 3645 },
{ PROC_LINKS(luis_goto_next_jump, 0), false, "luis_goto_next_jump", 19, "If a buffer containing jump locations has been locked in, goes to the next jump in the buffer, skipping sub jump locations.", 123, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1234 },
{ PROC_LINKS(luis_goto_prev_jump, 0), false, "luis_goto_prev_jump", 19, "If a buffer containing jump locations has been locked in, goes to the previous jump in the buffer, skipping sub jump locations.", 127, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1250 },
{ PROC_LINKS(luis_home, 0), false, "luis_home", 9, "go start of visual line", 23, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 163 },
{ PROC_LINKS(luis_horizontal_mouse_wheel_scroll, 0), false, "luis_horizontal_mouse_wheel_scroll", 34, "Reads the scroll wheel value from the mouse state and scrolls accordingly.", 74, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 779 },
{ PROC_LINKS(luis_indent_range, 0), false, "luis_indent_range", 17, "indent_range", 12, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 346 },
{ PROC_LINKS(luis_interactive_open_or_new, 0), false, "luis_interactive_open_or_new", 28, "open in new in same tab", 23, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 149 },
{ PROC_LINKS(luis_left_word, 0), false, "luis_left_word", 14, "move left", 9, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 171 },
{ PROC_LINKS(luis_list_functions_all_buffers, 0), false, "luis_list_functions_all_buffers", 31, "Show code indexes for all buffer", 32, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1800 },
{ PROC_LINKS(luis_list_nests_this_buffer, 0), false, "luis_list_nests_this_buffer", 27, "Show code nests for this buffer", 31, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1379 },
{ PROC_LINKS(luis_list_notes_all_buffers, 0), false, "luis_list_notes_all_buffers", 27, "Show code indexes for all buffer", 32, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1696 },
{ PROC_LINKS(luis_list_notes_no_duplicates, 0), false, "luis_list_notes_no_duplicates", 29, "Show code indexes for all buffer with no duplicates", 51, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1946 },
{ PROC_LINKS(luis_list_notes_this_buffer, 0), false, "luis_list_notes_this_buffer", 27, "Show code indexes for buffer", 28, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1447 },
{ PROC_LINKS(luis_matching_file_cpp, 0), false, "luis_matching_file_cpp", 22, "If the current file is a *.cpp or *.h, attempts to open the corresponding *.h or *.cpp file in the other view.", 110, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1125 },
{ PROC_LINKS(luis_matching_file_cpp_same_buffer, 0), false, "luis_matching_file_cpp_same_buffer", 34, "If the current file is a *.cpp or *.h, attempts to open the corresponding *.h or *.cpp file in the other view.", 110, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1115 },
{ PROC_LINKS(luis_multiline_comment_toggle, 0), false, "luis_multiline_comment_toggle", 29, "Deletes all whitespace at cursor, going backwards", 49, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 794 },
{ PROC_LINKS(luis_open_build_panel, 0), false, "luis_open_build_panel", 21, "open build panel", 16, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1306 },
{ PROC_LINKS(luis_open_up_braces, 0), false, "luis_open_up_braces", 19, "open braces up", 14, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 320 },
{ PROC_LINKS(luis_print_scope_variables, 0), false, "luis_print_scope_variables", 26, "print scope variables", 21, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 2104 },
{ PROC_LINKS(luis_remove_space, 0), false, "luis_remove_space", 17, "remove space from line start", 28, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 466 },
{ PROC_LINKS(luis_remove_tab, 0), false, "luis_remove_tab", 15, "remove tab from line start", 26, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 417 },
{ PROC_LINKS(luis_reselect_selection, 0), false, "luis_reselect_selection", 23, "create selection where cursor is and mark was", 45, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 620 },
{ PROC_LINKS(luis_return, 0), false, "luis_return", 11, "If the buffer in the active view is writable, inserts a character, otherwise performs goto_jump_at_cursor.", 106, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1139 },
{ PROC_LINKS(luis_right_word, 0), false, "luis_right_word", 15, "move right", 10, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 178 },
{ PROC_LINKS(luis_rsearch, 0), false, "luis_rsearch", 12, "search backwards", 16, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 3652 },
{ PROC_LINKS(luis_scope_braces, 0), false, "luis_scope_braces", 17, "writes {}", 9, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 271 },
{ PROC_LINKS(luis_select_line, 0), false, "luis_select_line", 16, "go end of visual line", 21, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 520 },
{ PROC_LINKS(luis_select_scope_contents, 0), false, "luis_select_scope_contents", 26, "if you have a scope selected, select just the contents of it", 60, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 675 },
{ PROC_LINKS(luis_select_surrounding_scope, 0), false, "luis_select_surrounding_scope", 29, "select surrounding scope", 24, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 638 },
{ PROC_LINKS(luis_select_surrounding_scope_maximal, 0), false, "luis_select_surrounding_scope_maximal", 37, "select surrounding scope", 24, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 717 },
{ PROC_LINKS(luis_select_word_under_cursor, 0), false, "luis_select_word_under_cursor", 29, "Select text at cursor", 21, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 208 },
{ PROC_LINKS(luis_set_mark, 0), false, "luis_set_mark", 13, "set mark", 8, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 202 },
{ PROC_LINKS(luis_show_search_panel, 0), false, "luis_show_search_panel", 22, "Enter Command Mode", 18, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 3395 },
{ PROC_LINKS(luis_startup, 0), false, "luis_startup", 12, "Default command for responding to a startup event", 49, "D:\\work\\4ed\\code\\custom\\luis_hooks.cpp", 38, 3 },
{ PROC_LINKS(luis_toggle_modal_mode, 0), false, "luis_toggle_modal_mode", 22, "Toggles modal mode", 18, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 132 },
{ PROC_LINKS(luis_try_exit, 0), false, "luis_try_exit", 13, "Default command for responding to a try-exit event", 50, "D:\\work\\4ed\\code\\custom\\luis_hooks.cpp", 38, 1101 },
{ PROC_LINKS(luis_view_input_handler, 0), false, "luis_view_input_handler", 23, "Input consumption loop for default view behavior", 48, "D:\\work\\4ed\\code\\custom\\luis_hooks.cpp", 38, 81 },
{ PROC_LINKS(luis_view_peek_as_split_window, 0), false, "luis_view_peek_as_split_window", 30, "view peek buffer in split window", 32, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 987 },
{ PROC_LINKS(luis_view_peek_in_parent_window, 0), false, "luis_view_peek_in_parent_window", 31, "view peek buffer in split window", 32, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1040 },
{ PROC_LINKS(luis_vim_string_matches, 0), true, "luis_vim_string_matches", 23, "Enter Command Mode", 18, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 3362 },
{ PROC_LINKS(luis_write_newline, 0), false, "luis_write_newline", 18, "", 0, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 744 },
{ PROC_LINKS(luis_write_pointer_arrow, 0), false, "luis_write_pointer_arrow", 24, "", 0, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 740 },
{ PROC_LINKS(luis_write_tab, 0), false, "luis_write_tab", 14, "", 0, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 748 },
{ PROC_LINKS(luis_write_underscore, 0), false, "luis_write_underscore", 21, "", 0, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 736 },
{ PROC_LINKS(make_directory_query, 0), false, "make_directory_query", 20, "Queries the user for a name and creates a new directory with the given name.", 76, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1559 },
{ PROC_LINKS(miblo_decrement_basic, 0), false, "miblo_decrement_basic", 21, "Decrement an integer under the cursor by one.", 45, "D:\\work\\4ed\\code\\custom\\4coder_miblo_numbers.cpp", 48, 44 },
{ PROC_LINKS(miblo_decrement_time_stamp, 0), false, "miblo_decrement_time_stamp", 26, "Decrement a time stamp under the cursor by one second. (format [m]m:ss or h:mm:ss", 81, "D:\\work\\4ed\\code\\custom\\4coder_miblo_numbers.cpp", 48, 237 },
{ PROC_LINKS(miblo_decrement_time_stamp_minute, 0), false, "miblo_decrement_time_stamp_minute", 33, "Decrement a time stamp under the cursor by one minute. (format [m]m:ss or h:mm:ss", 81, "D:\\work\\4ed\\code\\custom\\4coder_miblo_numbers.cpp", 48, 249 },
{ PROC_LINKS(miblo_increment_basic, 0), false, "miblo_increment_basic", 21, "Increment an integer under the cursor by one.", 45, "D:\\work\\4ed\\code\\custom\\4coder_miblo_numbers.cpp", 48, 29 },
{ PROC_LINKS(miblo_increment_time_stamp, 0), false, "miblo_increment_time_stamp", 26, "Increment a time stamp under the cursor by one second. (format [m]m:ss or h:mm:ss", 81, "D:\\work\\4ed\\code\\custom\\4coder_miblo_numbers.cpp", 48, 231 },
{ PROC_LINKS(miblo_increment_time_stamp_minute, 0), false, "miblo_increment_time_stamp_minute", 33, "Increment a time stamp under the cursor by one minute. (format [m]m:ss or h:mm:ss", 81, "D:\\work\\4ed\\code\\custom\\4coder_miblo_numbers.cpp", 48, 243 },
{ PROC_LINKS(mouse_wheel_change_face_size, 0), false, "mouse_wheel_change_face_size", 28, "Reads the state of the mouse wheel and uses it to either increase or decrease the face size.", 92, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 867 },
{ PROC_LINKS(mouse_wheel_scroll, 0), false, "mouse_wheel_scroll", 18, "Reads the scroll wheel value from the mouse state and scrolls accordingly.", 74, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 334 },
{ PROC_LINKS(move_down, 0), false, "move_down", 9, "Moves the cursor down one line.", 31, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 408 },
{ PROC_LINKS(move_down_10, 0), false, "move_down_10", 12, "Moves the cursor down ten lines.", 32, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 420 },
{ PROC_LINKS(move_down_textual, 0), false, "move_down_textual", 17, "Moves down to the next line of actual text, regardless of line wrapping.", 72, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 426 },
{ PROC_LINKS(move_down_to_blank_line, 0), false, "move_down_to_blank_line", 23, "Seeks the cursor down to the next blank line.", 45, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 479 },
{ PROC_LINKS(move_down_to_blank_line_end, 0), false, "move_down_to_blank_line_end", 27, "Seeks the cursor down to the next blank line and places it at the end of the line.", 82, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 503 },
{ PROC_LINKS(move_down_to_blank_line_skip_whitespace, 0), false, "move_down_to_blank_line_skip_whitespace", 39, "Seeks the cursor down to the next blank line and places it at the end of the line.", 82, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 491 },
{ PROC_LINKS(move_left, 0), false, "move_left", 9, "Moves the cursor one character to the left.", 43, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 509 },
{ PROC_LINKS(move_left_alpha_numeric_boundary, 0), false, "move_left_alpha_numeric_boundary", 32, "Seek left for boundary between alphanumeric characters and non-alphanumeric characters.", 87, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 586 },
{ PROC_LINKS(move_left_alpha_numeric_or_camel_boundary, 0), false, "move_left_alpha_numeric_or_camel_boundary", 41, "Seek left for boundary between alphanumeric characters or camel case word and non-alphanumeric characters.", 106, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 600 },
{ PROC_LINKS(move_left_token_boundary, 0), false, "move_left_token_boundary", 24, "Seek left for the next beginning of a token.", 44, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 558 },
{ PROC_LINKS(move_left_whitespace_boundary, 0), false, "move_left_whitespace_boundary", 29, "Seek left for the next boundary between whitespace and non-whitespace.", 70, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 543 },
{ PROC_LINKS(move_left_whitespace_or_token_boundary, 0), false, "move_left_whitespace_or_token_boundary", 38, "Seek left for the next end of a token or boundary between whitespace and non-whitespace.", 88, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 572 },
{ PROC_LINKS(move_line_down, 0), false, "move_line_down", 14, "Swaps the line under the cursor with the line below it, and moves the cursor down with it.", 90, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1599 },
{ PROC_LINKS(move_line_up, 0), false, "move_line_up", 12, "Swaps the line under the cursor with the line above it, and moves the cursor up with it.", 88, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1593 },
{ PROC_LINKS(move_right, 0), false, "move_right", 10, "Moves the cursor one character to the right.", 44, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 517 },
{ PROC_LINKS(move_right_alpha_numeric_boundary, 0), false, "move_right_alpha_numeric_boundary", 33, "Seek right for boundary between alphanumeric characters and non-alphanumeric characters.", 88, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 579 },
{ PROC_LINKS(move_right_alpha_numeric_or_camel_boundary, 0), false, "move_right_alpha_numeric_or_camel_boundary", 42, "Seek right for boundary between alphanumeric characters or camel case word and non-alphanumeric characters.", 107, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 593 },
{ PROC_LINKS(move_right_token_boundary, 0), false, "move_right_token_boundary", 25, "Seek right for the next end of a token.", 39, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 551 },
{ PROC_LINKS(move_right_whitespace_boundary, 0), false, "move_right_whitespace_boundary", 30, "Seek right for the next boundary between whitespace and non-whitespace.", 71, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 535 },
{ PROC_LINKS(move_right_whitespace_or_token_boundary, 0), false, "move_right_whitespace_or_token_boundary", 39, "Seek right for the next end of a token or boundary between whitespace and non-whitespace.", 89, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 565 },
{ PROC_LINKS(move_up, 0), false, "move_up", 7, "Moves the cursor up one line.", 29, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 402 },
{ PROC_LINKS(move_up_10, 0), false, "move_up_10", 10, "Moves the cursor up ten lines.", 30, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 414 },
{ PROC_LINKS(move_up_to_blank_line, 0), false, "move_up_to_blank_line", 21, "Seeks the cursor up to the next blank line.", 43, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 473 },
{ PROC_LINKS(move_up_to_blank_line_end, 0), false, "move_up_to_blank_line_end", 25, "Seeks the cursor up to the next blank line and places it at the end of the line.", 80, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 497 },
{ PROC_LINKS(move_up_to_blank_line_skip_whitespace, 0), false, "move_up_to_blank_line_skip_whitespace", 37, "Seeks the cursor up to the next blank line and places it at the end of the line.", 80, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 485 },
{ PROC_LINKS(multi_paste, 0), false, "multi_paste", 11, "Paste multiple entries from the clipboard at once", 49, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 230 },
{ PROC_LINKS(multi_paste_interactive, 0), false, "multi_paste_interactive", 23, "Paste multiple lines from the clipboard history, controlled with arrow keys", 75, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 372 },
{ PROC_LINKS(multi_paste_interactive_quick, 0), false, "multi_paste_interactive_quick", 29, "Paste multiple lines from the clipboard history, controlled by inputing the number of lines to paste", 100, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 381 },
{ PROC_LINKS(music_start, 0), false, "music_start", 11, "Starts the music.", 17, "D:\\work\\4ed\\code\\custom\\4coder_examples.cpp", 43, 213 },
{ PROC_LINKS(music_stop, 0), false, "music_stop", 10, "Stops the music.", 16, "D:\\work\\4ed\\code\\custom\\4coder_examples.cpp", 43, 234 },
{ PROC_LINKS(navigate_back, 0), false, "navigate_back", 13, "Jump back to previous view location", 35, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 897 },
{ PROC_LINKS(navigate_forward, 0), false, "navigate_forward", 16, "Jump back to next view location", 31, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 918 },
{ PROC_LINKS(open_all_code, 0), false, "open_all_code", 13, "Open all code in the current directory. File types are determined by extensions. An extension is considered code based on the extensions specified in 4coder.config.", 164, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 838 },
{ PROC_LINKS(open_all_code_recursive, 0), false, "open_all_code_recursive", 23, "Works as open_all_code but also runs in all subdirectories.", 59, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 847 },
{ PROC_LINKS(open_file_in_10x, 0), false, "open_file_in_10x", 16, "Open current file in 10x", 24, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 103 },
{ PROC_LINKS(open_file_in_quotes, 0), false, "open_file_in_quotes", 19, "Reads a filename from surrounding '\"' characters and attempts to open the corresponding file.", 94, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1640 },
{ PROC_LINKS(open_file_in_visual_studio, 0), false, "open_file_in_visual_studio", 26, "Open current file in visual studio", 34, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 45 },
{ PROC_LINKS(open_in_other, 0), false, "open_in_other", 13, "Interactively opens a file in the other panel.", 46, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 2123 },
{ PROC_LINKS(open_long_braces, 0), false, "open_long_braces", 16, "At the cursor, insert a '{' and '}' separated by a blank line.", 62, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 46 },
{ PROC_LINKS(open_long_braces_break, 0), false, "open_long_braces_break", 22, "At the cursor, insert a '{' and '}break;' separated by a blank line.", 68, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 62 },
{ PROC_LINKS(open_long_braces_semicolon, 0), false, "open_long_braces_semicolon", 26, "At the cursor, insert a '{' and '};' separated by a blank line.", 63, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 54 },
{ PROC_LINKS(open_matching_file_cpp, 0), false, "open_matching_file_cpp", 22, "If the current file is a *.cpp or *.h, attempts to open the corresponding *.h or *.cpp file in the other view.", 110, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1723 },
{ PROC_LINKS(open_panel_hsplit, 0), false, "open_panel_hsplit", 17, "Create a new panel by horizontally splitting the active panel.", 62, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 382 },
{ PROC_LINKS(open_panel_vsplit, 0), false, "open_panel_vsplit", 17, "Create a new panel by vertically splitting the active panel.", 60, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 372 },
{ PROC_LINKS(page_down, 0), false, "page_down", 9, "Scrolls the view down one view height and moves the cursor down one view height.", 80, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 444 },
{ PROC_LINKS(page_up, 0), false, "page_up", 7, "Scrolls the view up one view height and moves the cursor up one view height.", 76, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 436 },
{ PROC_LINKS(paste, 0), false, "paste", 5, "At the cursor, insert the text at the top of the clipboard.", 59, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 130 },
{ PROC_LINKS(paste_and_indent, 0), false, "paste_and_indent", 16, "Paste from the top of clipboard and run auto-indent on the newly pasted text.", 77, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 208 },
{ PROC_LINKS(paste_next, 0), false, "paste_next", 10, "If the previous command was paste or paste_next, replaces the paste range with the next text down on the clipboard, otherwise operates as the paste command.", 156, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 165 },
{ PROC_LINKS(paste_next_and_indent, 0), false, "paste_next_and_indent", 21, "Paste the next item on the clipboard and run auto-indent on the newly pasted text.", 82, "D:\\work\\4ed\\code\\custom\\4coder_clipboard.cpp", 44, 215 },
{ PROC_LINKS(place_in_scope, 0), false, "place_in_scope", 14, "Wraps the code contained in the range between cursor and mark with a new curly brace scope.", 91, "D:\\work\\4ed\\code\\custom\\4coder_scope_commands.cpp", 49, 106 },
{ PROC_LINKS(play_with_a_counter, 0), false, "play_with_a_counter", 19, "Example of query bar", 20, "D:\\work\\4ed\\code\\custom\\4coder_examples.cpp", 43, 29 },
{ PROC_LINKS(profile_clear, 0), false, "profile_clear", 13, "Clear all profiling information from 4coder's self profiler.", 60, "D:\\work\\4ed\\code\\custom\\4coder_profile.cpp", 42, 226 },
{ PROC_LINKS(profile_disable, 0), false, "profile_disable", 15, "Prevent 4coder's self profiler from gathering new profiling information.", 72, "D:\\work\\4ed\\code\\custom\\4coder_profile.cpp", 42, 219 },
{ PROC_LINKS(profile_enable, 0), false, "profile_enable", 14, "Allow 4coder's self profiler to gather new profiling information.", 65, "D:\\work\\4ed\\code\\custom\\4coder_profile.cpp", 42, 212 },
{ PROC_LINKS(profile_inspect, 0), true, "profile_inspect", 15, "Inspect all currently collected profiling information in 4coder's self profiler.", 80, "D:\\work\\4ed\\code\\custom\\4coder_profile_inspect.cpp", 50, 886 },
{ PROC_LINKS(project_command_F1, 0), false, "project_command_F1", 18, "Run the command with index 1", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1117 },
{ PROC_LINKS(project_command_F10, 0), false, "project_command_F10", 19, "Run the command with index 10", 29, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1171 },
{ PROC_LINKS(project_command_F11, 0), false, "project_command_F11", 19, "Run the command with index 11", 29, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1177 },
{ PROC_LINKS(project_command_F12, 0), false, "project_command_F12", 19, "Run the command with index 12", 29, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1183 },
{ PROC_LINKS(project_command_F13, 0), false, "project_command_F13", 19, "Run the command with index 13", 29, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1189 },
{ PROC_LINKS(project_command_F14, 0), false, "project_command_F14", 19, "Run the command with index 14", 29, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1195 },
{ PROC_LINKS(project_command_F15, 0), false, "project_command_F15", 19, "Run the command with index 15", 29, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1201 },
{ PROC_LINKS(project_command_F16, 0), false, "project_command_F16", 19, "Run the command with index 16", 29, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1207 },
{ PROC_LINKS(project_command_F2, 0), false, "project_command_F2", 18, "Run the command with index 2", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1123 },
{ PROC_LINKS(project_command_F3, 0), false, "project_command_F3", 18, "Run the command with index 3", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1129 },
{ PROC_LINKS(project_command_F4, 0), false, "project_command_F4", 18, "Run the command with index 4", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1135 },
{ PROC_LINKS(project_command_F5, 0), false, "project_command_F5", 18, "Run the command with index 5", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1141 },
{ PROC_LINKS(project_command_F6, 0), false, "project_command_F6", 18, "Run the command with index 6", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1147 },
{ PROC_LINKS(project_command_F7, 0), false, "project_command_F7", 18, "Run the command with index 7", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1153 },
{ PROC_LINKS(project_command_F8, 0), false, "project_command_F8", 18, "Run the command with index 8", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1159 },
{ PROC_LINKS(project_command_F9, 0), false, "project_command_F9", 18, "Run the command with index 9", 28, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1165 },
{ PROC_LINKS(project_command_lister, 0), false, "project_command_lister", 22, "Open a lister of all commands in the currently loaded project.", 62, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1069 },
{ PROC_LINKS(project_fkey_command, 0), false, "project_fkey_command", 20, "Run an 'fkey command' configured in a project.4coder file.  Determines the index of the 'fkey command' by which function key or numeric key was pressed to trigger the command.", 175, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1007 },
{ PROC_LINKS(project_go_to_root_directory, 0), false, "project_go_to_root_directory", 28, "Changes 4coder's hot directory to the root directory of the currently loaded project. With no loaded project nothing hapepns.", 125, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1033 },
{ PROC_LINKS(project_reprint, 0), false, "project_reprint", 15, "Prints the current project to the file it was loaded from; prints in the most recent project file version", 105, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1079 },
{ PROC_LINKS(query_replace, 0), false, "query_replace", 13, "Queries the user for two strings, and incrementally replaces every occurence of the first string with the second string.", 120, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1346 },
{ PROC_LINKS(query_replace_identifier, 0), false, "query_replace_identifier", 24, "Queries the user for a string, and incrementally replace every occurence of the word or token found at the cursor with the specified string.", 140, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1367 },
{ PROC_LINKS(query_replace_selection, 0), false, "query_replace_selection", 23, "Queries the user for a string, and incrementally replace every occurence of the string found in the selected range with the specified string.", 141, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1383 },
{ PROC_LINKS(quick_swap_buffer, 0), false, "quick_swap_buffer", 17, "Change to the most recently used buffer in this view - or to the top of the buffer stack if the most recent doesn't exist anymore", 129, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1770 },
{ PROC_LINKS(redo, 0), false, "redo", 4, "Advances forwards through the undo history of the current buffer.", 65, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1950 },
{ PROC_LINKS(redo_all_buffers, 0), false, "redo_all_buffers", 16, "Advances forward through the undo history in the buffer containing the most recent regular edit.", 96, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 2047 },
{ PROC_LINKS(reload_project, 0), false, "reload_project", 14, "Reload project.4coder", 21, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 1628 },
{ PROC_LINKS(rename_file_query, 0), false, "rename_file_query", 17, "Queries the user for a new name and renames the file of the current buffer, altering the buffer's name too.", 107, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1524 },
{ PROC_LINKS(reopen, 0), false, "reopen", 6, "Reopen the current buffer from the hard drive.", 46, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1808 },
{ PROC_LINKS(replace_in_all_buffers, 0), false, "replace_in_all_buffers", 22, "Queries the user for a needle and string. Replaces all occurences of needle with string in all editable buffers.", 112, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1260 },
{ PROC_LINKS(replace_in_buffer, 0), false, "replace_in_buffer", 17, "Queries the user for a needle and string. Replaces all occurences of needle with string in the active buffer.", 109, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1251 },
{ PROC_LINKS(replace_in_range, 0), false, "replace_in_range", 16, "Queries the user for a needle and string. Replaces all occurences of needle with string in the range between cursor and the mark in the active buffer.", 150, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1242 },
{ PROC_LINKS(reverse_search, 0), false, "reverse_search", 14, "Begins an incremental search up through the current buffer for a user specified string.", 87, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1183 },
{ PROC_LINKS(reverse_search_identifier, 0), false, "reverse_search_identifier", 25, "Begins an incremental search up through the current buffer for the word or token under the cursor.", 98, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1195 },
{ PROC_LINKS(revert_buffer, 0), false, "revert_buffer", 13, "Revert buffer to contents on disk", 33, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 143 },
{ PROC_LINKS(save, 0), false, "save", 4, "Saves the current buffer.", 25, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1798 },
{ PROC_LINKS(save_all_dirty_buffers, 0), false, "save_all_dirty_buffers", 22, "Saves all buffers marked dirty (showing the '*' indicator).", 59, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 454 },
{ PROC_LINKS(save_to_query, 0), false, "save_to_query", 13, "Queries the user for a file name and saves the contents of the current buffer, altering the buffer's name too.", 110, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1491 },
{ PROC_LINKS(search, 0), false, "search", 6, "Begins an incremental search down through the current buffer for a user specified string.", 89, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1177 },
{ PROC_LINKS(search_identifier, 0), false, "search_identifier", 17, "Begins an incremental search down through the current buffer for the word or token under the cursor.", 100, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1189 },
{ PROC_LINKS(seek_beginning_of_line, 0), false, "seek_beginning_of_line", 22, "Seeks the cursor to the beginning of the visual line.", 53, "D:\\work\\4ed\\code\\custom\\4coder_helper.cpp", 41, 2257 },
{ PROC_LINKS(seek_beginning_of_textual_line, 0), false, "seek_beginning_of_textual_line", 30, "Seeks the cursor to the beginning of the line across all text.", 62, "D:\\work\\4ed\\code\\custom\\4coder_helper.cpp", 41, 2245 },
{ PROC_LINKS(seek_end_of_line, 0), false, "seek_end_of_line", 16, "Seeks the cursor to the end of the visual line.", 47, "D:\\work\\4ed\\code\\custom\\4coder_helper.cpp", 41, 2263 },
{ PROC_LINKS(seek_end_of_textual_line, 0), false, "seek_end_of_textual_line", 24, "Seeks the cursor to the end of the line across all text.", 56, "D:\\work\\4ed\\code\\custom\\4coder_helper.cpp", 41, 2251 },
{ PROC_LINKS(select_all, 0), false, "select_all", 10, "Puts the cursor at the top of the file, and the mark at the bottom of the file.", 79, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 609 },
{ PROC_LINKS(select_next_scope_absolute, 0), false, "select_next_scope_absolute", 26, "Finds the first scope started by '{' after the cursor and puts the cursor and mark on the '{' and '}'.", 102, "D:\\work\\4ed\\code\\custom\\4coder_scope_commands.cpp", 49, 57 },
{ PROC_LINKS(select_next_scope_after_current, 0), false, "select_next_scope_after_current", 31, "If a scope is selected, find first scope that starts after the selected scope. Otherwise find the first scope that starts after the cursor.", 139, "D:\\work\\4ed\\code\\custom\\4coder_scope_commands.cpp", 49, 66 },
{ PROC_LINKS(select_prev_scope_absolute, 0), false, "select_prev_scope_absolute", 26, "Finds the first scope started by '{' before the cursor and puts the cursor and mark on the '{' and '}'.", 103, "D:\\work\\4ed\\code\\custom\\4coder_scope_commands.cpp", 49, 82 },
{ PROC_LINKS(select_prev_top_most_scope, 0), false, "select_prev_top_most_scope", 26, "Finds the first scope that starts before the cursor, then finds the top most scope that contains that scope.", 108, "D:\\work\\4ed\\code\\custom\\4coder_scope_commands.cpp", 49, 99 },
{ PROC_LINKS(select_surrounding_scope, 0), false, "select_surrounding_scope", 24, "Finds the scope enclosed by '{' '}' surrounding the cursor and puts the cursor and mark on the '{' and '}'.", 107, "D:\\work\\4ed\\code\\custom\\4coder_scope_commands.cpp", 49, 27 },
{ PROC_LINKS(select_surrounding_scope_maximal, 0), false, "select_surrounding_scope_maximal", 32, "Selects the top-most scope that surrounds the cursor.", 53, "D:\\work\\4ed\\code\\custom\\4coder_scope_commands.cpp", 49, 39 },
{ PROC_LINKS(set_eol_mode_from_contents, 0), false, "set_eol_mode_from_contents", 26, "Sets the buffer's line ending mode to match the contents of the buffer.", 71, "D:\\work\\4ed\\code\\custom\\4coder_eol.cpp", 38, 125 },
{ PROC_LINKS(set_eol_mode_to_binary, 0), false, "set_eol_mode_to_binary", 22, "Puts the buffer in bin line ending mode.", 40, "D:\\work\\4ed\\code\\custom\\4coder_eol.cpp", 38, 112 },
{ PROC_LINKS(set_eol_mode_to_crlf, 0), false, "set_eol_mode_to_crlf", 20, "Puts the buffer in crlf line ending mode.", 41, "D:\\work\\4ed\\code\\custom\\4coder_eol.cpp", 38, 86 },
{ PROC_LINKS(set_eol_mode_to_lf, 0), false, "set_eol_mode_to_lf", 18, "Puts the buffer in lf line ending mode.", 39, "D:\\work\\4ed\\code\\custom\\4coder_eol.cpp", 38, 99 },
{ PROC_LINKS(set_face_size, 0), false, "set_face_size", 13, "Set face size of the face used by the current buffer.", 53, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 794 },
{ PROC_LINKS(set_face_size_this_buffer, 0), false, "set_face_size_this_buffer", 25, "Set face size of the face used by the current buffer; if any other buffers are using the same face a new face is created so that only this buffer is effected", 157, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 836 },
{ PROC_LINKS(set_mark, 0), false, "set_mark", 8, "Sets the mark to the current position of the cursor.", 52, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 181 },
{ PROC_LINKS(set_mode_to_notepad_like, 0), false, "set_mode_to_notepad_like", 24, "Sets the edit mode to Notepad like.", 35, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 499 },
{ PROC_LINKS(set_mode_to_original, 0), false, "set_mode_to_original", 20, "Sets the edit mode to 4coder original.", 38, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 493 },
{ PROC_LINKS(setup_build_bat, 0), false, "setup_build_bat", 15, "Queries the user for several configuration options and initializes a new build batch script.", 92, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1051 },
{ PROC_LINKS(setup_build_bat_and_sh, 0), false, "setup_build_bat_and_sh", 22, "Queries the user for several configuration options and initializes a new build batch script.", 92, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1063 },
{ PROC_LINKS(setup_build_sh, 0), false, "setup_build_sh", 14, "Queries the user for several configuration options and initializes a new build shell script.", 92, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1057 },
{ PROC_LINKS(setup_new_project, 0), false, "setup_new_project", 17, "Queries the user for several configuration options and initializes a new 4coder project with build scripts for every OS.", 120, "D:\\work\\4ed\\code\\custom\\4coder_project_commands.cpp", 51, 1044 },
{ PROC_LINKS(show_file_in_explorer, 0), false, "show_file_in_explorer", 21, "Opens file explorer in hot directory", 36, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 27 },
{ PROC_LINKS(show_filebar, 0), false, "show_filebar", 12, "Sets the current view to show it's filebar.", 43, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 765 },
{ PROC_LINKS(show_scrollbar, 0), false, "show_scrollbar", 14, "Sets the current view to show it's scrollbar.", 45, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 751 },
{ PROC_LINKS(show_the_log_graph, 0), true, "show_the_log_graph", 18, "Parses *log* and displays the 'log graph' UI", 44, "D:\\work\\4ed\\code\\custom\\4coder_log_parser.cpp", 45, 991 },
{ PROC_LINKS(snipe_backward_whitespace_or_token_boundary, 0), false, "snipe_backward_whitespace_or_token_boundary", 43, "Delete a single, whole token on or to the left of the cursor and post it to the clipboard.", 90, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 245 },
{ PROC_LINKS(snipe_forward_whitespace_or_token_boundary, 0), false, "snipe_forward_whitespace_or_token_boundary", 42, "Delete a single, whole token on or to the right of the cursor and post it to the clipboard.", 91, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 253 },
{ PROC_LINKS(snippet_lister, 0), true, "snippet_lister", 14, "Opens a snippet lister for inserting whole pre-written snippets of text.", 72, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 237 },
{ PROC_LINKS(string_repeat, 0), false, "string_repeat", 13, "Example of query_user_string and query_user_number", 50, "D:\\work\\4ed\\code\\custom\\4coder_examples.cpp", 43, 179 },
{ PROC_LINKS(suppress_mouse, 0), false, "suppress_mouse", 14, "Hides the mouse and causes all mosue input (clicks, position, wheel) to be ignored.", 83, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 475 },
{ PROC_LINKS(swap_panels, 0), false, "swap_panels", 11, "Swaps the active panel with it's sibling.", 41, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1748 },
{ PROC_LINKS(theme_lister, 0), true, "theme_lister", 12, "Opens an interactive list of all registered themes.", 51, "D:\\work\\4ed\\code\\custom\\4coder_lists.cpp", 40, 804 },
{ PROC_LINKS(to_lowercase, 0), false, "to_lowercase", 12, "Converts all ascii text in the range between the cursor and the mark to lowercase.", 82, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 635 },
{ PROC_LINKS(to_uppercase, 0), false, "to_uppercase", 12, "Converts all ascii text in the range between the cursor and the mark to uppercase.", 82, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 622 },
{ PROC_LINKS(toggle_filebar, 0), false, "toggle_filebar", 14, "Toggles the visibility status of the current view's filebar.", 60, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 779 },
{ PROC_LINKS(toggle_fps_meter, 0), false, "toggle_fps_meter", 16, "Toggles the visibility of the FPS performance meter", 51, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 788 },
{ PROC_LINKS(toggle_fullscreen, 0), false, "toggle_fullscreen", 17, "Toggle fullscreen mode on or off.  The change(s) do not take effect until the next frame.", 89, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 529 },
{ PROC_LINKS(toggle_highlight_enclosing_scopes, 0), false, "toggle_highlight_enclosing_scopes", 33, "In code files scopes surrounding the cursor are highlighted with distinguishing colors.", 87, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 513 },
{ PROC_LINKS(toggle_highlight_line_at_cursor, 0), false, "toggle_highlight_line_at_cursor", 31, "Toggles the line highlight at the cursor.", 41, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 505 },
{ PROC_LINKS(toggle_line_numbers, 0), false, "toggle_line_numbers", 19, "Toggles the left margin line numbers.", 37, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 893 },
{ PROC_LINKS(toggle_line_wrap, 0), false, "toggle_line_wrap", 16, "Toggles the line wrap setting on this buffer.", 45, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 901 },
{ PROC_LINKS(toggle_mouse, 0), false, "toggle_mouse", 12, "Toggles the mouse suppression mode, see suppress_mouse and allow_mouse.", 71, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 487 },
{ PROC_LINKS(toggle_paren_matching_helper, 0), false, "toggle_paren_matching_helper", 28, "In code files matching parentheses pairs are colored with distinguishing colors.", 80, "D:\\work\\4ed\\code\\custom\\4coder_default_framework.cpp", 52, 521 },
{ PROC_LINKS(toggle_show_whitespace, 0), false, "toggle_show_whitespace", 22, "Toggles the current buffer's whitespace visibility status.", 58, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 884 },
{ PROC_LINKS(toggle_text_drop_shadow, 0), false, "toggle_text_drop_shadow", 23, "Toggle drop shadows", 19, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 5 },
{ PROC_LINKS(toggle_virtual_whitespace, 0), false, "toggle_virtual_whitespace", 25, "Toggles virtual whitespace for all files.", 41, "D:\\work\\4ed\\code\\custom\\4coder_code_index.cpp", 45, 1707 },
{ PROC_LINKS(tutorial_maximize, 0), false, "tutorial_maximize", 17, "Expand the tutorial window", 26, "D:\\work\\4ed\\code\\custom\\4coder_tutorial.cpp", 43, 20 },
{ PROC_LINKS(tutorial_minimize, 0), false, "tutorial_minimize", 17, "Shrink the tutorial window", 26, "D:\\work\\4ed\\code\\custom\\4coder_tutorial.cpp", 43, 34 },
{ PROC_LINKS(uncomment_line, 0), false, "uncomment_line", 14, "If present, delete '//' at the beginning of the line after leading whitespace.", 78, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 137 },
{ PROC_LINKS(undo, 0), false, "undo", 4, "Advances backwards through the undo history of the current buffer.", 66, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1898 },
{ PROC_LINKS(undo_all_buffers, 0), false, "undo_all_buffers", 16, "Advances backward through the undo history in the buffer containing the most recent regular edit.", 97, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1976 },
{ PROC_LINKS(view_buffer_other_panel, 0), false, "view_buffer_other_panel", 23, "Set the other non-active panel to view the buffer that the active panel views, and switch to that panel.", 104, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 1736 },
{ PROC_LINKS(view_jump_list_with_lister, 0), false, "view_jump_list_with_lister", 26, "When executed on a buffer with jumps, creates a persistent lister for all the jumps", 83, "D:\\work\\4ed\\code\\custom\\4coder_jump_lister.cpp", 46, 59 },
{ PROC_LINKS(word_complete, 0), false, "word_complete", 13, "Iteratively tries completing the word to the left of the cursor with other words in open buffers that have the same prefix string.", 130, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 464 },
{ PROC_LINKS(word_complete_drop_down, 0), false, "word_complete_drop_down", 23, "Word complete with drop down menu.", 34, "D:\\work\\4ed\\code\\custom\\4coder_search.cpp", 41, 710 },
{ PROC_LINKS(write_block, 0), false, "write_block", 11, "At the cursor, insert a block comment.", 38, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 94 },
{ PROC_LINKS(write_filename, 0), false, "write_filename", 14, "Write file name", 15, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 82 },
{ PROC_LINKS(write_filepath, 0), false, "write_filepath", 14, "Write file full path", 20, "D:\\work\\4ed\\code\\custom\\luis_commands.cpp", 41, 69 },
{ PROC_LINKS(write_hack, 0), false, "write_hack", 10, "At the cursor, insert a '// HACK' comment, includes user name if it was specified in config.4coder.", 99, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 82 },
{ PROC_LINKS(write_note, 0), false, "write_note", 10, "At the cursor, insert a '// NOTE' comment, includes user name if it was specified in config.4coder.", 99, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 88 },
{ PROC_LINKS(write_space, 0), false, "write_space", 11, "Inserts a space.", 16, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 133 },
{ PROC_LINKS(write_text_and_auto_indent, 0), false, "write_text_and_auto_indent", 26, "Inserts text and auto-indents the line on which the cursor sits if any of the text contains 'layout punctuation' such as ;:{}()[]# and new lines.", 145, "D:\\work\\4ed\\code\\custom\\4coder_auto_indent.cpp", 46, 486 },
{ PROC_LINKS(write_text_input, 0), false, "write_text_input", 16, "Inserts whatever text was used to trigger this command.", 55, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 116 },
{ PROC_LINKS(write_todo, 0), false, "write_todo", 10, "At the cursor, insert a '// TODO' comment, includes user name if it was specified in config.4coder.", 99, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 76 },
{ PROC_LINKS(write_underscore, 0), false, "write_underscore", 16, "Inserts an underscore.", 22, "D:\\work\\4ed\\code\\custom\\4coder_base_commands.cpp", 48, 139 },
{ PROC_LINKS(write_zero_struct, 0), false, "write_zero_struct", 17, "At the cursor, insert a ' = {};'.", 33, "D:\\work\\4ed\\code\\custom\\4coder_combined_write_commands.cpp", 58, 100 },
};
static i32 fcoder_metacmd_ID___default_try_exit = 0;
static i32 fcoder_metacmd_ID_allow_mouse = 1;
static i32 fcoder_metacmd_ID_auto_indent_line_at_cursor = 2;
static i32 fcoder_metacmd_ID_auto_indent_range = 3;
static i32 fcoder_metacmd_ID_auto_indent_whole_file = 4;
static i32 fcoder_metacmd_ID_backspace_alpha_numeric_boundary = 5;
static i32 fcoder_metacmd_ID_backspace_char = 6;
static i32 fcoder_metacmd_ID_basic_change_active_panel = 7;
static i32 fcoder_metacmd_ID_begin_clipboard_collection_mode = 8;
static i32 fcoder_metacmd_ID_build_in_build_panel = 9;
static i32 fcoder_metacmd_ID_build_search = 10;
static i32 fcoder_metacmd_ID_center_view = 11;
static i32 fcoder_metacmd_ID_change_active_panel = 12;
static i32 fcoder_metacmd_ID_change_active_panel_backwards = 13;
static i32 fcoder_metacmd_ID_change_to_build_panel = 14;
static i32 fcoder_metacmd_ID_clean_all_lines = 15;
static i32 fcoder_metacmd_ID_clean_trailing_whitespace = 16;
static i32 fcoder_metacmd_ID_clear_all_themes = 17;
static i32 fcoder_metacmd_ID_clear_clipboard = 18;
static i32 fcoder_metacmd_ID_click_set_cursor = 19;
static i32 fcoder_metacmd_ID_click_set_cursor_and_mark = 20;
static i32 fcoder_metacmd_ID_click_set_cursor_if_lbutton = 21;
static i32 fcoder_metacmd_ID_click_set_mark = 22;
static i32 fcoder_metacmd_ID_clipboard_record_clip = 23;
static i32 fcoder_metacmd_ID_close_all_code = 24;
static i32 fcoder_metacmd_ID_close_build_panel = 25;
static i32 fcoder_metacmd_ID_close_panel = 26;
static i32 fcoder_metacmd_ID_command_documentation = 27;
static i32 fcoder_metacmd_ID_command_lister = 28;
static i32 fcoder_metacmd_ID_comment_line = 29;
static i32 fcoder_metacmd_ID_comment_line_toggle = 30;
static i32 fcoder_metacmd_ID_copy = 31;
static i32 fcoder_metacmd_ID_ctrl_tab_switch_buffer = 32;
static i32 fcoder_metacmd_ID_cursor_mark_swap = 33;
static i32 fcoder_metacmd_ID_custom_api_documentation = 34;
static i32 fcoder_metacmd_ID_cut = 35;
static i32 fcoder_metacmd_ID_decrease_face_size = 36;
static i32 fcoder_metacmd_ID_default_file_externally_modified = 37;
static i32 fcoder_metacmd_ID_default_startup = 38;
static i32 fcoder_metacmd_ID_default_view_input_handler = 39;
static i32 fcoder_metacmd_ID_delete_alpha_numeric_boundary = 40;
static i32 fcoder_metacmd_ID_delete_char = 41;
static i32 fcoder_metacmd_ID_delete_current_scope = 42;
static i32 fcoder_metacmd_ID_delete_file_query = 43;
static i32 fcoder_metacmd_ID_delete_line = 44;
static i32 fcoder_metacmd_ID_delete_range = 45;
static i32 fcoder_metacmd_ID_display_key_codes = 46;
static i32 fcoder_metacmd_ID_display_text_input = 47;
static i32 fcoder_metacmd_ID_double_backspace = 48;
static i32 fcoder_metacmd_ID_duplicate_line = 49;
static i32 fcoder_metacmd_ID_emacs_copy = 50;
static i32 fcoder_metacmd_ID_emacs_cut = 51;
static i32 fcoder_metacmd_ID_emacs_kill_line = 52;
static i32 fcoder_metacmd_ID_emacs_swap_cursor_mark = 53;
static i32 fcoder_metacmd_ID_execute_any_cli = 54;
static i32 fcoder_metacmd_ID_execute_previous_cli = 55;
static i32 fcoder_metacmd_ID_exit_4coder = 56;
static i32 fcoder_metacmd_ID_go_to_user_directory = 57;
static i32 fcoder_metacmd_ID_goto_beginning_of_file = 58;
static i32 fcoder_metacmd_ID_goto_end_of_file = 59;
static i32 fcoder_metacmd_ID_goto_first_jump = 60;
static i32 fcoder_metacmd_ID_goto_first_jump_same_panel_sticky = 61;
static i32 fcoder_metacmd_ID_goto_jump_at_cursor = 62;
static i32 fcoder_metacmd_ID_goto_jump_at_cursor_same_panel = 63;
static i32 fcoder_metacmd_ID_goto_line = 64;
static i32 fcoder_metacmd_ID_goto_next_jump = 65;
static i32 fcoder_metacmd_ID_goto_next_jump_no_skips = 66;
static i32 fcoder_metacmd_ID_goto_prev_jump = 67;
static i32 fcoder_metacmd_ID_goto_prev_jump_no_skips = 68;
static i32 fcoder_metacmd_ID_grep = 69;
static i32 fcoder_metacmd_ID_hide_filebar = 70;
static i32 fcoder_metacmd_ID_hide_scrollbar = 71;
static i32 fcoder_metacmd_ID_hit_sfx = 72;
static i32 fcoder_metacmd_ID_hms_demo_tutorial = 73;
static i32 fcoder_metacmd_ID_if0_off = 74;
static i32 fcoder_metacmd_ID_if_read_only_goto_position = 75;
static i32 fcoder_metacmd_ID_if_read_only_goto_position_same_panel = 76;
static i32 fcoder_metacmd_ID_increase_face_size = 77;
static i32 fcoder_metacmd_ID_interactive_kill_buffer = 78;
static i32 fcoder_metacmd_ID_interactive_new = 79;
static i32 fcoder_metacmd_ID_interactive_open = 80;
static i32 fcoder_metacmd_ID_interactive_open_or_new = 81;
static i32 fcoder_metacmd_ID_interactive_switch_buffer = 82;
static i32 fcoder_metacmd_ID_jump_to_definition = 83;
static i32 fcoder_metacmd_ID_jump_to_definition_at_cursor = 84;
static i32 fcoder_metacmd_ID_jump_to_last_point = 85;
static i32 fcoder_metacmd_ID_keyboard_macro_finish_recording = 86;
static i32 fcoder_metacmd_ID_keyboard_macro_replay = 87;
static i32 fcoder_metacmd_ID_keyboard_macro_start_recording = 88;
static i32 fcoder_metacmd_ID_kill_buffer = 89;
static i32 fcoder_metacmd_ID_kill_tutorial = 90;
static i32 fcoder_metacmd_ID_left_adjust_view = 91;
static i32 fcoder_metacmd_ID_list_all_functions_all_buffers = 92;
static i32 fcoder_metacmd_ID_list_all_functions_all_buffers_lister = 93;
static i32 fcoder_metacmd_ID_list_all_functions_current_buffer = 94;
static i32 fcoder_metacmd_ID_list_all_functions_current_buffer_lister = 95;
static i32 fcoder_metacmd_ID_list_all_locations = 96;
static i32 fcoder_metacmd_ID_list_all_locations_case_insensitive = 97;
static i32 fcoder_metacmd_ID_list_all_locations_of_identifier = 98;
static i32 fcoder_metacmd_ID_list_all_locations_of_identifier_case_insensitive = 99;
static i32 fcoder_metacmd_ID_list_all_locations_of_selection = 100;
static i32 fcoder_metacmd_ID_list_all_locations_of_selection_case_insensitive = 101;
static i32 fcoder_metacmd_ID_list_all_locations_of_type_definition = 102;
static i32 fcoder_metacmd_ID_list_all_locations_of_type_definition_of_identifier = 103;
static i32 fcoder_metacmd_ID_list_all_substring_locations = 104;
static i32 fcoder_metacmd_ID_list_all_substring_locations_case_insensitive = 105;
static i32 fcoder_metacmd_ID_load_project = 106;
static i32 fcoder_metacmd_ID_load_theme_current_buffer = 107;
static i32 fcoder_metacmd_ID_load_themes_default_folder = 108;
static i32 fcoder_metacmd_ID_load_themes_hot_directory = 109;
static i32 fcoder_metacmd_ID_luis_add_space = 110;
static i32 fcoder_metacmd_ID_luis_add_tab = 111;
static i32 fcoder_metacmd_ID_luis_adjust_horizontal_view_toggle = 112;
static i32 fcoder_metacmd_ID_luis_backspace_char = 113;
static i32 fcoder_metacmd_ID_luis_build = 114;
static i32 fcoder_metacmd_ID_luis_center_view_top = 115;
static i32 fcoder_metacmd_ID_luis_close_all_other_panels = 116;
static i32 fcoder_metacmd_ID_luis_close_current_panel = 117;
static i32 fcoder_metacmd_ID_luis_code_index_next = 118;
static i32 fcoder_metacmd_ID_luis_code_index_prev = 119;
static i32 fcoder_metacmd_ID_luis_delete_char = 120;
static i32 fcoder_metacmd_ID_luis_eat_whitespace_backwards = 121;
static i32 fcoder_metacmd_ID_luis_end = 122;
static i32 fcoder_metacmd_ID_luis_escape = 123;
static i32 fcoder_metacmd_ID_luis_fsearch = 124;
static i32 fcoder_metacmd_ID_luis_goto_next_jump = 125;
static i32 fcoder_metacmd_ID_luis_goto_prev_jump = 126;
static i32 fcoder_metacmd_ID_luis_home = 127;
static i32 fcoder_metacmd_ID_luis_horizontal_mouse_wheel_scroll = 128;
static i32 fcoder_metacmd_ID_luis_indent_range = 129;
static i32 fcoder_metacmd_ID_luis_interactive_open_or_new = 130;
static i32 fcoder_metacmd_ID_luis_left_word = 131;
static i32 fcoder_metacmd_ID_luis_list_functions_all_buffers = 132;
static i32 fcoder_metacmd_ID_luis_list_nests_this_buffer = 133;
static i32 fcoder_metacmd_ID_luis_list_notes_all_buffers = 134;
static i32 fcoder_metacmd_ID_luis_list_notes_no_duplicates = 135;
static i32 fcoder_metacmd_ID_luis_list_notes_this_buffer = 136;
static i32 fcoder_metacmd_ID_luis_matching_file_cpp = 137;
static i32 fcoder_metacmd_ID_luis_matching_file_cpp_same_buffer = 138;
static i32 fcoder_metacmd_ID_luis_multiline_comment_toggle = 139;
static i32 fcoder_metacmd_ID_luis_open_build_panel = 140;
static i32 fcoder_metacmd_ID_luis_open_up_braces = 141;
static i32 fcoder_metacmd_ID_luis_print_scope_variables = 142;
static i32 fcoder_metacmd_ID_luis_remove_space = 143;
static i32 fcoder_metacmd_ID_luis_remove_tab = 144;
static i32 fcoder_metacmd_ID_luis_reselect_selection = 145;
static i32 fcoder_metacmd_ID_luis_return = 146;
static i32 fcoder_metacmd_ID_luis_right_word = 147;
static i32 fcoder_metacmd_ID_luis_rsearch = 148;
static i32 fcoder_metacmd_ID_luis_scope_braces = 149;
static i32 fcoder_metacmd_ID_luis_select_line = 150;
static i32 fcoder_metacmd_ID_luis_select_scope_contents = 151;
static i32 fcoder_metacmd_ID_luis_select_surrounding_scope = 152;
static i32 fcoder_metacmd_ID_luis_select_surrounding_scope_maximal = 153;
static i32 fcoder_metacmd_ID_luis_select_word_under_cursor = 154;
static i32 fcoder_metacmd_ID_luis_set_mark = 155;
static i32 fcoder_metacmd_ID_luis_show_search_panel = 156;
static i32 fcoder_metacmd_ID_luis_startup = 157;
static i32 fcoder_metacmd_ID_luis_toggle_modal_mode = 158;
static i32 fcoder_metacmd_ID_luis_try_exit = 159;
static i32 fcoder_metacmd_ID_luis_view_input_handler = 160;
static i32 fcoder_metacmd_ID_luis_view_peek_as_split_window = 161;
static i32 fcoder_metacmd_ID_luis_view_peek_in_parent_window = 162;
static i32 fcoder_metacmd_ID_luis_vim_string_matches = 163;
static i32 fcoder_metacmd_ID_luis_write_newline = 164;
static i32 fcoder_metacmd_ID_luis_write_pointer_arrow = 165;
static i32 fcoder_metacmd_ID_luis_write_tab = 166;
static i32 fcoder_metacmd_ID_luis_write_underscore = 167;
static i32 fcoder_metacmd_ID_make_directory_query = 168;
static i32 fcoder_metacmd_ID_miblo_decrement_basic = 169;
static i32 fcoder_metacmd_ID_miblo_decrement_time_stamp = 170;
static i32 fcoder_metacmd_ID_miblo_decrement_time_stamp_minute = 171;
static i32 fcoder_metacmd_ID_miblo_increment_basic = 172;
static i32 fcoder_metacmd_ID_miblo_increment_time_stamp = 173;
static i32 fcoder_metacmd_ID_miblo_increment_time_stamp_minute = 174;
static i32 fcoder_metacmd_ID_mouse_wheel_change_face_size = 175;
static i32 fcoder_metacmd_ID_mouse_wheel_scroll = 176;
static i32 fcoder_metacmd_ID_move_down = 177;
static i32 fcoder_metacmd_ID_move_down_10 = 178;
static i32 fcoder_metacmd_ID_move_down_textual = 179;
static i32 fcoder_metacmd_ID_move_down_to_blank_line = 180;
static i32 fcoder_metacmd_ID_move_down_to_blank_line_end = 181;
static i32 fcoder_metacmd_ID_move_down_to_blank_line_skip_whitespace = 182;
static i32 fcoder_metacmd_ID_move_left = 183;
static i32 fcoder_metacmd_ID_move_left_alpha_numeric_boundary = 184;
static i32 fcoder_metacmd_ID_move_left_alpha_numeric_or_camel_boundary = 185;
static i32 fcoder_metacmd_ID_move_left_token_boundary = 186;
static i32 fcoder_metacmd_ID_move_left_whitespace_boundary = 187;
static i32 fcoder_metacmd_ID_move_left_whitespace_or_token_boundary = 188;
static i32 fcoder_metacmd_ID_move_line_down = 189;
static i32 fcoder_metacmd_ID_move_line_up = 190;
static i32 fcoder_metacmd_ID_move_right = 191;
static i32 fcoder_metacmd_ID_move_right_alpha_numeric_boundary = 192;
static i32 fcoder_metacmd_ID_move_right_alpha_numeric_or_camel_boundary = 193;
static i32 fcoder_metacmd_ID_move_right_token_boundary = 194;
static i32 fcoder_metacmd_ID_move_right_whitespace_boundary = 195;
static i32 fcoder_metacmd_ID_move_right_whitespace_or_token_boundary = 196;
static i32 fcoder_metacmd_ID_move_up = 197;
static i32 fcoder_metacmd_ID_move_up_10 = 198;
static i32 fcoder_metacmd_ID_move_up_to_blank_line = 199;
static i32 fcoder_metacmd_ID_move_up_to_blank_line_end = 200;
static i32 fcoder_metacmd_ID_move_up_to_blank_line_skip_whitespace = 201;
static i32 fcoder_metacmd_ID_multi_paste = 202;
static i32 fcoder_metacmd_ID_multi_paste_interactive = 203;
static i32 fcoder_metacmd_ID_multi_paste_interactive_quick = 204;
static i32 fcoder_metacmd_ID_music_start = 205;
static i32 fcoder_metacmd_ID_music_stop = 206;
static i32 fcoder_metacmd_ID_navigate_back = 207;
static i32 fcoder_metacmd_ID_navigate_forward = 208;
static i32 fcoder_metacmd_ID_open_all_code = 209;
static i32 fcoder_metacmd_ID_open_all_code_recursive = 210;
static i32 fcoder_metacmd_ID_open_file_in_10x = 211;
static i32 fcoder_metacmd_ID_open_file_in_quotes = 212;
static i32 fcoder_metacmd_ID_open_file_in_visual_studio = 213;
static i32 fcoder_metacmd_ID_open_in_other = 214;
static i32 fcoder_metacmd_ID_open_long_braces = 215;
static i32 fcoder_metacmd_ID_open_long_braces_break = 216;
static i32 fcoder_metacmd_ID_open_long_braces_semicolon = 217;
static i32 fcoder_metacmd_ID_open_matching_file_cpp = 218;
static i32 fcoder_metacmd_ID_open_panel_hsplit = 219;
static i32 fcoder_metacmd_ID_open_panel_vsplit = 220;
static i32 fcoder_metacmd_ID_page_down = 221;
static i32 fcoder_metacmd_ID_page_up = 222;
static i32 fcoder_metacmd_ID_paste = 223;
static i32 fcoder_metacmd_ID_paste_and_indent = 224;
static i32 fcoder_metacmd_ID_paste_next = 225;
static i32 fcoder_metacmd_ID_paste_next_and_indent = 226;
static i32 fcoder_metacmd_ID_place_in_scope = 227;
static i32 fcoder_metacmd_ID_play_with_a_counter = 228;
static i32 fcoder_metacmd_ID_profile_clear = 229;
static i32 fcoder_metacmd_ID_profile_disable = 230;
static i32 fcoder_metacmd_ID_profile_enable = 231;
static i32 fcoder_metacmd_ID_profile_inspect = 232;
static i32 fcoder_metacmd_ID_project_command_F1 = 233;
static i32 fcoder_metacmd_ID_project_command_F10 = 234;
static i32 fcoder_metacmd_ID_project_command_F11 = 235;
static i32 fcoder_metacmd_ID_project_command_F12 = 236;
static i32 fcoder_metacmd_ID_project_command_F13 = 237;
static i32 fcoder_metacmd_ID_project_command_F14 = 238;
static i32 fcoder_metacmd_ID_project_command_F15 = 239;
static i32 fcoder_metacmd_ID_project_command_F16 = 240;
static i32 fcoder_metacmd_ID_project_command_F2 = 241;
static i32 fcoder_metacmd_ID_project_command_F3 = 242;
static i32 fcoder_metacmd_ID_project_command_F4 = 243;
static i32 fcoder_metacmd_ID_project_command_F5 = 244;
static i32 fcoder_metacmd_ID_project_command_F6 = 245;
static i32 fcoder_metacmd_ID_project_command_F7 = 246;
static i32 fcoder_metacmd_ID_project_command_F8 = 247;
static i32 fcoder_metacmd_ID_project_command_F9 = 248;
static i32 fcoder_metacmd_ID_project_command_lister = 249;
static i32 fcoder_metacmd_ID_project_fkey_command = 250;
static i32 fcoder_metacmd_ID_project_go_to_root_directory = 251;
static i32 fcoder_metacmd_ID_project_reprint = 252;
static i32 fcoder_metacmd_ID_query_replace = 253;
static i32 fcoder_metacmd_ID_query_replace_identifier = 254;
static i32 fcoder_metacmd_ID_query_replace_selection = 255;
static i32 fcoder_metacmd_ID_quick_swap_buffer = 256;
static i32 fcoder_metacmd_ID_redo = 257;
static i32 fcoder_metacmd_ID_redo_all_buffers = 258;
static i32 fcoder_metacmd_ID_reload_project = 259;
static i32 fcoder_metacmd_ID_rename_file_query = 260;
static i32 fcoder_metacmd_ID_reopen = 261;
static i32 fcoder_metacmd_ID_replace_in_all_buffers = 262;
static i32 fcoder_metacmd_ID_replace_in_buffer = 263;
static i32 fcoder_metacmd_ID_replace_in_range = 264;
static i32 fcoder_metacmd_ID_reverse_search = 265;
static i32 fcoder_metacmd_ID_reverse_search_identifier = 266;
static i32 fcoder_metacmd_ID_revert_buffer = 267;
static i32 fcoder_metacmd_ID_save = 268;
static i32 fcoder_metacmd_ID_save_all_dirty_buffers = 269;
static i32 fcoder_metacmd_ID_save_to_query = 270;
static i32 fcoder_metacmd_ID_search = 271;
static i32 fcoder_metacmd_ID_search_identifier = 272;
static i32 fcoder_metacmd_ID_seek_beginning_of_line = 273;
static i32 fcoder_metacmd_ID_seek_beginning_of_textual_line = 274;
static i32 fcoder_metacmd_ID_seek_end_of_line = 275;
static i32 fcoder_metacmd_ID_seek_end_of_textual_line = 276;
static i32 fcoder_metacmd_ID_select_all = 277;
static i32 fcoder_metacmd_ID_select_next_scope_absolute = 278;
static i32 fcoder_metacmd_ID_select_next_scope_after_current = 279;
static i32 fcoder_metacmd_ID_select_prev_scope_absolute = 280;
static i32 fcoder_metacmd_ID_select_prev_top_most_scope = 281;
static i32 fcoder_metacmd_ID_select_surrounding_scope = 282;
static i32 fcoder_metacmd_ID_select_surrounding_scope_maximal = 283;
static i32 fcoder_metacmd_ID_set_eol_mode_from_contents = 284;
static i32 fcoder_metacmd_ID_set_eol_mode_to_binary = 285;
static i32 fcoder_metacmd_ID_set_eol_mode_to_crlf = 286;
static i32 fcoder_metacmd_ID_set_eol_mode_to_lf = 287;
static i32 fcoder_metacmd_ID_set_face_size = 288;
static i32 fcoder_metacmd_ID_set_face_size_this_buffer = 289;
static i32 fcoder_metacmd_ID_set_mark = 290;
static i32 fcoder_metacmd_ID_set_mode_to_notepad_like = 291;
static i32 fcoder_metacmd_ID_set_mode_to_original = 292;
static i32 fcoder_metacmd_ID_setup_build_bat = 293;
static i32 fcoder_metacmd_ID_setup_build_bat_and_sh = 294;
static i32 fcoder_metacmd_ID_setup_build_sh = 295;
static i32 fcoder_metacmd_ID_setup_new_project = 296;
static i32 fcoder_metacmd_ID_show_file_in_explorer = 297;
static i32 fcoder_metacmd_ID_show_filebar = 298;
static i32 fcoder_metacmd_ID_show_scrollbar = 299;
static i32 fcoder_metacmd_ID_show_the_log_graph = 300;
static i32 fcoder_metacmd_ID_snipe_backward_whitespace_or_token_boundary = 301;
static i32 fcoder_metacmd_ID_snipe_forward_whitespace_or_token_boundary = 302;
static i32 fcoder_metacmd_ID_snippet_lister = 303;
static i32 fcoder_metacmd_ID_string_repeat = 304;
static i32 fcoder_metacmd_ID_suppress_mouse = 305;
static i32 fcoder_metacmd_ID_swap_panels = 306;
static i32 fcoder_metacmd_ID_theme_lister = 307;
static i32 fcoder_metacmd_ID_to_lowercase = 308;
static i32 fcoder_metacmd_ID_to_uppercase = 309;
static i32 fcoder_metacmd_ID_toggle_filebar = 310;
static i32 fcoder_metacmd_ID_toggle_fps_meter = 311;
static i32 fcoder_metacmd_ID_toggle_fullscreen = 312;
static i32 fcoder_metacmd_ID_toggle_highlight_enclosing_scopes = 313;
static i32 fcoder_metacmd_ID_toggle_highlight_line_at_cursor = 314;
static i32 fcoder_metacmd_ID_toggle_line_numbers = 315;
static i32 fcoder_metacmd_ID_toggle_line_wrap = 316;
static i32 fcoder_metacmd_ID_toggle_mouse = 317;
static i32 fcoder_metacmd_ID_toggle_paren_matching_helper = 318;
static i32 fcoder_metacmd_ID_toggle_show_whitespace = 319;
static i32 fcoder_metacmd_ID_toggle_text_drop_shadow = 320;
static i32 fcoder_metacmd_ID_toggle_virtual_whitespace = 321;
static i32 fcoder_metacmd_ID_tutorial_maximize = 322;
static i32 fcoder_metacmd_ID_tutorial_minimize = 323;
static i32 fcoder_metacmd_ID_uncomment_line = 324;
static i32 fcoder_metacmd_ID_undo = 325;
static i32 fcoder_metacmd_ID_undo_all_buffers = 326;
static i32 fcoder_metacmd_ID_view_buffer_other_panel = 327;
static i32 fcoder_metacmd_ID_view_jump_list_with_lister = 328;
static i32 fcoder_metacmd_ID_word_complete = 329;
static i32 fcoder_metacmd_ID_word_complete_drop_down = 330;
static i32 fcoder_metacmd_ID_write_block = 331;
static i32 fcoder_metacmd_ID_write_filename = 332;
static i32 fcoder_metacmd_ID_write_filepath = 333;
static i32 fcoder_metacmd_ID_write_hack = 334;
static i32 fcoder_metacmd_ID_write_note = 335;
static i32 fcoder_metacmd_ID_write_space = 336;
static i32 fcoder_metacmd_ID_write_text_and_auto_indent = 337;
static i32 fcoder_metacmd_ID_write_text_input = 338;
static i32 fcoder_metacmd_ID_write_todo = 339;
static i32 fcoder_metacmd_ID_write_underscore = 340;
static i32 fcoder_metacmd_ID_write_zero_struct = 341;
#endif
