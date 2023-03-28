
global View_ID vim_lister_view_id;
global f32 vim_nxt_filebar_offset;
global f32 vim_cur_filebar_offset;

#define foreach(i,N) for(i32 i=0; i<N; i++)
#define in_range(bot,mid,top) ((bot) <= (mid) && (mid) < (top))
#define ArrayInc(a,i) ((i+1) % ArrayCount(a))
#define ArrayDec(a,i) ((i + ArrayCount(a)-1) % ArrayCount(a))
#define printf_message(app, arena, str, ...) print_message(app, push_stringf(arena, str, __VA_ARGS__))

// TODO(BYP): Clean this up a bit more
function String_Const_u8
ctrl_backspace_utf8(String_Const_u8 string){
	if(string.size <= 0){ return string; }
	b32 clearing_whitespace = true;
	u64 i = string.size-2;
	for(; i>0; --i){
		if(!clearing_whitespace &&
		   (string.str[i] <= 0x7F || string.str[i] >= 0xC0) &&
		   (string.str[i] == '_' || !character_is_alpha_numeric(string.str[i])))
		{
			i++;
			break;
		}
		if(clearing_whitespace && !character_is_whitespace(string.str[i])){ clearing_whitespace=false; }
	}
	return SCu8(string.str, i);
}

function void
vim_lister__backspace(Application_Links *app){
	View_ID view = get_this_ctx_view(app, Access_Always);
	Lister *lister = view_get_lister(app, view);
	if(lister){
		User_Input input = get_current_input(app);
		if(has_modifier(&input, KeyCode_Control)){
			if(has_modifier(&input, KeyCode_Shift)){
				lister->text_field.size = 0;
                minibar_string.size = 0;
			}else{
				lister->text_field.string = ctrl_backspace_utf8(lister->text_field.string);
                minibar_string.string = ctrl_backspace_utf8(minibar_string.string);
			}
		}else{
			lister->text_field.string = backspace_utf8(lister->text_field.string);
            minibar_string.string = backspace_utf8(minibar_string.string);
		}
        
		lister_set_key(lister, lister->text_field.string);
		lister->item_index = 0;
		lister_zero_scroll(lister);
		lister_update_filtered_list(app, lister);
	}
}



function Lister_Activation_Code
vim_lister__write_character(Application_Links *app){
    Lister_Activation_Code result = ListerActivation_Continue;
    View_ID view = get_active_view(app, Access_Always);
    Lister *lister = view_get_lister(app, view);
    if (lister != 0){
        User_Input in = get_current_input(app);
        String_Const_u8 string = to_writable(&in);
        if (string.str != 0 && string.size > 0){
            lister_append_text_field(lister, string);
            lister_append_key(lister, string);
            
            string_append(&minibar_string, string);
            
            lister->item_index = 0;
            lister_zero_scroll(lister);
            lister_update_filtered_list(app, lister);
        }
    }
    return(result);
}


function void
vim_lister_file__backspace(Application_Links *app){
	View_ID view = get_this_ctx_view(app, Access_Always);
	Lister *lister = view_get_lister(app, view);
	if(lister){
		User_Input input = get_current_input(app);
		if(has_modifier(&input, KeyCode_Control)){
			if(has_modifier(&input, KeyCode_Shift)){
				lister->text_field.size = 0;
				while(!character_is_slash(lister->text_field.str[++lister->text_field.size])){}
				++lister->text_field.size;
				String_u8 temp = lister->text_field;
                
				set_hot_directory(app, temp.string);
                
                minibar_string.size = Min(minibar_string.cap, temp.string.size);
                block_copy(minibar_string.str, temp.string.str, minibar_string.size);
                
				lister_call_refresh_handler(app, lister);
				lister->text_field = temp;
			}else{
				String_Const_u8 string = lister->text_field.string;
				if(character_is_slash(string.str[string.size-1])){
					string.size--;
					i64 slash_index = string_find_last_slash(string);
					if(slash_index >= 0){
						string.size = slash_index+1;
						lister->text_field.string = string;
                        
						set_hot_directory(app, string);
                        minibar_string.size = Min(minibar_string.cap, string.size);
                        block_copy(minibar_string.str, string.str, minibar_string.size);
                        
						String_u8 temp = lister->text_field;
						lister_call_refresh_handler(app, lister);
						lister->text_field = temp;
					}
				}else{
					lister->text_field.string = ctrl_backspace_utf8(lister->text_field.string);
                    minibar_string.string = ctrl_backspace_utf8(minibar_string.string);
				}
			}
		}else{
			lister->text_field.string = backspace_utf8(lister->text_field.string);
            minibar_string.string = backspace_utf8(minibar_string.string);
		}
        
		String_Const_u8 text_field = lister->text_field.string;
		String_Const_u8 new_key = string_front_of_path(text_field);
		lister_set_key(lister, new_key);
        
		lister->item_index = 0;
		lister_zero_scroll(lister);
		lister_update_filtered_list(app, lister);
	}
}


function Lister_Activation_Code
vim_lister__write_character__file_path(Application_Links *app){
	Lister_Activation_Code result = ListerActivation_Continue;
	View_ID view = get_this_ctx_view(app, Access_Always);
	Lister *lister = view_get_lister(app, view);
	if(lister != 0){
		User_Input in = get_current_input(app);
		String_Const_u8 string = to_writable(&in);
		if(string.str != 0 && string.size > 0){
			lister_append_text_field(lister, string);
            string_append(&minibar_string, string);
			if(character_is_slash(string.str[0])){
				lister->out.text_field = lister->text_field.string;
                result = ListerActivation_Finished;
			}
			else{
				String_Const_u8 front_name = string_front_of_path(lister->text_field.string);
				lister_set_key(lister, front_name);
			}
			lister->item_index = 0;
			lister_zero_scroll(lister);
			lister_update_filtered_list(app, lister);
		}
	}
	return(result);
}

function void
vim_lister_set_default_handlers(Lister *lister){
	Lister_Handlers handlers = lister_get_default_handlers();
	handlers.backspace       = vim_lister__backspace;
    handlers.write_character = vim_lister__write_character;
	lister_set_handlers(lister, &handlers);
}

function File_Name_Result
vim_convert_lister_result_to_file_name_result(Lister_Result l_result){
	File_Name_Result result = {};
	result.canceled = l_result.canceled;
	if(l_result.canceled){ return result; }
	result.clicked = l_result.activated_by_click;
	if(l_result.user_data != 0){
		String_Const_u8 name = SCu8((u8*)l_result.user_data);
		result.file_name_activated = name;
		result.is_folder = character_is_slash(string_get_character(name, name.size - 1));
	}
	result.file_name_in_text_field = string_front_of_path(l_result.text_field);
    
	String_Const_u8 path = {};
	if(l_result.user_data && result.file_name_in_text_field.size && l_result.text_field.size > 0){
		result.file_name_in_text_field = string_front_folder_of_path(l_result.text_field);
		path = string_remove_front_folder_of_path(l_result.text_field);
	}else{
		path = string_remove_front_of_path(l_result.text_field);
	}
	if(character_is_slash(string_get_character(path, path.size-1))){
		path = string_chop(path, 1);
	}
	result.path_in_text_field = path;
    
	return(result);
}


function f32 
vim_lister_get_block_height(f32 line_height){ 
    return 1.5f*line_height; 
}

function void*
vim_lister_user_data_at_p(Application_Links *app, View_ID view, Lister *lister, Vec2_f32 m_p, i32 col_num) {
    #if 0
	Rect_f32 region = vim_get_bottom_rect(app);
    #else
    Rect_f32 region = global_get_screen_rectangle(app);
    f32 lineheight = get_face_metrics(app, get_face_id(app, 0)).line_height;
	region.y1 -= 1*lineheight;
	region.y0 = region.y1 - 4*lineheight;
    #endif
    
	f32 line_height = get_face_metrics(app, get_face_id(app, 0)).line_height;
	f32 block_height = vim_lister_get_block_height(line_height);
	f32 block_width = rect_width(region)/col_num;
    
    if(rect_contains_point(region, m_p)){
        f32 y = m_p.y - region.y0 + lister->scroll.position.y;
        i32 index = i32(m_p.x/block_width) + col_num*i32(y/block_height);
        if(in_range(0, index, lister->filtered.count)){
            return lister->filtered.node_ptrs[index]->user_data;
        }
    }
    
    return 0;
}

struct Vim_Calc_Col_Row_Count {
    i32 visible_col_count;
    i32 visible_row_count; 
    i32 max_row_count;
};

function Vim_Calc_Col_Row_Count
calc_col_row(Application_Links *app, Lister *lister){
	Vec2_f32 dim = rect_dim(global_get_screen_rectangle(app));
	Face_ID face_id = get_face_id(app, 0);
	Face_Metrics metrics = get_face_metrics(app, face_id);
	f32 line_height = metrics.line_height;
	f32 max_advance = metrics.max_advance;
	f32 block_height = vim_lister_get_block_height(line_height);
	//f32 max_lister_height = dim.y*VIM_LISTER_MAX_RATIO - 2.f*line_height;
    f32 max_lister_height = dim.y*0.35f - 1.f*line_height;
    
    
	u64 max_name_size = 0;
	//Range_i32 lister_range = Ii32(VIM_LISTER_RANGE);
    Range_i32 lister_range = Ii32(3, 5);
	const i32 N = Min(lister_range.max*i32(block_height/max_lister_height), lister->filtered.count);
	foreach(i,N){
		if(lister->filtered.node_ptrs[i]->string.size > max_name_size){
			max_name_size = lister->filtered.node_ptrs[i]->string.size;
		}
	}
    
	i32 col_num = i32(dim.x/((max_name_size+7)*max_advance));
	col_num = clamp(lister_range.min, col_num, lister_range.max);
    
    i32 max_column_count = lister->vim_max_col_count;
    if (max_column_count == 0) max_column_count = 16; 
    col_num = Min(col_num, max_column_count);
    
	i32 max_row_num = 1 + lister->filtered.count/col_num;
	i32 row_num = i32((dim.y*0.5f - 2.f*line_height)/block_height);
	row_num = Min(row_num, max_row_num);
    
    Vim_Calc_Col_Row_Count result = {};
    result.visible_col_count   = col_num;
    result.visible_row_count   = row_num;
    result.max_row_count = max_row_num;
    if (lister->filtered.count == 0) {
        result.visible_row_count = 0; //NOTE shouldn't this be 1... for some reason BYP makes it 0?...
        result.max_row_count = 1;
    }
    return result;
}

// TODO(BYP): Be more rigorous here. I'm sure there are off-by-1's and other incorrect assumptions
function void
vim_lister_render(Application_Links *app, Frame_Info frame_info, View_ID view){
	Scratch_Block scratch(app);
    
	Lister *lister = view_get_lister(app, view);
	if(lister == 0) { return; }
    
 // Set up region for drawing
    Rect_f32 global_rect = global_get_screen_rectangle(app); 
	Rect_f32 region = global_rect;
	Rect_f32 prev_clip = draw_set_clip(app, region);
    
	Face_ID face_id = get_face_id(app, 0);
	Face_Metrics metrics = get_face_metrics(app, face_id);
	f32 line_height = metrics.line_height;
	
	f32 block_height = vim_lister_get_block_height(line_height);
    
    Vim_Calc_Col_Row_Count col_row_count = calc_col_row(app, lister);
    
        
    i32 col_num = col_row_count.visible_col_count;
    i32 row_num = col_row_count.visible_row_count;
	
    
	lister->visible_count = Min(col_num*row_num, lister->filtered.count);
    
 // TODO(BYP) check exactly why row_num+2. Had to update when changing block_height
    #if 0
	//region = rect_split_top_bottom_neg(region, (row_num+2)*block_height).max;
    region = rect_split_top_bottom_neg(region, (row_num+1)*block_height).max;
	region = rect_split_top_bottom_neg(region, 1.f*line_height).min;
    #else
    region.y1 -= 1.f*line_height;
    region.y0 = region.y1 - vim_cur_filebar_offset;
    //f32 filebar_y = global_rect.y1 - 1.f*line_height - vim_cur_filebar_offset;
    //region.y0 = filebar_y;
    #endif
	vim_nxt_filebar_offset = row_num*block_height + 0.1f;
 // non-zero so when lister displays no results, it still displays the cursor
    
 // Render the view
	Render_Caller_Function *render_caller = (Render_Caller_Function *)get_custom_hook(app, HookID_RenderCaller);
	render_caller(app, frame_info, view);
	draw_set_clip(app, region);
    
	Mouse_State mouse = get_mouse_state(app);
	Vec2_f32 m_p = V2f32(mouse.p);
    
 // NOTE(allen): auto scroll to the item if the flag is set.
	f32 scroll_y = lister->scroll.position.y;
    
	if(lister->set_vertical_focus_to_item){
		lister->set_vertical_focus_to_item = false;
		Range_f32 item_y = If32_size((lister->item_index/col_num)*block_height, block_height);
		f32 view_h = rect_height(region);
		Range_f32 view_y = If32_size(scroll_y, view_h);
		if(view_y.min > item_y.min || item_y.max > view_y.max){
			f32 item_center = (item_y.min + item_y.max)*0.5f;
			f32 view_center = (view_y.min + view_y.max)*0.5f;
			f32 margin = view_h*.3f;
			margin = clamp_top(margin, block_height*3.f);
			if(item_center < view_center){
				lister->scroll.target.y = item_y.min - margin;
            }
			else{
				f32 target_bot = item_y.max + margin;
				lister->scroll.target.y = target_bot - view_h;
            }
        }
    }
    
 // NOTE(allen): clamp scroll target and position; smooth scroll rule
	i32 count = lister->filtered.count;
 //Range_f32 scroll_range = If32(0.f, clamp_bot(0.f, (count/col_num)*block_height));
	Range_f32 scroll_range = If32(0.f, clamp_bot(0.f, (count/col_num - 1)*block_height));
	lister->scroll.target.y = clamp_range(scroll_range, lister->scroll.target.y);
	lister->scroll.target.x = 0.f;
    
	Vec2_f32_Delta_Result delta = delta_apply(app, view, frame_info.animation_dt, lister->scroll);
	lister->scroll.position = delta.p;
	if(delta.still_animating){ animate_in_n_milliseconds(app, 0); }
    
	lister->scroll.position.y = clamp_range(scroll_range, lister->scroll.position.y);
	lister->scroll.position.x = 0.f;
    
	scroll_y = lister->scroll.position.y;
	i32 first_index = (i32)(col_num*scroll_y/(block_height));
    
	f32 x_base = region.x0;
	f32 y_base = region.y1 - vim_cur_filebar_offset;
	f32 block_width = rect_width(region)/col_num;
	Rect_f32 back_rect = region;
	back_rect.y0 = y_base;
	back_rect.y1 = y_base + rect_height(region);
	draw_rectangle_fcolor(app, back_rect, 0.f, fcolor_id(defcolor_back));
    
	Fancy_Block block = {};
	for(i32 i=first_index; i<count; i++){
		Lister_Node *node = lister->filtered.node_ptrs[i];
        
		f32 x0 = x_base + block_width*((i-first_index) % col_num);
		f32 y0 = y_base + block_height*((i-first_index) / col_num);
		if(y0 > region.y1){ break; }
		Rect_f32 item_rect = Rf32(x0, y0, x0+block_width, y0+block_height);
		Rect_f32 item_inner = rect_inner(item_rect, 3.f);
        
		b32 hovered = rect_contains_point(item_rect, m_p);
		if(hovered){
			Fancy_Line *line1 = push_fancy_line(scratch, &block, face_id);
			u64 index = string_find_last(node->status, '\n');
            
			if(index < node->status.size){
				push_fancy_string(scratch, line1, fcolor_id(defcolor_pop2), string_prefix(node->status, index));
				Fancy_Line *line2 = push_fancy_line(scratch, &block, face_id);
				push_fancy_string(scratch, line2, fcolor_id(defcolor_text_default), string_postfix(node->status, node->status.size-1 - index));
            }else{
				push_fancy_string(scratch, line1, fcolor_id(defcolor_pop2), node->status);
            }
        }
		UI_Highlight_Level highlight = UIHighlight_None;
		if(node == lister->highlighted_node){
			highlight = UIHighlight_Active;
        }else if(node->user_data == lister->hot_user_data){
			highlight = hovered ? UIHighlight_Active : UIHighlight_Hover;
        }else if(hovered){
			highlight = UIHighlight_Hover;
        }
        
		u64 lister_roundness_100 = def_get_config_u64(app, vars_save_string_lit("lister_roundness"));
		f32 roundness = block_height*lister_roundness_100*0.01f;
		draw_rectangle_fcolor(app, item_rect, roundness, get_item_margin_color(highlight));
		draw_rectangle_fcolor(app, item_inner, roundness, get_item_margin_color(highlight, 1));
        
		Fancy_Line line = {};
		push_fancy_string(scratch, &line, fcolor_id(defcolor_text_default), node->string);
		push_fancy_stringf(scratch, &line, " ");
		u64 index = string_find_last(node->status, '\n');
		push_fancy_string(scratch, &line, fcolor_id(defcolor_pop2), string_prefix(node->status, index));
        
		Vec2_f32 p = item_inner.p0 + V2f32(3.f, (block_height - line_height)*0.5f);
		draw_fancy_line(app, face_id, fcolor_zero(), &line, p);
    }
	f32 x_padding = metrics.normal_advance;
	f32 x_half_padding = x_padding*0.5f;
	draw_drop_down(app, face_id, &block, m_p, region, x_padding, x_half_padding, fcolor_id(defcolor_margin_hover), fcolor_id(defcolor_back));
    
	if(lister->visible_count != 0){
		Rect_f32 rect = Rect_f32{region.x0, region.y1 - 4.f, region.x1, region.y1};
		draw_rectangle_fcolor(app, rect, 0.f, get_item_margin_color(UIHighlight_Active));
    }
    
	draw_set_clip(app, prev_clip);
}

function void vim_change_lister_view_back(Application_Links *app){
	view_set_active(app, vim_lister_view_id);
	vim_lister_view_id = 0;
}

function Lister_Result
vim_run_lister(Application_Links *app, Lister *lister){
    View_ID active_view = get_active_view(app, Access_ReadVisible);
    Buffer_ID active_buffer = view_get_buffer(app, active_view, Access_ReadVisible);
    
	Lister_Result ret;
	View_ID view = get_this_ctx_view(app, Access_Always);
	vim_lister_view_id = view;
	Scratch_Block scratch(app);
	lister->filter_restore_point = begin_temp(lister->arena);
	lister_update_filtered_list(app, lister);
    
	//vim_use_bottom_cursor = true;
	bool do_invalidate = true;
	//vim_show_buffer_peek = false;
    
	View_Context ctx = view_current_context(app, view);
	Rect_f32 global_rect = global_get_screen_rectangle(app);
    
	ctx.render_caller = vim_lister_render;
	ctx.hides_buffer = false;
	View_Context_Block ctx_block(app, view, &ctx);
    
    
	User_Input in = {};
	for(;;) {
        Vim_Calc_Col_Row_Count col_row = calc_col_row(app, lister);
		i32 col_num = col_row.visible_col_count;
		i32 visible_count = col_row.visible_col_count*col_row.visible_row_count;
        
        
		animate_in_n_milliseconds(app, 0);
        
		Lister_Activation_Code result = ListerActivation_Continue;
		b32 handled = true;
        
        auto append_textfield_and_update_list = [app, &lister](String_Const_u8 string) {
            lister_append_text_field(lister, string);
            lister_append_key(lister, string);
            
            string_append(&minibar_string, string);
            
            lister->item_index = 0;
            lister_zero_scroll(lister);
            lister_update_filtered_list(app, lister);
        };
        
		in = get_next_input(app, EventPropertyGroup_Any, EventProperty_Escape);
		if (in.abort){
			block_zero_struct(&lister->out);
			lister->out.canceled = true;
			//vim_reset_bottom_text();
			break;
		}
        
        
		if (in.event.kind == InputEventKind_KeyStroke) {
            if (in.event.key.code == KeyCode_W) {
                if (has_modifier(&in, KeyCode_Control)) {
                    block_zero_struct(&lister->out);
                    lister->out.canceled = true;
                    //lister_textfield_modified = true;
                    //vim_reset_bottom_text();
                    break;
                }
            } 
		}
        
		switch(in.event.kind){
        
        case InputEventKind_TextInsert:{
            //vim_cursor_blink = 0;
            if(lister->handlers.write_character) {
                result = lister->handlers.write_character(app);
                //lister_textfield_modified = true;
            }
        } break;
        
        case InputEventKind_KeyStroke: {
            //vim_cursor_blink = 0;
            
            switch(in.event.key.code){
            
            case KeyCode_Tab:{
                i32 delta = (has_modifier(&in.event, KeyCode_Shift) ? -1 : 1);
                if(lister->handlers.navigate != 0){
                    lister->handlers.navigate(app, view, lister, delta);
                }else if(lister->handlers.key_stroke != 0){
                    result = lister->handlers.key_stroke(app);
                }else{ handled = false; }
            } break;
            
            case KeyCode_Return:{
                void *user_data = 0;
                if(in_range(0, lister->raw_item_index, lister->options.count)){
                    user_data = lister_get_user_data(lister, lister->raw_item_index);
                    
                    //upate minibar string to fill in what the entry string was
                    //minibar_current_length = Min(minibar_max_size_can_write, lister->highlighted_node->string.size);
                    //block_copy(minibar_string_at, lister->highlighted_node->string.str, minibar_current_length);
                }
                lister_activate(app, lister, user_data, false);
                result = ListerActivation_Finished;
            } break;
            
            case KeyCode_Space: if (has_modifier(&in, KeyCode_Control)) {
                void *user_data = 0;
                if(in_range(0, lister->raw_item_index, lister->options.count)){
                    user_data = lister_get_user_data(lister, lister->raw_item_index);
                    
                    //upate minibar string to fill in what the entry string was
                    //minibar_current_length = Min(minibar_max_size_can_write, lister->highlighted_node->string.size);
                    //block_copy(minibar_string_at, lister->highlighted_node->string.str, minibar_current_length);
                }
                lister_activate(app, lister, user_data, false);
                result = ListerActivation_Finished;
            } else {
                handled = false;
            } 
            break;
            
            case KeyCode_Backspace:{
                if(lister->handlers.backspace != 0){
                    lister->handlers.backspace(app);
                    //lister_textfield_modified = true;
                }else if(lister->handlers.key_stroke != 0){
                    result = lister->handlers.key_stroke(app);
                }else{ handled = false; }
            } break;
            
            case KeyCode_Down:
            case KeyCode_Up: {
                #if 0
                if(lister->handlers.navigate != 0){
                    i32 delta = (in.event.key.code == KeyCode_Up ? -1 : 1);
                    lister->handlers.navigate(app, view, lister, delta*col_num);
                }else if(lister->handlers.key_stroke != 0){
                    result = lister->handlers.key_stroke(app);
                }else{ handled = false; }
                #else
                //NOTE this still doesn't work too well because I'm not sure why but scolling up/down the lister
                //shifts the entrires around visually but we still match with the entry we were goint to (it's just drawn at a different location)
                //it's super wierd...
                if (lister->handlers.navigate) {
                    i32 ypos = lister->item_index / col_num;
                    i32 xpos = lister->item_index - ypos*col_num;
                    
                    if (in.event.key.code == KeyCode_Down) {
                        ypos += 1;
                        if (ypos >= col_row.max_row_count) ypos = 0;
                    }
                    else {
                        ypos -= 1;
                        if (ypos < 0) ypos = col_row.max_row_count-1;
                    }
                    
                    i32 new_item_index = ypos*col_num + xpos;    
                    lister->handlers.navigate(app, view, lister, new_item_index - lister->item_index);
                }
                #endif
            } break;
            
            case KeyCode_E:
            case KeyCode_D: if (has_modifier(&in, KeyCode_Control)) {
                if (lister->handlers.navigate) {
                    i32 ypos = lister->item_index / col_num;
                    i32 xpos = lister->item_index - ypos*col_num;
                    
                    if (in.event.key.code == KeyCode_D) {
                        ypos += 1;
                        if (ypos >= col_row.max_row_count) ypos = 0;
                    }
                    else {
                        ypos -= 1;
                        if (ypos < 0) ypos = col_row.max_row_count-1;
                    }
                    
                    i32 new_item_index = ypos*col_num + xpos;    
                    lister->handlers.navigate(app, view, lister, new_item_index - lister->item_index);
                }
            } else {
                handled = false;   
            }
            break;
            
            case KeyCode_Right:
            case KeyCode_Left:{
                #if 0 //prev way
                i32 delta = (in.event.key.code == KeyCode_Left ? -1 : 1);
                if(lister->handlers.navigate != 0){
                    lister->handlers.navigate(app, view, lister, delta);
                }else if(lister->handlers.key_stroke != 0){
                    result = lister->handlers.key_stroke(app);
                }else{ handled = false; }
                #else
                
                if (lister->handlers.navigate) {
                    i32 ypos = lister->item_index / col_num;
                    i32 xpos = lister->item_index - ypos*col_num;
                    
                    if (in.event.key.code == KeyCode_Left) {
                        xpos -= 1;
                        if (xpos < 0) xpos = col_num-1;
                    }
                    else {
                        xpos += 1;
                        if (xpos >= col_num) xpos = 0;
                    }
                    
                    i32 new_item_index = ypos*col_num + xpos;    
                    lister->handlers.navigate(app, view, lister, new_item_index - lister->item_index);
                }
                #endif
            } break;
            
            case KeyCode_S:
            case KeyCode_F: if (has_modifier(&in, KeyCode_Control)) {
                if (lister->handlers.navigate) {
                    i32 ypos = lister->item_index / col_num;
                    i32 xpos = lister->item_index - ypos*col_num;
                    
                    if (in.event.key.code == KeyCode_S) {
                        xpos -= 1;
                        if (xpos < 0) xpos = col_num-1;
                    }
                    else {
                        xpos += 1;
                        if (xpos >= col_num) xpos = 0;
                    }
                    
                    i32 new_item_index = ypos*col_num + xpos;    
                    lister->handlers.navigate(app, view, lister, new_item_index - lister->item_index);
                }
            } else {
                handled = false;   
            }
            break;
            
            case KeyCode_PageDown:
            case KeyCode_PageUp:{
                if(lister->handlers.navigate != 0){
                    i32 delta = (in.event.key.code == KeyCode_PageUp ? -1 : 1);
                    lister->handlers.navigate(app, view, lister, delta*visible_count);
                }else if(lister->handlers.key_stroke != 0){
                    result = lister->handlers.key_stroke(app);
                }else{ handled = false; }
            } break;
            
            //NOTE(luis) setting handling to false will create a textinput event next iteration
            case KeyCode_Comma: { //NOTE(luis) added this
                if (has_modifier(&in, KeyCode_Control)) {
                    append_textfield_and_update_list(SCu8("_"));
                } else {
                    handled = false;    
                }
            } break;
            
            case KeyCode_Period: { //NOTE(luis) added this
                if (has_modifier(&in, KeyCode_Control)) {
                    append_textfield_and_update_list(SCu8("->"));
                } 
                else handled = false;
            } break;
            
            case KeyCode_J: { //NOTE(luis) added this
                if (has_modifier(&in, KeyCode_Control)) {
                    if (lister->text_field.size == 0) {
                        i64 pos = view_get_cursor_pos(app, active_view);
                        Token *token = get_token_from_pos(app, active_buffer, pos);
                        if (token && (token->size > 0)) {
                            String_Const_u8 word = push_buffer_range(app, scratch, active_buffer, Ii64(token));
                            append_textfield_and_update_list(word);
                        }
                    }
                } 
                else handled = false;
            } break;
            
            default: {
                if(lister->handlers.key_stroke != 0){
                    result = lister->handlers.key_stroke(app);
                } else{ handled = false; }
            } break;
            }
        } break;
        
        case InputEventKind_MouseWheel:{
            lister->scroll.target.y += get_mouse_state(app).wheel;
            lister_update_filtered_list(app, lister);
        } break;
        
        case InputEventKind_MouseMove:{ lister_update_filtered_list(app, lister); } break;
        
        case InputEventKind_Core:{
            switch(in.event.core.code){
            case CoreCode_Animate:{ lister_update_filtered_list(app, lister); } break;
            
            default:{ handled = false; } break;
            }
        } break;
        
        default:{ handled = false; } break;
		}
        
        // NOTE: This is a bit of a hack to make clicking the lister not change views
		Mouse_State mouse_state = get_mouse_state(app);
		Vec2_f32 mouse_pos = V2f32(mouse_state.p);
		if(mouse_state.press_l){
			void *clicked = vim_lister_user_data_at_p(app, view, lister, mouse_pos, col_num);
			if(clicked){
				lister_activate(app, lister, clicked, true);
				result = ListerActivation_Finished;
				do_invalidate = false;
				view_enqueue_command_function(app, view, vim_change_lister_view_back);
			}
		}
        
		if(result == ListerActivation_ContinueAndRefresh){
			lister_call_refresh_handler(app, lister);
		}
		else if(result == ListerActivation_Finished){ break; }
        
		if(handled){ continue; }
		Fallback_Dispatch_Result disp_result = fallback_command_dispatch(app, lister->mapping, lister->map, &in);
		if(disp_result.code == FallbackDispatch_DelayedUICall){
			call_after_ctx_shutdown(app, view, disp_result.func);
			break;
		}
		if(disp_result.code == FallbackDispatch_Unhandled){ leave_current_input_unhandled(app); }
		else{ lister_call_refresh_handler(app, lister); }
	}
    
	//vim_use_bottom_cursor = false;
	vim_nxt_filebar_offset = 0.f;
	if(do_invalidate){ vim_lister_view_id = 0; }
    
	if(!in.abort){
		//String_Const_u8 command_name = string_substring(vim_bot_text.string, Ii64(base_size,vim_bot_text.size));
		//vim_register_copy(&vim_registers.command, command_name);
		//vim_update_registers(app);
	}
    
	ret = lister->out;
	vim_lister_view_id = 0;
    
    minibar_string.size   = 0;
    minibar_string.str[0] = 0;
	return ret;
}

function void
vim_lister__navigate__default(Application_Links *app, View_ID view, Lister *lister, i32 delta){
    i32 new_index = lister->item_index + delta;
    if (new_index < 0 && lister->item_index == 0){
        lister->item_index = lister->filtered.count - 1;
    }
    else if (new_index >= lister->filtered.count &&
             lister->item_index == lister->filtered.count - 1){
        lister->item_index = 0;
    }
    else{
        lister->item_index = clamp(0, new_index, lister->filtered.count - 1);
    }
    lister->set_vertical_focus_to_item = true;
    lister_update_selection_values(lister);
}


function Lister_Result
vim_run_lister_with_refresh_handler(Application_Links *app, Arena *arena, String_Const_u8 query, Lister_Handlers handlers, 
                                    bool is_switch_lister_move_right = false){
	Lister_Result result = {};
	if(handlers.refresh != 0){
		Lister_Block lister(app, arena);
		lister_set_query(lister, query);
		lister_set_handlers(lister, &handlers);
                
		handlers.refresh(app, lister);
        
        //if (is_switch_lister_move_right) {
            //View_ID view = get_this_ctx_view(app, Access_Always);
            //handlers.navigate(app, view, lister, 1);
        //}
        
		result = vim_run_lister(app, lister);
	}
	else{
        #define M "ERROR: No refresh handler specified for lister (query_string = \"%.*s\")\n"
		print_message(app, push_u8_stringf(arena, M, string_expand(query)));
        #undef M
		result.canceled = true;
	}
	return(result);
}


function File_Name_Result
vim_get_file_name_from_user(Application_Links *app, Arena *arena, String_Const_u8 query, View_ID view){
	Lister_Handlers handlers = lister_get_default_handlers();
	handlers.refresh = generate_hot_directory_file_list;
	handlers.write_character = vim_lister__write_character__file_path;
	handlers.backspace = vim_lister_file__backspace;
    handlers.navigate = vim_lister__navigate__default;
    
    //minibar_string.size = 0;
	//vim_reset_bottom_text(); //TODO make string zero basically
	Lister_Result l_result = vim_run_lister_with_refresh_handler(app, arena, query, handlers);
	return vim_convert_lister_result_to_file_name_result(l_result);
}
