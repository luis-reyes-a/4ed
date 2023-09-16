/*
4coder_code_index.h - Generic code indexing system for layout, definition jumps, etc.
*/

// TOP

#if !defined(FCODER_CODE_INDEX_H)
#define FCODER_CODE_INDEX_H

struct Code_Index_Nest_List{
    struct Code_Index_Nest *first;
    struct Code_Index_Nest *last;
    i32 count;
};

struct Code_Index_Nest_Ptr_Array{
    struct Code_Index_Nest **ptrs;
    i32 count;
};

typedef i32 Code_Index_Nest_Kind;
enum{
    CodeIndexNest_Scope,
    CodeIndexNest_Paren,
    CodeIndexNest_Preprocessor,
    CodeIndexNest_Statement,
    
    
};

enum Code_Index_Nest_Flags : u32 {
    CODE_INDEX_NEST_IS_CLOSED    = (1 << 0),
    CODE_INDEX_NEST_IS_STRUCT    = (1 << 1),
    CODE_INDEX_NEST_IS_UNION     = (1 << 2),
    CODE_INDEX_NEST_IS_NAMESPACE = (1 << 3),
    CODE_INDEX_NEST_IS_FUNCTION  = (1 << 4),
};

struct Code_Index_Nest {
    Code_Index_Nest *next;
    
    Code_Index_Nest_Kind kind;
    //b32 is_closed;
    u32 flags;
    Range_i64 open;
    Range_i64 close;
    
    String_Const_u8 text; //name of nest for namespaces, classes, etc.
    struct Code_Index_File *file;
    Code_Index_Nest *parent;
    
    Code_Index_Nest_List nest_list;
    Code_Index_Nest_Ptr_Array nest_array;
};

typedef i64 Code_Index_Note_Kind;
enum{
    CodeIndexNote_Type,
    CodeIndexNote_Function,
    CodeIndexNote_Macro,
    CodeIndexNote_4coderCommand,
    CodeIndexNote_Namespace, //NOTE(lui$) added this
    
    //TODO this probably want to be flags instead
    CodeIndexNote_Type_Definition,
    CodeIndexNote_Function_Definition,
};

struct Code_Index_Note {
    Code_Index_Note *next;
    Code_Index_Note_Kind note_kind;
    Range_i64 pos;
    String_Const_u8 text;
    String_Const_u8 func_arg_string; //NOTE (luis) added this
    struct Code_Index_File *file;
    
    Range_i64 nest_range; //NOTE(luis) added this
    
    Code_Index_Nest *parent;
    Code_Index_Nest *nest; //for type structs/unions and functions
    
    Code_Index_Note *prev_in_hash;
    Code_Index_Note *next_in_hash;
};

struct Code_Index_Note_List{
    Code_Index_Note *first;
    Code_Index_Note *last;
    i32 count;
};

struct Code_Index_Note_Ptr_Array{
    Code_Index_Note **ptrs;
    i32 count;
};

struct Code_Index_File {
    Code_Index_Nest_List nest_list;
    Code_Index_Nest_Ptr_Array nest_array;
    Code_Index_Note_List note_list;
    Code_Index_Note_Ptr_Array note_array;
    Buffer_ID buffer;
};

struct Code_Index_File_Storage{
    Code_Index_File_Storage *next;
    Code_Index_File_Storage *prev;
    Arena arena;
    Code_Index_File *file;
};

struct Code_Index{
    System_Mutex mutex;
    Arena node_arena;
    Table_u64_u64 buffer_to_index_file;
    Code_Index_File_Storage *free_storage;
    Code_Index_File_Storage *storage_first;
    Code_Index_File_Storage *storage_last;
    i32 storage_count;
    
    Code_Index_Note_List name_hash[4099];
};

////////////////////////////////

typedef void Generic_Parse_Comment_Function(Application_Links *app, Arena *arena, Code_Index_File *index,
                                            Token *token, String_Const_u8 contents);

struct Generic_Parse_State{
    Application_Links *app;
    Arena *arena;
    String_Const_u8 contents;
    Token_Iterator_Array it;
    Generic_Parse_Comment_Function *handle_comment;
    u8 *prev_line_start;
    b32 finished;
    
    i32 scope_counter;
    i32 paren_counter;
    b32 in_preprocessor;
    b32 in_statement;
    
    b32 do_cpp_parse;
};

#endif

// BOTTOM

