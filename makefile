all:
	g++ *.cpp ./src/*.cpp ./soundplayer/*.cpp -I./ -I./include -I./soundplayer -L/usr/lib/x86_64-linux-gnu -Wall -Wextra -std=c++17 -o mplay -lpulse-simple -lpulse -g
