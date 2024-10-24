#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

#define BLACK 0
#define RED 4
#define GREEN 2
#define YELLOW 6
#define BLUE 1
#define MAGENTA 5
#define CYAN 3
#define WHITE 15
#define LIGHTBLUE 9
#define LIGHTGREEN 10

const char* gameOverTxt[8] =
{
    "+----------------------------------------------------------------------+",
    "|  _______ _______ _______ _______    _______ _     _ _______ ______   |",
    "| (_______|_______|_______|_______)  (_______|_)   (_|_______|_____ \\  |",
    "|  _   ___ _______ _  _  _ _____      _     _ _     _ _____   _____) ) |",
    "| | | (_  |  ___  | ||_|| |  ___)    | |   | | |   | |  ___) |  __  /  |",
    "| | |___) | |   | | |   | | |_____   | |___| |\\ \\ / /| |_____| |  \\ \\  |",
    "|  \\_____/|_|   |_|_|   |_|_______)   \\_____/  \\___/ |_______)_|   |_| |",
    "+----------------------------------------------------------------------+"
};

const char* youWonTxt[8] =
{
    "+------------------------------------------------------+",
    "|  _     _ _______ _     _    _  _  _ _______ _______  |",
    "| | |   | (_______|_)   (_)  (_)(_)(_|_______|_______) |",
    "| | |___| |_     _ _     _    _  _  _ _     _ _     _  |",
    "| |_____  | |   | | |   | |  | || || | |   | | |   | | |",
    "|  _____| | |___| | |___| |  | || || | |___| | |   | | |",
    "| (_______|\\_____/ \\_____/    \\_____/ \\_____/|_|   |_| |",
    "+------------------------------------------------------+"
};

const char* mineSweeperTxt[8] =
{
"+------------------------------------------------------------------------------------+",
"|  _______ _ _______ _______  ______ _  _  _ _______ _______ ______ _______ ______   |",
"| (_______) (_______|_______)/ _____|_)(_)(_|_______|_______|_____ (_______|_____ \\  |",
"|  _  _  _| |_     _ _____  ( (____  _  _  _ _____   _____   _____) )____   _____) ) |",
"| | ||_|| | | |   | |  ___)  \\____ \\| || || |  ___) |  ___) |  ____/  ___) |  __  /  |",
"| | |   | | | |   | | |______ ____) ) || || | |_____| |_____| |    | |_____| |  \\ \\  |",
"| |_|   |_|_|_|   |_| _______|_____/ \\_____/|_______)_______)_|    |_______)_|   |_| |",
"+------------------------------------------------------------------------------------+"
};

struct Square {
    int x;
    int y;
    bool isMine;
    bool isRevealed;
    bool isMarked;
    char display;
    int color;
    bool isHighlighted;
};

struct Grid
{
    Square** array;
    int sizeX;
    int sizeY;
};

char getPlayerInputLettre(const char* str, const char c1, const char c2)
{
    char input = ' ';
    bool isCorrectChar = ((int)input != (int)(c1) && (int)input != (int)(c1)+32 && (int)input != (int)(c2) && (int)input != (int)(c2)+32);
    std::cout << str;
    std::cin >> input;

    while (!isCorrectChar || std::cin.peek() != '\n')
    // Si l'entrée contient plus d'un char ou il n'est parmis les choix indiqués
    {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        
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
    //  Si l'entrée n'est pas parmis les choix indiqué 
    {
        if (std::cin.fail())
        // Si l'entrée n'est pas un entier
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
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

    int maxMine = ((sizeX * sizeY) * 17) / 100;

    for (int j = 0; j < sizeY; j++)
    {
        for (int i = 0; i < sizeX; i++)
        {
            int randomNum = rand() % 100;
            if (randomNum <= 15 && *mineCount < maxMine)
            {
                grid[j][i] = { i, j, true, false, false, 'X', CYAN, false }; 
                *mineCount = *mineCount + 1;
            }
            else
            {
                grid[j][i] = { i, j, false, false, false, 'X', CYAN, false }; 
            }
        }
    }

    return { grid, sizeX, sizeY };
}

void SetColor(int color, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,(bgColor << 4) | color);
}

void ResetColor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (0 << 4) | 7);
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

void DisplayCoord(int column, int line, int difficulty)
{
    int offSetArr[3] = {18, 32, 60};
    int offSet;
    const char* line1;
    const char* line2;
    if (column > 9)
    {
        offSet = offSetArr[difficulty] - 1;
        line1 = "+----";
    }
    else
    {
        offSet = offSetArr[difficulty];
        line1 = "+---";
    }
    if (line > 9)
    {
        line2 = "+----+";
    }
    else
    {
        line2 = "+---+";
    }

    for (int i = 0; i < offSet; i++)
        std::cout << ' ';
    std::cout << line1;
    std::cout << line2 << std::endl;
    for (int i = 0; i < offSet; i++)
        std::cout << ' ';
    std::cout << "| ";
    SetColor(LIGHTBLUE, BLACK);
    std::cout << column;
    ResetColor();
    std::cout << " | ";
    SetColor(LIGHTGREEN, BLACK);
    std::cout << line;
    ResetColor();
    std::cout << " |" << std::endl;
    for (int i = 0; i < offSet; i++)
        std::cout << ' ';
    std::cout << line1;
    std::cout << line2 << std::endl;
}

void DisplayGrid(int sizeX, int sizeY, Grid* grid, int column, int line, int difficulty)
{
    system("cls");

    // Ligne des coordonnées X 
    std::cout << "      ";
    SetColor(LIGHTBLUE, BLACK);
    for (int i = 0; i < sizeX; i++)
    {
        if (i < 10)
            std::cout << i << "   ";
        else
            std::cout << i << "  ";
    }
    ResetColor();
    std::cout << std::endl;
    DisplayLine(sizeX);

    for (int i = 0; i < sizeY; i++)
    {
        //  Ligne des Coordonnées Y
        SetColor(LIGHTGREEN, BLACK);
        if (i < 10)
            std::cout << "  " << i << " ";
        else
            std::cout << " " << i << " ";
        ResetColor();

        for (int j = 0; j < sizeX; j++)
        {
            std::cout << "| ";
            if (!grid->array[i][j].isHighlighted)
            {
                SetColor(grid->array[i][j].color, BLACK);
                std::cout << grid->array[i][j].display;
                ResetColor();
            }
            else
            {
                SetColor(BLACK, grid->array[i][j].color);
                std::cout << grid->array[i][j].display;
                ResetColor();
            }
            std::cout << " ";
        }
        std::cout << "|" << std::endl;
        DisplayLine(sizeX);
    }
    DisplayCoord(column, line, difficulty);
}

void DisplayAsciiArt(const char** text, int length, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < length; j++)
        {
            std::cout << text[i][j];
        }
        std::cout << std::endl;
    }
}

void HighlightCursor(Grid* grid, int line, int column)
{
    grid->array[line][column].isHighlighted = true;
}

void removeHighlight(Grid* grid, int line, int column)
{
    grid->array[line][column].isHighlighted = false;
}

void ChooseColor(Square* square, int adjacentMinesCount)
{
    switch (adjacentMinesCount)
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

    ChooseColor(square, count);

    return count;
}

void RevealSquare(Grid* grid, Square* square, int* emptyCellCount, bool* isGameOver)
{
    if (!square->isMarked && !square->isRevealed && !square->isMine)
    {
        square->isRevealed = true;

        int mineCount = GetAdjacentMinesCount(grid, square);
        if (mineCount > 0)
        {
            // La cellule devient vide
            square->display = (char)(mineCount + 48);
            square->isRevealed = true;
            *emptyCellCount = *emptyCellCount - 1;
        }
        else
        {
            *emptyCellCount = *emptyCellCount - 1;
            square->display = ' ';
            for (int j = square->y - 1; j <= square->y + 1; j++)
            {
                for (int i = square->x - 1; i <= square->x + 1; i++)
                {
                    if (0 <= i && i < grid->sizeX && 0 <= j && j < grid->sizeY && !grid->array[j][i].isRevealed && !grid->array[j][i].isMarked)
                    {
                        // Appel récursif
                        RevealSquare(grid, &grid->array[j][i], emptyCellCount, isGameOver);
                    }
                }
            }
        }
    }
    else if (square->isMine)
    {
        *isGameOver = true;
    }
    else if (square->isMarked)
    {
        char answer = getPlayerInputLettre("CAREFULL, this cell is marked, are you sure you want to reveal it ? (Y : yes, N : no) : ", 'Y', 'N');
        if (answer == 'Y' || answer == 'y')
        {
            if (square->isMine)
            {
                *isGameOver = true;
            }
            square->isMarked = false;
            RevealSquare(grid, square, emptyCellCount, isGameOver);
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
    return square->isMine;
}

void RevealAllMines(Grid* grid, Square* square, int difficulty)
{
    // On révèle la mine selectionnée par le joueur en premier
    square->display = 'O';
    square->color = RED;
    square->isRevealed = true;

    DisplayGrid(grid->sizeX, grid->sizeY, grid, square->x, square->y, difficulty);

    for (int j = 0; j < grid->sizeY; j++)
    {
        for (int i = 0; i < grid->sizeX; i++)
        {
            if (grid->array[j][i].isMine && !grid->array[j][i].isRevealed)
            {
                Sleep(500); // Délai de 0.5s
                grid->array[j][i].display = 'O';
                grid->array[j][i].color = RED;
                grid->array[j][i].isRevealed = true;

                DisplayGrid(grid->sizeX, grid->sizeY, grid, i, j, difficulty);
            }
        }
    }
}

void SafeStart(Grid* grid, Square* square, int* emptyCellCount)
{
    // On remplace la case selectionné ainsi que ses cases adjacentes par des case vides
    for (int j = square->y - 1; j <= square->y + 1; j++)
    {
        for (int i = square->x - 1; i <= square->x + 1; i++)
        {
            if (0 <= i && i < grid->sizeX && 0 <= j && j < grid->sizeY)
            {
                if (grid->array[j][i].isMine)
                {
                    *emptyCellCount = *emptyCellCount + 1;
                }
                grid->array[j][i].isMine = false;
                grid->array[j][i].color = CYAN;
            }
        }
    }
}

void playerActions(Grid* grid, int* tryCount, int* line, int* column, int* emptyCellCount, bool* isGameOver, int difficulty)
{
    char ch[3] = { 0, 0 };

    // On récupère l'entrée du joueur
    ch[0] = _getch();

    if ((int)ch[0] == -32)
    // Si le premier code renvoyée correspond aux touches fonctionnelles
    {
        ch[1] = _getch();

        switch ((int)ch[1])
        {
        case 72:
            // UP
            if (*line > 0)
            {
                removeHighlight(grid, *line, *column);
                *line = *line - 1;
                HighlightCursor(grid, *line, *column);
            }
            break;
        case 80:
            // DOWN
            if (*line < grid->sizeY-1)
            {
                removeHighlight(grid, *line, *column);
                *line = *line + 1;
                HighlightCursor(grid, *line, *column);
            }
            break;
        case 77:
            // RIGHT
            if (*column < grid->sizeX - 1)
            {
                removeHighlight(grid, *line, *column);
                *column = *column + 1;
                HighlightCursor(grid, *line, *column);
            }
            break;
        case 75:
            // LEFT
            if (*column > 0)
            {
                removeHighlight(grid, *line, *column);
                *column = *column - 1;
                HighlightCursor(grid, *line, *column);
            }
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
            if (*tryCount == 0)
            // Si il s'agit du premier essaie du joueur
            {
                SafeStart(grid, &grid->array[*line][*column], emptyCellCount);
                RevealSquare(grid, &grid->array[*line][*column], emptyCellCount, isGameOver);
            }
            else
            {
                RevealSquare(grid, &grid->array[*line][*column], emptyCellCount, isGameOver);
            }
            *tryCount = *tryCount + 1;
            break;
        case 102:
        case 70:
            // F
            MarkSquare(&grid->array[*line][*column]);
            break;
        }
    }
    DisplayGrid(grid->sizeX, grid->sizeY, grid, *column, *line, difficulty);
}

int main()
{
    srand(time(NULL));

    int difficultySizes[3][2] = { {9, 9}, {16, 16}, {30, 16} };

    bool wantsToPlayAgain = true;

    DisplayAsciiArt(mineSweeperTxt, 87, 8);
    for (int i = 0; i < 10; i++)
    {
        std::cout << std::endl;
    }
    std::cout << "                              PRESS ANY KEY TO  START";
    _getch();

    while (wantsToPlayAgain)
    {
        system("cls");
        int difficulty = getPlayerInputInt(" You may choose a difficulty (1 : EASY, 2 : MEDIUM, 3 : HARD) : ", 1, 3) - 1;
        int lineCount = difficultySizes[difficulty][1];
        int columnCount = difficultySizes[difficulty][0];

        int mineCount = 0;
        Grid grid = CreateGrid(columnCount, lineCount, &mineCount);
        std::cout << mineCount << std::endl;

        int emptyCellCount = lineCount * columnCount - mineCount;
        int* pEmptyCellCount = &emptyCellCount;
        bool isGameOver = false;
        int column = columnCount / 2;
        int line = lineCount / 2;
        int tryCount = 0;

        HighlightCursor(&grid, line, column);
        DisplayGrid(columnCount, lineCount, &grid, column, line, difficulty);
        playerActions(&grid, &tryCount, &line, &column, &emptyCellCount, &isGameOver, difficulty);

        while (emptyCellCount != 0 && !isGameOver)
        {
            playerActions(&grid, &tryCount, &line, &column, &emptyCellCount, &isGameOver, difficulty);
            if (isGameOver)
            {
                RevealAllMines(&grid, &grid.array[line][column], difficulty);
            }
        }

        if (isGameOver)
        {
            system("cls");
            SetColor(RED, BLACK);
            DisplayAsciiArt(gameOverTxt, 73, 8);
            ResetColor();
        }
        else
        {
            system("cls");
            SetColor(GREEN, BLACK);
            DisplayAsciiArt(youWonTxt, 57, 8);
            ResetColor();
        }

        for (int i = 0; i < 10; i++)
        {
            std::cout << std::endl;
        }

        char retry = getPlayerInputLettre(" Would you like to play again ? (Y : yes, N : no) : ", 'Y', 'N');
        if (retry == 'N' || retry == 'n')
        {
            wantsToPlayAgain = false;
        }

    }
}


