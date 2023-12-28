# Custom-Chess
A chess program that allows you to write custom chess variations on the fly in .chess files. All written in C with visual studio x64 command line tools.

-user32.lib

-gdi32.lib

-msimg32.lib

So far:
A simple main.c that reads contents of a background.bmp file and board.bmp file, and draws things appropriately, as well as piece highlights.
Uses a few header files from the Headers directory. I learned to manually read .bmp files and .tga files for fun and built a standardized sprite structure for use throughout the program.

NOTE:
-
Project on temporary hiatus for explorations in self-developing programming languages, as the goal is to have an IDE built into the game for players to be able to write custom chess variants.

Planning on said language so far:

-(Almost) JSON-like header providing meta-data on the spritesheet and how to use the code to follow

-A lot of generators for generating piece moves and side effects in function syntax

-The goal is to use set syntax and other built in features (like linear or non linear raycasting, as seen with rooks and bishops) for a language optimized for chess variants.

-If possible, the code compiles directly to assembly and into memory at runtime, however a virtual bytecode executed with a jump table might be the case instead, if that proves too complicated.
