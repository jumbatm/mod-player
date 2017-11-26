all:
	g++ *.cpp ./src/*.cpp -I./ -I./include -Wall -Wextra -std=c++17 -o mplay
