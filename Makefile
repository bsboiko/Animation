#!smake
#  @@ Makefile for CMSC 435/634 - Project 4: Animation
#  @@ Jian Chen (jichen@umbc.edu)
#

MACHINE= $(shell uname -s)


ifeq ($(MACHINE),Darwin)
	OPENGL_INC= -FOpenGL
	OPENGL_LIB= -framework OpenGL -framework GLUT -framework Cocoa
else
	OPENGL_INC= -I/usr/include
	OPENGL_LIB= -L/usr/lib64 
	LLDLIBS = -lglut -lGL -lGLU -lGL -lXmu -lXext -lX11 -lm
endif

CXX=g++
COMPILER_FLAGS= -g

INCLUDE= $(OPENGL_INC)
LIBS= $(OPENGL_LIB) $(LLDLIBS) 

TARGETS = Raytracer

SRCS = Raytracer.c vector.c

default : $(TARGETS)

OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

Raytracer: Raytracer.o vector.o
	$(CXX) $(COMPILER_FLAGS) Raytracer.o vector.o -o Raytracer $(LIBS) 

clean:
	rm -f $(OBJS) $(TARGETS) *~
