all:
	g++ -std=c++11 -w -c cpu.cpp
	g++ -std=c++11 -w -c main.cpp
	g++ -std=c++11 -w cpu.o main.o -o main