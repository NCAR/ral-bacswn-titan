add_custom_command(
        OUTPUT Params.cc
        OUTPUT Params.hh
        PRE_BUILD
        COMMAND $ENV{RAP_BIN_DIR}/tdrp_gen -f ${CMAKE_CURRENT_SOURCE_DIR}/paramdef.DataMapper -c++ -prog DataMapper
        COMMAND ${CMAKE_COMMAND} -E copy Params.cc ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy Params.hh ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCE_FILES
    Params.hh
    Args.hh
    DataMapper.hh
    DmapServer.hh
    InfoStore.hh
    Params.cc
    Args.cc
    DataMapper.cc
    DmapServer.cc
    InfoStore.cc
    Main.cc
    )

add_executable(DataMapper ${SOURCE_FILES})
target_link_libraries(DataMapper
    -ldsserver
    -ldidss
    -ltoolsa
    -ldataport
    -ltdrp
    -lpthread
    -lz
    -lbz2
    -lm
    )
