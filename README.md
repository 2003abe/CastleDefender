Castle Defender - Fall 2025
===========================

How to Compile
--------------
This project was developed in Microsoft Visual Studio (MSVC).
To compile using Visual Studio:

1. Create an Empty C++ Project.
2. Add all .cpp and .h files to the project.
3. Ensure towers.txt is placed in the executable directory.
4. Build and run the solution.

Alternatively, if using a g++ environment:

    g++ -std=c++17 main.cpp [other .cpp files] -o CastleDefender


How to Run
----------
On Windows (Command Prompt, PowerShell, or Visual Studio):

    CastleDefender.exe

The game begins immediately after reading tower positions from towers.txt.
No user input is required during gameplay.


Running in Eclipse
------------------
Eclipse CDT may place the compiled executable in a different directory than Visual Studio.
Because the game reads towers.txt at startup, the file must be located in the program’s
working directory (typically Debug/ or Release/).

To ensure correct behaviour:

1. Place towers.txt inside the Debug/ or Release/ folder.
2. OR in Eclipse: open Run Configurations and set the working directory to the project root.

Note: Eclipse on Linux will require replacing Windows-specific console functions such as
SetConsoleCursorPosition and SetConsoleTextAttribute before compiling.


Configuring Tower Positions
---------------------------
Tower placements in the game are fully configurable through the towers.txt file.
The game reads this file at startup and places towers on the grid according to the
coordinates listed.

Editing Tower Positions:

1. Open towers.txt in any text editor.
2. Each line represents one tower in the format:

       r c

   where r = row index and c = column index on the 20×20 grid.

3. Modify the numbers to adjust tower positions. Example:

       2 5
       4 10
       7 3

4. Save the file.
5. Re-run CastleDefender.exe.


Important Notes
---------------
- Rows and columns are zero-indexed (0–19).
- Do not place towers on the castle row (bottom row); these towers will be ignored.
- Avoid duplicate tower positions.
- Invalid coordinates (outside 0–19) are automatically discarded.


Files Included
--------------
- main.cpp
- Game.cpp, Game.h
- Grid.cpp, Grid.h
- Tower.cpp, Tower.h
- Enemy.cpp, Enemy.h
- FastEnemy.cpp, FastEnemy.h
- StrongEnemy.cpp, StrongEnemy.h
- Castle.cpp, Castle.h
- AI.cpp, AI.h
- towers.txt
- Castle_Defender_Report_Template_F2025.pdf
- README.txt


Notes
-----
Tested on:
- Windows 11
- Microsoft Visual Studio 2022 (MSVC)
