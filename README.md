# Starship Fontana #

This is an example C++ application using the SDL library.
It tries to be as nicely C++11 as possible but do keep in
mind that SDL is written in C and, at some stage, you have
to interface with it.

## Story ##
The evil b’Kuhn has stolen the code to Earth’s defence system.
With this code he can, at any time, defeat the entire human race.
Only one woman is brave enough to go after b’Kuhn. Will she be
Earth’s hero? Puzzle your way though the universe in the company
of Commander Fontana in **Starship Fontana**.

## Installation ##
You will have to have the SDL development libraries installed on
your system.
You will have to clone this repository into your own pc and all what you have to do to compile and run the game is by running the 'build' file from the command line.

```bash
$ ./build
``` 

## Issues ##
I think the SDL port has introduced some bounding box collision issues.

## Credits ##
The sprites in this game come directly from 
[SpriteLib](http://www.widgetworx.com/widgetworx/portfolio/spritelib.html) and are used
under the terms of the [CPL 1.0](http://opensource.org/licenses/cpl1.0.php).


The event system is based on [Libsigc++](http://libsigc.sourceforge.net/)
and is used under the [LGPL](http://www.gnu.org/copyleft/lgpl.html).

Also some of the sprites used are created and edited by me.
