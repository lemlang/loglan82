#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(loglangen)
add_definitions(-O  -DHUGE -DTALK=20 -m32)
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -m32")
set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} -m32")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -m32")

add_executable(loglangen gen.c mainvar.c genio.c lists.c genprot.c oxen.c back.c logen.c deb.c lcode.c)

install (TARGETS loglangen DESTINATION bin)

