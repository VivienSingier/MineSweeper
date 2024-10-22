#include <iostream>
#include <cstdlib>

#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

struct Square {
    int x;
    int y;
    bool isMine;
    bool isRevealed;
    bool isMarked;
    char display;
    const char* color;
};

struct Grid
{
    Square** array;
    int sizeX;
    int sizeY;
};

char getPlayerInputLettre(const char* str, const char c1, const char c2)
{
    char input;
    std::cout << str;
    std::cin >> input;
    while ((int)input != (int)(c1) && (int)input != (int)(c1)+32 && (int)input != (int)(c2) && (int)input != (int)(c2)+32)
    {
        std::cout << "INVALID INPUT, please try again" << std::endl;
        std::cout << str;
        std::cin >> input;
    }

    return input;
}

int getPlayerInputInt(const char* str, int i1, int i2)
{
    int input;
    std::cout << str;
    std::cin >> input;
    while (input < i1 || input > i2)
    {
        std::cout << "INVALID INPUT, please try again" << std::endl;
        std::cout << str;
        std::cin >> input;
    }
    return input;
}

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
                grid[j][i] = { i, j, true, false, false, 'O', RED };
                *mineCount = *mineCount + 1;
            }
            else
            {
                grid[j][i] = { i, j, false, false, false, 'X', WHITE };
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
    system("cls");

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
            std::cout << "| " << grid->array[i][j].color << grid->array[i][j].display << WHITE << " ";
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

void RevealSquare(Grid* grid, Square* square)
{
    if (!square->isMine)
    {
        if (square->isRevealed)
        {
            std::cout << "This CELL is already REVEALED" << std::endl;
        }

        else if (!square->isMarked)
        {
            square->isRevealed = true;

            int mineCount = GetAdjacentMinesCount(grid, square);
            if (mineCount > 0)
            {
                square->display = (char)(mineCount + 48);
                square->isRevealed = true;
            }
            else
            {
                square->display = ' ';
                for (int j = square->y - 1; j <= square->y + 1; j++)
                {
                    for (int i = square->x - 1; i <= square->x + 1; i++)
                    {
                        if (0 <= i && i < grid->sizeX && 0 <= j && j < grid->sizeY)
                        {
                            if (!grid->array[j][i].isRevealed && !grid->array[j][i].isMarked)
                            {
                                RevealSquare(grid, &grid->array[j][i]);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            char answer = getPlayerInputLettre("CAREFULL, this cell is marked, are you sure you want to reveal it ? (Y : yes, N : no) : ", 'Y', 'N');
            if (answer == 'Y' || answer == 'y')
            {
                square->display = ' ';
                square->isMarked = false;
                square->isRevealed = true;
            }
        }
    }
}

void MarkSquare(Square* square)
{
    square->isMarked = true;
    square->color = RED;
    square->display = 'F';
}

bool IsMine(Square* square)
{
    if (square->isMine)
        return true;
    return false;
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

int main()
{
    srand(time(NULL));

    int difficultySizes[3][2] = { {9, 9}, {16, 16}, {30, 16} };
    int difficulty = getDifficulty();
    int columnCount = difficultySizes[difficulty][1];
    int lineCount = difficultySizes[difficulty][0];

    int mineCount = 0;
    Grid grid = CreateGrid( lineCount , columnCount , &mineCount );
    std::cout << mineCount << std::endl;
    DisplayGrid( lineCount , columnCount, &grid );

    int emptyCellCount = columnCount * lineCount - mineCount;
    bool isGameOver = false;

    while (emptyCellCount != 0 && !isGameOver)
    {
        int column = getPlayerInputInt("Please choose a column : ", 0, columnCount);
        int line = getPlayerInputInt("Please choose a line : ", 0, lineCount);
        char action = getPlayerInputLettre("Do you want to reveal this cell or add a flag ? (R : reveal, F : flag) : ", 'R', 'F');

        if (action == 'R' || action == 'r')
        {
            if (!IsMine(&grid.array[line][column]))
            {
                RevealSquare(&grid, &grid.array[line][column]);
            }
            else
            {
                isGameOver = true;
            }
        }
        else
        {
            MarkSquare(&grid.array[line][column]);
        }
        DisplayGrid(lineCount, columnCount, &grid);
    }
}


