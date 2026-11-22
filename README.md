# CLI Minesweeper

## What is this?
* CLI minesweeper is a simple minesweeper game for the command line
* It was written entirely in C++, and is also my first shippable project ever in C++
* The entire game is played through command line arguments

## Why did I make this?
* This was my first project in C++ which is an immense learning opportunity
    * I've used C before, so switching to C++ wasn't that bad
    * It was a bit trippy with the new syntax and the standard namespace stuff
    * Thankfully some stuff is pre-built and easier, such as vectors. I also don't have to deal with scanf() which is nice
* I also wanted to understand some concepts/structures better, including:
    * 2D arrays (which I used to set up the grid)
    * Depth first search (used in the flood function)
    * ANSI escape codes (used to colour the text)
* Also I kind of just like minesweeper and wanted to make my own version

## How to play
### For yalls that havent played minesweeper before, here's how:
* You start by digging any tile, and a bunch of tiles will open up, some with numbers (above 0)
* The number represents the amount of mines around that tile (including diagonals)
    * For example, if a tile has a 2 around it, it means that 2 tiles around it are mines
    * You have to figure out which tiles around it are mines (in the above example, which 2 tiles are mines)
* If you know that a certain tile is a mine, you can flag it
    * This disables you from digging that tile, and also acts as a visual indicator for where a mine is
    * If you do get a flag wrong, it can mess you up really bad
* If you dig a mine, you lose (this is your lose condition)
* If you dig all tiles that aren't mines, you win! (this is your win condition)
### Now for how to navigate this command line version :DD
* This game works in a 10x10 grid, with rows labeled A-J and columns labeled from 0-9
* To dig a tile, you just gotta input that tile as a command line argument
    * The tile must be inputed with the row letter followed by the column number and nothing else, no spaces
    * For example: E4, A9, I6, C0
* To flag a tile, you write the tile the same way as digging, but follow it with a space and -f, all in the same line
    * For example: A2 -f, G5 -f
* Like stated before, dig all non-mine tiles and you win

## Extra notes
* This was cool to make honestly
* A highlight was when I figured out ANSI colour codes and accidentally made my terminal blue, that was really funny
* I'm also super proud of myself for implementing recirsive DFS because that was super intimidating

## Images
* Insert these through github, its easier

# Have fun!

    