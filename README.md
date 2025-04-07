# Croc & Doc
This is Croc&Doc™, a 2D platformer for the vector-based Vectrex gaming console, which was released in 1982.

## Build
As stated, the Vectrex is old; Very old. It therefore requires some hardware and toolchain that is generally not found today.\  
It is based on the 8-Bit µProcessor [Motorola 6809](https://en.wikipedia.org/wiki/Motorola_6809) and for compilation you require\  
an appropriate GCC C compiler for this target. I used the compiler found inside this\  
[Pre-Release Tag](https://github.com/albrechu/CrocAndDoc/releases/tag/PreProd-Toolchain), which contains all\  
required compilers and linkers as well as RAM and ROM functions. You have to either: 
    * Change the Makefile variables at the top appropriately to the correct path.
    * Or you fetch the repository yourself to the lib folder, or with a script ([py](scripts/fetch_vide.py), [ps1](scripts/fetch_vide.ps1)).

You can then compile using `make` at root.

## How To Play

## How It Works
As this game would probably not be a financial milestone and found in every kids Vectrex-Game arsenal in 2025 ;), 
you can use any [part of this code ](LICENSE) as you wish. Hopefully you can get some inspiration of how this 
game works (Probably not).
