set(SOURCE_FILES
    Args.hh
    print_procmap.hh
    Args.cc
    Main.cc
    print_procmap.cc
    )

add_executable(print_procmap ${SOURCE_FILES})
target_link_libraries(print_procmap
    -ldsserver
    -ldidss
    -ltoolsa
    -ldataport
    -lpthread
    -lz
    -lbz2
    -ludunits2
    -lhdf5_hl
    -lhdf5_hl_cpp
    -lhdf5
    -lhdf5_cpp
    -lnetcdf
    )
