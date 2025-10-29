# Croc & Doc
This is Croc&Doc, a 2D platformer for the vector-based Vectrex gaming console. [PLAY](https://drsnuggles.github.io/jsvecx/?rom=http://eitidaten.fh-pforzheim.de%2Fdaten%2Fmitarbeiter%2Fjohannsen%2Fvectrex_2025%2Fdownload%2Fcroc_n_doc.zip)

Dangerous creatures roam the levels.    | Understand the gravitas of the situation.
:--------------------------------------:|:-----------------------------------------:
![](assets/images/jump.png?raw=true "") | ![](assets/images/gravity.png?raw=true "")

| Beware of deadly spikes. | You only have 3 lives. Don't lose them all and get the highest score!
:-------------------------:|:-------------------------:
![](assets/images/water.png?raw=true "") | ![](assets/images/lives.png?raw=true "")

## Build
Unfortunately the tool chain to build the project, besides the tooling, is currently private. 

The game should be easy to build with the batch file that was slightly changed. You can for example call `make` at root to compile 
and run the game. It just calls the batch file with `make_c.bat` and `make_c.bat run`. You don't need make, but this would be 
easier for testing without relying on geany. Note that the geany file was never tested, because I use Visual Studio as an editor 
for development with a seperate terminal instance and Make to run the project. The root folder has to be inside the `projects` 
folder as provided by the Vectrex course.

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
The game works through a general entity type, which when created gets the appropriate procedure (behavior) function and a collision function, which 
is a tile collision function. Collision is checked for the current tile, as well as potentially the tile in x- and one in y-direction depending on 
the entities velocity. An entity (also the player) is always looked at as having a position in global (16-Bit) and in local space (visible). The 
players local space position is fixed at (y=0, x=0). The player is also called `CAMERA` in the source because he is the anchor to other entities 
local space. For bit width or performance reasons are other entities only drawn or updated if they are visible.
The world is drawn in a N by N tile area around the player relying on 8-bit integer overflows. While it is bad practice, will it avoid some needless 
clock cycles. 

## Build Tools
To build the stage editor, you need cmake with version ≥3.14 and a c++ compiler that supports C++20. Then 
generate the buildsystem with your favorite generator. Inside the stageeditor folder run for example
`cmake -S . -B build`,  `cmake -S . -B build -G "Visual Studio 17 2022"` or `cmake -S . -B build -G "Ninja"`, etc.
Inside roots assets/levels folder, you can find the levels of the game as well as meshes.

The most important one of the tools implemented for this project is the stage editor, which features full level creation (save, load, export ...) 
and a sprite tab for a vector list for easy drawing of sprites. Both can be exported, which automatically copies the C-code to clipboard.

 Stage Editor | Sprite Editor
 :-----------:|:-------------:
![](assets/images/stageeditor.png?raw=true "Stage Editor") | ![](assets/images/spriteeditor.png?raw=true "Sprite Editor")

