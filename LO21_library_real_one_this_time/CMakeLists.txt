add_library(LO21_library_real_one_this_time STATIC
    src/proposition.c
    src/regle.c
    src/liste.c
    src/parser.c
    src/inference.c
    src/io.c
)

target_include_directories(LO21_library_real_one_this_time PUBLIC include)