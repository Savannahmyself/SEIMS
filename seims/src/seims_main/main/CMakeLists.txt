MESSAGE(STATUS "      Compiling SEIMS_subdir: main...")
ADD_SUBDIRECTORY(./test)
ADD_SUBDIRECTORY(./main_omp)
IF (PARALLEL STREQUAL MPI)
    ADD_SUBDIRECTORY(./main_mpi)
ENDIF ()
