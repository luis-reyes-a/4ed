#ifndef LUIS_CUSTOM_LAYER_H //created on June 26th 2022 
#define LUIS_CUSTOM_LAYER_H

#if !SHIP_MODE
	#define assert(x) ((x) ? true : (*((i32*)0) = 1)) 
#else
	#define assert(x)  
#endif

#define SWAP(a,b) do { auto glue(__swap_var,__LINE__) = a; a = b; b = glue(__swap_var,__LINE__); } while(0)

#define for_index_inc(type, varname, array_count) for(type varname = 0; varname < (array_count); varname += 1)
#define countof(array) _countof(array) //nicer than sizeof(array)/sizeof(array[0]) because it'll emit error with pointers
#define CLAMP_HI(var, max) (((var) > (max)) ? ((var) = (max)) : (var))
#define CLAMP_LO(var, min) (((var) < (min)) ? ((var) = (min)) : (var))
#define CLAMP(var, min, max) (assert(max > min), CLAMP_LO(var, min), CLAMP_HI(var, max))

struct View_Jump_History_Entry {
    Buffer_ID buffer;
    Buffer_Scroll scroll;
    i64 pos;
};
 
struct View_Jump_History {
    View_Jump_History_Entry entries[8]; 
    i32 entry_count;
    i32 prev_at;
    i32 at;
}; 





CUSTOM_ID(attachment, view_jump_history);
CUSTOM_ID(attachment, view_custom_flags);
CUSTOM_ID(attachment, view_code_peek_state);
//CUSTOM_ID(attachment, view_tab_group_index);
//CUSTOM_ID(attachment, view_tab_group);
CUSTOM_ID(attachment, view_prev_render_caller);
CUSTOM_ID(attachment, view_prev_buffer_location);
CUSTOM_ID(attachment, view_prev_mark_pos_before_snap_to_cursor);
//CUSTOM_ID(attachment, view_current_tab); not good idea, you'd have to store per view, per BUFFER_TAB_GROUP_COUNT


CUSTOM_ID(colors, luiscolor_namespace);
CUSTOM_ID(colors, luiscolor_type);
CUSTOM_ID(colors, luiscolor_macro);
CUSTOM_ID(colors, luiscolor_function);
CUSTOM_ID(colors, luiscolor_variable_decl);
CUSTOM_ID(colors, luiscolor_modal_cursor);
CUSTOM_ID(colors, luiscolor_tab_current);
CUSTOM_ID(colors, luiscolor_tab_regular);
CUSTOM_ID(colors, luiscolor_logical_not);

global bool g_mark_is_active;
global b32 IN_MODAL_MODE;
global b32 SHOW_BRACE_LINE_ANNOTATIONS;
global b32 HIGHLIGHT_PRINTF_SPECIFIER = true;

// global s32 g_recently_opened_files_count;
// global String_Const_u8 g_recently_opened_files[8];

bool *luis_get_mark_is_active() {
    return &g_mark_is_active;
} 

//global b32 MAKE_NEW_BUFFER_TAB_GROUP_ON_VIEW_CREATION;
global Face_ID SMALL_CODE_FACE;
global Face_ID ITALICS_CODE_FACE;
global Face_ID BOLD_CODE_FACE;
global i64 PREV_PASTE_INIT_CURSOR_POS = -1;
//global i32 CURSOR_PEEK_CODE_INDEX_RELATIVE_LINE_OFFSET = -1;

global u8 minibar_string_buffer[512];
global String_u8 minibar_string;

enum Custom_View_Flags 
{
   VIEW_IS_PEEK_WINDOW        = (1 << 0),
   VIEW_NOTEPAD_MODE_MARK_SET = (1 << 1),
   VIEW_ADD_NEW_BUFFER_AS_NEW_TAB = (1 << 2), //otherwise we overwrite current_tab
   VIEW_LISTER_INIT_RENDER_SNAP_TO_LINE = (1 << 3),
   //VIEW_KILL_TAB_GROUP_ON_VIEW_CLOSE = (1 << 3), //search in luis_hooks.cpp if you want this behaviour
};


struct View_Buffer_Location 
{
   Buffer_ID buffer;
   i64 cursor;
};


//NOTE(luis) you can build panel layouts into this and just store a global current_workspace
//but I tend to just prefer to have one panel open, so I wouldn't get much out of it
/*
struct Buffer_Tab_Group
{
   Buffer_ID tabs[8];
   i32 tab_count;
   i32 current_tab;
};

global i32 BUFFER_TAB_GROUP_COMPILATION = -1; //tab group for the view with *compilation*
global i32 BUFFER_TAB_GROUP_COMPILATION_SCRATCH = -1; //scratch group we make when iterating through compile errors
global i32 BUFFER_TAB_GROUP_COUNT;
global Buffer_Tab_Group BUFFER_TAB_GROUPS[12]; //NOTE must always be greater than 2
*/

function b32
luis_view_has_flags(Application_Links *app, View_ID view, u32 flags)
{
   Managed_Scope scope = view_get_managed_scope(app, view);
   u32 *view_flags = scope_attachment(app, scope, view_custom_flags, u32);
   b32 result = view_flags && ((*view_flags & flags) == flags);
   return result;
}

function void
luis_view_set_flags(Application_Links *app, View_ID view, u32 flags)
{
   Managed_Scope scope = view_get_managed_scope(app, view);
   u32 *view_flags = scope_attachment(app, scope, view_custom_flags, u32);
   if(view_flags)
      *view_flags |= flags;
}

function void
luis_view_clear_flags(Application_Links *app, View_ID view, u32 flags)
{
   Managed_Scope scope = view_get_managed_scope(app, view);
   u32 *view_flags = scope_attachment(app, scope, view_custom_flags, u32);
   if(view_flags)
      *view_flags &= ~flags;
}

static View_Jump_History *get_view_jump_history(Application_Links *app, View_ID view) {
    Managed_Scope scope = view_get_managed_scope(app, view);
    return scope_attachment(app, scope, view_jump_history, View_Jump_History);
}

#if 0
internal i32
find_tab_with_buffer_id(Buffer_Tab_Group *group, Buffer_ID id)
{
    i32 index = -1;
    for_index_inc(i32, i, group->tab_count) {
        if(group->tabs[i] == id) {
            index = i;
            break;
        }
    } 
        
    return index;
}
#endif

internal Panel_ID
get_sibling_panel(Application_Links *app, Panel_ID parent_panel, Panel_ID child_panel) {
    if (!child_panel || !parent_panel) return 0;
    
    Panel_ID min_panel = panel_get_child(app, parent_panel, Side_Min);
    Panel_ID max_panel = panel_get_child(app, parent_panel, Side_Max);
    assert ((min_panel == child_panel) || (max_panel == child_panel));
    Panel_ID bro_panel = (child_panel == min_panel) ? max_panel : min_panel;
    return bro_panel;
}

internal Panel_ID
get_sibling_panel(Application_Links *app, Panel_ID panel) {
    Panel_ID parent_panel = panel_get_parent(app, panel);
    return get_sibling_panel(app, parent_panel, panel);
    
}

internal View_ID
luis_get_other_child_view(Application_Links *app, View_ID view)
{
   View_ID bro_view = 0;
   Panel_ID view_panel = view_get_panel(app, view);
   Panel_ID parent_panel = panel_get_parent(app, view_panel);
   if(panel_is_split(app, parent_panel))
   {
      Panel_ID min_panel = panel_get_child(app, parent_panel, Side_Min);
      Panel_ID max_panel = panel_get_child(app, parent_panel, Side_Max);
      Panel_ID bro_panel = (view_panel == min_panel) ? max_panel : min_panel;
      if(panel_is_leaf(app, bro_panel))
         bro_view = panel_get_view(app, bro_panel, Access_Always);
   }
   return bro_view;
}

//if the view itself is a peek window, we return it
//otherwise we check it's sibling and return it if it's a peek window
internal View_ID
luis_get_peek_window(Application_Links *app, View_ID view)
{
   View_ID peek_view = 0;
   if(luis_view_has_flags(app, view, VIEW_IS_PEEK_WINDOW))
      peek_view = view;
   else
   {
      View_ID bro_view = luis_get_other_child_view(app, view);
      if(bro_view && luis_view_has_flags(app, bro_view, VIEW_IS_PEEK_WINDOW))
         peek_view = bro_view;
   }
   return peek_view;
}

//get view of whatever triggered this peek view
internal View_ID
luis_get_peek_windows_parent_view(Application_Links *app, View_ID peek)
{
   View_ID view = 0;
   if(luis_view_has_flags(app, peek, VIEW_IS_PEEK_WINDOW))
      view = luis_get_other_child_view(app, peek);
   return view;
}


internal View_ID
luis_get_or_split_peek_window(Application_Links *app, View_ID view, View_Split_Position split_kind) {
   View_ID peek = luis_get_peek_window(app, view);
   if(!peek)
   {
      //MAKE_NEW_BUFFER_TAB_GROUP_ON_VIEW_CREATION = true;
      peek = open_view(app, view, split_kind);
      if(peek)
      {
         luis_view_set_flags(app, peek, VIEW_IS_PEEK_WINDOW);
         Rect_f32 view_rect = view_get_screen_rect(app, view);
         //view_set_split_pixel_size(app, peek, (i32)((view_rect.y1 - view_rect.y0)*0.4f));
         view_set_split(app, peek, ViewSplitKind_Ratio, 1.0f / 3);
      }
      //else MAKE_NEW_BUFFER_TAB_GROUP_ON_VIEW_CREATION = false; //open view failed, remove this "argument"
   }
   return peek;
}

//also taken from RION
function b32
RION_is_file_readable(String_Const_u8 path)
{
   b32 result = 0;
   FILE *file = fopen((char *)path.str, "r");
   if(file)
   {
      result = 1;
      fclose(file);
   }
   return result;
}

//stole this from RION, thanks!
internal i32
RION_load_face_id(Application_Links *app, String_Const_u8 font_filename, i32 pt_size_delta_from_normal)
{
   Scratch_Block scratch(app);
   String_Const_u8 bin_path = system_get_path(scratch, SystemPath_Binary);
   
   // NOTE(rjf): Fallback font.
   Face_ID result = get_face_id(app, 0); 
   
   Face_Description normal_code_desc = get_face_description(app, get_face_id(app, 0));
   //i32 pt_size = normal_code_desc.parameters.pt_size;
   //i32 pt_size = (i32)def_get_config_u64(app, vars_save_string_lit("default_font_size"));
   i32 pt_size = (i32)def_get_config_u64(app, vars_save_string_lit("default_font_size"));
   pt_size += pt_size_delta_from_normal;
   
   if(pt_size > 0)
   {
      Face_Description desc = {};      
      desc.font.file_name =  push_u8_stringf(scratch, "%.*sfonts/%.*s", string_expand(bin_path), string_expand(font_filename));
      
      desc.parameters.pt_size = (u32)pt_size;
      desc.parameters.bold = 0;
      desc.parameters.italic = 0;
      desc.parameters.hinting = 0;
      
      if(RION_is_file_readable(desc.font.file_name)) 
         result = try_create_new_face(app, &desc);
   }
   
   
   return result;
}



#if 0
function void 
luis_essential_mapping(Mapping *mapping, i64 global_id, i64 file_id, i64 code_id, i64 modal_id){
   MappingScope();
   SelectMapping(mapping);
   
   SelectMap(global_id);
   BindCore(luis_startup, CoreCode_Startup);
   BindCore(default_try_exit, CoreCode_TryExit);
   BindCore(clipboard_record_clip, CoreCode_NewClipboardContents);
   BindMouseWheel(mouse_wheel_scroll);
   BindMouseWheel(mouse_wheel_change_face_size, KeyCode_Control);
   //BindMouseRelease(click_set_cursor, MouseCode_Left);
   
   SelectMap(file_id);
   ParentMap(global_id);
   BindTextInput(write_text_input);
   BindMouse(luis_mouse_click, MouseCode_Left);
   BindCore(luis_mouse_click,  CoreCode_ClickActivateView);
   BindMouseRelease(luis_mouse_release, MouseCode_Left);
   BindMouseMove(luis_mouse_drag);
   
   SelectMap(code_id);
   ParentMap(file_id);
   BindTextInput(write_text_input);
   
   SelectMap(modal_id);
   ParentMap(global_id);
   BindMouse(luis_mouse_click, MouseCode_Left);
   BindCore(luis_mouse_click,  CoreCode_ClickActivateView);
   BindMouseRelease(luis_mouse_release, MouseCode_Left);
   BindMouseMove(luis_mouse_drag);
} 
#endif

internal Range_i64
get_visual_line_start_end_pos(Application_Links *app, View_ID view, i64 linenum)
{
   Range_i64 result = {};
   Scratch_Block scratch(app);
   Buffer_ID buffer_id = view_get_buffer(app, view, Access_Always);
   result.min = get_line_start_pos(app, buffer_id, linenum);
   result.max   = get_line_end_pos(app, buffer_id, linenum);
   String_Const_u8 string = push_buffer_line(app, scratch, buffer_id, linenum);
   result.min += string_find_first_non_whitespace(string);
   result.max -= string.size - (string_find_last_non_whitespace(string) + 1);
   return result;
}



internal void
center_view(Application_Links *app, View_ID view, float shift_y) {
   if(!view)	return;
   //View_ID view = get_active_view(app, Access_ReadVisible);
   Rect_f32 region = view_get_buffer_region(app, view);
   i64 pos = view_get_cursor_pos(app, view);
   Buffer_Cursor cursor = view_compute_cursor(app, view, seek_pos(pos));
   f32 view_height = rect_height(region);
   Buffer_Scroll scroll = view_get_buffer_scroll(app, view);
   scroll.target.line_number = cursor.line;
   scroll.target.pixel_shift.y = -view_height*shift_y;
   view_set_buffer_scroll(app, view, scroll, SetBufferScroll_SnapCursorIntoView);
   no_mark_snap_to_cursor(app, view);
}

CUSTOM_COMMAND_SIG(luis_center_view_top)
CUSTOM_DOC("Centers the view vertically on the line on which the cursor sits.") {	
    center_view(app, get_active_view(app, Access_ReadVisible), 0.333333333333f);	
}




internal b32
find_next_scope_absolute(Application_Links *app, Buffer_ID buffer, i64 pos, Range_i64 *out_range)
{
   b32 found = false;
   Find_Nest_Flag flags = FindNest_Scope;
   Range_i64 range = {};
   if (find_nest_side(app, buffer, pos + 1, flags, Scan_Forward, NestDelim_Open,
                      &range) &&
       find_nest_side(app, buffer, range.end,
                      flags|FindNest_Balanced|FindNest_EndOfToken, Scan_Forward,
                      NestDelim_Close, &range.end))
   {
      
      found = true;
      *out_range = range;
   }
   return found;
}

internal b32
find_maximal_scope(Application_Links *app, Buffer_ID buffer, i64 pos, Range_i64 *out_range)
{
   b32 found = false;
   Range_i64 range = {};
   if (find_surrounding_nest(app, buffer, pos, FindNest_Scope, &range)){
      for (;;){
         pos = range.min;
         if (!find_surrounding_nest(app, buffer, pos, FindNest_Scope, &range)){
            break;
         }
      }
      found = true;
      *out_range = range;
   }
   return found;
}

internal b32 
is_paren_range(Application_Links *app, Buffer_ID buffer, Range_i64 range)
{	return buffer_get_char(app, buffer, range.min) == '(' && buffer_get_char(app, buffer, range.max-1) == ')';	}

internal b32
find_maximal_parens(Application_Links *app, Buffer_ID buffer, i64 pos, Range_i64 *out_range) {
   b32 found = false;
   Range_i64 range = {};
   if(find_surrounding_nest(app, buffer, pos, FindNest_Paren, &range)) {
      Range_i64 last_paren_range_found = {};
      if(is_paren_range(app, buffer, range)) last_paren_range_found = range;
         
      for (;;){
         pos = range.min;
         if(find_surrounding_nest(app, buffer, pos, FindNest_Paren, &range))
         {
            if(is_paren_range(app, buffer, range)) last_paren_range_found = range;
         }
         else break;
      }
      
      if(last_paren_range_found.max > last_paren_range_found.min)
      {
         found = true;
         *out_range = last_paren_range_found;
      }
   }
   return found;
}

internal b32
find_next_parens_absolute(Application_Links *app, Buffer_ID buffer, i64 pos, Range_i64 *out_range)
{
   b32 found = false;
   Find_Nest_Flag flags = FindNest_Paren;
   Range_i64 range = {};
   while(find_nest_side(app, buffer, pos + 1, flags, Scan_Forward, NestDelim_Open,
                        &range) &&
         find_nest_side(app, buffer, range.end,
                        flags|FindNest_Balanced|FindNest_EndOfToken, Scan_Forward,
                        NestDelim_Close, &range.end))
   {
      if(is_paren_range(app, buffer, range))
      {
         found = true;
         *out_range = range;
         break;
      }
      else pos = range.min;
   }
   return found;
}

internal void
add_fancy_strings_for_tab(Application_Links *app, Fancy_Line *list, Arena *scratch, Buffer_ID tab, b32 is_current_tab)
{
   Assert(buffer_exists(app, tab));  
   //String_Const_u8 unique_name = push_buffer_unique_name(app, scratch, tab);
   String_Const_u8 unique_name = push_buffer_base_name(app, scratch, tab);
   FColor color = (is_current_tab) ? fcolor_id(luiscolor_tab_current) : fcolor_id(luiscolor_tab_regular);
   push_fancy_string(scratch, list, color, unique_name);
   if(is_current_tab)
   {
      Dirty_State dirty = buffer_get_dirty_state(app, tab);
      if(HasFlag(dirty, DirtyState_UnsavedChanges))  
         push_fancy_string(scratch, list, color, SCu8("*"));
      if(HasFlag(dirty, DirtyState_UnloadedChanges)) 
         push_fancy_string(scratch, list, color, SCu8("!"));
   }
   push_fancy_string(scratch, list, color,  SCu8(" "));
}

#define LOCAL_STRING_BUILDER(_name, _buffer_size) u8 buffer_for_ ## _name[_buffer_size]; String_Builder _name = make_string_builder(buffer_for_ ## _name, (_buffer_size))
#define PTR_IN_BASE_SIZE(pointer, base, size)   ( (u8 *)(pointer) >= (u8 *)(base) && (u8 *)(pointer) < ((u8 *)(base) + (size)))
#define PTR_IN_BASE_COUNT(pointer, base, count) ( (pointer) >= (base) && (pointer) <= ((base) + (count) - 1) )

struct String_Builder 
{
   u8 *buffer;
   u32 size;
   u32 current_string_offset;
   u32 current_string_length;
};

internal String_Builder
make_string_builder(u8 *buffer, i32 buffer_size)
{
   assert(buffer && buffer_size > 1);
   String_Builder builder = {};
   builder.buffer = buffer;
   builder.size = buffer_size;
   return builder;
}

internal void
append(String_Builder *builder, u8 *str, u64 length)
{
   u8 *at = builder->buffer + builder->current_string_offset + builder->current_string_length;
   assert(PTR_IN_BASE_COUNT(at, builder->buffer, builder->size));
   
   i32 amt_added = 0;
   i32 amt_left  = (i32)((builder->buffer + builder->size) - at - 1); //-1 for null terminator
   //assert(amt_left > 0);
   
   while(amt_added < amt_left && amt_added < length)
   {
      at[amt_added] = str[amt_added];
      assert(at[amt_added]); //otherwise length is incorrect
      amt_added += 1;
   }
   builder->current_string_length += amt_added;
}

internal void
append(String_Builder *builder, char *str) 
{   append(builder, (u8 *)str, cstring_length(str));   }

internal void
append(String_Builder *builder, String_Const_u8 string) 
{   append(builder, string.str, string.size);   }


internal void
append_valist(String_Builder *builder, u8 *fmt, va_list list)
{
   u8 *at = builder->buffer + builder->current_string_offset + builder->current_string_length;
   assert(PTR_IN_BASE_COUNT(at, builder->buffer, builder->size));
   
   i32 amt_left  = (i32)((builder->buffer + builder->size) - at); //no -1, because  snprintf null terminates always 
   //assert(amt_left > 0);
   if(amt_left > 0)
   {
      i32 amt_added = vsnprintf((char *)at, (size_t)amt_left, (char *)fmt, list); 
      builder->current_string_length += amt_added;   
   }
}

internal void
appendf(String_Builder *builder, u8 *fmt, ...)
{   
   va_list list;
   va_start(list, fmt);
   append_valist(builder, fmt, list);
   va_end(list);
}

#if 0
//this is shorthand for appendf, mkstr(), we have this we can inline it as function argument...
internal String_Const_u8
mkstrf(String_Builder *builder, u8 *fmt, ...)
{
   va_list list;
   va_start(list, fmt);
   append_valist(builder, fmt, list);
   va_end(list);
   return mkstr(builder);
}
#endif

internal void
remove_trailing_whitespace(String_Builder *builder)
{
   //foreach_index_dec(i, to_write.length) 
   for(u32 i = builder->current_string_offset + builder->current_string_length - 1;
       i >= builder->current_string_offset;
       i -= 1)
   {
      if(builder->buffer[i] == ' ' || builder->buffer[i] == '\t' ||
         builder->buffer[i] == '\n' || builder->buffer[i] == '\r')
      {
         builder->buffer[i] = 0;
         assert(builder->current_string_length > 0);
         builder->current_string_length -= 1;
      }
      else break;
   }
}

//NOTE final call to mkstr null terminates the current string and returns it
internal String_Const_u8
mkstr(String_Builder *builder)
{
   String_Const_u8 result = {};
   if(builder->current_string_length)
   {
      //ensure we have room to null terminate
      assert((builder->current_string_offset + builder->current_string_length) < builder->size);
      result = SCu8(builder->buffer + builder->current_string_offset, builder->current_string_length);
      //mkstr(builder->buffer + builder->current_string_offset,
      //builder->current_string_length,
      //builder->current_string_length + 1);
      result.str[result.size] = 0; 
      
      builder->current_string_offset += builder->current_string_length + 1;
      builder->current_string_length = 0;   
   }
   return result;
}

internal void
logprintf(Application_Links *app, char *fmt, ...)
{
   LOCAL_STRING_BUILDER(builder, 1024);
   va_list list;
   va_start(list, fmt);
   append_valist(&builder, (u8 *)fmt, list);
   va_end(list);
   
   remove_trailing_whitespace(&builder);
   append(&builder, "\n");
   print_message(app, mkstr(&builder));
}



//returns newly made tab group_index
#if 0
internal i32
view_new_tab_group(Application_Links *app, View_ID view)
{
   i32 tab_group_index_result = -1;
   Managed_Scope scope = view_get_managed_scope(app, view);
   i32 *tab_group_index = scope_attachment(app, scope, view_tab_group_index, i32);
   if(tab_group_index && BUFFER_TAB_GROUP_COUNT < countof(BUFFER_TAB_GROUPS))
   {
      foreach_index_inc(i, countof(BUFFER_TAB_GROUPS))
      {
         Buffer_Tab_Group *group = BUFFER_TAB_GROUPS + i; 
         if(group->tab_count == 0)
         {
            tab_group_index_result = i;
            *tab_group_index = i;
            *group = {};
            group->tabs[0] = view_get_buffer(app, view, Access_Always);
            group->tab_count = 1;
            BUFFER_TAB_GROUP_COUNT += 1;
            break;
         }
      }
   }   
   return tab_group_index_result;
}

internal Buffer_Tab_Group *
view_get_tab_group(Application_Links *app, View_ID view)
{
   Buffer_Tab_Group *group = 0;
   Managed_Scope scope = view_get_managed_scope(app, view);
   i32 *tab_group_index = scope_attachment(app, scope, view_tab_group_index, i32);
   if(tab_group_index && *tab_group_index >= 0 && *tab_group_index < countof(BUFFER_TAB_GROUPS))
      group = BUFFER_TAB_GROUPS + *tab_group_index;
   return group;
}




internal i32
view_get_tab_group_index(Application_Links *app, View_ID view)
{
   Managed_Scope scope = view_get_managed_scope(app, view);
   i32 *tab_group_index = scope_attachment(app, scope, view_tab_group_index, i32);
   if(tab_group_index) return *tab_group_index;
   else return -1;
}

internal b32
is_valid_tab_group_index(i32 tab_group_index)
{
   b32 result = tab_group_index >= 0 && tab_group_index < countof(BUFFER_TAB_GROUPS); 
   return result;
}

internal void
kill_tab_group(Application_Links *app, i32 tab_group_index)
{
   if(!is_valid_tab_group_index(tab_group_index))	return;
   
   b32 do_kill = false;
   if(tab_group_index == BUFFER_TAB_GROUP_COMPILATION || tab_group_index == BUFFER_TAB_GROUP_COMPILATION_SCRATCH)
      do_kill = true;
   else
   {
      b32 num_regular_tab_groups = BUFFER_TAB_GROUP_COUNT;
      if(is_valid_tab_group_index(BUFFER_TAB_GROUP_COMPILATION))
         num_regular_tab_groups -= 1;
      if(is_valid_tab_group_index(BUFFER_TAB_GROUP_COMPILATION_SCRATCH))
         num_regular_tab_groups -= 1;
      do_kill = num_regular_tab_groups > 1;
   }
   
   if(do_kill)
   {  
      BUFFER_TAB_GROUPS[tab_group_index] = {};
      BUFFER_TAB_GROUP_COUNT -= 1;
      
      i32 original_tab_group_index = tab_group_index;
      i32 safe_group_index = 0;
      foreach_index_inc(group_index, countof(BUFFER_TAB_GROUPS))
      {
         if(BUFFER_TAB_GROUPS[group_index].tab_count > 0)
         {
            safe_group_index = group_index;
            break;
         }
      }
      
      //View_ID v = view;
      //do
      for(View_ID v = get_view_next(app, 0, Access_Always); v; v = get_view_next(app, v, Access_Always))
      {
         Managed_Scope s = view_get_managed_scope(app, v);
         i32 *index = scope_attachment(app, s, view_tab_group_index, i32);
         if(index && *index == original_tab_group_index)
            *index = safe_group_index;
         //v = get_next_view_looped_all_panels(app, v, Access_Always);
      } //while(v != view);
   }
}


internal Buffer_Tab_Group *
view_get_tab_group(Application_Links *app, View_ID view)
{
   Managed_Scope scope = view_get_managed_scope(app, view);
   Buffer_Tab_Group *group = scope_attachment(app, scope, view_tab_group, Buffer_Tab_Group);
   return group;
}
#endif

internal Render_Caller_Function **
view_get_prev_render_caller(Application_Links *app, View_ID view)
{
   Managed_Scope scope = view_get_managed_scope(app, view);
   Render_Caller_Function **func = (Render_Caller_Function **)scope_attachment(app, scope, view_prev_render_caller, void *);
   return func;
}

internal View_Buffer_Location *
view_get_prev_buffer_location(Application_Links *app, View_ID view)
{
   Managed_Scope scope = view_get_managed_scope(app, view);
   View_Buffer_Location *loc = scope_attachment(app, scope, view_prev_buffer_location, View_Buffer_Location);
   return loc;
}

internal String_Const_u8
get_directory_for_buffer(Application_Links *app, Arena *arena, Buffer_ID buffer_id)
{
   String_Const_u8 directory = push_buffer_file_name(app, arena, buffer_id);
   if (directory.size) 
   {
      int32_t filename_length = 0;
      while (directory.str[directory.size - filename_length - 1] != '\\') filename_length += 1;
      
      directory.size -= filename_length;
      directory.str[directory.size] = 0;
   }
   return directory;
}

internal void 
update_buffer_bindings_for_modal_toggling(Application_Links *app, Buffer_ID buffer_id)
{
   if(!buffer_id)	return;
	//change buffer command map id
   Scratch_Block scratch(app);
	b32 treat_as_code = false;
   String_Const_u8 file_name = push_buffer_file_name(app, scratch, buffer_id);
	String_Const_u8 ext = string_file_extension(file_name);
	if (string_match(ext, string_u8_litexpr("cpp")) ||
       string_match(ext, string_u8_litexpr("h")) || 
       string_match(ext, string_u8_litexpr("c")) ||    
       string_match(ext, string_u8_litexpr("hpp")) ||       
       string_match(ext, string_u8_litexpr("cc")))  
   {
      treat_as_code = true;
      
   }
	
	Command_Map_ID map_id;
	if(IN_MODAL_MODE) map_id = vars_save_string_lit("keys_modal");
	else              map_id = treat_as_code ? vars_save_string_lit("keys_code") :vars_save_string_lit("keys_file");
	
	Managed_Scope scope = buffer_get_managed_scope(app, buffer_id);
	Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID); 
	*map_id_ptr = map_id;
}




struct Peek_Code_Index_State {
    b32 initialized;
    b32 is_first_time_getting;
    i32 index; //must be signed
    i32 note_count;
    Code_Index_Note *notes[16];
    
    String_u8 identifier;
    u8 identifier_buffer[64];
};


struct Scope_Prefix {
    String_Const_u8 string;
    i32 scope_count;
};

function Scope_Prefix
get_entire_scope_prefix(Application_Links *app, Arena *arena, Buffer_ID buffer, i64 pos) {
    
    Code_Index_File *code_index_file = code_index_get_file(buffer);
    if (!code_index_file) return {};
    
    Scratch_Block scratch(app, arena);
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
    
    Scope_Prefix result = {};
    result.scope_count = 1;
    if (outer_note_count > 0) {
        String_u8 string = string_u8_push(arena, 512); 
         
        string_append(&string, SCu8("::"));    
        for (i32 i = 0; i < outer_note_count; i += 1) {
            Code_Index_Note *note = outer_notes[i];
            string_append(&string, note->text);
            string_append(&string, SCu8("::"));
            result.scope_count += 1;
            
        //Code_Index_Note *note = outer_notes[i];
        //push_fancy_stringf(scratch, &list, base_color, "%.*s::", string_expand(note->text)); //makes it in reverse order ezSadge
        }
        
        result.string = SCu8(string);
    }
    else { //smaller allocation path
        result.string = push_stringf(arena, "::");
    }
    
    return result;
}

#endif //LUIS_CUSTOM_LAYER_H
