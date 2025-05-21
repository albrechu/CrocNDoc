🔧⚠️ HERE BE DRAGONS - UNDER CONSTRUCTION ⚠️🔧
# Croc & Doc
This is Croc&Doc, a 2D platformer for the vector-based Vectrex gaming console.


## Build
You can for example call `make` at root to compile and run the game. It just calls the batch file 
with `make_c.bat` and `make_c.bat run`. You don't need make. The geany file, I have not tested.

## Project Structure
### General Folder Structure
<pre>
  crocndoc
    ├── assets    # Contains all assets of the game created with the Level Editor, meaning all json files of entities line meshes in the game, as well as each level, overlay and manual.
    ├── build     # Compiled files (If folder is generated)
    ├── include   # All CrocNDoc (cnd) and the modified base library header files
    ├── src       # This is the source folder
    ├── tools     # Utilities (e.g. for creating levels, entities, tiles)
    ├── LICENSE   # License which I added for completeness. Doesn't really matter (WTFPL). 
    ├── Makefile  # A "helper" file that calls make_c.bat.
    ├── make_c.bat # The real deal. Compiles the root project and may be used to run it.
    └── README.md # < You are here!
</pre>

### Source Structure
Beware that all bets are off in the source. So everyone knows anyone and everything can handle anything.
But the following diagram will still roughly map to how code interacts with each other.
<pre>
  main.c              # Entry that interfaces with game.c
    ├─ globals.c      # Contains most important global variables
    └─ game.c         # Handles very high level game logic and connects everything.
         ├─ entities/ # Contains all entities (characters, enemies, props) and their update and draw logic
         |     └ mesh.c # Contains all meshes of all entities and more complex tiles.
         ├─ plot.c    # Possible story beads. Currently not working. Not clear yet if it makes it in the final product.
         ├─ track.c   # All music and sfx that makes the game not feel as empty.
         └ world.c    # Handles how the level is drawn and how entities interact with the world, as well as general book keeping of them.
             ├─ mesh.c 
             └─ levels.c # Contains all levels and their metadata (starting tile, entities, ...)
</pre>

## Build Tools
To build the stage editor, you need cmake with version ≥3.14 and a c++ compiler that supports C++20. Then 
generate the buildsystem with your favorite generator. Inside the stageeditor folder run for example
`cmake -S . -B build`,  `cmake -S . -B build -G "Visual Studio 17 2022"` or `cmake -S . -B build -G "Ninja"`, etc.
Inside roots assets/levels folder, you can find the levels of the game as well as meshes.

