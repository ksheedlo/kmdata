CC = gcc
CXX = g++
CFLAGS = -g -std=gnu99 -Wall -Wno-unused-result 
CXXFLAGS = -O2 -Wall -Wno-unused-result

error_handling.o: error_handling.c error_handling.h

tuple.o: tuple.h tuple.c

list.o: list.c list.h error_handling.o

dict.o: dict.c dict.h 

oat.o: oat.c oat.h

rbtree.o: rbtree.c rbtree.h list.o

unittest: unittest.c unittest.h rbtree.o dict.o oat.o list.o error_handling.o tuple.o

clean :
	rm unittest *.o *.h.gch 
