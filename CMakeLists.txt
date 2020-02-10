cmake_minimum_required(VERSION 3.1)

project(strong_flags VERSION 0.1.0
		LANGUAGES CXX)
		#DESCRIPTION "A header only C++ library that provides type safe bit-flags"
		#HOMEPAGE_URL "https://github.com/TLaviron/strong_flags"

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include/)

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/strong_flags/strong_flags.hpp)

enable_testing()
add_subdirectory(test)


