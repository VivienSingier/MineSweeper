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

Grid CreateGrid(int sizeX, int sizeY, int* mineCount)
{
    Square** grid = (Square**)malloc(sizeY * sizeof(Square*));
    for (int i = 0; i < sizeY; i++)
    {
        grid[i] = (Square*)malloc(sizeX * sizeof(Square));
    }

    int maxMine = ((sizeX * sizeY) * 15) / 100;

    for (int j = 0; j < sizeY; j++)
    {
        for (int i = 0; i < sizeX; i++)
        {
            int randomNum = rand() % 100;
            if (randomNum <= 15 && *mineCount < maxMine)
            {
                grid[j][i] = { i, j, true, false, false, ' ' };
                *mineCount = *mineCount + 1;
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

void SafeStart(Grid* grid, Square* square)
{
    for (int j = square->y - 1; j <= square->y + 1; j++)
    {
        for (int i = square->x - 1; i <= square->x + 1; i++)
        {
            if (0 <= i && i < grid->sizeX && 0 <= j && j < grid->sizeY)
            {
                grid->array[j][i].isMine = false;
            }
        }
    }
}

int getDifficulty()
{
    std::cout << "You may choose your difficulty (1: EASY, 2: MEDIUM, 3: HARD) : ";
    int difficulty;
    std::cin >> difficulty;
    return difficulty - 1;
}

int* getPlayerSquare()
{
    std::cout << "Please select a line : ";
    int line;
    std::cin >> line;
    std::cout << "Please select a column : ";
    int column;
    std::cin >> column;
    int coord[2] = { line, column };

    return coord;
}

int main()
{
    srand(time(NULL));

    int difficultySizes[3][2] = { {9, 9}, {16, 16}, {30, 16} };
    
    int difficulty = getDifficulty();

    int mineCount = 0;
    Grid grid = CreateGrid(difficultySizes[difficulty][0], difficultySizes[difficulty][1], &mineCount);
    std::cout << mineCount << std::endl;
    DisplayGrid(difficultySizes[difficulty][0], difficultySizes[difficulty][1], &grid);


}

