cmake_minimum_required(VERSION 2.6)
Project(BioAmp)

SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BioAmp_BINARY_DIR}/lib )
SET( CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries")
SET( CMAKE_INSTALL_PREFIX /usr)
add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(tools)