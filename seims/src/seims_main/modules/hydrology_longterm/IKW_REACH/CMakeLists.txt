MESSAGE(STATUS "        Compiling hydrology_longterm_subdir: IKW_REACH...")
SET(SRC_LIST ${MODULE_SRC} IKW_REACH.cpp api.cpp)
ADD_LIBRARY(IKW_REACH SHARED ${SRC_LIST})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
TARGET_LINK_LIBRARIES(IKW_REACH util data bmps module_setting)
INSTALL(TARGETS IKW_REACH DESTINATION ${INSTALL_DIR})
IF (MSVC)
    SET_PROPERTY(TARGET IKW_REACH PROPERTY FOLDER "modules/hydrology_longterm")
ENDIF ()
