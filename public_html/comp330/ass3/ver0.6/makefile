SHELL = /bin/sh

libs_for_cygwin = -lopengl32 -lglut32 -lglu32
libs_for_pompeii = -lglut -lGLU -lGL
# removed -lXext -lX11 -lm

include_for_pompeii = -I/usr/local/opengl/Mesa/include

ifeq ($(OSTYPE),cygwin)
LLDLIBS = $(libs_for_cygwin)
INCLUDE =
else
LLDLIBS = $(libs_for_pompeii)
INCLUDE = $(include_for_pompeii)
endif

shinysphere: sphere.o SDL.o RGBpixmap.o
	g++ -o shinysphere sphere.o SDL.o RGBpixmap.o $(LLDLIBS) $(INCLUDE)

sphere.o : sphere.cc
	g++ -c sphere.cc $(INCLUDE)

SDL.o : SDL.cc SDL.h
	g++ -c SDL.cc $(INCLUDE)

RGBpixmap.o : RGBpixmap.cc RGBpixmap.h
	g++ -c RGBpixmap.cc $(INCLUDE)

clean:
	rm -f sphere.o SDL.o RGBpixmap.o shinysphere shinysphere.exe *~
