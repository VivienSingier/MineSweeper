#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>

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

    int maxMine = ((sizeX * sizeY) * 16) / 100;

    for (int j = 0; j < sizeY; j++)
    {
        for (int i = 0; i < sizeX; i++)
        {
            int randomNum = rand() % 100;
            if (randomNum <= 16 && *mineCount < maxMine)
            {
                grid[j][i] = { i, j, true, false, false, 'X', CYAN }; 
                *mineCount = *mineCount + 1;
            }
            else
            {
                grid[j][i] = { i, j, false, false, false, 'X', CYAN }; 
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
    switch (count)
    {
    case 1:
        square->color = GREEN;
        break;
    case 2:
        square->color = YELLOW;
        break;
    case 3:
        square->color = RED;
        break;
    case 4:
    case 5:
        square->color = MAGENTA;
        break;
    case 6:
    case 7:
    case 8:
        square->color = BLUE;
        break;
    }

    
    return count;
}

void RevealSquare(Grid* grid, Square* square, int* ptr)
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
                *ptr = *ptr - 1;
            }
            else
            {
                *ptr = *ptr - 1;
                square->display = ' ';
                for (int j = square->y - 1; j <= square->y + 1; j++)
                {
                    for (int i = square->x - 1; i <= square->x + 1; i++)
                    {
                        if (0 <= i && i < grid->sizeX && 0 <= j && j < grid->sizeY)
                        {
                            if (!grid->array[j][i].isRevealed && !grid->array[j][i].isMarked)
                            {
                                RevealSquare(grid, &grid->array[j][i], ptr);
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
                square->isMarked = false;
                RevealSquare(grid, square, ptr);
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

void RevealAllMines(Grid* grid, Square* square)
{
    square->display = 'O';
    square->color = RED;
    square->isRevealed = true;

    int count = 0;
    for (int j = 0; j < grid->sizeY; j++)
    {
        for (int i = 0; i < grid->sizeX; i++)
        {
            while (count < 100000)
            {
                count++;
            }
            if (grid->array[j][i].isMine && !grid->array[j][i].isRevealed && count == 100000)
            {
                grid->array[j][i].display = 'O';
                grid->array[j][i].color = RED;
                grid->array[j][i].isRevealed = true;

                count = 0;

                DisplayGrid(grid->sizeX, grid->sizeY, grid);
            }
        }
    }
}

void SafeStart(Grid* grid, Square* square, int* ptr)
{
    for (int j = square->y - 1; j <= square->y + 1; j++)
    {
        for (int i = square->x - 1; i <= square->x + 1; i++)
        {
            if (0 <= i && i < grid->sizeX && 0 <= j && j < grid->sizeY)
            {
                if (grid->array[j][i].isMine)
                {
                    *ptr = *ptr + 1;
                }
                grid->array[j][i].isMine = false;
                grid->array[j][i].color = WHITE;
            }
        }
    }
}

void getChInput()
{
    char ch[3] = { 0, 0 };

    ch[0] = _getch();

    if ((int)ch[0] == -32)
    {
        ch[1] = _getch();

        switch ((int)ch[1])
        {
        case 72:
            // UP
            std::cout << "UP" << std::endl;
            break;
        case 80:
            // DOWN
            std::cout << "DOWN" << std::endl;
            break;
        case 77:
            // RIGHT
            std::cout << "RIGHT" << std::endl;
            break;
        case 75:
            // LEFT
            std::cout << "LEFT" << std::endl;
            break;
        }
    }
    else
    {
        switch ((int)ch[0])
        {
        case 114:
        case 82:
            // R
            std::cout << "R" << std::endl;
            break;
        case 102:
        case 70:
            // F
            std::cout << "F" << std::endl;
            break;
        }
    }
}

int main()
{
    srand(time(NULL));

    //int difficultySizes[3][2] = { {9, 9}, {16, 16}, {30, 16} };

    //bool wantsToPlayAgain = true;

    //while (wantsToPlayAgain)
    //{
    //    int difficulty = getPlayerInputInt("You may choose a difficulty (1 : EASY, 2 : MEDIUM, 3 : HARD) : ", 1, 3) - 1;
    //    int lineCount = difficultySizes[difficulty][1];
    //    int columnCount = difficultySizes[difficulty][0];

    //    int mineCount = 0;
    //    Grid grid = CreateGrid(columnCount, lineCount, &mineCount);
    //    std::cout << mineCount << std::endl;
    //    DisplayGrid(columnCount, lineCount, &grid);

    //    int emptyCellCount = lineCount * columnCount - mineCount;
    //    int* pEmptyCellCount = &emptyCellCount;
    //    bool isGameOver = false;

    //    int column = getPlayerInputInt("Please choose a column : ", 0, columnCount - 1);
    //    int line = getPlayerInputInt("Please choose a line : ", 0, lineCount - 1);
    //    char action = getPlayerInputLettre("Do you want to reveal this cell or add a flag ? (R : reveal, F : flag) : ", 'R', 'F');

    //    SafeStart(&grid, &grid.array[line][column], pEmptyCellCount);
    //    RevealSquare(&grid, &grid.array[line][column], pEmptyCellCount);
    //    DisplayGrid(columnCount, lineCount, &grid);

    //    while (emptyCellCount != 0 && !isGameOver)
    //    {
    //        int column = getPlayerInputInt("Please choose a column : ", 0, columnCount - 1);
    //        int line = getPlayerInputInt("Please choose a line : ", 0, lineCount - 1);
    //        char action = getPlayerInputLettre("Do you want to reveal this cell or add a flag ? (R : reveal, F : flag) : ", 'R', 'F');

    //        if (action == 'R' || action == 'r')
    //        {
    //            if (!IsMine(&grid.array[line][column]))
    //            {
    //                RevealSquare(&grid, &grid.array[line][column], pEmptyCellCount);
    //            }
    //            else
    //            {
    //                isGameOver = true;
    //            }
    //        }
    //        else
    //        {
    //            MarkSquare(&grid.array[line][column]);
    //        }
    //        if (isGameOver)
    //        {
    //            RevealAllMines(&grid, &grid.array[line][column]);
    //        }
    //        DisplayGrid(columnCount, lineCount, &grid);
    //    }

    //    if (isGameOver)
    //    {
    //        std::cout << "GAME OVER" << std::endl;
    //    }
    //    else
    //    {
    //        std::cout << "CONGRATULATION, YOU WON !!" << std::endl;
    //    }

    //    char retry = getPlayerInputLettre("Would you like to play again ? (Y : yes, N : no) : ", 'Y', 'N');
    //    if (retry == 'N' || retry == 'n')
    //    {
    //        wantsToPlayAgain = false;
    //    }

    //}

    while (true)
    {
        getChInput();
    }
}


