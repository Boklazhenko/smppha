cmake_minimum_required(VERSION 3.16)
project(smppha)

set(CMAKE_CXX_STANDARD 17)

include_directories(./libs)

add_library(smppha smpp/constants.h smpp/pdu/mandatoryparams.h smpp/pdu/types.h smpp/util.cpp smpp/pdu/optionalparams.h smpp/errors.h smpp/pdu/pdu.h smpp/pdu/pduvisitor.h)