cmake_minimum_required(VERSION 2.8.11)
project(vlp)
set(wxWidgets_USE_DEBUG OFF)
set(wxWidgets_USE_STATIC OFF)
set(wxWidgets_EXCLUDE_COMMON_LIBRARIES TRUE)
find_package(wxWidgets COMPONENTS core base net REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(Boost 1.48.0 REQUIRED  COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})


set(SRCSL launchermain.cpp Launcher.cpp VLPMainWindow.cpp backward.cpp)
set(SRCSR vmmain.cpp VMServerThread.cpp VM.cpp Configurations.cpp backward.cpp)
set(SRCSG graphicsmain.cpp Graphics.cpp GraphicsWindow.cpp backward.cpp)

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

add_executable(vlp ${SRCSL})
add_executable(vlpvm ${SRCSR})
add_executable(vlpgr ${SRCSG})

target_link_libraries(vlp ${wxWidgets_LIBRARIES})
target_link_libraries(vlp ${Boost_LIBRARIES})
target_link_libraries(vlpvm ${wxWidgets_LIBRARIES})
target_link_libraries(vlpgr ${wxWidgets_LIBRARIES})

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
