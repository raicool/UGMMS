cmake_minimum_required(VERSION 3.15)
project("UGMMS")

set(CMAKE_CXX_STANDARD 23)
set(CXXFLAGS ${CXXFLAGS} "-m32")
set(CFLAGS ${CFLAGS} "-m32")

file(GLOB_RECURSE SOURCES
	"GMLScriptEnv/*.cpp"
)

set(UGMMS_SOURCES ${SOURCES})