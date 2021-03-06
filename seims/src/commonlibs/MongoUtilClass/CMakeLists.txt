# CMake build for Windows, Linux, and macOS
# Updated by Liang-Jun Zhu
# zlj@lreis.ac.cn
cmake_minimum_required(VERSION 3.1)
project(MongoUtil)
IF (COMMAND CMAKE_POLICY)
    CMAKE_POLICY(SET CMP0015 NEW)
    SET(CMAKE_MACOSX_RPATH 1)
ENDIF (COMMAND CMAKE_POLICY)
################# Install directory ##################
IF (INSTALL_PREFIX)
    SET(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
    SET(INSTALL_DIR ${INSTALL_PREFIX})
ELSE ()
    SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
    SET(INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
ENDIF ()
################# OS platform   ##################
if (WIN32)
    add_definitions(-Dwindows)
    if (MSVC)
        add_definitions(-DMSVC)
    endif ()
elseif (APPLE)
    add_definitions(-Dmacos)
else ()
    add_definitions(-Dlinux)
endif ()
################# Find packages ##################
# 1. Bson and MongoC are required
include(FindBson.cmake)
include(FindMongoC.cmake)
# 2. Add support for OpenMP (if supported)
find_package(OpenMP)
IF (OPENMP_FOUND)
    add_definitions(-DSUPPORT_OMP)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
ENDIF ()
### Active C++11 standard, cross-platform. This requires CMake 3.1+
# SET(CMAKE_CXX_STANDARD 11) CMake may make mistake in intel c/c++ compiler
# 12.1, so make some test when setting the C++ Standard.
# refers to https://github.com/biicode/client/issues/10
include(CheckCXXCompilerFlag)
set(ENABLE_CXXFLAGS_TO_CHECK
    -std=gnu++11 
    -std=c++11
    -std=gnu++0x 
    -std=c++0x)

foreach(flag ${ENABLE_CXXFLAGS_TO_CHECK})
    string(REPLACE "-std=" "_" flag_var ${flag})
    string(REPLACE "+" "x" flag_var ${flag_var})
    check_cxx_compiler_flag("${flag}" COMPILER_HAS_CXX_FLAG${flag_var})
    if(COMPILER_HAS_CXX_FLAG${flag_var})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
        break()
    endif()
endforeach()
################ Add executables #################
set(UTILS_INC ${CMAKE_CURRENT_SOURCE_DIR}/../UtilsClass)
set(UTILS_FILES ${UTILS_INC}/utils.cpp ${UTILS_INC}/ModelException.cpp)
set(SOURCE_FILES main.cpp MongoUtil.cpp)
include_directories(${UTILS_INC} ${BSON_INCLUDE_DIR} ${MONGOC_INCLUDE_DIR})
add_executable(MongoUtil ${SOURCE_FILES} ${UTILS_FILES})
target_link_libraries(MongoUtil ${BSON_LIBRARIES} ${MONGOC_LIBRARIES})
install(TARGETS MongoUtil DESTINATION ${INSTALL_DIR})
### For CLion to implement the "make install" command
add_custom_target(install_${PROJECT_NAME}
        $(MAKE) install
        DEPENDS ${PROJECT_NAME}
        COMMENT "Installing ${PROJECT_NAME}")
################# Windows Only  ##################
# Since mongoc_lib_dir is stored in Environment path, there is no need to
# copy dynamic libraries to the bin directory.
# Just remain for backup. -- LJ
# IF(WIN32)
#   # for compile executables
#   install(FILES ${BSON_DLL} ${MONGOC_DLL} DESTINATION ${INSTALL_DIR})
#   # for build vs project
#   IF(MSVC)
#     SET(EXTERNAL_DLLS ${BSON_DLL} ${MONGOC_DLL})
#     FILE(COPY ${EXTERNAL_DLLS} DESTINATION Debug)
#     FILE(COPY ${EXTERNAL_DLLS} DESTINATION Release)
#   ENDIF()
# ENDIF()
