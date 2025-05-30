set(SOURCE_FILES
    src/include/advect/Advector.hh
    src/include/advect/GridAdvect.hh
    src/include/advect/SoundingAdvector.hh
    src/include/advect/VectorsAdvector.hh
    src/GridAdvect/GridAdvect.cc
    src/GridAdvect/SoundingAdvector.cc
    src/GridAdvect/VectorsAdvector.cc
    )

add_library(advect ${SOURCE_FILES})
