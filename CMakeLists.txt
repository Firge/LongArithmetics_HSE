cmake_minimum_required(VERSION 3.25)
project(BigNum)  
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O1")
set(CMAKE_CXX_FLAGS --coverage)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCES
	src/BigNum.cpp
)

enable_testing()

include_directories(include)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE 
	PROJECT_NAME=${PROJECT_NAME}
)

add_executable(${PROJECT_NAME}_pi tests/pi_calc.cpp)
target_link_libraries(${PROJECT_NAME}_pi PRIVATE ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}_pi PRIVATE include)

# Tests
add_executable(${PROJECT_NAME}_test tests/tests.cpp)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

target_link_libraries(
	${PROJECT_NAME}_test
	PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
