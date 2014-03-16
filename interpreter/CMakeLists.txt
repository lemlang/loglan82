#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 2.6)
#Name your project here
project(logint)
# OBJECTADDR - switch on special process addressing - object simulates
#              process pointer
# CDBG - turn on debugging of compactifier, compactifier appends to file
#        'trace' state of memory before & after the compactification,
#        also a history of compactification & process number is dumped
# RPCDBG - debugging of alien call, all actions : alien call, acknowledges,
#          passivations and returns are written to stderr
# NO_GRAPH -    nonstand.c defines only INKEY grom IIUWGRAPH
add_definitions(-DOBJECTADDR -DSYSV -m32)
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -m32")
set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} -m32")
set(CMAKE_SHARED_LIBRARY_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -m32")
if(UNIX)
  link_libraries(m)
endif(UNIX)
add_executable(logint cint.c compact.c control.c util.c handler.c intdt.c memory.c object.c runsys.c standard.c execute.c fileio.c nonstand.c process.c procaddr.c queue.c rpcall.c typchk.c)
