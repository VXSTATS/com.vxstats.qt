# cmake

## Configuration files for cmake build environment

env.cmake
Environment variables

doxygen.cmake
Template to run doxygen - not yet usable

find.cmake
Overall find package or libraries to use these variables in other cmake files

## Add environment variables

Open env.cmake and create a new include directory:
set(SOURCES_MYSHAREDINCLUDE ${SOURCES_SHARED}/MySharedInclude)
