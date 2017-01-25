# hivolts
# By Jaldhar h. Vyas <jaldhar@braincells.com>
# Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
# "Do What Thou Wilt." shall be the whole of the license.
SHELL=/bin/sh
CXX=c++
CXXFLAGS=-std=c++14 -O2 -g -Wall -Wextra -Wcast-qual -Wformat=2
LDFLAGS= -lncurses
.SUFFIXES: .cc
PROGRAM=hivolts
OBJECTS= hivolts.o \
	collision.o \
	create.o \
	explosion.o \
	gameover.o \
	input.o \
	mhoai.o \
	movement.o \
	render.o

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

collision.o: collision.h world.h

create.o: create.h world.h

explosion.o: explosion.h world.h

gameover.o: gameover.h world.h

hivolts.o: collision.h create.h explosion.h gameover.h input.h mhoai.h movement.h render.h victory.h world.h

input.o: input.h world.h

mhoai.o: mhoai.h world.h

movement.o: movement.h world.h

render.o: render.h world.h

components.h: appearance.h command.h

gameover.h: victory.h

render.h: victory.h

world.h: components.h entity.h

memcheck: $(PROGRAM)
	valgrind --leak-check=full --show-leak-kinds=all -v ./$(PROGRAM)

clean:
	-rm *.o *.gch

distclean: clean
	-rm $(PROGRAM)

.PHONY: memcheck clean distclean
