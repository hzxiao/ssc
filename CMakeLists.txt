cmake_minimum_required(VERSION 3.5)
project(ssc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/proto/protob.pb.h src/proto/protob.pb.cc src/proto/message.pb.cc src/proto/message.pb.h src/util/msg_queue.h src/util/sema.cpp src/util/sema.h src/main.cpp src/arduino/cArduino.cpp src/arduino/cArduino.h src/server/ikcp.c src/server/ikcp.h src/main.cpp src/server/sess.cpp src/server/sess.h src/server/io.h)
add_executable(ssc ${SOURCE_FILES})


#
#SET(PROJECT_ROOT_PATH ".")
#INCLUDE_DIRECTORIES("${PROJECT_ROOT_PATH}/include/")
#INCLUDE_DIRECTORIES("${PROJECT_ROOT_PATH}/src/proto")
#
#
#link_directories(${PROJECT_ROOT_PATH}/lib)
#
#add_executable(app ${PROJECT_ROOT_PATH}/main.cpp ${PROJECT_ROOT_PATH}/src/proto/message.pb.cc)
#target_link_libraries(app libprotobuf-lite)