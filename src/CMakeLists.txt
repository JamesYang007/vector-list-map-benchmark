add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(main
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
