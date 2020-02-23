
option(STRONG_FLAGS_BUILD_TESTS "Determines whether to build tests." ON)
if (STRONG_FLAGS_BUILD_TESTS)
	include(ExternalProject)
	ExternalProject_Add(catch_external
		PREFIX ${CMAKE_BINARY_DIR}/external/catch
		GIT_REPOSITORY https://github.com/catchorg/Catch2
		GIT_TAG d10b9bd02e098476670f5eb0527d2c7281476e8a
		CONFIGURE_COMMAND ""
		BUILD_COMMAND ""
		INSTALL_COMMAND ""
		)
#	${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/external/catch/src/catch_external/single_include/catch.hpp ${CMAKE_BINARY_DIR}/external/catch/include/catch.hpp
	add_library(catch INTERFACE)
	add_dependencies(catch catch_external)
	target_include_directories(catch INTERFACE ${CMAKE_BINARY_DIR}/external/catch/src/catch_external/single_include/)
	
	set(TEST_SOURCES test_main.cpp unsigned_test.cpp)
	
	add_executable(strong_flags_test ${TEST_SOURCES})
	target_link_libraries(strong_flags_test PRIVATE catch strong_flags)
	
	add_test(NAME main_test COMMAND strong_flags_test)
	
endif()