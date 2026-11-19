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

int main(void)
{
    std::vector<std::vector<Tile>> grid(10, std::vector<Tile>(10));
    // defines an array of arrays of tiles (nested/2d arrays)

    std::random_device randomnum;
    std::mt19937 randgen(randomnum());
    std::uniform_int_distribution<> dist(0, 9);
    // creates a random number generator
    // now to generate a random number from 0-9, call dist(randgen)


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


    for(int minecount = 0; minecount < 15; minecount++)
    {
        int row = dist(randgen);
        int col = dist(randgen);

        if(grid[row][col].ismine == false)
        {
            grid[row][col].ismine = true;
            std::cout << grid[row][col].rowval << grid[row][col].colval << "\n";
            continue;
        }
        else
        {
            minecount--;
        }
    }
    // randomly scatters mines by generating 2 random values from 0-9, then putting a mine in the tile corresponding to rand1, rand2 
    // ex: (grid[row][col])
    // maybe move this part after the first click so user doesnt die first try

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

    while(true)
    {
        std::string input;
        char enteredinputrow;
        char enteredinputcol;
        int inputrow;
        int inputcol;
        int minesaround = 0;

        std::cout << "What tile do you want to dig? (ex: A0) ";
        std::cin >> input;

        if(input.length() != 2)
        {
            std::cout << "This isn't proper format. Write tiles in a row column format (ex: E7). They must always be 2 characters";
            continue;
        }
        // check if there are any more or less characters than 2 in the input

        enteredinputrow = input[0];
        enteredinputcol = input[1];

        if(!(static_cast<int>(enteredinputcol) >= 48 && static_cast<int>(enteredinputcol) <= 57) || !(static_cast<int>(enteredinputrow) >= 65 && static_cast<int>(enteredinputrow) <= 74))
        {
            std::cout << "This isn't a valid tile. Write tiles in a row column format (ex: E7), and make sure the tile exists\n";
            continue;
        }

        inputrow = static_cast<int>(enteredinputrow) - 65;
        inputcol = static_cast<int>(enteredinputcol) - 48;

        if(grid[inputrow][inputcol].dug)
        {
            std::cout << "You already dug this tile\n";
            continue;
        }

        std::cout << "\033[2J\033[1;1H" << std::flush; // clears terminal window


        for(int i = -1; i < 2; i++)
        {
            if(grid[inputrow][inputcol].ismine)
            {
                grid[inputrow][inputcol].displayval = 'M';
                break;
            }
            for(int j = -1; j < 2; j++)
            {
                if(i == 0 && j == 0)
                {
                    continue;
                }
                else if((inputrow + i < 0) || (inputrow + i > 9) || (inputcol + j < 0) || (inputcol + j > 9))
                {
                    continue;
                }
                else if(grid[inputrow+i][inputcol+j].ismine)
                {
                    minesaround++;
                }
            }
        }

        if(grid[inputrow][inputcol].displayval != 'M')
        {
            grid[inputrow][inputcol].displayval = minesaround + '0';
            grid[inputrow][inputcol].dug = true;
        }
        else
        {
            printgrid(grid);
            std::cout << "You Lose!\n";
            break;
        }
        printgrid(grid);
        std::cout << grid[inputrow][inputcol].rowval << grid[inputrow][inputcol].colval << "\n";
        // make this have visual output (tile digged becomes D instead of #, ignore mines for now i just wanna get game loop done)
        // also turn the drawing of board into a function that can just randomly be called on to easier life in game loop
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

    for(int i = 0; i < 10; i++)
    {
        std::cout << static_cast<char>(65+i) << " ";
        for(int j = 0; j < 10; j++)
        {
            std::cout << grid[i][j].displayval << " ";
        }
        std::cout << "\n";
    }
}