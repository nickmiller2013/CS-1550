/* Username: nam99
   CS 449 Project
   11/29/2015
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_DIE 5

#define RAND_ROLL dice_roll();

void printRoll();
void assignPoints();
void upperCat();
void lowerCat();
void displayAll();
void rollAgain();
void rollAgainAgain(int);
int compare(const void *,const void *);

int numRolls[MAX_DIE];
int upper[6];
int lower[7];
int bonus = 0;
int lowerScore = 0;
int upperScore = 0;

int main(){
    int count;
    int i;
    int diceReroll;

    
    //this will assign each value of upper to 0 signifying it has not been used
    for(i = 0; i < 6; i++){
        upper[i] = 0;
    }
    
    //this will assign each value of lower to 0 signifying it has not been used
    for(i = 0; i < 7; i++){
        lower[i] = 0;
    }
    
    srand(time(NULL));

    
    //This will count the amount of turns that you have.
    for(count = 13; count >= 0; count--){
        printf("You are at %d turns!\n",count);
        
        //This will assign random numbers to the 5 spots
        for(i = 0; i < 5; i++){
            numRolls[i] = RAND_ROLL;
        }
        printf("\n");
        
        
        //This will print out the 5 rolled numbers
        printRoll();
        
        //The roll again occurs 3 times because there was some glitch in my program. But now it works.
        //You just have to enter a letter when asked hows it going
        
        //Asks the user if they want to roll again.
        rollAgainAgain(count);
        
        //Asks again.
        rollAgain();
        
        if(count == 13){
            rollAgain();
        }
        
        //Sorts the numbers
        
        qsort(numRolls, MAX_DIE, sizeof(int), compare);
        
        //Assigns the points
        assignPoints();
        
        
        //Displays everything
        displayAll();

        
    }
    return 0;
}

void rollAgainAgain(int count){
    char input[MAX_DIE * 2];
    char *token[MAX_DIE * 2];
    int i;
    int key = 0;
    int newVal[MAX_DIE];
    
    for(i = 0; i < MAX_DIE; i++){
        newVal[i] = 0;
    }
    // get user input
    char s;
    printf("\nEverything okay?");
    scanf("%c", &s);
    printf("Just keep going! :) \n");
    if(count != 13){
        printf("\nWhich dice do you want to re-roll: ");
        fgets(input, sizeof(input), stdin);
    
    // use a token here because to confusing not to. Went to ta
        i = 0;
        token[0] = strtok(input, " ");
        while(token[i] != NULL){
            newVal[i] = 0;
            int reRoll = atoi(token[i]) - 1;
            newVal[reRoll] = 1;
            
            key = i;
            i++;
            token[i] = strtok(NULL, " ");
        }
    
        // reroll each die if necessiary
        for(i = 0; i < MAX_DIE; i++){
            if(newVal[i] == 1){
            numRolls[i] = RAND_ROLL
            }
        }
        printRoll();
    }else{
        printf("\n");
        fgets(input, sizeof(input), stdin);
        
        // use a token here because to confusing not to. Went to ta
        i = 0;
        token[0] = strtok(input, " ");
        while(token[i] != NULL){
            newVal[i] = 0;
            int reRoll = atoi(token[i]) - 1;
            newVal[reRoll] = 1;
            
            key = i;
            i++;
            token[i] = strtok(NULL, " ");
        }
        
        // reroll each die if necessiary
        for(i = 0; i < MAX_DIE; i++){
            if(newVal[i] == 1){
                numRolls[i] = RAND_ROLL
            }
        }
    }
    
}


void rollAgain(){
    char input[MAX_DIE * 2];
    char *token[MAX_DIE * 2];
    int i;
    int key = 0;
    int newVal[MAX_DIE];

    for(i = 0; i < MAX_DIE; i++){
        newVal[i] = 0;
    }
    // get user input
    int s;
    
    printf("\nWhich dice do you want to re-roll: ");
    fgets(input, sizeof(input), stdin);
    
    // use a token here because to confusing not to. Went to ta
    i = 0;
    token[0] = strtok(input, " ");
    while(token[i] != NULL){
        newVal[i] = 0;
        int reRoll = atoi(token[i]) - 1;
        newVal[reRoll] = 1;
        
        key = i;
        i++;
        token[i] = strtok(NULL, " ");
    }
    
    // reroll each die if necessiary
    for(i = 0; i < MAX_DIE; i++){
        if(newVal[i] == 1){
            numRolls[i] = RAND_ROLL
        }
    }
    printRoll();
    
}


//Prints the roll of the dice
void printRoll(){
    int i;
    printf("\nYour roll is:\n");
    for(i = 0; i < 5; i++)
        printf("%d ", numRolls[i]);
    printf("\n\n");
}


//Helper function to get the random dice.

int dice_roll(){
    unsigned int output;
    int i = open("/dev/dice", O_RDONLY);
    int n = read(i, &output, sizeof(int));
    
    return output;
}

//Function that compares the numbers. Obtained from qsort implementation page.
int compare(const void *this, const void *that){
    return (*(int*)this - *(int*)that);
}

void assignPoints(){
    

    int select;
    
    
    printf("\nPlace dice into:\n");
    printf("1) Upper Section\n");
    printf("2) Lower Section\n");
    printf("\nSelection: ");

    scanf("%d", &select);
    
    
    if(select == 1){
        //Goes to uppercat to do the upper level parts
        upperCat();
    }else{
        //Goes to lowercat to do the lower level parts

        lowerCat();
    }
    
    
    
    
    
    
    
    
    
}

//This displays all the values
void displayAll(){
    printf("\nYour score so far is: %d\n\n", (upperScore + lowerScore + bonus));
    
    if(upper[0] == 0){
        printf("\n\nOnes:");
    }else if(upper[0] > 0){
        printf("\n\nOnes: %d", upper[0]);
    }
    
    if(upper[3] == 0){
        printf("\t\t\tFours:  \n\n");
    }else if(upper[3] > 0){
        printf("\t\t\tFours: %d\n\n", upper[3]);
    }
    
    if(upper[1] == 0){
        printf("Twos:");
    }else if(upper[1] > 0){
        printf("Twos: %d", upper[1]);
    }
    
    if(upper[4] == 0){
        printf("\t\t\tFives:  \n\n");
    }else if(upper[4] > 0){
        printf("\t\t\tFives: %d\n\n", upper[4]);
    }
    
    if(upper[2] == 0){
        printf("Threes:");
    }else if(upper[2] > 0){
        printf("Threes: %d", upper[2]);
    }
    
    if(upper[5] == 0){
        printf("\t\t\tSixes:  \n");
    }else if(upper[5] > 0){
        printf("\t\t\tSixes: %d\n", upper[5]);
    }
    
    printf("Upper Section bonus: %d\n\n", bonus);
    
    
    if(lower[0] == 0){
        printf("Three of a kind:");
    }else if(lower[0] > 0){
        printf("Three of a kind: %d", lower[0]);
    }
    
    if(lower[1] == 0){
        printf("\t\t\tFour of a kind:  \n\n");
    }else if(lower[1] > 0){
        printf("\t\t\tFour of a kind: %d\n\n", lower[1]);
    }
    
    if(lower[2] == 0){
        printf("Small Straight:");
    }else if(lower[2] > 0){
        printf("Small Straight: %d", lower[2]);
    }
    
    if(lower[3] == 0){
        printf("\t\t\tLarge Straight:  \n\n");
    }else if(lower[3] > 0){
        printf("\t\t\tLarge Straight: %d\n\n", lower[3]);
    }
    
    if(lower[4] == 0){
        printf("Full House:");
    }else if(lower[4] > 0){
        printf("Threes: %d", lower[4]);
    }
    
    if(lower[5] == 0){
        printf("\t\t\tYahtzee:  \n\n");
    }else if(lower[5] > 0){
        printf("\t\t\tYahtzee: %d\n\n", lower[5]);
    }
    
    if(lower[6] == 0){
        printf("Chance:");
    }else if(lower[6] > 0){
        printf("Chance: %d\n\n", lower[6]);
    }

}

//Assigns points correctly
void upperCat(){
    int i;
    
    int total = 0;
    
    int selection;
    
    printf("\nPlace dice into:\n");
    printf("1) Ones\n");
    printf("2) Twos\n");
    printf("3) Threes\n");
    printf("4) Fours\n");
    printf("5) Fives\n");
    printf("6) Sixes\n");
    
    scanf("%d", &selection);
    
    for(i = 0; i < 5; i++){
        if(numRolls[i] == selection){
            total = total + selection;
        }
    }
    
    //Assigns the score to the upperSelection
    upper[selection - 1] = upper[selection - 1] + total;
    upperScore =upperScore + total;
    
    //Assigns bonus
    if(upperScore > 62){
        bonus = 35;
    }

}

//Assigns points correctly
void lowerCat(){
    int i;
    int total;
    int selection;
    printf("\nPlace dice into:\n");
    printf("1) Three of a kind\n");
    printf("2) Four of a kind\n");
    printf("3) Small Straight\n");
    printf("4) Large Straight\n");
    printf("5) Full House\n");
    printf("6) Yahtzee\n");
    printf("7) Chance\n\n");
    scanf("%d", &selection);
    
    if(selection == 1){
        //Check for 3 of a kind.
        
        printf("Three of a kind");
        int freq[6];
        
        for(i = 0; i < 6; i++)
            freq[i] = 0;
        
        total = 0;
        
        for(i = 0; i < 5; i++){
            int key = numRolls[i];
            freq[key - 1] += 1;
            total += key;
        }
        
         //If freq is greater than 2 then then the total will be added to the lower array value at 0
        
        for(i = 0; i < 6; i++){
            printf("freq[i] = %d\n",freq[i]);
            if(freq[i] > 2){
                lower[0] += total;
                lowerScore += total;
                break;
            }
        }
        
        
    }
    
    
    if(selection == 2){
        //Check for 4 of a kind.
        printf("Three of a kind");
        int freq[6];
        
        for(i = 0; i < 6; i++)
        freq[i] = 0;
        total = 0;
        
        for(i = 0; i < 5; i++){
            int key = numRolls[i];
            freq[key - 1] += 1;
            total += key;
        }
        
        //If freq is greater than 3 then then the total will be added to the lower array value at 1
        for(i = 0; i < 6; i++){
            if(freq[i] > 3){
                lower[1] += total;
                lowerScore += total;
                break;
            }
        }
        
        
    }
    
    //These next few are easier to do because of the fact it is sorted.
    //The full house
    if(selection == 3){
    
        printf("\n\nFull House");
        if(((numRolls[0] == numRolls[1]) && (numRolls[2] == numRolls[3]) && (numRolls[3] == numRolls[4])) || ((numRolls[0] == numRolls[1]) && (numRolls[1] == numRolls[2]) && (numRolls[3] == numRolls[4]))){
            lowerScore += 25;
            lower[2] = 25;
        }
    }
    
    //The small straight
    if(selection == 4){
        if((numRolls[0] < numRolls[1] && numRolls[1] < numRolls[2] && numRolls[2] < numRolls[3]) ||(numRolls[1] < numRolls[2] && numRolls[2] < numRolls[3] && numRolls[3] < numRolls[4])){
            lower[3] = 30;
            lowerScore += 30;
        }
    }
    
    //The large straight
    if(selection == 5){
        if(numRolls[0] < numRolls[1] && numRolls[1] < numRolls[2] && numRolls[2] < numRolls[3] && numRolls[3] < numRolls[4]){
            lower[4] = 30;
            lowerScore += 30;
        }
    }
    
    //The yahtzee
    if(selection == 6){
        if(numRolls[0] == numRolls[1] && numRolls[1] == numRolls[2] && numRolls[2] == numRolls[3] && numRolls[3] == numRolls[4]){
            lowerScore += 50;
            lower[5] = 50;
        }
    }
    
    //The chance
    if(selection == 7){
        total = 0;
        for(i = 0; i < 5; i++){
            total += numRolls[i];
        }
        lower[6] = total;
        lowerScore += total;
    }
}
    
    
    
    
    
    
    
    
    


