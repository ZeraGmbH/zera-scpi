add_subdirectory(lib)

if(NOT SKIP_UNITTESTS)
    add_subdirectory(testlibs)
    add_subdirectory(tests)
endif()
