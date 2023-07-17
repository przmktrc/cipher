cmake_minimum_required(VERSION 3.26)
project(Cipher)


find_package(ICU COMPONENTS i18n io uc REQUIRED)


add_executable(antext src/antext.cpp)
target_link_libraries(antext fmt ICU::i18n ICU::io ICU::uc)
