#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define ROWS 6
#define COLUMNS 7

void freeGrid(char **grid)
{
    for(int i = 0 ; i < ROWS ; i++) free(grid[i]);
    free(grid);
}

void printGrid(char **grid)
{
    printf("  ");
    for(int i = 0 ; i < COLUMNS ; i++)
    {
        printf("%d " , i + 1);
    }
    printf("\n");
    for(int i = 0 ; i < ROWS ; i++)
    {
        printf("%d " , i + 1);
        for(int j = 0 ; j < COLUMNS ; j++)
        {
            printf("%c " , grid[i][j]);
        }
        printf("\n");
    }
}

//----------------------------------------------------------------

int checkWinningMove(char **grid , int row , int col , int turn)
{
    char target = turn == 1 ? 'R' : 'Y';
    int count , r , c;

    //Checking Current Row
    count = 0;
    r = row;
    c = col - 1;
    while(c >= 0)
    {
        if(grid[r][c] == target) count += 1;
        else break;
        c--;
    }
    r = row;
    c = col + 1;
    while(c < COLUMNS)
    {
        if(grid[r][c] == target) count += 1;
        else break;
        c++;
    }
    if(count + 1 >= 4)
    {
        return 1;
    }

    //---------------------------------

    //Checking Current Column
    count = 0;
    r = row;
    c = col;
    while(r < ROWS)
    {
        if(grid[r][c] == target) count += 1;
        else break;
        r++;
    }
    if(count >= 4)
    {
        return 1;
    }

    //-------------------------------------

    //Checking Diagonal
    count = 0;
    r = row + 1;
    c = col + 1;
    while(r < ROWS && c < COLUMNS)
    {
        if(grid[r][c] == target) count += 1;
        else break;
        r++;c++;
    }
    r = row - 1;
    c = col - 1;
    while(r >= 0 && c >= 0)
    {
        if(grid[r][c] == target) count += 1;
        else break;
        r--;c--;
    }

    if(count + 1 >= 4)
    {
        return 1;
    }

    //------------------------------

    //Checking Alternate Diagonal
    count = 0;
    r = row + 1;
    c = col - 1;
    while(r < ROWS && c >= 0)
    {
        if(grid[r][c] == target) count += 1;
        else break;
        r++;c--;
    }
    r = row - 1;
    c = col + 1;
    while(r >= 0 && c < COLUMNS)
    {
        if(grid[r][c] == target) count += 1;
        else break;
        r--;c++;
    }
    if(count + 1 >= 4)
    {
        return 1;
    }

    return 0;
}

//---------------------------------------------------------------


int getBestMoveUtil(char **grid , int *top , int moveCount , int turn , int depth)
{
    if(moveCount == ROWS * COLUMNS || depth == 8)
    {
        return 0;
    }

    if(turn == 1)
    {
        int mx = -99999;
        int tmp;

        for(int j = 0 ; j < COLUMNS ; j++)
        {
            if(top[j] == 6)
            {
                continue;
            }

            grid[ROWS - top[j] - 1][j] = 'R';
            if(checkWinningMove(grid , ROWS - top[j] - 1 , j , turn) == 1)
            {
                grid[ROWS - top[j] - 1][j] = '-';
                return 1;
            }
            top[j] += 1;

            tmp = getBestMoveUtil(grid , top , moveCount + 1 , 2 , depth + 1);
            mx = mx >= tmp ? mx : tmp;

            top[j] -= 1;
            grid[ROWS - top[j] - 1][j] = '-';
        }
        return mx;
    }
    else if(turn == 2)
    {
        int mn = 99999;
        int tmp;

        for(int j = 0 ; j < COLUMNS ; j++)
        {
            if(top[j] == 6)
            {
                continue;
            }

            grid[ROWS - top[j] - 1][j] = 'Y';
            if(checkWinningMove(grid , ROWS - top[j] - 1 , j , turn) == 1)
            {
                grid[ROWS - top[j] - 1][j] = '-';
                return -1;
            }
            top[j] += 1;

            tmp = getBestMoveUtil(grid , top , moveCount + 1 , 1 , depth + 1);
            mn = mn <= tmp ? mn : tmp;

            top[j] -= 1;
            grid[ROWS - top[j] - 1][j] = '-';
        }

        return mn;
    }
    return 0;
}

int getBestMove(char **grid , int *top , int moveCount)
{
    int min = 99999;
    int bestMove = -1;
    int tmp;

    for(int j = 0 ; j < COLUMNS ; j++)
    {
        if(top[j] == 6)
        {
            continue;
        }

        grid[ROWS - top[j] - 1][j] = 'Y';
        top[j] += 1;

        tmp = getBestMoveUtil(grid , top , moveCount + 1 , 1 , 0);
        if(min > tmp)
        {
            min = tmp;
            bestMove = j;
        }

        top[j] -= 1;
        grid[ROWS - top[j] - 1][j] = '-';
    }
    return bestMove + 1;
}

//----------------------------------------------------------------

int main(int argc , char **argv , char **envp)
{
    char **grid = (char**)(malloc(sizeof(char*) * ROWS));
    for(int i = 0 ; i < ROWS ; i++)
    {
        grid[i] = (char*)(malloc(sizeof(char) * COLUMNS));
        for(int j = 0 ; j < COLUMNS ; j++) grid[i][j] = '-';
    }

    int *top = (int*)(malloc(sizeof(int) * COLUMNS));
    for(int i = 0 ; i < COLUMNS ; i++) top[i] = 0;

    int turn = 1;
    int moveCount = 0;
    int inp;

    int gameMode = 0;
    printf("This Game can be played in Single player and Two Player Mode\n");
    printf("Press 1 : Single Player Mode\n");
    printf("Press 2 : Two Player Mode\n");
    printf("Press -1 when the game has started : To exit the Game\n");
    
    while(gameMode != 1 && gameMode != 2)
    {
        printf("Enter Game Mode : ");
        scanf("%d" , &gameMode);
    }

    while(1)
    {
        printGrid(grid);
        if(moveCount == ROWS * COLUMNS)
        {
            printf("GAME OVER : It is a Draw\n");
            break;
        }
        if(turn == 1)
        {
            printf("It is Player R Turn Now : ");
            scanf("%d" , &inp);

            if(inp == -1)
            {
                printf("GAME OVER : Game Exited\n");
                break;
            }
            if(inp < 1 || inp > 7)
            {
                printf("GAME ERROR : Enter a Valid Input\n");
                continue;
            }

            if(top[inp - 1] == 6)
            {
                printf("GAME ERROR : Column already Full\n");
                continue;
            }

            grid[ROWS - top[inp - 1] - 1][inp - 1] = 'R';
            moveCount += 1;

            if(checkWinningMove(grid , ROWS - top[inp - 1] - 1 , inp - 1 , turn) == 1)
            {
                printf("GAME OVER : Player R Won the Game\n");
                printGrid(grid);
                break;
            }

            top[inp - 1] += 1;

            turn = turn == 1 ? 2 : 1;
        }
        else if(turn == 2)
        {
            printf("It is Player Y Turn Now : ");
            if(gameMode == 1)
            {
                inp = getBestMove(grid , top , moveCount);
                printf("%d\n" , inp);
            }
            else if(gameMode == 2)
            {
                scanf("%d" , &inp);
            }

            if(inp == -1)
            {
                printf("GAME OVER : Game Exited\n");
                break;
            }
            if(inp < 1 || inp > 7)
            {
                printf("GAME ERROR : Enter a Valid Input\n");
                continue;
            }
            
            if(top[inp - 1] == 6)
            {
                printf("GAME ERROR : Column already Full\n");
                continue;
            }

            grid[ROWS - top[inp - 1] - 1][inp - 1] = 'Y';
            moveCount += 1;

            if(checkWinningMove(grid , ROWS - top[inp - 1] - 1 , inp - 1 , turn) == 1)
            {
                printf("GAME OVER : Player Y Won the Game\n");
                printGrid(grid);
                break;
            }

            top[inp - 1] += 1;

            turn = turn == 1 ? 2 : 1;
        }
    }

    freeGrid(grid);
    return 0;
}