cmake_minimum_required (VERSION 2.8)
project (loglan)
set(WSIZE 4)
find_package (F2C)
set(CMAKE_CPP_FLAGS -P -I. -DWSIZE=${WSIZE}    -DLMEMSIZE=100000 -DLPMEMSIZE=70000      -DWORDS_IN_REAL=1  -DDISABLE_H=1 )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O -DLMEMSIZE=100000 -DLPMEMSIZE=70000 -I. -DWSIZE=${WSIZE} -m32")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -m32")
set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} -m32")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -m32")
set(SRC memfil.c ml3.c main.c stdio.c blank2.h blank3.h blank.h stos.h option.h)
set(FF_FILES al11 al12 al13 it0 it1 scan wan1 )
#set(FF_FILES al11.F al12.F al13.F it0.F it1.F scan.F wan1.F)
FOREACH (FF_FILE ${FF_FILES})
add_custom_command (
  OUTPUT ${FF_FILE}.c
  COMMAND cpp ${CMAKE_CPP_FLAGS} ${PROJECT_SOURCE_DIR}/${FF_FILE}.ff > ${PROJECT_SOURCE_DIR}/${FF_FILE}.f 
  COMMAND f2c -I${WSIZE} -NL400 ${PROJECT_SOURCE_DIR}/${FF_FILE}.f 
  VERBATIM
)
set(SRC ${SRC} ${FF_FILE}.c)
ENDFOREACH(FF_FILE)

set(F_FILES debug  dsw  hash  ifun  ml2  resume  spgrec  statist wan2 wan3 )
FOREACH (F_FILE ${F_FILES})
add_custom_command (
  OUTPUT ${F_FILE}.c
  COMMAND f2c -I${WSIZE} -NL400 ${PROJECT_SOURCE_DIR}/${F_FILE}.f 
  VERBATIM
)
set(SRC ${SRC} ${F_FILE}.c)
ENDFOREACH(F_FILE)
add_executable(loglan ${SRC} )
target_link_libraries(loglan f2c.a m)
install (TARGETS loglan DESTINATION bin)

