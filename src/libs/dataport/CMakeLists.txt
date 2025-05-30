set(SOURCE_FILES
        src/include/dataport/bigend.h
        src/include/dataport/port_types.h
        src/include/dataport/smallend.h
        src/include/dataport/swap.h
        src/bigend/bigend.c
        src/smallend/smallend.c
        src/swap/swap.c
)
add_library(dataport ${SOURCE_FILES})
target_include_directories(dataport PUBLIC
        ./src/include
)