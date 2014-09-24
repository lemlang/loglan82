cmake_minimum_required (VERSION 2.8)
project (loglan)
set(WSIZE 4)

find_library(F2C_LIBRARIES NAME libf2c.a PATHS
            /usr/lib
            /usr/lib32
            /usr/local/lib
            /usr/local/lib32
            c:/f2c
            REQUIRED
            )

find_path(F2C_INCLUDE_DIRS NAME f2c.h PATHS
            /usr/include
            /usr/local/include
            c:/f2c
            REQUIRED
            )
if(WIN32)
set(F2C_COMMAND ${F2C_INCLUDE_DIRS}/f2c.exe)
else(WIN32)
set(F2C_COMMAND f2c)
endif(WIN32)

include_directories(${F2C_INCLUDE_DIRS})
set(CMAKE_CPP_FLAGS -P -I. -DWSIZE=${WSIZE}    -DLMEMSIZE=100000 -DLPMEMSIZE=70000      -DWORDS_IN_REAL=1  -DDISABLE_H=1 )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O -DLMEMSIZE=100000 -DLPMEMSIZE=70000 -I. -DWSIZE=${WSIZE} -m32")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -m32")
set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} -m32")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -m32")
set(SRC memfil.c ml3.c main.c stdio.c blank2.h blank3.h blank.h stos.h option.h)
set(FF_FILES al11 al12 al13 it0 it1 scan wan1 )

FOREACH (FF_FILE ${FF_FILES})
add_custom_command (
  OUTPUT ${FF_FILE}.c
  COMMAND cpp ${CMAKE_CPP_FLAGS} ${PROJECT_SOURCE_DIR}/${FF_FILE}.ff > ${PROJECT_SOURCE_DIR}/${FF_FILE}.f
  COMMAND ${F2C_COMMAND} -I${WSIZE} -NL400 ${PROJECT_SOURCE_DIR}/${FF_FILE}.f
  VERBATIM
)
set(SRC ${SRC} ${FF_FILE}.c)
ENDFOREACH(FF_FILE)

set(F_FILES debug  dsw  hash  ifun  ml2  resume  spgrec  statist wan2 wan3 )
FOREACH (F_FILE ${F_FILES})
add_custom_command (
  OUTPUT ${F_FILE}.c
  COMMAND ${F2C_COMMAND} -I${WSIZE} -NL400 ${PROJECT_SOURCE_DIR}/${F_FILE}.f
  VERBATIM
)
set(SRC ${SRC} ${F_FILE}.c)
ENDFOREACH(F_FILE)
add_executable(loglan ${SRC} )
target_link_libraries(loglan ${F2C_LIBRARIES} m)
install (TARGETS loglan DESTINATION bin)
