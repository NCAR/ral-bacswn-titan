set(SOURCE_FILES
        tdrp_gen.h
        ctype_list.c
        enum_list.c
        parse_defs.c
        set_item.c
        struct_list.c
        tdrp_gen.c
        write_c_files.c
        write_c++_files.c
)

add_executable(tdrp_gen ${SOURCE_FILES})
target_link_libraries(tdrp_gen
        tdrp
        -lm
)
