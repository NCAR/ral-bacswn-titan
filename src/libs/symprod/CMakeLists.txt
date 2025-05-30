set(SOURCE_FILES
        src/include/symprod/prod_queue.h
        src/include/symprod/ProdQueue.h
        src/include/symprod/spdb.h
        src/include/symprod/spdb_client.h
        src/include/symprod/spdb_products.h
        src/include/symprod/symprod.h
        src/prod_queue/prod_queue.c
        src/ProdQueue/ProdQueue.cc
        src/spdb/spdb.c
        src/spdb/spdb_client.c
        src/symprod/symprod.c
        src/symprod/symprod_trans.c
)
add_library(symprod ${SOURCE_FILES})
