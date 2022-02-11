students:
	g++ -pthread -o main main.cpp teams.cpp -DSTUDENTS
	g++ -pthread -o new_process new_process.cpp

all:
	g++ -pthread -o main main.cpp teams.cpp
	g++ -pthread -o new_process new_process.cpp
