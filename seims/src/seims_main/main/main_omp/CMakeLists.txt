MESSAGE(STATUS "        Compiling main_subdir: seims_omp...")
SET(SRC_LIST ModelMain.cpp main.cpp invoke.cpp)
IF (PARALLEL STREQUAL MPI)
    ADD_LIBRARY(main_mongodb STATIC ${SRC_LIST})
    SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
    TARGET_LINK_LIBRARIES(main_mongodb util data bmps module_setting)
    INSTALL(TARGETS main_mongodb DESTINATION ${INSTALL_DIR})
    IF (MSVC)
        SET_PROPERTY(TARGET main_mongodb PROPERTY FOLDER "main")
    ENDIF ()
ELSE ()
    ADD_EXECUTABLE(seims_omp ${SRC_LIST})
    SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
    TARGET_LINK_LIBRARIES(seims_omp util data bmps module_setting ${CMAKE_DL_LIBS})
    INSTALL(TARGETS seims_omp DESTINATION ${INSTALL_DIR})
    IF (MSVC)
        SET_PROPERTY(TARGET seims_omp PROPERTY FOLDER "main")
    ENDIF ()
ENDIF ()
