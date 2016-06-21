CC = g++
CFLAGS = -Wall -g -std=c++11

all: main

clean:
	rm -rf *o
	rm fork_pipe
	
main.o:
	${CC} ${CFLAGS} -c Fork_pipe_ex.cxx
	
	
main: main.o
	${CC} ${CFLAGS} Fork_pipe_ex.o -o fork_pipe
