MESSAGE(STATUS "        Compiling erosion_subdir: IUH_SED_OL...")
SET(SRC_LIST ${MODULE_SRC} IUH_SED_OL.cpp api.cpp)
ADD_LIBRARY(IUH_SED_OL SHARED ${SRC_LIST})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
TARGET_LINK_LIBRARIES(IUH_SED_OL util data bmps module_setting)
INSTALL(TARGETS IUH_SED_OL DESTINATION ${INSTALL_DIR})
IF (MSVC)
    SET_PROPERTY(TARGET IUH_SED_OL PROPERTY FOLDER "modules/erosion")
ENDIF ()
