#include <iostream>
#include <cstdlib>

struct Square {
    int x;
    int y;
    bool isMine;
    bool  isRevealed;
    bool isSuspected;
    char display;
};

struct Grid
{
    Square** array;
    int sizeX;
    int sizeY;
};

Grid CreateGrid(int sizeX, int sizeY)
{
    Square** grid = (Square**)malloc(sizeY * sizeof(Square*));
    for (int i = 0; i < sizeY; i++)
    {
        grid[i] = (Square*)malloc(sizeX * sizeof(Square));
    }

    int maxMine = ((sizeX * sizeY) * 15) / 100;
    int mineCount = 0;

    for (int j = 0; j < sizeY; j++)
    {
        for (int i = 0; i < sizeX; i++)
        {
            int randomNum = rand() % 100;
            if (randomNum <= 15 && mineCount < maxMine)
            {
                grid[j][i] = { i, j, true, false, false, 'X' };
                mineCount++;
            }
            else
            {
                grid[j][i] = { i, j, false, false, false, 'X' };
            }
        }
    }

    return { grid, sizeX, sizeY };
}

void DisplayLine(int sizeX)
{
    std::cout << "    ";
    for (int i = 0; i < sizeX; i++)
    {
        std::cout << "+---";
    }
    std::cout << "+" << std::endl;
}

void DisplayGrid(int sizeX, int sizeY, Grid* grid)
{
    std::cout << "      ";
    for (int i = 0; i < sizeX; i++)
    {
        if (i < 10)
            std::cout << i << "   ";
        else
            std::cout << i << "  ";
    }
    std::cout << std::endl;
    DisplayLine(sizeX);

    for (int i = 0; i < sizeY; i++)
    {
        if (i < 10)
            std::cout << "  " << i << " ";
        else
            std::cout << " " << i << " ";

        for (int j = 0; j < sizeX; j++)
        {
            std::cout << "| " << grid->array[i][j].display << " ";
        }
        std::cout << "|" << std::endl;
        DisplayLine(sizeX);
    }
}

int GetAdjacentMinesCount(Grid* grid, Square* square)
{
    int count = 0;
    for (int j = square->y - 1; j <= square->y + 1; j++)
    {
        for (int i = square->x - 1; i <= square->x + 1; i++)
        { 
            if (0 <= i && i < grid->sizeX && 0 <= j && j < grid->sizeY)
            {
                if (grid->array[j][i].isMine)
                {
                    count++;
                }
            }
        }
    }
    return count;
}

bool RevealSquare(Grid* grid, Square* square)
{
    if (!square->isMine)
    {
        square->isRevealed = true;

        int mineCount = GetAdjacentMinesCount(grid, square);
        if (mineCount > 0)
        {
            square->display = (char)(mineCount + 48);
        }
        else
        {
            square->display = ' ' ;
            for (int j = square->y - 1; j <= square->y + 1; j++)
            {
                for (int i = square->x - 1; i <= square->x + 1; i++)
                {
                    if (0 <= i && i < grid->sizeX && 0 <= j && j < grid->sizeY)
                    {                    
                        if (!grid->array[j][i].isRevealed)
                        {
                            RevealSquare(grid, &grid->array[j][i]);
                        }
                    }
                }
            }
        }

        
        return false;
    }
    return true;
}

int getDifficulty()
{
    std::cout << "You may choose your difficulty (1: EASY, 2: MEDIUM, 3: HARD) : ";
    int difficulty;
    std::cin >> difficulty;
    return difficulty - 1;
}

int main()
{
    srand(time(NULL));

    int difficultySizes[3][2] = { {9, 9}, {16, 16}, {32, 16} };
    Grid grid = CreateGrid(30, 16);
    DisplayGrid(30, 16, &grid);
    RevealSquare(&grid, &grid.array[5][5]);
    DisplayGrid(30, 16, &grid);
}

