cmake_minimum_required(VERSION 2.8)
project(loglanint)
# OBJECTADDR - switch on special process addressing - object simulates
#              process pointer
# CDBG - turn on debugging of compactifier, compactifier appends to file
#        'trace' state of memory before & after the compactification,
#        also a history of compactification & process number is dumped
# RPCDBG - debugging of alien call, all actions : alien call, acknowledges,
#          passivations and returns are written to stderr
# NO_GRAPH -    nonstand.c defines only INKEY grom IIUWGRAPH
add_definitions(-DOBJECTADDR -DSYSV )
IF(${BASIC})
add_definitions(-DBASIC=1)
ENDIF()

find_package(wxWidgets COMPONENTS base net REQUIRED)
# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ")
set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} ")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} ")
if(UNIX)
  link_libraries(m)
endif(UNIX)

if(WIN32)
link_libraries(wsock32)
endif(WIN32)

set(SRC cint.c getline.c sockets.c compact.c control.c util.c handler.c intdt.c memory.c object.c runsys.c standard.c execute.c fileio.c nonstand.c process.c procaddr.c queue.c rpcall.c typchk.c)

add_executable(loglanint ${SRC})
target_link_libraries(loglanint ${wxWidgets_LIBRARIES})
install (TARGETS loglanint DESTINATION bin)