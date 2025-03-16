cmake_minimum_required (VERSION 3.8)

set(SrcFiles
  ${MY_SRC_DIR}/SearchEngine.cpp
  ${MY_SRC_DIR}/ConverterJSON.cpp
  ${MY_SRC_DIR}/InvertedIndex.cpp
  test_search_engine.cpp
)

add_executable(GtestTests ${SrcFiles})

cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL
    https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_disable_pthreads on)
enable_testing()
include(GoogleTest)

gtest_discover_tests(GtestTests)

target_link_libraries(GtestTests PRIVATE nlohmann_json::nlohmann_json gtest_main)
target_include_directories(GtestTests PUBLIC ${MY_INCLUDE_DIR})
