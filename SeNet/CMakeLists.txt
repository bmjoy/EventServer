cmake_minimum_required(VERSION 3.11)
project(SeNet)
add_definitions(-D__LINUX__)
include_directories(../Common ../ ./ /usr/include /usr/local/include /usr/include/mysql)
link_directories()
set(SOURCE_FILE http/http_client.cpp http/http_server.cpp http/mongoose.c SeFClientBase.cpp SeFServerBase.cpp SeNet.cpp SeEpoll.cpp SeFNetClient.cpp SeFNet.cpp SeFNodeNet.pb.cc SeIOCP.cpp SeKqueue.cpp ServerPublic.pb.cc SeSelect.cpp Session.cpp Socket.cpp SocketBuffer.cpp SocketDefine.cpp)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -o2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
        
add_library(SeNet ${SOURCE_FILE})
target_link_libraries(SeNet  )