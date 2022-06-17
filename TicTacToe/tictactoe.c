#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define ROWS 3
#define COLUMNS 3

//-------------------------------------------------

void freeGrid(char **grid)
{
    for(int i = 0 ; i < ROWS ; i++) free(grid[i]);
    free(grid);
    return;
}

void printBoard(char **board)
{
    for(int i = 0 ; i < ROWS ; i++)
    {
        for(int j = 0 ; j < COLUMNS ; j++) printf("%c " , board[i][j]);
        printf("\n");
    }
}

int checkWinningMove(char **board , int turn)
{
    int count;
    char target = turn == 1 ? 'X' : 'O';

    for(int i = 0 ; i < ROWS ; i++)
    {
        count = 0;
        for(int j = 0 ; j < COLUMNS ; j++)
        {
            if(board[i][j] == target) count += 1;
        }
        if(count == 3) return 1;
    }

    for(int j = 0 ; j < COLUMNS ; j++)
    {
        count = 0;
        for(int i = 0 ; i < ROWS ; i++)
        {
            if(board[i][j] == target) count += 1;
        }
        if(count == 3) return 1;
    }

    if(board[0][0] == target && board[1][1] == target && board[2][2] == target) return 1;
    if(board[0][2] == target && board[1][1] == target && board[2][0] == target) return 1;

    return 0;
}

//------------------------------------------------

struct pair
{
    int first;
    int second;
};

int getBestMoveUtil(char **board , int moveCount , int turn)
{
    if(moveCount == 9)
    {
        if(checkWinningMove(board , 1) == 1) return +1;
        if(checkWinningMove(board , 2) == 1) return -1;
        return 0;
    }

    if(checkWinningMove(board , turn == 1 ? 2 : 1) == 1)
    {
        return (turn == 1 ? -1 : +1);
    }

    if(turn == 1)
    {
        int mx = INT_MIN;
        int tmp;
        for(int i = 0 ; i < ROWS ; i++)
        {
            for(int j = 0 ; j < COLUMNS ; j++)
            {
                if(board[i][j] != '-')
                {
                    continue;
                }
                board[i][j] = 'X';

                tmp = getBestMoveUtil(board , moveCount + 1 , turn == 1 ? 2 : 1);
                mx = mx >= tmp ? mx : tmp;

                board[i][j] = '-';
            }
        }
        return mx;
    }
    else if(turn == 2)
    {
        int mn = INT_MAX;
        int tmp;
        for(int i = 0 ; i < ROWS ; i++)
        {
            for(int j = 0 ; j < COLUMNS ; j++)
            {
                if(board[i][j] != '-')
                {
                    continue;
                }
                board[i][j] = 'O';

                tmp = getBestMoveUtil(board , moveCount + 1 , turn == 1 ? 2 : 1);
                mn = mn <= tmp ? mn : tmp;

                board[i][j] = '-';
            }
        }

        return mn;
    }
    return 0;
}

struct pair getBestMove(char **board , int moveCount)
{
    struct pair bestMove;
    bestMove.first = -1;
    bestMove.second = -1;

    int min = INT_MAX;
    int tmp;
    for(int i = 0 ; i < ROWS ; i++)
    {
        for(int j = 0 ; j < COLUMNS ; j++)
        {
            if(board[i][j] != '-')
            {
                continue;
            }
            board[i][j] = 'O';

            tmp = getBestMoveUtil(board , moveCount + 1 , 1);
            if(min > tmp)
            {
                min = tmp;
                bestMove.first = i;
                bestMove.second = j;
            }

            board[i][j] = '-';
        }
    }
    return bestMove;
}

//-------------------------------------------------

int main(int argc , char **argv , char **envp)
{
    char **board = (char**)(malloc(sizeof(char*) * ROWS));
    for(int i = 0 ; i < ROWS ; i++)
    {
        board[i] = (char*)(malloc(sizeof(char) * COLUMNS));
        for(int j = 0 ; j < COLUMNS ; j++) board[i][j] = '-';
    }

    int moveCount = 0;
    int turn = 1;
    int x , y;

    int gameMode = 0;
    printf("This Game can be played in Single player and Two Player Mode\n");
    printf("Press 1 : Single Player Mode\n");
    printf("Press 2 : Two Player Mode\n");
    printf("Press -1 -1 when the game has started : To exit the Game\n");
    
    while(gameMode != 1 && gameMode != 2)
    {
        printf("Enter Game Mode : ");
        scanf("%d" , &gameMode);
    }

    while(1)
    {
        printBoard(board);
        if(moveCount == 9)
        {
            printf("GAME OVER : It is Draw\n");
            break;
        }

        if(turn == 1)
        {
            printf("It is Player X Turn now : ");
            scanf("%d %d" , &x , &y);

            if(x == -1 && y == -1)
            {
                printf("GAME OVER : Game Exited\n");
                break;
            }
            if(x < 1 || x > 3 || y < 1 || y > 3)
            {
                printf("ERROR : Enter Valid Input\n");
                continue;
            }
            if(board[x - 1][y - 1] != '-')
            {
                printf("ERROR : Enter the Coordinates of an Empty Spot\n");
                continue;
            }

            board[x - 1][y - 1] = 'X';
            moveCount += 1;

            if(checkWinningMove(board , turn) == 1)
            {
                printf("GAME OVER : Player X Won the Game\n");
                printBoard(board);
                break;
            }

            turn = turn == 1 ? 2 : 1;
        }
        else if(turn == 2)
        {
            printf("It is Player O Turn now : ");
            if(gameMode == 1)
            {
                struct pair bestMove = getBestMove(board , moveCount);
                x = bestMove.first + 1;
                y = bestMove.second + 1;
                printf("%d %d\n" , x , y);
            }
            else if(gameMode == 2)
            {
                scanf("%d %d" , &x , &y);
            }

            if(x == -1 && y == -1)
            {
                printf("GAME OVER : Game Exited\n");
            }
            if(x < 1 || x > 3 || y < 1 || y > 3)
            {
                printf("ERROR : Enter Valid Input\n");
                continue;
            }
            if(board[x - 1][y - 1] != '-')
            {
                printf("ERROR : Enter the Coordinates of an Empty Spot\n");
                continue;
            }

            board[x - 1][y - 1] = 'O';
            moveCount += 1;

            if(checkWinningMove(board , turn) == 1)
            {
                printf("GAME OVER : Player O Won the Game\n");
                printBoard(board);
                break;
            }

            turn = turn == 1 ? 2 : 1;
        }
    }

    freeGrid(board);
    return 0;
}