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

