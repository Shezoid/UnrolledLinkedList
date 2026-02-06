add_library(unrolled_list 
            unrolled_list.h
            my_iterator.h
            node.h
)
target_include_directories(unrolled_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})