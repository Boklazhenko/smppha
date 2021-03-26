cmake_minimum_required(VERSION 3.16)
project(smppha)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Ofast")

include_directories(./libs)
include_directories(./libs/concurrentqueue/include)

add_library(smppha smpp/constants.h
        smpp/pdu/mandatory_params.h
        smpp/pdu/types.h
        smpp/util.cpp
        smpp/pdu/optional_params.h
        smpp/pdu/pdu.h
        smpp/pdu/pdu_visitor.h
        smpp/socket/socket.h smpp/socket/socket.cpp smpp/pdu/segmented_info.h smpp/pdu/segmented_info.cpp smpp/pdu/delivery_receipt_info.cpp smpp/pdu/delivery_receipt_info.h)