add_custom_command(
        OUTPUT Params.cc
        OUTPUT Params.hh
        PRE_BUILD
        COMMAND $ENV{RAP_BIN_DIR}/tdrp_gen -f ${CMAKE_CURRENT_SOURCE_DIR}/paramdef.Radx2Grid -c++ -prog Radx2Grid
        COMMAND ${CMAKE_COMMAND} -E copy Params.cc ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy Params.hh ${CMAKE_CURRENT_SOURCE_DIR}
)

set(SOURCE_FILES
        Params.hh
        Args.hh
        CartInterp.hh
        Interp.hh
        OutputMdv.hh
        PpiInterp.hh
        PolarInterp.hh
        Radx2Grid.hh
        ReorderInterp.hh
        SatInterp.hh
        Params.cc
        Args.cc
        CartInterp.cc
        Interp.cc
        Main.cc
        OutputMdv.cc
        PpiInterp.cc
        PolarInterp.cc
        Radx2Grid.cc
        ReorderInterp.cc
        SatInterp.cc
        SvdData.cc
)

add_executable(Radx2Grid ${SOURCE_FILES})
target_link_libraries(Radx2Grid
        radar
        Radx
        kd
        Mdv
        Ncxx
        dsserver
        didss
        rapformats
        rapmath
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
