MESSAGE(STATUS "        Compiling nutrient_subdir: NUTR_TF...")
SET(SRC_LIST ${MODULE_SRC} Nutrient_Transformation.cpp api.cpp)
ADD_LIBRARY(NUTR_TF SHARED ${SRC_LIST})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
TARGET_LINK_LIBRARIES(NUTR_TF util data bmps module_setting)
INSTALL(TARGETS NUTR_TF DESTINATION ${INSTALL_DIR})
IF (MSVC)
    SET_PROPERTY(TARGET NUTR_TF PROPERTY FOLDER "modules/nutrient")
ENDIF ()
