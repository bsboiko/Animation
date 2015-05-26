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

TARGETS = Bezier

SRCS = Bezier.c

default : $(TARGETS)

OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CXX) -c $(COMPILER_FLAGS) -o $@ $< $(INCLUDE)

Bezier: Bezier.o
	$(CXX) $(COMPILER_FLAGS) Bezier.o -o Bezier $(LIBS) 

clean:
	rm -f $(OBJS) $(TARGETS) *~
