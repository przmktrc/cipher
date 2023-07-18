cmake_minimum_required(VERSION 3.26)
project(Cipher)


set(CMAKE_CXX_STANDARD 20)


find_package(ICU COMPONENTS i18n io uc REQUIRED)


add_executable(antext src/antext.cpp)
target_link_libraries(antext PUBLIC fmt ICU::i18n ICU::io ICU::uc)



add_executable(cipher src/cipher.cpp)
target_link_libraries(cipher PUBLIC fmt ICU::i18n ICU::io ICU::uc)
