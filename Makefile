students:
	g++ -pthread -o main main.cpp teams.cpp -DSTUDENTS

async:
	g++ -pthread -o main main.cpp teams.cpp
