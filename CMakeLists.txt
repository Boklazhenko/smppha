cmake_minimum_required(VERSION 3.16)
project(smppha)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2")

include_directories(./libs)

add_library(smppha smpp/constants.h
        smpp/pdu/mandatory_params.h
        smpp/pdu/types.h
        smpp/util.cpp
        smpp/pdu/optional_params.h
        smpp/errors.h
        smpp/pdu/pdu.h
        smpp/pdu/pdu_visitor.h
        smpp/socket/socket.h smpp/socket/socket.cpp)