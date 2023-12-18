CC = g++
Target = CourseWork

sources = $(wildcard ./*.cpp)

main:
	${CC} -o ./build/${Target} -std=c++17 ${sources}
	MAKE -C ./build