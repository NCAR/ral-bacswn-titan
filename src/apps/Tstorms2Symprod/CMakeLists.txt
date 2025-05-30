add_custom_command(
        OUTPUT Params.hh
        OUTPUT Params.cc
        PRE_BUILD
        COMMAND $ENV{RAP_BIN_DIR}/tdrp_gen -f ${CMAKE_CURRENT_SOURCE_DIR}/paramdef.Tstorms2Symprod -c++ -prog Tstorms2Symprod
        COMMAND ${CMAKE_COMMAND} -E copy Params.cc ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy Params.hh ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCE_FILES
    Params.hh
    Args.hh
    Driver.hh
    Server.hh
    StormInstance.hh
    Params.cc
    Args.cc
    Driver.cc
    Server.cc
    Main.cc
    StormInstance.cc
    )

add_executable(Tstorms2Symprod ${SOURCE_FILES})
target_link_libraries(Tstorms2Symprod
        -lSpdb
        -ldsserver
        -ldidss
        -lrapformats
        -leuclid
        -ltoolsa
        -ldataport
        -ltdrp
        -lpthread
        -lz
        -lbz2
        -lhdf5
        -lhdf5_cpp
        -lhdf5_hl
        -lhdf5_hl_cpp
        -lnetcdf
        -lm
    )
