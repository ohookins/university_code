SHELL = /bin/sh

libs_for_cygwin = -lopengl32 -lglut32 -lglu32
libs_for_pompeii = -lglut -lGLU -lGL -lXext -lX11 -lm
include_for_pompeii = -I/usr/local/opengl/Mesa/include

ifeq ($(OSTYPE),cygwin)
LLDLIBS = $(libs_for_cygwin)
INCLUDE =
else
LLDLIBS = $(libs_for_pompeii)
INCLUDE = $(include_for_pompeii)
endif

nodemaster : nodemaster.o Node.o Canvas.o
	g++ -o nodemaster nodemaster.o Node.o Canvas.o $(LLDLIBS) $(INCLUDE)

nodemaster.o : nodemaster.cc Node.h Canvas.h
	g++ -c nodemaster.cc $(INCLUDE)

Node.o : Node.cc
	g++ -c Node.cc $(INCLUDE)

Canvas.o : Canvas.cc Point2.o RealRect.o IntRect.o
	g++ -c Canvas.cc $(INCLUDE)

RealRect.o : RealRect.cc
	g++ -c RealRect.cc $(INCLUDE)

IntRect.o : IntRect.cc
	g++ -c IntRect.cc $(INCLUDE)

Point2.o : Point2.cc
	g++ -c Point2.cc $(INCLUDE)

clean:
	rm -f nodemaster.o Node.o nodemaster nodemaster.exe Canvas.o RealRect.o 	IntRect.o Point2.o
