.PHONY:all
all:server client

client:client.cpp
	g++ -o $@ $^ -std=c++11 -g -lpthread
server:server.cpp
	g++ -o $@ $^ -std=c++14 -g -lpthread

.PHONY:clean
clean:
	rm -f server client