project(MdvMerge2)
set(CMAKE_CXX_STANDARD 14)

if(COMMAND run_tdrp_gen)
    run_tdrp_gen(${PROJECT_NAME})
endif()

set(SOURCE_FILES
    Params.hh
    Args.hh
    InputFile.hh
    OutputFile.hh
    Trigger.hh
    MdvMerge2.hh
    XyLookup.hh
    Params.cc
    Args.cc
    InputFile.cc
    Main.cc
    MdvMerge2.cc
    OutputFile.cc
    Trigger.cc
    XyLookup.cc
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}
    Fmq
    Mdv
    Radx
    Ncxx
    dsdata
    dsserver
    didss
    euclid
    rapformats
    toolsa
    tdrp
    dataport
    ${NETCDF4_LIBS}
    ${UDUNITS_LIBS}
    -lz
    -lbz2
)
