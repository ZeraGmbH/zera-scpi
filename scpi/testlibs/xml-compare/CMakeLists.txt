add_subdirectory(lib)

if(NOT SKIP_UNITTESTS)
    add_subdirectory(tests)
endif()
