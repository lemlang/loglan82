cmake_minimum_required(VERSION 2.8.11)
project(vlpc)
set(wxWidgets_USE_DEBUG OFF)
set(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
find_package(wxWidgets 3.0 COMPONENTS base REQUIRED)
include(${wxWidgets_USE_FILE})

set(SRCS main.cpp Compiller.cpp IncludeFile.cpp)
add_executable(vlpc ${SRCS})
target_link_libraries(vlpc ${wxWidgets_LIBRARIES})
install (TARGETS vlpc DESTINATION bin)
get_filename_component(EXAMPLES_DIR ${PROJECT_SOURCE_DIR} PATH)
set(EXAMPLES_DIR ${EXAMPLES_DIR}/examples)
message(STATUS ${EXAMPLES_DIR})
enable_testing()
set(TEST_FILES bank2.log chartres.log complex.log differ.log geo.log graphcol.log mat.log merge.log roundcm.log treegen.log winograd.log )
FOREACH (TEST_FILE ${TEST_FILES})
add_test(
    NAME "compile_${TEST_FILE}"
    WORKING_DIRECTORY ${EXAMPLES_DIR}
    COMMAND ${CMAKE_INSTALL_PREFIX}/bin/vlpc ${TEST_FILE} )
ENDFOREACH(TEST_FILE)
add_test(
    NAME "md5check"
    WORKING_DIRECTORY ${EXAMPLES_DIR}
    COMMAND md5sum -c md5sums )
