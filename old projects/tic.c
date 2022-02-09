/**
 * @file TicTacToe.c
 * @author Keshab Mondal
 * @brief This is a TicTacToe game I created in C. You can play in       
 * single player mode or double player mode. This was my first tume      
 * createing an AI so I hope it turned out good. Also, look at me being
 * all fancy and
 * @version 1
 * @date 2022-02-07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define RANDINT(limit) (int)rand()/((RAND_MAX-1000)/limit)
#define CLEAR while(getchar() != '\n')
// structure for the board 
typedef struct{
    char **board;
    int size;
} BoardVals;

// structure for the players
typedef struct{
    char name[30];
    int *rowspicked;
    int *colspicked;
}Players;

// fuction prototypes
int getpos(int max,char type[]);
void createBoard(BoardVals *bv);
void drawBoard(BoardVals *bv);
void freeVals(BoardVals *bv);
bool checkWinner(Players *p, BoardVals *bv, char sym);
bool checkdraw(Players *p1, Players *p2, BoardVals *bv);
void aiFuction(Players *p2, Players *p1, BoardVals *bv, int *pos);
void clear();

int main(void) {
    // initalizing variables
    srand(time(NULL));
    BoardVals bv;
    Players p1,p2;
    char charInput;
    int intInput;
    int playernum;
    int pos[2];//current pos player picked
    int round = 0;
    // initalizing with player inputs
    do{
        printf("Pick Board size (max 9, min 3): ");
        scanf("%d",&intInput);
        CLEAR;
    }while(intInput >9 || intInput < 3);
    bv.size = intInput;

    do{
        printf("Single player or double player: \n Enter 1 || 2: ");
        scanf("%d",&intInput);
        CLEAR;
    }while(!(intInput == 1 || intInput == 2));
    playernum = intInput;

    playernum == 1 ? printf("What is your name? ") : printf("What is player 1's name? ");
    fgets(p1.name,29,stdin);
    p1.name[strcspn(p1.name, "\n")] = 0;

    playernum == 1 ? printf("What is the AI's name? ") : printf("What is player 2's name? ");
    fgets(p2.name,29,stdin);
    p2.name[strcspn(p2.name, "\n")] = 0;
    
    restart:; 
    // draws board and allcocates memory
    createBoard(&bv);
    p1.rowspicked = malloc(bv.size*sizeof(int));
    p1.colspicked = malloc(bv.size*sizeof(int));
    p2.rowspicked = malloc(bv.size*sizeof(int));
    p2.colspicked = malloc(bv.size*sizeof(int));
    
    while(true){
        // draws the board
        drawBoard(&bv);
        // player enters positions
        if(playernum == 1){
            if(round %2 == 0){
                // player action
                printf("It's your turn:\n");
                pos[0] =getpos(bv.size-1,"row");
                pos[1] =getpos(bv.size-1,"column");
                // ai action
            } else{aiFuction(&p2, &p1, &bv, pos);}
        } else{
            printf("It's %s's turn:\n",round%2 ? p1.name : p2.name);
            reEnter:;
            pos[0] =getpos(bv.size-1,"row");
            pos[1] =getpos(bv.size-1,"column");          
        }
        // sets the new board or goes back to entering
        if(bv.board[pos[0]][pos[1]] == '-'){
            if(round%2 == 0){
                p1.rowspicked[pos[0]]++;
                p1.colspicked[pos[1]]++;
                bv.board[pos[0]][pos[1]] = 'O';
            }
            else{
                p2.rowspicked[pos[0]]++;
                p2.colspicked[pos[1]]++;
                bv.board[pos[0]][pos[1]] = 'X';
            }
        } else{
            printf("That positon is already taken!\n Re-");
            goto reEnter;
        }
        clear();
        // implament a restart
        if(checkWinner(&p1,&bv,'O')){
            printf("%s won!\n",p1.name);
            break;
        }else if(checkWinner(&p2,&bv, 'X')){
            printf("%s won!\n",p2.name);
            break;
        } else if(checkdraw(&p1,&p2,&bv)){
            printf("It is a draw!");
            break;
        }
        if(round%2 ==1 && playernum == 1){printf("%s picked pos (%d,%d)\n",p2.name,pos[0],pos[1]);}
        round++; 
    }
    drawBoard(&bv);
    freeVals(&bv);
    free(p1.colspicked);
    free(p1.rowspicked);
    free(p2.rowspicked);
    free(p2.colspicked);
    // asks for a restart
    printf("Would you like to play again (y/n): ");
    scanf("%c",&charInput);
    charInput = tolower(charInput);
    // re-initalize variables
    if(charInput == 'y'){
        do{
            printf("Pick Board size (max 9, min 3): ");
            scanf("%d",&intInput);
            CLEAR;
        }while(intInput >9 || intInput < 3);
        bv.size = intInput;
        printf("Would you like to change you're names (y/n): ");
        scanf("%c",&charInput);
        charInput = tolower(charInput);
        if(charInput == 'y'){
            playernum == 1 ? printf("What is your name? ") : printf("What is player 1's name? ");
            fgets(p1.name,29,stdin);
            p1.name[strcspn(p1.name, "\n")] = 0;

            playernum == 1 ? printf("What is the AI's name? ") : printf("What is player 2's name? ");
            fgets(p2.name,29,stdin);
            p2.name[strcspn(p2.name, "\n")] = 0;
        }
        goto restart;
    } else{printf("Thank You for Playing!");}
    
    return 0;
}
// allocates memory and creates the base board.
void createBoard(BoardVals *bv){
    bv->board = (char**)malloc(bv->size*10);
    for(int i = 0; i<bv->size;i++){
        bv->board[i] = (char*)malloc(bv->size);
        for(int j = 0;j<bv->size;j++){
            bv->board[i][j] = '-';
        }
    }
}
// draws the board and the row/col number
void drawBoard(BoardVals *bv){
    for(int i = 0;i<=bv->size;i++){
            i == 0 ? printf("  ") : printf(" %d  ",i-1);
    }
    printf("\n");
    for(int i = 0;i<bv->size;i++){
        printf("%d",i);
        for(int j = 0; j<bv->size;j++){
            printf("| %c ",bv->board[i][j]);
        }
        printf("|\n");
    }
}
void freeVals(BoardVals *bv){
    for(int i = 0; i<bv->size;i++){
        free(bv->board[i]);       
    }
    free(bv->board);
    bv->board = NULL;
}
// get the positions inputs from the player
int getpos(int max,char type[]){
    int input;
    do{
        printf("Enter your %s number: ",type);
        scanf("%d",&input);
        CLEAR;
    }while(input >max);
    return input;
}

bool checkWinner(Players *p, BoardVals *bv, char sym){
    // checks horizontal and vertical
    for(int i=0; i<bv->size;i++){
        if(p->rowspicked[i] == bv->size){return true;}
        if(p->colspicked[i] == bv->size){return true;}
    }
    // check diagonally starting from the left 
    bool diwon = true;
    for(int i=0; i<bv->size;i++){
        if(bv->board[i][i] != sym){diwon =false;break;}
    }
    if(diwon){return true;}
    // checks diagonally starting from the right
    diwon = true;
    for(int i=0; i<bv->size;i++){
        if(bv->board[i][(bv->size-1)-i] != sym){diwon =false;break;}
    }
    if(diwon){return true;}

    return false;
}
 
bool checkdraw(Players *p1, Players *p2, BoardVals *bv){
    int rowsFull = 0;
    // counts how many rowes are full
    for(int i = 0; i<bv->size;i++){
        if(p1->rowspicked[i] + p2->rowspicked[i] == bv->size){
            rowsFull++;
        }
    }
    return rowsFull == bv->size ? true : false;
}

void aiFuction(Players *p2, Players *p1, BoardVals *bv, int *pos){
    bool pickrandrow = true,pickrandcol = true, pickdi = true;
    int diagonalnum = 0;
    // finds if the player or it is close to winning in a row or column 
    for(int i = 0; i<bv->size;i++){
        // player is close to winning
        if(p1->rowspicked[i] - p2->rowspicked[i] == bv->size-1){
            pos[0] = i;
            pickrandrow = false;
            pickdi = false;
        }
        if(p1->colspicked[i] - p2->colspicked[i] == bv->size-1){
            pos[1] = i;
            pickrandcol = false;
            pickdi = false;
        }
        // AI is close to wining
        if(p2->rowspicked[i] - p1->rowspicked[i] == bv->size-1){
            pos[0] = i;
            pickrandrow = false;
            pickdi = false;
            break;
        }
        if(p2->colspicked[i] - p1->colspicked[i] == bv->size-1){
            pos[1] = i;
            pickrandcol = false;
            pickdi = false;
            break;
        }
    }
    if(pickdi){
        // checks if it is 1 away from wining diagonaly
        for(int i=0; i<bv->size;i++){
            if(bv->board[i][i] == 'X'){diagonalnum++;}
        }
        if(diagonalnum == bv->size-1){  
            // finds the golden position
            for(int i=0; i<bv->size;i++){
                pos[0] = i;pos[1] = i;
                if(bv->board[pos[0]][pos[1]] != 'X'){
                    pickrandcol = false; 
                    pickrandrow = false;
                    pickdi = false;
                    break;
                }
            }
        }
    }
    if(pickdi){
        diagonalnum = 0;
        for(int i=0; i<bv->size;i++){
            if(bv->board[i][(bv->size-1)-i] == 'X'){diagonalnum++;}
        }
        if(diagonalnum == bv->size-1){
            // finds the golden position
            for(int i=0; i<bv->size;i++){
                pos[0] = i;pos[1] = (bv->size-1)-i;
                if(bv->board[pos[0]][pos[1]] != 'X'){
                    pickrandcol = false;
                    pickrandrow = false;
                    pickdi = false;
                    break;
                }
            } 
        }
    }
    if(pickdi){
        // checks if the player is 1 away from winning diagnally
        diagonalnum = 0;
        for(int i=0; i<bv->size;i++){
            if(bv->board[i][i] == 'O'){diagonalnum++;}
        }
        if(diagonalnum == bv->size-1){    
            // finds the dangerous position
            for(int i=0; i<bv->size;i++){
                pos[0] = i;pos[1] = i;
                if(bv->board[pos[0]][pos[1]] != 'O'){
                    pickrandcol = false;
                    pickrandrow = false;
                    pickdi = false;
                    break;
                }
            }  
        }
    }
    if(pickdi){
        diagonalnum = 0;
        for(int i=0; i<bv->size;i++){
            if(bv->board[i][(bv->size-1)-i] == 'O'){diagonalnum++;}
        }
        if(diagonalnum == bv->size-1){
            // finds the dangerous position
            for(int i=0; i<bv->size;i++){
                pos[0] = i;pos[1] = (bv->size-1)-i;
                if(bv->board[pos[0]][pos[1]] != 'O'){
                    pickrandcol = false; 
                    pickrandrow = false;
                    pickdi = false;
                    break;
                }
            } 
        }
    }
    // picks random if the above statments wern't true
    do{
        if(pickrandrow){pos[0] = RANDINT(bv->size);}
        if(pickrandcol){pos[1] = RANDINT(bv->size);}
    }while(bv->board[pos[0]][pos[1]] != '-');
    if(pos[0] == bv->size){pos[0] -= 1;}
    if(pos[1] == bv->size){pos[1] -= 1;}
}
void clear(){
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif  
}