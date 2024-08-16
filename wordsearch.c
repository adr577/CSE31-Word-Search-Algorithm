#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*

Wordsearch created by Adrian Hernandez

gotten help from:  friend(curr software engineer) Terrence, tutor at Wyzant, John
*/

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
bool searchHelper(char **arr, char *word, int r, int c, int i, int n, int **s);
void convertToUpperCase(char *word);
bool boundsAreValid(int row, int col);

int bSize;

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
	// This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    // Your implementation here...

    for (int i = 0; i < bSize;i++){
        for (int j = 0; j < bSize; j++){
            printf("%c ", *(*(arr + i) + j));
        }

        printf("\n");
    }
    printf("\n");
}


void searchPuzzle(char** arr, char* word){

    int **s_matrix = (int **)malloc(bSize * sizeof(int *)); //allocates memory for a 2D arry to store results
    for (int i = 0; i < bSize; i++)
    {
        *(s_matrix + i) = (int *)malloc(bSize * sizeof(int));
        for (int j = 0; j < bSize; j++)
        {
            *(*(s_matrix + i) + j) = 0; // initalizes all elements in "sol" arr to 0, 
        }
    }

    int len = (strlen(word) - 1); //len of word pointed to by word
    int level = 0;
    for (int k = 0; k < len;k++){
        convertToUpperCase(word);
    }

    // once all of the letters are uppercased, we search for the word
    for (int r = 0; r < bSize;r++){
        for (int c = 0; c < bSize; c++){ //nest iterates over each cell of puzzle grid
            if(*(*(arr + r) + c) == *(word)){ //if char in (row, col) matches first char of word, proceed
                level = searchHelper(arr, word, r, c, 0, len, s_matrix); //pass in word srch grid, word, row, col, i = index, word length, 0 int grid)
                if(level){  //if it returns true
                    printf("Word found!\n");
                    printf("Printing the search path:\n");

                    for (int a = 0; a < bSize; a++){   
                        for (int b = 0; b < bSize; b++){
                            printf("%d\t", *(*(s_matrix + a) + b)); // sol[a][b]);
                        }
                        printf("\n");
                    }
                    return;
                }
            }
        }
    }
    printf("Word not found!\n"); //if the loops complete without finding a word

    free(s_matrix);  //frees sol dynammically
}

bool boundsAreValid(int row, int col)
{ // checks whether a given row and column pair falls within the bounds of a square grid of size bSize.
    return (col >= 0) && (col < bSize) && (row >= 0) && (row < bSize);
}

bool searchHelper(char **arr,char *word, int r, int c, int i, int wordLen,int **s){ // i is for the starting indx of the word, and used to keep track of curr pos
    int x[] = {-1, -1, -1, 0, 0, 1, 1, 1}; //one row up, no change, (1) one row down*(south, sw, se)
                                                       //corresponds to a change in row and column indices (north, nw, ne ... etc)
    int y[] = {-1, 0, 1, -1, 1, -1, 0, 1}; //changes in column index for each direction (-1) one col to left

    if (*(word + i) != *((*(arr + r)) + c) || i > (wordLen)){  // if char at the position doesnt match char at word 
        return false;
        }

    if(i == wordLen){
        *(*(s + r) + c) = *(*(s + r) + c) * 10 + i + 1;  /*retrieves value in 2D arr
                                                            shifts digits of curr val to the left
                                                            creating space for the new digits to be appended */ //(i+1 represents len of word found so far)
        return true;
    }

    for (int dir = 0; dir < 8;dir++){  // iterates through 8 directions around a given position in grid, recursively searching for the word in each direction. if word is found in an dir, update the solution matrix and returns true   
        if(i == wordLen){  //checks curr index, if true then word has been found
            return true; 
        }

        if(boundsAreValid(r+x[dir], c + y[dir])){  //prevents accessing elements outside grid , *(r + x + dir)    *(c + y + dir)
            if (searchHelper(arr, word, r + x[dir], c + y[dir], i + 1,wordLen,s)){ //updates coordinates
                *(*(s + r) + c) = *(*(s + r) + c) * 10 + i + 1; //if the recurive remains, true, updates value in 2D array 's" at (r,c) with i + 1
                return true;
            }
        }
    }

    return false;
}

void convertToUpperCase(char *word)
{
    char *c = word; // points to the first char of word
    while (*c)
    { // iterates as long as c isn't null
        if (*c >= 'a' && *c <= 'z')
        {
            *c -= 32; // In ASCII, the difference between lowercase and uppercase letters is 32.
            //converts to uppercase
        }
        c++; // next char in the string
    }
}
