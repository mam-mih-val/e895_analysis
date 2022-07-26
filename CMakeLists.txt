cmake_minimum_required(VERSION 3.15)
project(e895)

find_package(ROOT REQUIRED COMPONENTS RIO Tree)
find_package(Boost REQUIRED COMPONENTS program_options)
include(${ROOT_USE_FILE})
include(FetchContent)

FetchContent_Declare(
        AnalysisTree
        GIT_REPOSITORY https://github.com/HeavyIonAnalysis/AnalysisTree
        GIT_TAG        v2.2.6
        CMAKE_ARGS
        "-CMAKE_CXX_STANDARD=17"
)
FetchContent_MakeAvailable(AnalysisTree)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${ROOT_INCLUDE_DIRS}
)

add_subdirectory(converter)
