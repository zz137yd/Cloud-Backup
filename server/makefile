.PHONY:cloud
cloud:cloud.cpp bundle.cpp
	g++ -g $^ -o $@ -lpthread -lstdc++fs -ljsoncpp -std=c++11
.PHONY:clean
clean:
	rm -f cloud
