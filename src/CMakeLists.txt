cmake_minimum_required(VERSION 2.8.11)
project(vlp)
set(wxWidgets_USE_DEBUG OFF)
set(wxWidgets_USE_STATIC OFF)
set(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
find_package(wxWidgets COMPONENTS core base net REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(Boost 1.46.0 REQUIRED  COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})


set(SRCSL vlp.cpp vlp/App.cpp vlp/MainWindow.cpp)
set(SRCSR vm.cpp vm/ServerThread.cpp vm/VM.cpp vm/Configurations.cpp)
set(SRCSG graphicsmain.cpp Graphics.cpp GraphicsWindow.cpp)

set(PATH_LIBDW_LIB "")
set(PATH_LIBDW_H "")
IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
  find_library(PATH_LIBDW_LIB NAMES dw)
  find_path(PATH_LIBDW_H "elfutils/libdw.h")

    message("debug mode")
if(PATH_LIBDW_LIB AND PATH_LIBDW_H)
  string(REGEX REPLACE "/libdw.*[.]${LIB_EXE}$" "" PATH_LIBDW_LIB "${PATH_LIBDW_LIB}")
  string(REGEX REPLACE "/libdw.h"               "" PATH_LIBDW_H   "${PATH_LIBDW_H}")

  include_directories(${PATH_LIBDW_H})
  link_directories(${PATH_LIBDW_LIB})

  IST(APPEND SRCSL backward.cpp)
  IST(APPEND SRCSR backward.cpp)
  IST(APPEND SRCSG backward.cpp)
else()
  message(FATAL_ERROR "Please install the libdw-dev package.")
endif()
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)


# If we build for windows systems, we also include the resource file
# containing the manifest, icon and other resources
if(WIN32)
  set(CMAKE_RC_COMPILER_INIT windres)
  ENABLE_LANGUAGE(RC)
  SET(CMAKE_RC_COMPILE_OBJECT
    "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
  set(SRCSL  vlp.rc ${SRCSL})
  set(CMAKE_CXX_FLAGS "-mwindows")
endif(WIN32)

add_executable(vlp ${SRCSL} vlp/MessageDialog.h vlp/MessageDialog.h vlp/MessageDialog.cpp vlp/MessageDialog.cpp vlp/MessageDialog.h)
add_executable(vlpvm ${SRCSR})
add_executable(vlpgr ${SRCSG})

target_link_libraries(vlp ${wxWidgets_LIBRARIES} ${PATH_LIBDW_LIB})
target_link_libraries(vlp ${Boost_LIBRARIES} ${PATH_LIBDW_LIB})
target_link_libraries(vlpvm ${wxWidgets_LIBRARIES} ${PATH_LIBDW_LIB})
target_link_libraries(vlpgr ${wxWidgets_LIBRARIES} ${PATH_LIBDW_LIB})

if( MINGW )
	MESSAGE( STATUS "    Installing system-libraries: wxWidgets DLLs." )
	SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${wxWidgets_LIB_DIR}/wxmsw30u_core_gcc_custom.dll ${wxWidgets_LIB_DIR}/wxbase30u_net_gcc_custom.dll ${wxWidgets_LIB_DIR}/wxbase30u_gcc_custom.dll )
endif( MINGW )
install (TARGETS vlp DESTINATION bin)
install (TARGETS vlpvm DESTINATION bin)
install (TARGETS vlpgr DESTINATION bin)

if(WIN32)
	INCLUDE( InstallRequiredSystemLibraries )
else(WIN32)
	install(FILES vlp.desktop DESTINATION share/applications)
	install(FILES vlp.png vlp.svg DESTINATION share/icons)
endif(WIN32)
