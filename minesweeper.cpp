// today add win condition, flag counter /15
#include <iostream>
#include <vector>
#include <random>
#include <string>

// struct definitions

struct Tile
{
    char displayval;
    char rowval;
    int colval;
    bool ismine;
    bool flagged;
    bool dug;
};

// function prototypes
void printgrid(std::vector<std::vector<Tile>> grid);
void printloss(std::vector<std::vector<Tile>> &grid);
void scattermines(std::vector<std::vector<Tile>> &grid, int row, int col);
int countmines(std::vector<std::vector<Tile>> &grid, int row, int col);
void flood(std::vector<std::vector<Tile>> &grid, int row, int col);
bool wincheckfunc(std::vector<std::vector<Tile>> grid);

int main(void)
{
    std::vector<std::vector<Tile>> grid(10, std::vector<Tile>(10));
    // defines an array of arrays of tiles (nested/2d arrays)


    for(int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            grid[i][j].displayval = '#';
            grid[i][j].rowval = static_cast<char>(65+i);
            grid[i][j].colval = j;
            grid[i][j].ismine = false;
            grid[i][j].flagged = false;
            grid[i][j].dug = false;
        }
    }
    // inputs all the values into the grid
    // grid[i][j] represents grid[row][column] where row is a char from A-J
    // for example: grid[0][0] is tile A0 and grid[7][3] is tile H3

    std::cout << "  ";
    for(int i = 0; i < 10; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
    // prints numbers 1-10

    for(int i = 0; i < 10; i++)
    {
        std::cout << static_cast<char>(65+i) << " ";
        for(int j = 0; j < 10; j++)
        {
            std::cout << grid[i][j].displayval << " ";
        }
        std::cout << "\n";
    }
    // prints map of mines from the 2d array

    bool minesscattered = false;
    int flagcount = 0;
    // initialize pre-loop variables to use in the loop

    while(true)
    {
        std::string input;
        char enteredinputrow;
        char enteredinputcol;
        int inputrow;
        int inputcol;
        int minesaround = 0;
        bool flaggingstate = false;
        // initializes variables

        std::cout << "What tile do you want to dig? (ex: A0) ";
        std::getline(std::cin, input);
        // saves the entire input into a string

        if(input.length() != 2 && input.length() != 5)
        {
            std::cout << "This isn't proper format. Write tiles in a row column format (ex: E7). They must always be 2 characters unless flagging\n";
            std::cout << "If flagging, you must write the tile, followed by a space, followed by '-f' (ex: E7 -f)\n";
            continue;
        }
        // check if there are any more or less characters than 2 in the input

        if(input.length() == 5)
        {
            if(input[2] == ' ' && input[3] == '-' && input[4] == 'f')
            {
                flaggingstate = true;
            }
            else
            {
                std::cout << "This isn't proper format. Write tiles in a row column format (ex: E7). They must always be 2 characters unless flagging\n";
                std::cout << "If flagging, you must write the tile, followed by a space, followed by '-f' (ex: E7 -f)\n";
                continue;
            }
        }

        enteredinputrow = input[0];
        enteredinputcol = input[1];
        // separates the string into 2 values, the row (first char) and the column (second char)

        if(!(static_cast<int>(enteredinputcol) >= 48 && static_cast<int>(enteredinputcol) <= 57) || !(static_cast<int>(enteredinputrow) >= 65 && static_cast<int>(enteredinputrow) <= 74))
        {
            std::cout << "This isn't a valid tile. Write tiles in a row column format (ex: E7), and make sure the tile exists\n";
            continue;
        }
        // checks if the first value is a proper letter, and second value is a proper number

        inputrow = static_cast<int>(enteredinputrow) - 65;
        inputcol = static_cast<int>(enteredinputcol) - 48;
        // converts the inputs (chars) into ints that i can use to call on that specific tile

        if(!minesscattered && !flaggingstate)
        {
            scattermines(grid, inputrow, inputcol);
            minesscattered = true;
        }

        if(grid[inputrow][inputcol].dug || (grid[inputrow][inputcol].flagged && flaggingstate == false))
        {
            std::cout << "You already dug or flagged this tile\n";
            continue;
        }
        // check for if they inputted a tile they already dug

        std::cout << "\033[2J\033[1;1H" << std::flush; // clears terminal window

        if(flaggingstate == true)
        {
            grid[inputrow][inputcol].flagged = !grid[inputrow][inputcol].flagged;
            if(grid[inputrow][inputcol].flagged)
            {
                flagcount++;
                if(flagcount > 15)
                {
                    grid[inputrow][inputcol].flagged = false;
                    flagcount--;
                }
            }
            else
            {
                flagcount--;
            }
            goto flagskip;
        }
        minesaround = countmines(grid, inputrow, inputcol);

        if(grid[inputrow][inputcol].displayval != 'M')
        {
            grid[inputrow][inputcol].displayval = minesaround + '0';
            grid[inputrow][inputcol].dug = true;
        } // if current tile is not a mine it updates the display value to be equal to minesaround (minesaround + '0' turns the int to a char)
        else
        {
            printloss(grid);
            printgrid(grid);
            std::cout << "You Lose!\n";
            break;
        } // if tile is a mine it ends the game
        
        flood(grid, inputrow, inputcol);

        flagskip:
        if(grid[inputrow][inputcol].flagged == true)
        {
            grid[inputrow][inputcol].displayval = 'F';
        }
        else if(grid[inputrow][inputcol].dug == false)
        {
            grid[inputrow][inputcol].displayval = '#';
        }

        printgrid(grid);
        std::cout << "Flags placed: " << flagcount << "/15\n";
        // make this have visual output (tile digged becomes D instead of #, ignore mines for now i just wanna get game loop done)
        // also turn the drawing of board into a function that can just randomly be called on to easier life in game loop

        if(wincheckfunc(grid))
        {
            std::cout << "You Win!\n";
            break;
        }
    }

    return 0;
}

// function definitions

void printgrid(std::vector<std::vector<Tile>> grid)
{
    std::cout << "  ";

    for(int i = 0; i < 10; i++)
    {
        std::cout << i << " ";
    }
    std::cout << "\n";
    // prints grid column numbers

    for(int i = 0; i < 10; i++)
    {
        std::cout << static_cast<char>(65+i) << " "; // letter for every new row (row value)
        for(int j = 0; j < 10; j++)
        {
            char displayval = grid[i][j].displayval;
            std::string colour;
            switch(displayval)
            {
                case '0': colour = "\033[90m"; break; // dark grey (bright black)
                case '1': colour = "\033[94m"; break; // bright blue
                case '2': colour = "\033[32m"; break; // green
                case '3': colour = "\033[91m"; break; // bright red
                case '4': colour = "\033[34m"; break; // dark blue
                case '5': colour = "\033[31m"; break; // dark red
                case '6': colour = "\033[36m"; break; // turquoise i think
                case '7': colour = "\033[35m"; break; // pink i think
                case '8': colour = "\033[37m"; break; // light grey (dark white)
                case 'M': colour = "\033[33m"; break; // yellow mines
                case 'F': colour = "\033[93m"; break; // bright yellow flags
                default: colour =  "\033[0m";  break; // basic terminal colour for everything else not listed
            }
            std::cout << colour << displayval << "\033[0m" << " ";
        }
        std::cout << "\n";
    }
    // prints grid itself
}

void printloss(std::vector<std::vector<Tile>> &grid)
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(grid[i][j].ismine == true)
            {
                grid[i][j].displayval = 'M';
            }
        }
    }
}

void scattermines(std::vector<std::vector<Tile>> &grid, int inputrow, int inputcol)
{
    std::random_device randomnum;
    std::mt19937 randgen(randomnum());
    std::uniform_int_distribution<> dist(0, 9);
    // sets up the random number generator to randomly scatter mines
    // range is 0-9 since thats the range of the grid (x and y)
    // call upon the random number generator with: dist(randgen)

    for(int minecount = 0; minecount < 15; minecount++)
    {
        int randrow = dist(randgen);
        int randcol = dist(randgen);
        bool canputmine = true;

        for(int i = -1; i < 2; i++)
        {
            for(int j = -1; j < 2; j++)
            {
                if(inputrow+i == randrow && inputcol+j == randcol)
                {
                    canputmine = false;
                }
            }
        }
        if(canputmine == true && !grid[randrow][randcol].ismine)
        {
            grid[randrow][randcol].ismine = true;
            std::cout << grid[randrow][randcol].rowval << grid[randrow][randcol].colval << "\n";
            // this std::cout prints all the tiles with mines
            // good for debugging but DELETE WHEN READY TO SHIP
            continue;
        }
        else
        {
            minecount--;
        }
    }
    // randomly scatters mines by generating 2 random values from 0-9, then putting a mine in the tile corresponding to rand1, rand2 
    // ex: (grid[row][col])
}

int countmines(std::vector<std::vector<Tile>> &grid, int row, int col)
{
    int minecount = 0;
    for(int i = -1; i < 2; i++)
    {
        if(grid[row][col].ismine)
        {
            grid[row][col].displayval = 'M';
            break;
        } // checks if the inputted tile is a mine, and sets the display value to M
        for(int j = -1; j < 2; j++)
        {
            if(i == 0 && j == 0)
            {
                continue; // repeats if on current tile
            }
            else if((row + i < 0) || (row + i > 9) || (col + j < 0) || (col + j > 9))
            {
                continue; // checks for edge cases where tile doesnt exist and skips over them so program doesnt break
            }
            else if(grid[row+i][col+j].ismine)
            {
                minecount++; // adds an extra mine to minecount if a surrounding tile is a mine
            }
        }
    }
    return minecount;
        // this entire nested loop checks around the current tile to see if there are any mines around it
        // it increments the variable "minecount" so it can accurately count # of mines around the tile
}

void flood(std::vector<std::vector<Tile>> &grid, int row, int col)
{
    int minecount = 0;
    if(grid[row][col].displayval == 48) // 48 means 0 but its the ascii code since displayval is a char
    {
        for(int i = -1; i < 2; i++)
        {
            for(int j = -1; j < 2; j++)
            {
                if(i == 0 && j == 0)
                {
                    continue;
                } // skips current tile
                else if(row+i < 0 || row + i > 9 || col + j < 0 || col + j > 9)
                {
                    continue;
                } // in case of out of bounds
                else if(!grid[row+i][col+j].dug && !grid[row+i][col+j].flagged)
                {
                    minecount = countmines(grid, row+i, col+j);
                    grid[row+i][col+j].displayval = minecount + '0'; // turns minecount into a char and saves it into displayval
                    grid[row+i][col+j].dug = true;
                    if(minecount == 0)
                    {
                        flood(grid, row+i, col+j);
                    }
                }
            }
        }
    }
    else
    {
        return;
    }
}

bool wincheckfunc(std::vector<std::vector<Tile>> grid)
{
    int dugcount = 0;
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(grid[i][j].dug)
            {
                dugcount++;
            }
        }
    }
    if(dugcount >= 85)
    {
        return true;
    }
    else
    {
        return false;
    }
}