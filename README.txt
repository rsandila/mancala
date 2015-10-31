What is this?
-------------

I got this board game as a present that I have never seen before: Mancala. 
So I looked at it and it seemed to be pretty simple. I decided to write a little
solver for it just to build a better understanding of the game and to have some fun
coding C++.

How to compile?
---------------
Install premake4. Run premake4 to generate the project for makefiles/xcode/vs2010 or whatever
your flavor of the month is. The code is standard C++ which should be portable to everywhere.

I tested it for Mac OS X and Xcode.

How to run?
-----------
./mancala will let the computer play against itself using the same rules. The "r" flag will make one
of the players always make a random move. Any other flag will enable manual mode where the user has to
enter a move (0-5) to make.

Known issues?
-------------
The printout of the board is not 100% correct. One of the player's depressions and mancala should be printed in reverse.

Rules of Mancala
----------------
Look at http://www.mancalarules.com

Legalize
--------
As far as I know there is no trademark/copyright protection on the name Mancala. The code is released under an Apache v2.0 license.


