add_custom_command(
        OUTPUT Params.cc
        OUTPUT Params.hh
        PRE_BUILD
        COMMAND $ENV{RAP_BIN_DIR}/tdrp_gen -f ${CMAKE_CURRENT_SOURCE_DIR}/paramdef.DsMdvServer -c++ -prog DsMdvServer
        COMMAND ${CMAKE_COMMAND} -E copy Params.cc ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy Params.hh ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCE_FILES
    Params.hh
    Args.hh
    Driver.hh
    DsMdvServer.hh
    Params.cc
    Args.cc
    Derived.cc
    Driver.cc
    DsMdvServer.cc
    HandleMdvx.cc
    Main.cc
    )

add_executable(DsMdvServer ${SOURCE_FILES})
target_link_libraries(DsMdvServer
        -lMdv
        -lRadx
        -lNcxx
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
    )
