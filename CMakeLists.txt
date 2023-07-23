cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
add_definitions(-w)

project(MArEX)

set(CMAKE_BUILD_TYPE Debug)

##############################################################
# Debug mode
option(DEBUG_MODE "Compile with debug options" OFF)
if(DEBUG_MODE)
    message(STATUS "Compiling in Debug mode.")
    add_compile_options(-g)
endif()
##############################################################

##############################################################
# profiling mode
option(PROFILING_MODE "Compile with profiling turned on" ON)
if(PROFILING_MODE)
    add_definitions(-DMAREX_PROFILING)
    message(STATUS "Compiling with profiling mode turned on")
endif()
##############################################################

##############################################################
# Multithreading mode
option(MULTITHREADING "Compile with multithreading mode" OFF)
if(MULTITHREADING)
    if(DEFINED ENV{G4MULTITHREADED})
        add_definitions(-DMAREX_MULTITHREADED)
        message(STATUS "Compiling with multithreading enabled.")
    else()
        message(STATUS 
            "Attempt to use multithreading failed - 
            G4MULTITHREADED is not defined! 
            Maybe you didn't compile G4 with multithreading."
        )
    endif()
endif()
##############################################################

##############################################################
# Geant version
message(STATUS "Compiling against Geant4 version 11.")
find_package(Geant4 11 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
message(STATUS "Geant4 installation directory -- ${Geant4_DIR}")
##############################################################

##############################################################
# ROOT
message(STATUS "Compiling with ROOT.")
find_package(ROOT 6.26 CONFIG REQUIRED COMPONENTS Core)
message(STATUS "ROOT installation directory -- ${ROOT_DIR}")
message(STATUS "ROOT include directories -- ${ROOT_INCLUDE_DIRS}")
add_definitions(-DMAREX_ROOT)
##############################################################

##############################################################
# YAML
message(STATUS "Compiling with yaml-cpp.")
add_definitions(-DMAREX_YAML)
set(YAML_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/vendor/yaml-cpp/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/yaml-cpp)
set(YAML_LIBS yaml-cpp)
##############################################################

##############################################################
# use the visualization manager
option(USE_VIS "Whether to use the visualization." ON)
if(USE_VIS)
    add_definitions(-DMAREX_USE_VIS)
    message(STATUS "Compiling with the visualization enabled.")
endif()
##############################################################

##############################################################
# use the ui executive/manager
option(USE_UI "Whether to use the ui interface." ON)
if(USE_UI)
    add_definitions(-DMAREX_USE_UI)
    message(STATUS "Compiling with the ui enabled.")
endif()
##############################################################

##############################################################
# Get all header directories
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.hh)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()
HEADER_DIRECTORIES(header_list)
##############################################################

##############################################################
# generate sources and headers
include_directories(
    ${header_list}
    ${ROOT_INCLUDE_DIRS}
    ${YAML_INCLUDE_DIR}
)

file(GLOB_RECURSE sources 
    ${PROJECT_SOURCE_DIR}/src/*.cxx
)
file(GLOB_RECURSE headers 
    ${PROJECT_SOURCE_DIR}/include/*.hh
)
##############################################################

##############################################################
# Create library
add_library(marex SHARED ${sources} ${headers})
target_link_libraries(marex PUBLIC ${Geant4_LIBRARIES})

if(USE_ROOT)
    target_link_libraries(
        marex PUBLIC ROOT::Core ${ROOT_LIBRARIES}
    )
endif()
if(USE_YAML)
    target_link_libraries(
        marex PUBLIC ${YAML_LIBS}
    )
endif()
##############################################################

##############################################################
# create programs
add_executable(marex_sim 
    src/programs/marex.cc 
)
target_link_libraries(marex_sim marex)
##############################################################

##############################################################
# create directories
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/macros/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/data/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/analysis/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/configs/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/plots/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/gdml/)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/outputs/)

##############################################################
# copy macro and data files
file(GLOB MACRO_FILES
    "macros/*.mac"
)
file(GLOB DATA_FILES
    "data/*.*"
)
file(GLOB ANALYSIS_FILES
    "analysis/*.py"
)
file(GLOB_RECURSE CONFIG_FILES
    "configs/*.yaml"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR}/macros/)
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR}/data/)
file(COPY ${ANALYSIS_FILES} DESTINATION ${PROJECT_BINARY_DIR}/analysis/)
file(COPY ${CONFIG_FILES} DESTINATION ${PROJECT_BINARY_DIR}/configs/)
##############################################################

##############################################################
add_custom_target(MArEX DEPENDS marex)
##############################################################