cmake_minimum_required (VERSION 3.8)

set(Src
	"ConverterJSON.cpp"
	"SearchEngine.cpp"
	"InvertedIndex.cpp"
	"search_engine.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(search_engine ${Src})

target_link_libraries(search_engine nlohmann_json::nlohmann_json)
target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})