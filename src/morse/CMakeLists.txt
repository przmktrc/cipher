add_library(morse morse.cpp)
target_compile_features(morse PUBLIC cxx_std_20)
target_include_directories(morse PUBLIC ${PROJECT_SOURCE_DIR}/include)



add_executable(from_morse from_morse.cpp)
target_link_libraries(from_morse PUBLIC morse)
target_link_libraries(from_morse PUBLIC fmt)
