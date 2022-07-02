add_library(morse morse.cpp)
target_compile_features(morse PUBLIC cxx_std_20)
target_include_directories(morse PUBLIC ${PROJECT_SOURCE_DIR}/include)

