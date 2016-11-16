#!/usr/bin/env bash
g++ \
    -std=c++11 -c arduino/cArduino.cpp

gcc \
    -c server/ikcp.c

g++ \
    -std=c++11 -c server/sess.cpp

g++ \
    -c util/sema.cpp

#compile proto file
#protoc \
#    --cpp_out=./proto/ ./proto/*.proto
g++ \
    -c -std=c++11 conf/config.cpp

g++ \
    -std=c++11 cArduino.o ikcp.o sema.o sess.o util/msg_queue.h config.o\
    main.cpp proto/protob.pb.cc  `pkg-config --cflags --libs protobuf` -lpthread \
    -o main