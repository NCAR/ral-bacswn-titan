set(SOURCE_FILES
        src/include/tdrp/tdrp.h
        src/include/tdrp/tdrp_obsolete.h
        src/include/tdrp/tdrp_p.h
        src/include/tdrp/tdrpbuf.h
        src/tdrp/check.c
        src/tdrp/convert.c
        src/tdrp/copy.c
        src/tdrp/find_item.c
        src/tdrp/load.c
        src/tdrp/mem.c
        src/tdrp/override.c
        src/tdrp/print.c
        src/tdrp/read_item.c
        src/tdrp/tdrp.c
        src/tdrp/tdrp_obsolete.c
        src/tdrp/tdrpbuf.c
        src/tdrp/token_list.c
        src/tdrp/tokenize.c
        src/tdrp/warn_on_extra_params.c
)

add_library(tdrp ${SOURCE_FILES})
target_include_directories(tdrp PUBLIC
        ./src/include
)