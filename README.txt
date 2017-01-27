Hivolts -- The Game of Electric Fences.

Hivolts was one of the earliest computer games.  It was developed by Doug Jones
for the PLATO system at the University of Illinois in the 1970's.

HOW TO PLAY
===========

In this game you the player (represented by a red '@') are being chased by 12
mho's (represented by green 'O's) around a 12x12 board.  The board is surrounded
by electric fences and there are another 20 fences randomly placed in the board.
Fences are represented as yellow '#'s.

Incidently a mho is a unit of electrical conductance equal to the reciprocal of
the ohm.  (mho is ohm backwards.)  In the SI system it is known as Siemens.

If you collide with a mho or a fence you will be electrocuted and lose the game.
If you can destroy all the mhos by causing them to collide with fences or with
each other, you will win.

When it is your turn, you are allowed to move to any adjacent square. Adjacent
is defined as one square away, either horizontal, vertical, or diagonal.

You can also jump to a random square.  Your destination is guaranteed to not be
a fence but you could possibly land on a mho.  (Which will cause you to lose.)
You can jump as many times as you want.  Your turn will only end once you have
moved.

After you have moved, it will be the mho's turn to move and try to catch you.

When a collision happens, you will see an 'explosion'; first a red 'O', then a
red 'o', then a magenta '*' before the objects that collided disappear. (Not
fences though.)

KEYS
====

Q, q or HOME: move up and left
W, w, or CURSOR UP: move up
E, e, or PAGE UP: move up and right
A, a, or CURSOR LEFT: move left
D, d, or CURSOR RIGHT: move right
Z, z, or END: down and move left
X, x, or CURSOR DOWN: move down
C, c, or PAGE DOWN: move down and right
J, j: jump
S, s,: sit (stay on the same square for one turn)

If the display gets corrupted, you can redraw it by pressing CONTROL+R.

To quit the game, press the ESCAPE key.

INSTALLATION
============

This game is distributed as source.  It should compile anywhere that has a C++14
compiler and the Curses library but it has only been tested on Debian GNU/Linux
7.0 and Ubuntu Linux 16.04 with g++ 5.4.0 and the ncurses library.

LICENSE/COPYRIGHT
=================

By Jaldhar H. Vyas <jaldhar@braincells.com>
Copyright (C) 2017, Consolidated Braincells Inc.  All rights reserved.
"Do What Thou Wilt." shall be the whole of the license.
