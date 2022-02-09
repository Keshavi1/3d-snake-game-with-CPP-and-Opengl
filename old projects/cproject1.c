#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


/**
    This is a small project that i used mainly to learn c. You're able to make a list for elements and save it to a .txt file.
    You can also open a .txt file. You can mess around with this for a bit if you want. 
*/


//creates the structure of the dynamic list
typedef struct{
    char** list;
    size_t itemNum;
    size_t itmSize;
    size_t lstSize;

}List;
//mainlist
// initalizes lists variables and allocates memory
void initList(List *lst, size_t initSize){
    lst->list = (char**)malloc(initSize);//memory for whole list
    
    lst->itemNum = 0;
    lst->itmSize = initSize; lst->lstSize = initSize;
}
void appendItem(List *lst,char *string,bool loaded){
    
    
    //adds any nessacery extras to the word.
    char buffer[255] = " ";
    if(!loaded && lst->itemNum >0){
        sprintf(buffer, "%d: %s",(int)lst->itemNum,string);
    } else if(!loaded){
        sprintf(buffer, "---%s",string);
    } else {
        sprintf(buffer, "%s",string);
    }
    //increases memory of list by the size of the item
    lst->lstSize += strlen(buffer)+1;
    lst->list = (char**)realloc(lst->list, lst->lstSize);
    // allocates memory for the string and copys the string
    lst->list[lst->itemNum] = (char*)malloc(strlen(buffer)+1);
    strcpy(lst->list[lst->itemNum], buffer);   
    lst->itemNum++;
}
void removeItem(List *lst,int index){
    char shiftStr[255];
    for(int dex1 = index; dex1<lst->itemNum;dex1++){
        // nulls out the current word
        for(int dex2 = 0;dex2<strlen(lst->list[dex1]);dex2++){
            lst->list[dex1][dex2] = '\0';
        }
        if(!lst->list[dex1+1]){break;}
        // removes the starting number from the word
        while(isdigit(lst->list[dex1+1][0])){
            for(int dex2 = 0;dex2<strlen(lst->list[dex1+1]);dex2++){
                if(lst->list[dex1+1][dex2]){
                    lst->list[dex1+1][dex2] = lst->list[dex1+1][dex2+1];
                }
            }
        }
        // gets the size of the next string
        lst->itmSize = strlen(lst->list[dex1+1])+1;
        lst->list[dex1] = (char*)realloc(lst->list[dex1], lst->itmSize);
        // sets the new starting index
        sprintf(shiftStr,"%d%s",dex1,lst->list[dex1+1]);
        strcpy(lst->list[dex1], shiftStr);
    }
    lst->itemNum--;
}
void freeList(List *lst){
    for(int i = 0; i < lst->itemNum;i++){
        free(lst->list[i]);
        lst->list[i] = NULL;
    }
    free(lst->list);
    lst->list = NULL;
    lst->itmSize = 0; lst->lstSize = 0;
    lst->itemNum=0;
    
}

int main(void) {
    //initalizing variables
    FILE *f;
    List lst;
    initList(&lst, 100);
    int choice;
    bool runing = true;
    int tsize = 220;
    char text[tsize];
    printf("Would you like to load a file (yes/no): ");
    scanf("%s",text);
    getchar();
    for(int i = 0; i < strlen(text);i++){text[i] = tolower(text[i]);}
    if(strcmp(text,"yes") == 0){
        printf("File Name: ");
        scanf("%s",text);
        getchar();
        f = fopen(text,"r"); //check if the new file is nesasery
        if(f == NULL){
            printf("unable to open file.");
            exit(1);
        }
        while(fgets(text,tsize+5,f)){
            appendItem(&lst,text,true);
        }
        fclose(f);
        
        
    }else{
        printf("What is the name of your list: ");
        fgets(text,tsize+5,stdin);
        appendItem(&lst,text,false);
    }
    while(runing){
        printf(" What would you like to do. \n 1: add Item\n 2: remove item\n 3: see current list\n 4: save and exit\n(enter number):");
        scanf("%d",&choice); 
        getchar();
        switch(choice){
            case 1: //add item
                printf("Enter Item: ");
                fgets(text,tsize+5,stdin);
                appendItem(&lst,text,false);
                break;
            case 2: //removes item
                printf("Enter item number: ");
                scanf("%d",&choice);
                if(choice < lst.itemNum && choice >0){
                    removeItem(&lst,choice);
                } else{printf("--invalid number--\n");}
                break;
            case 3: //shows item
                for(int i = 0;i<lst.itemNum;i++){printf("%s",lst.list[i]);}
                break;
            case 4: //saves a file
                printf("File name: ");
                scanf("%s",text);
                sprintf(text,"%s%s",text,".txt");
                f = fopen(text, "w");
                for(int i = 0;i<lst.itemNum;i++){fprintf(f,"%s",lst.list[i]);}
                fclose(f);
                runing = false;
                break;
            default:printf("--invalid input--\n");
        } 
    }
    freeList(&lst);  
    return 0;
}