all:
	g++ *.cpp */*.cpp -I./ -L/usr/lib/x86_64-linux-gnu -Wall -Wextra -std=c++17 -o mplay `pkg-config --cflags --libs libpulse` -lpulse-simple -g

