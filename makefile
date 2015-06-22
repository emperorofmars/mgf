# Author: Martin Schwarz
# Makefile for mgf

CC=g++
CFLAGS=-g -Wall -std=c++11 -O -Wl,-rpath=./libs/
LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lGLEW -lGL -lassimp
NAME=mgf

Debug: mgf
Release: mgf
all: mgf

mgf: tmpobj.o
	mkdir -p deploy
	${CC} ${CFLAGS} -o $(NAME) *.o ${LIBS}
	mv $(NAME) deploy/$(NAME)

tmpobj.o: *.cpp
	${CC} ${CFLAGS} -c `find ./src -type f -name "*.cpp"` main.cpp ${LIBS}

clean:
	rm -f *.o bin/${NAME}
