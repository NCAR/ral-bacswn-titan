project(GridForecast)
if(COMMAND run_tdrp_gen)
    run_tdrp_gen(${PROJECT_NAME})
endif()

set(SOURCE_FILES
    Params.hh
    Args.hh
    GridForecast.hh
    Params.cc
    Args.cc
    Main.cc
    GridForecast.cc
    )

add_executable(GridForecast ${SOURCE_FILES})
target_link_libraries(GridForecast
    advect
    -ldsdata
    -lMdv
    -lRadx
    -lNcxx
    -lSpdb
    -ldsserver
    -ltitan
    -leuclid
    -lrapmath
    -ldidss
    -lrapformats
    -ltoolsa
    -ldataport
    -ltdrp
    -lz
    -lbz2
    -lhdf5_hl
    -lhdf5_hl_cpp
    -lhdf5
    -lhdf5_cpp
    -lnetcdf
    )
