#define foreach(i,N) for(i32 i=0; i<N; i++)
#define in_range(bot,mid,top) ((bot) <= (mid) && (mid) < (top))
#define ArrayInc(a,i) ((i+1) % ArrayCount(a))
#define ArrayDec(a,i) ((i + ArrayCount(a)-1) % ArrayCount(a))
#define printf_message(app, arena, str, ...) print_message(app, push_stringf(arena, str, __VA_ARGS__))


#if 0
function Rect_f32 
vim_get_bottom_rect(Application_Links *app){
	Rect_f32 result = global_get_screen_rectangle(app);
	result.y1 -= 2.f*get_face_metrics(app, get_face_id(app, 0)).line_height;
	result.y0 = result.y1 - vim_cur_filebar_offset;
	return result;
}
#endif



