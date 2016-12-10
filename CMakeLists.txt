cmake_minimum_required(VERSION 3.5)
project(ssc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(SOURCE_FILES
        src/proto/protob.pb.h src/proto/protob.pb.cc
        src/proto/message.pb.cc src/proto/message.pb.h
        src/util/msg_queue.h src/util/sema.cpp
        src/util/sema.h
        src/main.cpp
        src/arduino/cArduino.cpp
        src/arduino/cArduino.h
        src/server/ikcp.c
        src/server/ikcp.h
        src/main.cpp src/server/sess.cpp
        src/server/sess.h src/server/io.h
        src/conf/config.cpp src/conf/config.h
        )
add_executable(ssc ${SOURCE_FILES})
target_link_libraries(ssc protobuf)