CC = gcc
XX = g++
CFLAGS = -Wall -O -g
TARGET = app

SrcFile = $(wildcard main.c main.cpp)
OBJ = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SrcFile)))

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

$(TARGET):$(OBJ)
	$(XX) $(OBJ) -o $(TARGET) 
.PHONY:
	clean app
clean:
	rm -rf *.o app
