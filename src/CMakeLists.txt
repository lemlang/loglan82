##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
## Update:      2008/12 by Werner Smekal
##---------------------------------------------------------------------------

# define minimum cmake version
cmake_minimum_required(VERSION 2.8.8)

# Our project is called 'minimal' this is how it will be called in
# visual studio, and in our makefiles.
project(vlp)

# Location where cmake first looks for cmake modules.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")

##---------------------------------------------------
## Please set your wxWidgets configuration here
##---------------------------------------------------

# Here you can define what libraries of wxWidgets you need for your
# application. You can figure out what libraries you need here;
# http://www.wxwidgets.org/manuals/2.8/wx_librarieslist.html
# We need the Find package for wxWidgets to work
# NOTE: if you're using aui, include aui in this required components list.

# It was noticed that when using MinGW gcc it is essential that 'core' is mentioned before 'base'.
find_package(wxWidgets COMPONENTS core base net REQUIRED)

##---------------------------------------------------
## Actual config file starts here
##---------------------------------------------------
# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

# For convenience we define the sources as a variable. You can add
# header files and cpp/c files and CMake will sort them out
set(SRCSL launchermain.cpp Launcher.cpp VLPMainWindow.cpp backward.cpp)
set(SRCSR vmmain.cpp VMServerThread.cpp VM.cpp Configurations.cpp backward.cpp)
set(SRCSG graphicsmain.cpp Graphics.cpp GraphicsWindow.cpp backward.cpp)

# If we build for windows systems, we also include the resource file
# containing the manifest, icon and other resources
if(WIN32)
  set(SRCSL ${SRCS} vlp.rc)
endif(WIN32)
# Here we define the executable minimal.exe or minimal on other systems
# the above paths and defines will be used in this build
add_executable(vlp WIN32 ${SRCSL})
add_executable(vlpvm WIN32 ${SRCSR})
add_executable(vlpgr WIN32 ${SRCSG})
# We add to our target 'vlp' the wxWidgets libraries. These are
# set for us by the find script. If you need other libraries, you
# can add them here as well.
target_link_libraries(vlp ${wxWidgets_LIBRARIES})
target_link_libraries(vlpvm ${wxWidgets_LIBRARIES})
target_link_libraries(vlpgr ${wxWidgets_LIBRARIES})
install (TARGETS vlp DESTINATION bin)
install (TARGETS vlpvm DESTINATION bin)
install (TARGETS vlpgr DESTINATION bin)
INSTALL(FILES vlp.desktop DESTINATION share/applications)
install(FILES vlp.png vlp.svg DESTINATION share/icons)
