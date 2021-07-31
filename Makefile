CXXFLAGS = -ggdb -Wall --std=c++11
# CFLAGS = -g -Wall

all: scheduler client hospitalA hospitalB hospitalC

scheduler: scheduler.cpp
	g++ $(CXXFLAGS) -o scheduler scheduler.cpp 

client: client.cpp
	g++ $(CXXFLAGS) -o client client.cpp

hospitalA: hospitalA.cpp
	g++ $(CXXFLAGS) -o hospitalA hospitalA.cpp

hospitalB: hospitalB.cpp
	g++ $(CXXFLAGS) -o hospitalB hospitalB.cpp

hospitalC: hospitalC.cpp
	g++ $(CXXFLAGS) -o hospitalC hospitalC.cpp

# scheduler_helper.o: scheduler_helper.h scheduler_helper.c
# 	gcc $(CFLAGS) -c scheduler_helper.c

clean: 
	rm -f *.o scheduler client hospitalA
