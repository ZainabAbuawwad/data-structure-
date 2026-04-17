#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILE_SIZE 1000
/*
Name : zainab abu awwad
ID : 1221410
Class : 2
*/

//structure
struct node
{
    char *addedstr;
    struct node *next;
    int index;
    int status;
};
struct node *undoStack = NULL;
struct node *redoStack = NULL;




//  queue
struct QueueNode
{
    char *str;
    struct QueueNode *next;
};

struct Queue
{
    struct QueueNode *front;
    struct QueueNode *rear;
};
struct Queue *q;



//////////////////////////////////////////////////////////////////////////
// functions prototypes
char *readInputFile();   // to load the text in the file
void printText();
void deQueue(struct Queue *q);   // dequeue text from queue
struct QueueNode *createNode(char *word);  // create a new node for the queue
struct node *createStack();    // for creating the queue
struct node *createStack();    // creating stack
void push(struct node **stack, char *word); // push new words to the stack
char* pop(struct node **stack);   // pop a word from the stack
char *getFront(struct Queue *q);  // to get the front of the queue
int findIndexInInitialText(char *wordToFind, char *initialText);//to find the index of the words
char *insertString(struct Queue *q, char *initialText);//to insert new text to the initial text
char* removeWord(char *initialText);//to remove specific word
char* Top( struct node* S );//get the top element of the stack
int checkOperation(struct node *stack);//check the type of operation that has happened on the stack node
void printStack(struct node *stack);//for printing undo stack and redo stack
int IsEmpty( struct node* S );
void MakeEmpty( struct node* S );
void readOutputFile(char* initialText);//to show the final text on output file
void undo(struct node **stack, struct node **redoStack, char* initialText);
void redo(struct node **stack, struct node **undoStack, char* initialText);

// function to read the stored equations from the input file
//////////readInputFile//////////
char* readInputFile(){

    FILE *input = fopen("original.txt", "r");
    if (input == NULL)
    {
        printf("Faild tO open the file. this file does not exist...\n");
        return;
    }

    char *text = malloc(MAX_FILE_SIZE * sizeof(char));
    if (text == NULL)
    {
        printf("out of space.\n");
        return;
    }

    text[0] = '\0';

    char line[MAX_FILE_SIZE];
    int counter = 0;

    while (fgets(line, sizeof(line), input))
    {
        // use function strtok to split the text
        char *word = strtok(line, " \n");

        while (word != NULL && counter < MAX_FILE_SIZE)
        {
            strcat(text, word);
            strcat(text, " ");


            counter += strlen(word) + 1;
            word = strtok(NULL, " \n");
        }
    }

    printf("text has been loaded to the memory...\n");



    fclose(input);
    return text;
}

///inserting new text to the initial text //////
char *insertString(struct Queue *q, char *initialText){
    char text[MAX_FILE_SIZE];
    char *newword;
    printf("Enter your new text:\n");
    // clear the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {

    }

    if (fgets(text, MAX_FILE_SIZE, stdin) != NULL)
    {
        int len = strlen(text);

        if (len > 0 && text[len - 1] == '\n')
        {
            text[--len] = '\0';
        }
    }
    // use strtok to split the text
    char *word = strtok(text, " ");
    while (word != NULL)
    {
        enQueue(q, word);         // enqueue each word
        word = strtok(NULL, " ");
    }

    // create a copy of the initial text
    char *initialTextCopy = strdup(initialText);
    if (initialTextCopy == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    char wordToFind[100];
    printf("Enter the word to insert new text after: ");
    scanf("%s", wordToFind);

    int index = findIndexInInitialText(wordToFind, initialTextCopy);
    if (index != -1)
    {
        printf("'%s' found at index %d in the initial text.\n", wordToFind, index);


        size_t updatedTextSize = strlen(initialText) + strlen(text) + strlen(wordToFind) + 2; // +2 for space and null terminator
        char *updatedText = (char *)malloc(updatedTextSize);
        if (updatedText == NULL)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        // copy the initial text up to the end of the found word
        strncpy(updatedText, initialText, index + strlen(wordToFind));
        updatedText[index + strlen(wordToFind)] = ' '; // add a space after the found word
        updatedText[index + strlen(wordToFind) + 1] = '\0';

        // concatenate the new text after the found word
        while (q->front != NULL)
        {
            newword = getFront(q); // get the front word from the queue
            deQueue(q);
            push(&undoStack, strdup(newword)); // push it to the undo stack
            undoStack->status=1;///for operation

            undoStack->index = index;///for index

            strcat(updatedText, newword);     // concatenate word to updated text
            strcat(updatedText, " ");

            free(newword);
        }
        // concatenate the remaining part of the initial text after the found word
        strcat(updatedText, initialText + index + strlen(wordToFind));

        printf("Updated text:\n%s\n", updatedText);


        free(initialTextCopy);


        return updatedText;
    }
    else
    {
        printf("'%s' not found in the initial text.\n", wordToFind);

        return initialText;
    }
}

///remove word from the text after inserting new text to the initial text
char* removeWord(char *initialText){
    char wordToFind[MAX_FILE_SIZE];
    printf("Enter the word you want to remove:\n");
    scanf("%s", wordToFind);

    int index = findIndexInInitialText(wordToFind, initialText);

    if (index != -1)
    {
        printf("'%s' found at index %d in the initial text.\n", wordToFind, index);

        char *token;

        char *result = (char *)malloc(MAX_FILE_SIZE * sizeof(char));
        if (!result)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        result[0] = '\0';

        // make a copy of the initial text to save the pointer from losing
        char *textCopy = strdup(initialText);
        if (!textCopy)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }



        token = strtok(textCopy, " ");
        while (token != NULL)
        {

            // if the token is not the word to remove, concatenate it to the result string
            if (strcmp(token, wordToFind) != 0)
            {

                strcat(result, token);
                strcat(result, " ");
            }
            token = strtok(NULL, " ");
        }

        push(&undoStack, strdup(wordToFind));

        undoStack->status = 0;///for operation

        undoStack->index = index;///for index



        // remove the extra space
        if (strlen(result) > 0)
        {
            result[strlen(result) - 1] = '\0';
        }

        printf("New text:\n%s\n", result);

        // Free memory allocated for result and textCopy
        return result;

    }
    else
    {
        printf("'%s' not found in the initial text.\n", wordToFind);
    }
}



//find the index of the word
int findIndexInInitialText( char *wordToFind,  char *initialText){
    const char *ptr = strstr(initialText, wordToFind);
    if (ptr != NULL)
    {
        return ptr - initialText;
    }
    return -1;
}


struct QueueNode *createNode(char *word){
    struct QueueNode *newNode = (struct QueueNode *)malloc(sizeof(struct QueueNode));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newNode->str = (char *)malloc(strlen(word) + 1);
    if (newNode->str == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newNode->str, word);
    newNode->next = NULL;
    return newNode;
}
/////////////create Queue
struct Queue *createQueue(){
    struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
    if (q == NULL)
    {
        printf("Memory overflow. Could not allocate memory.\n");
        exit(0);
    }
    q->front = q->rear = NULL;
    return q;
}
/////////insert Queue
void enQueue(struct Queue *q, char *word){
    struct QueueNode *temp = createNode(word);


    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
    }
    else
    {

        q->rear->next = temp;
        q->rear = temp;
    }
}

//////////dequeue function////////////////
void deQueue(struct Queue *q){
    if (q->front == NULL)
    {
        printf("Queue is empty.\n");
        return NULL;
    }
    struct QueueNode *temp = q->front;
    char *word = temp->str;
    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    free(temp);
    //printf("%s", word);
}
////// get front function ///////////////////////
char *getFront(struct Queue *q){

    if (q->front == NULL)
    {
        return NULL;
    }


    struct QueueNode *temp = q->front;


    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    return temp->str;
}

int IsEmpty( struct node* S ){

return S->next == NULL;

}
////make empty stack
void MakeEmpty( struct node* S ){

if( S == NULL )

printf( "Out of space!" );

else

while( !IsEmpty( S ))

pop( S );

}

// function to create a stack
//////////createStack//////////

struct node *createStack(){
    struct node *S;
    S = (struct node *)malloc(sizeof(struct node));
    if (S == NULL)
    {
        printf("Could not create stack. Stack is not found..\n");
        return NULL;
    }
    else
    {
        S->next = NULL;
    }

    return S;
}


// Function to return the top node of the stack
char* Top(struct node* S) {
    if (!IsEmpty(S)) {
        printf("%s", S->addedstr);
        return S->addedstr;
    } else {
        printf("Empty stack\n");
        return NULL;
    }
}

////function to push new word to the stack
void push(struct node **stack, char *word){
    struct node *newNode = (struct node *)malloc(sizeof(struct node));

    if (newNode == NULL)
    {
        printf("out of space...\n");
        return;
    }

    newNode->addedstr = strdup(word);


    newNode->next = *stack;

    *stack = newNode;
   // printf("element %s pushed to the stack. \n", word);
}

////function to pop a word from the stack
char *pop(struct node **stack){
    if (*stack == NULL)
    {
        printf("*ERROR the stack is empty, nothing to pop*\n");
        return NULL;
    }

    struct node *temp = *stack;

    *stack = (*stack)->next;
    char *poppedElement = strdup(temp->addedstr);
    free(temp);
    //printf("element %s popped from the stack.\n", poppedElement);
    return poppedElement;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
///undo stack function ///
void undo(struct node **stack, struct node **redoStack, char* initialText) {
    //pop the top of the stack
    char* wordToFind = pop(stack);

    // push the word that you have poped to the redo stack
    push(redoStack, wordToFind);

    // find the word in the initial text
    int index = findIndexInInitialText(wordToFind, initialText);

    if (index != -1)
    {
        printf("'%s' found at index %d in the initial text.\n", wordToFind, index);



        char *result = (char *)malloc(MAX_FILE_SIZE * sizeof(char));
        if (!result)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        result[0] = '\0';



        // make a copy of the initial text to save the pointer from losing
        char *textCopy = strdup(initialText);
        if (!textCopy)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }


        if((*stack)->status == 1){
             char *token;

        token = strtok(textCopy, " ");
        while (token != NULL)
        {

            // if the token is not the word to remove, concatenate it to the result string
            if (strcmp(token, wordToFind) != 0)
            {

                strcat(result, token);
                strcat(result, " ");
            }
            token = strtok(NULL, " ");
        }



        // remove the extra space
        if (strlen(result) > 0)
        {
            result[strlen(result) - 1] = '\0';
        }

        printf("New text:\n%s\n", result);



        } else if ((*stack)->status == 0) {



           // copy the initial text up to the found word
            strncpy(result, initialText, index);
            result[index] = '\0'; // Null terminate

            // Concatenate the word to be inserted
            strcat(result, wordToFind);

            // concatenate the remaining part of the initial text after the found word
            strcat(result, initialText + index);

            printf("New text:\n%s\n", result);
        }

        // update initialText to the result
        strcpy(initialText, result);



    } else {
        printf("Word not found in the initial text.\n");
    }

}
//////////////////////////////////////////////
//////////////////////////////////////////////
void redo(struct node **stack, struct node **undoStack, char* initialText){
   // pop the top word from the undo stack
        char* wordToFind = pop(stack);

        // push the word to the redo stack
       push(undoStack, wordToFind);

        // find the word in the initial text
    int index = findIndexInInitialText(wordToFind, initialText);

    if (index != -1)
    {
        printf("'%s' found at index %d in the initial text.\n", wordToFind, index);

        char *token;

        char *result = (char *)malloc(MAX_FILE_SIZE * sizeof(char));
        if (!result)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }

        result[0] = '\0';



        // make a copy of the initial text to save the pointer from losing
        char *textCopy = strdup(initialText);
        if (!textCopy)
        {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }


        if((*stack)->status == 0){


        token = strtok(textCopy, " ");
        while (token != NULL)
        {

            // if the token is not the word to remove, concatenate it to the result string
            if (strcmp(token, wordToFind) != 0)
            {

                strcat(result, token);
                strcat(result, " ");
            }
            token = strtok(NULL, " ");
        }



        // remove the extra space
        if (strlen(result) > 0)
        {
            result[strlen(result) - 1] = '\0';
        }

        printf("New text:\n%s\n", result);





        } else if ((*stack)->status == 1) {


 // copy the initial text up to the found word
            strncpy(result, initialText, index);
            result[index] = '\0'; // Null terminate

            // Concatenate the word to be inserted
            strcat(result, wordToFind);

            // Concatenate the remaining part of the initial text after the found word
            strcat(result, initialText + index);

            printf("New text:\n%s\n", result);
        }

        // update initialText to the result
        strcpy(initialText, result);



    } else {
        printf("Word not found in the initial text.\n");
    }

}


// Function to print the stack
void printStack(struct node *stack){
    struct node *temp = stack;
    while (temp != NULL)
    {
        if(temp->status == 1){
            printf("%s  inserted   %d\n", temp->addedstr,temp->index);
        }else if(temp->status== 0){

        printf("%s  removed   %d\n", temp->addedstr,temp->index);
    }

        temp = temp->next;
    }
}
//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////function to read the output /////////
void readOutputFile(char* initialText){
// Open the output file
    FILE* output = fopen("result.txt", "w");
    // check if the file was opened
    if (output == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    printf("text has been printed on the result file...\n");
    fprintf(output, " \n%s\n", initialText);


    fclose(output);


}
int main()
{

    char* initialText;
    struct Queue *q;
    q = createQueue();
    undoStack = createStack();
    redoStack = createStack();
    int choice;

    do
    {
        printf("\nMenu:\n");
        printf("1. Load the input file which contains the initial text.\n");
        printf("2. Print the loaded text.\n");
        printf("3. Insert strings to the text\n");
        printf("4. Remove strings from the text. \n");
        printf("5. Perform Undo operation\n");
        printf("6. Perform Redo operation\n");
        printf("7. Print the Undo Stack and the Redo stack \n");
        printf("8. Save the updated text to the output file. \n");
        printf("9. Exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:

           initialText= readInputFile();
            break;

        case 2: printf("Initial text:\n%s\n", initialText);

            break;
        case 3:
          initialText= insertString(q,initialText);

            break;
        case 4:
             initialText =  removeWord(initialText);

            break;
        case 5:undo(&undoStack,&redoStack,initialText);


            break;
        case 6:redo(&redoStack,&undoStack,initialText);

            break;
        case 7:printStack(undoStack);
               printf("_________________________________________________\n");
               printStack(redoStack);


            break;

        case 8:
            readOutputFile(initialText);


            break;

        case 9:
            printf("Exiting the program.\n");

            return 0;
        default:
            printf("Invalid choice. Please enter a number between 1 and 14.\n");
        }
    } while (choice != 9);

    return 0;
}
