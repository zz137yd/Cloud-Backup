.PHONY:cloud
cloud:cloud.cpp
	g++ -g $^ -o $@ -L./lib -lpthread -lstdc++fs -ljsoncpp -lbundle -std=c++11
.PHONY:clean
clean:
	rm -f cloud