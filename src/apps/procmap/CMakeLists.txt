set(SOURCE_FILES
    Args.cc
    Main.cc
    Procmap.cc
    InfoStore.cc
    )

add_executable(procmap ${SOURCE_FILES})
target_link_libraries(procmap
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
