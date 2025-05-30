add_custom_command(
        OUTPUT Params.cc
        OUTPUT Params.hh
        PRE_BUILD
        COMMAND $ENV{RAP_BIN_DIR}/tdrp_gen -f ${CMAKE_CURRENT_SOURCE_DIR}/paramdef.RadxConvert -c++ -prog RadxConvert
        COMMAND ${CMAKE_COMMAND} -E copy Params.cc ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy Params.hh ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCE_FILES
        Params.hh
        Args.hh
        RadxConvert.hh
        VarTransform.hh
        Params.cc
        Args.cc
        Main.cc
        RadxConvert.cc
        VarTransform.cc
)

add_executable(RadxConvert ${SOURCE_FILES})
target_link_libraries(RadxConvert
        Mdv
        Radx
        Ncxx
        dsserver
        didss
        rapformats
        euclid
        toolsa
        tdrp
        dataport
        pthread
        z
        bz2
        hdf5_hl
        hdf5_hl_cpp
        hdf5
        hdf5_cpp
        netcdf
)
