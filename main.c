#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include<windows.h>
#include <time.h>

#define ENEMY 0
#define PLAYER -666
#define FREE -69
#define WALL -1
#define PALLET -4
#define TRACK -11
#define DIRECTION_KEY 224
#define UP 72
#define LEFT 75
#define DOWN 80
#define RIGHT 77
#define ESC 27
#define USEDENEMY -9
#define NOMOVE -83
#define CURRENTENEMY -88
#define RANDOMENEMY -22
#define SCAREDENEMY -39
#define USEDRANDOMENEMY -54
#define USEDSCAREDENEMY -40


#define RED 60
#define WHITE 15
#define GREEN 2
#define YELLOW 14
#define BLUE 9
#define MAGNETA 5


COORD coord={0,0};

int **buffBuilder(char **buffOG, int *width, int *height);
void gotoxy();
void printer(int **buff, int width, int height);
void numFinder(int **buff, int step, int width, int height,int *found, void (*f)(int**,int,int,int,int *, int, int));
void stepper(int **buff, int width, int height, int step, int *found, int x, int y);
void tracker(int **buff, int width, int height, int step, int *found, int x, int y);
void pathFinder(int **buff, int width, int height);
void trackMaker(int **buff, int width, int height);
int keyChecker(int **buff, int width, int height, int *palletCount, int *game_over, int *ENEMYCanMove);
int palletGetter(int **buff, int width, int height);
void PLAYERclearBuff(int **buff, int width, int height);
void ENEMYclearBuff(int **buff, int width, int height);
int ENEMYMove(int **buff, int width, int height, int *onPallet);
int PLAYERMover(int **buff,int num, int width, int height, int *score);
int betterEnemyMove(int **buff, int width, int height);
int **buildTempBuff(int **buff, int width, int height, int y, int x);
int **palletBuffer(int **buff, int width, int height);
void insertPallets(int **buff, int **pallets,int width, int height);
void hidecursor();
int randomMove(int **buff, int width, int height, int y, int x, int enemyType);
void SetColor(int ForgC);
void gameCycle(int**buff,int width, int height);
int StandardEnemy(int **buff,int width,int height, int x, int y);
int ScaredEnemy(int **buff,int width,int height, int x, int y, int enemyType);
int **buildScaredTempBuff(int **buff, int width, int height, int y, int x, int enemyType);
void scaredTracker(int **buff, int width, int height, int step, int *found, int x, int y);
void scaredTrackMaker(int **buff, int width, int height);


int main()
{
    int height, width, **new_buff, **palletBuff;

    hidecursor();
    //S....ENEMY
    //C....PLAYER

    char *buff[]  =       { "XP              XXPXXX X",
                            "XXXXXX  R               ",
                            "  P   X XX   X       P X",
                            "X    X P         XX XXXX",
                            "  X  X XXXXXXXXXX      X",
                            " S       X      X      X",
                            "X    X  X  X    X   CX X",
                            "   XXX X        XX XXXXX",
                            " XX   s   P      X     X",
                            "       S   X     XX    X",
                            "  X        X      X    X",
                            "                  X    X",
                            "  X        X     XX  P X",
                            " P   X X    X          X"};
    height = 14;
    width = strlen(buff[0]);

    //printf("%d\n", height);
    //printf("%d\n", width);
    new_buff = buffBuilder(buff,&width,&height);
    //printf("%d", height);
    //pathFinder(new_buff,width,height);
    //trackMaker(new_buff,width,height);
    //printer(new_buff, width,height);
    //clearBuff(new_buff,width, height);
    //printf("\n");
    printer(new_buff,width,height);
    int d = 0;
    int game_over = 0;
    int pallet_max = palletGetter(new_buff,width,height);
    int score = 0;
    int ENEMYCanMove = 0;
    palletBuff = palletBuffer(new_buff,width,height);
    gameCycle(new_buff,width,height);



     //printer(new_buff, width,height);

    return 0;
}

void timer(int n_seconds)
{
   time_t start, now;

   start = time(NULL); now = start;

   while(difftime(now, start) < (double)n_seconds) {
        now = time(NULL);
   }
}

void gameCycle(int**buff,int width, int height)
{
    //findGameCycle
    int d,game_over, pallet_max,score,ENEMYCanMove, palletBuff, suicide, murder;
    d = 0;
    game_over = 0;
    pallet_max = palletGetter(buff,width,height);
    score = 0;
    murder = 0;
    ENEMYCanMove = 0;
    suicide = 0;
    palletBuff = palletBuffer(buff,width,height);
    int count = 0;
    while(game_over != 1 && score<pallet_max){

        timer(0.5);


        if(count != 6){
            count++;
        }
        else{
            count = 0;
        }
        if(kbhit()){
            int fnum = getch();
            if( fnum == DIRECTION_KEY){
                int num = getch();
                if(num == LEFT || num == RIGHT || num == UP || num == DOWN){
                    suicide = PLAYERMover(buff,num,width,height,&score);
                }
            }
            else if( fnum == ESC){
                break;
            }
        }

        if(count == 6){
            murder = betterEnemyMove(buff,width,height);
        }

        if(murder == 1 || suicide == 1){
            break;
        }


        /*
        else{
                missedCount++;
                for(int x = 0; x < missedCount; x++){
                    printf("MISSED\n");
                }
            printf("MISSED");
        }
        */

        gotoxy();
        printer(buff,width,height);
        insertPallets(buff,palletBuff,width,height);
        d++;
        printf("%s %d\n","Score:    ", score);
        printf("%s %d\n","ENEMY can move:    ", ENEMYCanMove);
    }
    gotoxy();
    printer(buff,width,height);
    if(score == pallet_max){
        printf("Congraturation! This story is happy end!");
    }
    else{
        printf("\n\n\n%s\n\n%s\n\n","GAME OVER","RETURN OF GANON");
    }

}

void SetColor(int ForgC)
{
     WORD wColor;
     //This handle is needed to get the current background attribute

     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //csbi is used for wAttributes word

     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //To mask out all but the background attribute, and to add the color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}


void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy()
{
 	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

int PLAYERMover(int **buff,int num, int width, int height, int *score)
{
    //printf(" World!\n");
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == PLAYER){
                    //printf("%s %d %s %d\n", "x = ", x, "y = ", y);
               if(num == UP){
                    if(buff[y-1][x] == FREE){
                        PLAYERclearBuff(buff,width,height);
                        buff[y-1][x] = PLAYER;
                        break;
                    }
                    else if(buff[y-1][x] == PALLET){
                        PLAYERclearBuff(buff,width,height);
                        buff[y-1][x] = PLAYER;
                        (*score)++;
                        break;
                    }
                    else if(buff[y-1][x] == ENEMY || buff[y-1][x] == RANDOMENEMY || buff[y-1][x] == SCAREDENEMY){
                        PLAYERclearBuff(buff,width,height);
                        return 1;
                    }
                }
                else if(num == LEFT){
                    if(buff[y][x-1] == FREE){
                        PLAYERclearBuff(buff,width,height);
                        buff[y][x-1] = PLAYER;
                        break;
                    }
                    else if(buff[y][x-1] == PALLET){
                        PLAYERclearBuff(buff,width,height);
                        buff[y][x-1] = PLAYER;
                        (*score)++;
                        break;
                    }
                    else if(buff[y][x-1] == ENEMY || buff[y][x-1] == RANDOMENEMY || buff[y][x-1] == SCAREDENEMY){
                        PLAYERclearBuff(buff,width,height);
                        return  1;
                    }

                }
                else if(num == RIGHT){
                    if(buff[y][x+1] == FREE){
                        PLAYERclearBuff(buff,width,height);
                        buff[y][x+1] = PLAYER;
                        break;
                    }
                    else if(buff[y][x+1] == PALLET){
                        PLAYERclearBuff(buff,width,height);
                        buff[y][x+1] = PLAYER;
                        (*score)++;
                        break;
                    }
                    else if(buff[y][x+1] == ENEMY || buff[y][x+1] == RANDOMENEMY){
                        PLAYERclearBuff(buff,width,height);
                        return 1;
                    }
                }
                else if(num == DOWN){
                    if(buff[y+1][x] == FREE){
                        PLAYERclearBuff(buff,width,height);
                        buff[y+1][x] = PLAYER;
                        y = height;
                        //printf("%d %d", y, x);exit(2);
                        break;
                    }
                    else if(buff[y+1][x] == PALLET){
                        PLAYERclearBuff(buff,width,height);
                        buff[y+1][x] = PLAYER;
                        y = height;
                        (*score)++;
                        //printf("%d", *score);
                        //printf("%d %d", y, x);exit(2);
                        break;
                    }
                    else if(buff[y+1][x] == ENEMY || buff[y+1][x] == RANDOMENEMY){
                        PLAYERclearBuff(buff,width,height);
                        return 1;
                    }
                }


            }
            //printf("%s %d %s %d\n", "x = ", x, "y = ", y);

        }


    }
    return 0;
     //printf("%d", num);
    //getch();
}


int killedPlayerCheck(int **buff, int width, int height, int x, int y, int addY, int addX){
    if(buff[y + addY][x + addX] == PLAYER){
        PLAYERclearBuff(buff,width,height);
        buff[y][x] = FREE;
        buff[y + addY][x + addX] = ENEMY;
        y = height;
        return 1;
    }
}



void insertPallets(int **buff, int **pallets,int width, int height)
{
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(pallets[y][x] == PALLET && buff[y][x] == FREE){
                buff[y][x] = PALLET;
            }
            else if(buff[y][x] == PLAYER && pallets[y][x] == PALLET){
                pallets[y][x] = FREE;
            }
        }
    }
}

int randomMove(int **buff, int width, int height, int y, int x, int enemyType)
{
    //findRandomMove
    int murder = 0;
    int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};
    int randVec[4] = {rand(), rand(), rand(), rand()};
    int max = -1;
    //printf("Got Here\n");
    //printf("%d\n", y);
    //printf("%d\n", x);
    //if(y == 16){
     //   printf("%d", buff[16][11]);
    //}
    //printf("%d", buff[y][x]);
    //printf("\nThe contents are: %d\n", buff[y + vec[1][0]][x + vec[1][1]]);
    for(int i = 0; i < 4; i++){

        if(randVec[i] > max && (buff[y + vec[i][0]][x + vec[i][1]] == FREE || buff[y + vec[i][0]][x + vec[i][1]] == PLAYER)){
            max = randVec[i];
        }
    }
    //printf("After for cycle\n");
    for(int i = 0; i < 4; i++){
        if(randVec[i] == max){
            buff[y][x] = FREE;
            if(buff[y + vec[i][0]][x + vec[i][1]] == PLAYER){
                murder = 1;
            }
            if(enemyType == ENEMY){
                buff[y + vec[i][0]][x + vec[i][1]] = USEDENEMY;
                break;
            }
            else if(enemyType == RANDOMENEMY){
                buff[y + vec[i][0]][x + vec[i][1]] = USEDRANDOMENEMY;
                break;
            }
            else if(enemyType == SCAREDENEMY){
                buff[y + vec[i][0]][x + vec[i][1]] = USEDSCAREDENEMY;
                break;
            }

        }
    }

    return murder;
}

int **buildScaredTempBuff(int **buff, int width, int height, int y, int x, int enemyType)
{
    //findBuildScaredTempBuff
    int **temp_buff;
   int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};
    temp_buff = (int**) malloc(height * sizeof(int));
    for(int t_y = 0; t_y < height; t_y++){
        temp_buff[t_y] = (int*) malloc(width*sizeof(int));
    }

    for(int t_y = 0; t_y < height;t_y++){
        for(int t_x = 0; t_x < width;t_x++){
            if(buff[t_y][t_x] == PALLET){
                temp_buff[t_y][t_x] = FREE;
            }
            else if(t_y == y && t_x == x){
                temp_buff[t_y][t_x] = enemyType;
            }
            else if(buff[t_y][t_x] == ENEMY || buff[t_y][t_x] == USEDENEMY || buff[t_y][t_x] == RANDOMENEMY || buff[t_y][t_x] == USEDRANDOMENEMY || buff[t_y][t_x] == SCAREDENEMY || buff[t_y][t_x] == USEDSCAREDENEMY){
                temp_buff[t_y][t_x] = WALL;
            }
            else{
                temp_buff[t_y][t_x] = buff[t_y][t_x];
            }

        }
    }
        //printf("\n%s %d\n","SS was in line: ", y);
        //printf("\n%s %d\n","SS was in column: ", x);
    pathFinder(temp_buff,width,height);

    //system("cls");
    //printer(buff,width,height);
    //printf("\nPoochi\n");
    //printf("\n------------------------------------------\n");
    //printer(temp_buff,width,height);
     //printf("\nDio\n");
    if(buff[0][0] != NOMOVE){
        scaredTrackMaker(temp_buff,width,height);
       // printf("\n------------------------------------------\n");
        //printer(temp_buff,width,height);
    }
    else{
        for(int i = 0; i < 4; i++){
            if(buff[y + vec[i][0]][x + vec[i][1]] == FREE){
                buff[y + vec[i][0]][x + vec[i][1]] = TRACK;
                i = 4;
                break;
            }
        }
    }
    //getch();
            //printf("HELLO"
}

int **buildTempBuff(int **buff, int width, int height, int y, int x)
{
   int **temp_buff;
   int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};
    temp_buff = (int**) malloc(height * sizeof(int));
    for(int t_y = 0; t_y < height; t_y++){
        temp_buff[t_y] = (int*) malloc(width*sizeof(int));
    }

    for(int t_y = 0; t_y < height;t_y++){
        for(int t_x = 0; t_x < width;t_x++){
            if(buff[t_y][t_x] == PALLET){
                temp_buff[t_y][t_x] = FREE;
            }
            else if(t_y == y && t_x == x){
                temp_buff[t_y][t_x] = ENEMY;
            }
            else if(buff[t_y][t_x] == ENEMY || buff[t_y][t_x] == USEDENEMY){
                temp_buff[t_y][t_x] = WALL;
            }
            else{
                temp_buff[t_y][t_x] = buff[t_y][t_x];
            }

        }
    }
        //printf("\n%s %d\n","SS was in line: ", y);
        //printf("\n%s %d\n","SS was in column: ", x);
    pathFinder(temp_buff,width,height);

    //system("cls");
    //printer(buff,width,height);
    //printf("\nPoochi\n");
    //printf("\n------------------------------------------\n");
    //printer(temp_buff,width,height);
     //printf("\nDio\n");
    if(buff[0][0] != NOMOVE){
        trackMaker(temp_buff,width,height);
       // printf("\n------------------------------------------\n");
        //printer(temp_buff,width,height);
    }
    else{
        for(int i = 0; i < 4; i++){
            if(buff[y + vec[i][0]][x + vec[i][1]] == FREE){
                buff[y + vec[i][0]][x + vec[i][1]] = TRACK;
                i = 4;
                break;
            }
        }
    }
    //getch();
            //printf("HELLO");
    return temp_buff;
}




int betterEnemyMove(int **buff, int width, int height)
{
    //findBetter
    int min,playerFound;
    int murder = 0;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == ENEMY){
                //printf("ENEMY");
                if(StandardEnemy(buff,width,height,x,y) == 1){
                    murder = 1;
                }

             }
             else if(buff[y][x] == RANDOMENEMY){
                    //printf("RANDOMENEMY");
                    //printf("%d\n", y);
                    //printf("%d\n", x);
                if(RandomEnemy(buff,width,height,x,y) == 1){
                    murder = 1;
                }
             }
             else if(buff[y][x] == SCAREDENEMY){
                 //printf("SCAREDENEMY");
                if(ScaredEnemy(buff,width,height,x,y,SCAREDENEMY) == 1){
                    murder = 1;
                }
             }

        }
    }
    for(int y = 0; y < height;y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == USEDENEMY){
                buff[y][x] = ENEMY;
            }
            else if(buff[y][x] == USEDRANDOMENEMY){
                buff[y][x] = RANDOMENEMY;
            }
            else if(buff[y][x] == USEDSCAREDENEMY){
                buff[y][x] = SCAREDENEMY;
            }
        }
    }

    return murder;
}


int StandardEnemy(int **buff,int width,int height, int x, int y)
{
    //findStandardEnemy
    int **temp_buff;
    int murder = 0;
    int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};

    for(int i = 0; i < 4; i++){
        if(killedPlayerCheck(buff,width,height,x,y,vec[i][0],vec[i][1]) == 1){
            buff[y][x] = FREE;
            buff[y + vec[i][0]][x + vec[i][1]] = USEDENEMY;
            return 1;
        }
    }

    temp_buff = buildTempBuff(buff,width,height,y,x);
    int moved = 0;
    if(temp_buff[0][0] != NOMOVE){
            if(temp_buff[y][x] == ENEMY){
                for(int i = 0; i < 4; i++){
                    if(temp_buff[y + vec[i][0]][x + vec[i][1]] == TRACK){
                       buff[y][x] = FREE;
                        buff[y + vec[i][0]][x + vec[i][1]] = USEDENEMY;
                        moved = 1;
                        //printf("\nAfter movement\n");
                        //printer(temp_buff,width,height);
                        //printf("\nMovement done\n");
                        //printf("HELLO");
                        break;

                }
            }
        }
    }

    if(moved == 0){
        murder = randomMove(buff,width,height,y,x, ENEMY);
    }

    return murder;
}
int RandomEnemy(int **buff,int width,int height, int x, int y)
{
//findRandomEnemy
    return randomMove(buff,width,height,y,x, RANDOMENEMY);
}

int ScaredEnemy(int **buff,int width,int height, int x, int y, int enemyType)
{
//findScaredEnemy

    int **temp_buff, pY, pX, corneredCount, playerSpotted;
    int murder = 0;
    int moved = 0;
    int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};
    corneredCount = 0;
    playerSpotted = 0;

    for(int i = 0; i < 4; i++){
        if(killedPlayerCheck(buff,width,height,x,y,vec[i][0],vec[i][1]) == 1){
            pY = y + vec[i][0];
            pX = x + vec[i][1];
            playerSpotted = 1;
        }
        else if(buff[y + vec[i][0]][x + vec[i][1]] == FREE){
            corneredCount++;
        }
    }

    if(corneredCount == 3 && playerSpotted == 1){
        buff[y][x] = FREE;
        if(enemyType == ENEMY){
            buff[pY][pX] = USEDENEMY;
        }
        else if(enemyType == SCAREDENEMY){
            buff[pY][pX] = USEDSCAREDENEMY;
        }
        else if(enemyType == RANDOMENEMY){
            buff[pY][pX] = USEDRANDOMENEMY;
        }
        return 1;

    }

    //THIS TEMP BUFF DOESNT SEEM TO WORK!!!!!!
    temp_buff = buildScaredTempBuff(buff,width,height,y,x, enemyType);
    printf("%d", temp_buff[0][0]);
    if(temp_buff[0][0] != NOMOVE){

            if(temp_buff[y][x] == enemyType){
                for(int i = 0; i < 4; i++){
                    if(temp_buff[y + vec[i][0]][x + vec[i][1]] == TRACK){
                       buff[y][x] = FREE;
                        if(enemyType == ENEMY){
                            buff[y + vec[i][0]][x + vec[i][1]] = USEDENEMY;
                        }
                        else if(enemyType == SCAREDENEMY){

                            buff[y + vec[i][0]][x + vec[i][1]] = USEDSCAREDENEMY;
                        }
                        else if(enemyType == RANDOMENEMY){
                            buff[y + vec[i][0]][x + vec[i][1]] = USEDRANDOMENEMY;
                        }
                        moved = 1;
                        //printf("\nAfter movement\n");
                        //printer(temp_buff,width,height);
                        //printf("\nMovement done\n");
                        //printf("HELLO");
                        break;

                }
            }
        }
    }

    if(moved == 0){
        murder = randomMove(buff,width,height,y,x, enemyType);
    }

    return murder;


}


int ENEMYMove(int **buff, int width, int height, int *onPallet)
{
    int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};
    int min,playerFound, **temp_buff;


    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == ENEMY){
                for(int i = 0; i < 4; i++){
                    if(buff[y + vec[i][0]][x + vec[i][1]] == PLAYER){
                        PLAYERclearBuff(buff,width,height);
                        buff[y][x] = FREE;
                        buff[y + vec[i][0]][x + vec[i][1]] = ENEMY;
                        y = height;
                        if(*onPallet == 1){
                            buff[y][x] = PALLET;
                            *onPallet = 0;
                        }
                        return 1;
                    }
                }
            }
        }
    }


    min = 100000;
    temp_buff = (int**) malloc(height * sizeof(int));
    for(int y = 0; y<height;y++){
        temp_buff[y] = (int*) malloc(width*sizeof(int));
    }

    for(int y = 0; y < height;y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == PALLET){
                temp_buff[y][x] = FREE;
            }
            else{
                temp_buff[y][x] = buff[y][x];
            }

        }
    }

    pathFinder(temp_buff,width,height);
    trackMaker(temp_buff,width,height);

    //printer(temp_buff,width,height);
    //getch();
    for(int y = 0; y < height;y++){
        for(int x = 0; x < width; x++){
            if(temp_buff[y][x] == ENEMY){
                for(int i = 0; i < 4; i++){
                    if(temp_buff[y + vec[i][0]][x + vec[i][1]] == TRACK){
                       ENEMYclearBuff(buff,width,height);
                        if(*onPallet == 1){
                            buff[y][x] = PALLET;
                            *onPallet = 0;
                        }
                        if(buff[y + vec[i][0]][x + vec[i][1]] == PALLET){
                            *onPallet = 1;
                        }
                        buff[y + vec[i][0]][x + vec[i][1]] = ENEMY;
                        break;
                    }
                }

            }
        }

    }
    return 0;
}


int keyChecker(int **buff, int width, int height, int *score, int *gameOver, int *ENEMYCanMove)
{
    int x, y, num, found, fnum, suicide, murder;
    fnum = getch();
    if(fnum == DIRECTION_KEY){
        //printf("Hello\n");
        num = getch();
        //printf("%d\n", num);
        if(num == LEFT || num == RIGHT || num == UP || num == DOWN){
            //printf("Hello");
            suicide = PLAYERMover(buff,num,width,height,score);
            if(suicide == 0){
                if(*ENEMYCanMove == 1){
                    murder = betterEnemyMove(buff,width,height);
                    *ENEMYCanMove = 0;
                }
                else{
                    *ENEMYCanMove = 1;
                }


            }
            if(suicide == 1 || murder == 1){
                *gameOver = 1;
            }

        }
     }
    return fnum;
}


int palletGetter(int **buff, int width, int height)
{
    int palletNum;
    palletNum = 0;
   for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == PALLET){
                palletNum++;
            }

        }
    }
    return palletNum;
}

void ENEMYclearBuff(int **buff, int width, int height)
{
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] >= 0 || buff[y][x] == TRACK){
                buff[y][x] = FREE;
            }
        }
    }
}

void PLAYERclearBuff(int **buff, int width, int height)
{
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == PLAYER || buff[y][x] == TRACK){
                buff[y][x] = FREE;
            }
        }
    }
}

int **buffBuilder(char **buffOG, int *width, int *height)
{
    //findBuffBuilder
    int **buff;
    *height += 2;
    *width +=2;
    buff = (int**) malloc(*height* sizeof(int));
    for(int y = 0; y<*height;y++){
        buff[y] = (int*) malloc(*width*sizeof(int));
    }

    for(int x = 0; x < *width; x++){
        buff[0][x] = WALL;
    }
    for(int y = 1; y < *height-1; y++){
        buff[y][0] = WALL;
        for(int x = 1; x < *width-1; x++){
            if(buffOG[y-1][x-1] == 'X'){
                buff[y][x] = WALL;
            }
            else if(buffOG[y-1][x-1] == 'S'){
                buff[y][x] = ENEMY;

            }
            else if(buffOG[y-1][x-1] == 'C'){
                buff[y][x] = PLAYER;
            }
            else if(buffOG[y-1][x-1] == 'R'){
                buff[y][x] = RANDOMENEMY;
            }
            else if(buffOG[y-1][x-1] == 'L'){
                buff[y][x] = SCAREDENEMY;
            }
            else if(buffOG[y-1][x-1] == 'P'){
                buff[y][x] = PALLET;
            }
            else{
                buff[y][x] = FREE;
            }
        }
        buff[y][*width-1] = WALL;
    }
    for(int x = 0; x < *width; x++){
        buff[0][x] = WALL;
    }
    for(int x = 0; x < *width; x++){
        buff[*height-1][x] = WALL;
    }
    return buff;
}

void printer(int **buff, int width, int height)
{
    //findPrinter
    for(int y = 1; y < height-1; y++){
        for(int x = 1; x < width -1; x++){
            if(buff[y][x] == WALL){
                SetColor(GREEN);
                printf("[]");
                SetColor(WHITE);
            }
            else if(buff[y][x] == TRACK){
                printf("* ");
            }
            else if(buff[y][x] == ENEMY){
                SetColor(RED);
                printf("SS");
                 SetColor(WHITE);
            }
            else if(buff[y][x] == RANDOMENEMY){
                SetColor(MAGNETA);
                printf("SS");
                SetColor(WHITE);
            }
            else if(buff[y][x] == SCAREDENEMY){
                SetColor(BLUE);
                printf("SS");
                SetColor(WHITE);
            }
            else if(buff[y][x] == PLAYER){
                printf("EE");
            }
            else if(buff[y][x] == FREE){
                printf("  ");
            }
             else if(buff[y][x] == CURRENTENEMY){
                printf("CE");
            }
            else if(buff[y][x] == PALLET){
                SetColor(YELLOW);
                printf("O ");
                SetColor(WHITE);
            }
            else{
                //printf("%2d", buff[y][x]);
            }
        }
        printf("\n");
    }

}

void numFinder(int **buff, int step, int width, int height,int *found, void (*f)(int**,int,int,int,int *, int, int))
{
    for(int temp_y = 0; temp_y < height; temp_y++){
        for(int temp_x = 0; temp_x < width; temp_x++){
            if(buff[temp_y][temp_x] == step){
               (*f)(buff, width, height, step, found, temp_x, temp_y);
            }
        }
    }
}

void stepper(int **buff, int width, int height, int step, int *found, int x, int y){
    int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};
    //printf("%d",buff[0][0]);
    //printf("%s %d %s %d\n","ENEMYed at position x = ", x, " y = ", y);
    for(int i = 0; i < 4; i++){
        //printf("%s %d %s %d\n\n", "The new position is x = ", x + vec[i][1], " y = ", y + vec[i][0]);
        //printf("%d\n",buff[ y + vec[i][0]][x + vec[i][1]]);
        if(buff[y + vec[i][0]][x + vec[i][1]] == FREE){
                //printf("here");
            buff[y + vec[i][0]][x + vec[i][1]] = step+1;
        }
       // printf("Hello\n");
       else if(buff[y + vec[i][0]][x + vec[i][1]] == PLAYER){
            *found = 1;

            break;
            //printf("Got here");
       }
    }
    //printf("%d\n", step);

}


int **palletBuffer(int **buff, int width, int height)
{
    int **palletBuff;
    palletBuff = (int**) malloc(height * sizeof(int));
    for(int t_y = 0; t_y < height; t_y++){
         palletBuff[t_y] = (int*) malloc(width*sizeof(int));
    }

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == PALLET){
                palletBuff[y][x] = PALLET;
            }
            else{
                palletBuff[y][x] = FREE;
            }
        }
    }

    return palletBuff;

}


void trackMaker(int **buff, int width, int height)
{
    int step, found;
    found = 0;
    step = PLAYER;
    numFinder(buff,step, width, height, &found,tracker);
}

void pathFinder(int **buff, int width, int height)
{
    //findPathFinder
    int step,found, stepMax;
    step = ENEMY;
    found = 0;
    stepMax = 0;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == PALLET || buff[y][x] == FREE){
                stepMax++;
            }
        }
    }


    while(found == 0 && step < stepMax){
          //printf("Hello");
        //printer(buff,width,height);
        numFinder(buff,step, width, height, &found,stepper);

        step++;
    }
    if(step >= (width*height)){
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                if(buff[y][x] == ENEMY){
                    buff[0][0] = NOMOVE;
                    //getch();
                }

            }
        }
    }
}

void scaredTrackMaker(int **buff, int width, int height)
{
     int step, found;
    found = 0;
    step = PLAYER;
    numFinder(buff,step, width, height, &found,scaredTracker);
}



void scaredTracker(int **buff, int width, int height, int step, int *found, int x, int y)
{
    int max, track_x, track_y, maxMoveCount, counter;
    int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};
    max = buff[y -1][x];
    track_x = x;
    track_y = y-1;
    maxMoveCount = 0;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == PALLET || buff[y][x] == FREE){
                maxMoveCount++;
            }
        }
    }

    counter = 0;
    while(found != ENEMY && counter <= maxMoveCount){

        //printf("__%d__", counter);
           // printf("%s %d %s %d %s %d\n","ENEMYed at position x = ", x, " y = ", y, " containing the number ", buff[y][x]);
        for(int i = 0; i < 4; i++){
            if(buff[y + vec[i][0]][x + vec[i][1]] == NOMOVE){
                found = ENEMY;
                break;
            }
            //printf("%s %d %s %d %s %d\n\n", "The new position is x = ", x + vec[i][1], " y = ", y + vec[i][0], " containing the number ", buff[y + vec[i][0]][x + vec[i][1]]);
            if((buff[y + vec[i][0]][x + vec[i][1]] > max && buff[y + vec[i][0]][x + vec[i][1]] > ENEMY) || max < ENEMY){

                max = buff[y + vec[i][0]][x + vec[i][1]];
                //printf("%s %d\n", "Storing ", min);
                track_x = x + vec[i][1];
                track_y = y + vec[i][0];

            }
            else if( buff[y + vec[i][0]][x + vec[i][1]] == ENEMY){
                    //printf("FOUND!!!\n");
                    found = ENEMY;
                    break;
            }

        }
        if(found != ENEMY && (buff[track_y][track_x] > 0 || buff[track_y][track_x] == TRACK)){
            buff[track_y][track_x] = TRACK;
           // printf("Continue");
            x = track_x;
            y = track_y;
        }
        else{
            //printf("%d", buff[track_y][track_x]);
        }
        counter++;
        //printf("%s %d %s %d %s %d\n", "The minimum was ", min, "at the position x = ", x, " y = ", y);

    }

}

void tracker(int **buff, int width, int height, int step, int *found, int x, int y)
{
    //findTracker
    int min, track_x, track_y, maxMoveCount, counter;
    int vec[4][2] = {{-1,0}, {1,0}, {0,-1},{0,1}};
    min = buff[y -1][x];
    track_x = x;
    track_y = y-1;
    maxMoveCount = 0;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(buff[y][x] == PALLET || buff[y][x] == FREE){
                maxMoveCount++;
            }
        }
    }

    counter = 0;
    while(found != ENEMY && counter <= maxMoveCount){

        //printf("__%d__", counter);
           // printf("%s %d %s %d %s %d\n","ENEMYed at position x = ", x, " y = ", y, " containing the number ", buff[y][x]);
        for(int i = 0; i < 4; i++){
            if(buff[y + vec[i][0]][x + vec[i][1]] == NOMOVE){
                found = ENEMY;
                break;
            }
            //printf("%s %d %s %d %s %d\n\n", "The new position is x = ", x + vec[i][1], " y = ", y + vec[i][0], " containing the number ", buff[y + vec[i][0]][x + vec[i][1]]);
            if((buff[y + vec[i][0]][x + vec[i][1]] < min && buff[y + vec[i][0]][x + vec[i][1]] > ENEMY) || min < ENEMY){

                min = buff[y + vec[i][0]][x + vec[i][1]];
                //printf("%s %d\n", "Storing ", min);
                track_x = x + vec[i][1];
                track_y = y + vec[i][0];

            }
            else if( buff[y + vec[i][0]][x + vec[i][1]] == ENEMY){
                    //printf("FOUND!!!\n");
                    found = ENEMY;
                    break;
            }

        }
        if(found != ENEMY && (buff[track_y][track_x] > 0 || buff[track_y][track_x] == TRACK)){
            buff[track_y][track_x] = TRACK;
           // printf("Continue");
            x = track_x;
            y = track_y;
        }
        else{
            //printf("%d", buff[track_y][track_x]);
        }
        counter++;
        //printf("%s %d %s %d %s %d\n", "The minimum was ", min, "at the position x = ", x, " y = ", y);

    }


    //printf("a");
}


