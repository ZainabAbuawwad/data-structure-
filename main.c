#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 11
/*
name : zainab abu awwad
id : 1221410
section : 2
*/
// AVL Tree Structure and Functions
typedef struct AVLnode *AVLNode;
struct AVLnode {
    char word[15];
    AVLNode Left;
    AVLNode Right;
    int Height;
    //i assigned one to it
    int frequency;
};

AVLNode tree = NULL;
////a linked list to insert words in
struct node{
   char word[15];
   int frequency;
   struct node* next;

};
//a hash structure with an array of linked list cause i chose to use the separate chaining way to solve the collision
struct hash{
   int size;
   struct node * table[MAX_SIZE];


};

struct hash* hashtable=NULL;

////prototypes////////////
void readFile();////read from file
struct AVLnode* Insert(char data[], struct AVLnode* T);//////insertion to create the avl tree and insert new node
struct AVLnode* MakeEmpty(struct AVLnode* T );
int Height( struct AVLnode* P );
int Max( int Lhs, int Rhs );
struct AVLnode* SingleRotateWithLeft( struct AVLnode* K2 );
struct AVLnode* SingleRotateWithRight( struct AVLnode* K1 );
struct AVLnode* DoubleRotateWithLeft(struct AVLnode* K3 );
struct AVLnode* DoubleRotateWithRight( struct AVLnode* K1 );
void PrintInOrder(struct AVLnode* t);
struct AVLnode* Delete(char data[], struct AVLnode* T);////to delete node from the tree
struct AVLnode* minRight(struct AVLnode* T);////find the min node in the right side
struct AVLnode* DeleteFrequency(char data[], struct AVLnode* T);
/////////////////////////////////////////////////////////////////////////
///start with hashing
/////////////////////////////////////////////////////////////////////////
struct hash* createhashtable(int size);
int hashfunction(char *word,int tablesize);
void inserttotable(struct hash* hashtable, char *word, int frequency);
void traverseAVLAndInsert(struct AVLnode* root, struct hash *hashtable);
void printStatistics(struct hash* hashtable, int threshold);
void deleteFromTable(struct hash* hashtable, char *word);
void searchAndPrintFrequency(struct hash* hashtable, char *word);



int main() {
    int choice;
    tree = MakeEmpty(tree);

    do {
        printf("\nMenu:\n");
        printf("1. Load data from the file\n");
        printf("2. Print the AVL tree\n");
        printf("3. Insert a word to the AVL tree\n");
        printf("4. Delete a word from the AVL tree\n");
        printf("5. Print the words as sorted in the AVL tree\n");
        printf("6. Create the Hash Table\n");
        printf("7. Insert a word to the Hash table\n");
        printf("8. Delete a word from the hash table\n");
        printf("9. Search for a word in the hash table and print its frequency\n");
        printf("10. Print words statistics\n");
        printf("11. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:////read the data from the file
            readFile();
            break;
        case 2:PrintInOrder(tree);////the tree has been created by using insert function in the readFile function///
               printf("\n node has been created...\n");

            break;
        case 3: {////insert new word to the tree
            char word[15];
            printf("Enter the word to insert: ");
            scanf("%s", word);
            tree = Insert(word, tree);
            printf("word has been inserted...\n");
            break;
        }
        case 4:{/////delete a node from the tree

    char word[15];
    printf("Enter the word to delete: ");
    scanf("%s", word);

    int op;
    printf("If you want to delete the node completely, enter 1\n");
    printf("If you want to reduce the frequency if it's more than one, enter 2\n");
    scanf("%d", &op);

    switch(op) {
        case 1:
            tree = Delete(word, tree);
            printf("Node has been deleted...\n");
            break;
        case 2:
            tree = DeleteFrequency(word, tree);
            break;
        default:
            printf("Invalid option\n");
            break;
    }
    break;
}

        case 5:////print the AVL tree
            printf("In-order traversal of AVL tree:\n");
            PrintInOrder(tree);
            printf("\n");
            break;
        case 6:if (hashtable != NULL) {
        // If the hashtable already exists, free it first
        for (int i = 0; i < hashtable->size; i++) {
            struct node *current = hashtable->table[i];
            while (current != NULL) {
                struct node *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(hashtable->table);
        free(hashtable);
    }

    hashtable = createhashtable(MAX_SIZE);
    if (hashtable != NULL) {
        traverseAVLAndInsert(tree,hashtable);
        printf("hash table has been created...\n");
    } else {
        printf("Failed to create the hash table.\n");
    }

            break;
        case 7:
            {////insert new word to the tree
            if (hashtable == NULL) {
        printf("Hash table is not created yet. Please create the hash table first (Option 6).\n");
        break;
    }
    char word[15];
    printf("Enter the word to insert: ");
    scanf("%s", word);

    // Insert the word into the hash table with initial frequency 1
    inserttotable(hashtable, word, 1);
  printf("word has been inserted...\n");
    break;
}

            break;
        case 8:{
            if (hashtable == NULL) {
        printf("Hash table is not created yet. Please create the hash table first (Option 6).\n");
        break;
    }
    char word[15];
    printf("Enter the word to delete: ");
    scanf("%s", word);
    deleteFromTable(hashtable, word);


}
            break;
        case 9:
            if (hashtable == NULL) {
        printf("Hash table is not created yet. Please create the hash table first (Option 6).\n");
        break;
    }
    char word[15];
    printf("Enter the word to search: ");
    scanf("%s", word);
    searchAndPrintFrequency(hashtable, word);
    break;
            break;
        case 10:
            {int threshold;
            printf("Enter number of threshold: ");
                scanf("%d", &threshold);
             printStatistics(hashtable,threshold);
             printf("___________________________\n");

            }
            break;
        case 11:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 11.\n");
        }
    } while (choice != 11);

    return 0;
}


////to read data from file ///////////
void readFile() {
    FILE* input;
    char array[100];
    char word[50];
    int j = 0;


    input = fopen("input.txt", "r");
    if (input == NULL) {
        printf("Error opening file...\n");
        return NULL;
    }

    while (fgets(array, sizeof(array), input) != NULL) {
        int i = 0;
        while (array[i] != '\0') {
            if ((array[i] >= 'a' && array[i] <= 'z') || (array[i] >= 'A' && array[i] <= 'Z')) {
                word[j++] = tolower(array[i]);
            } else if (array[i]== ' ') {
                if (j > 0) {
                    word[j] = '\0';
                    tree = Insert(word, tree);
                    j = 0;
                }
            }
            i++;
        }
        if (j > 0) {
            word[j] = '\0';
            tree = Insert(word, tree);
            j = 0;
        }
    }

    fclose(input);
    printf("Data has been loaded...\n");
}

/////MakeEmpty/////////////////
struct AVLnode* MakeEmpty(struct AVLnode* T) {
    if (T != NULL) {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}
//get the height of the tree for every node
int Height(AVLNode P) {
    if (P == NULL)
        return -1;
    else
        return P->Height;
}
//get max side i mean max height
int Max(int Lhs, int Rhs) {
    return Lhs > Rhs ? Lhs : Rhs;
}
////this function used just when the node has a left child /////////////
struct AVLnode* SingleRotateWithLeft(struct AVLnode* K2) {
    AVLNode K1;
    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;
    K2->Height = Max(Height(K2->Left), Height(K2->Right)) + 1;
    K1->Height = Max(Height(K1->Left), K2->Height) + 1;
    return K1;
}
////this function used just when the node has a right child /////////////
struct AVLnode* SingleRotateWithRight(struct AVLnode* K1) {
    AVLNode K2;
    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;
    K1->Height = Max(Height(K1->Left), Height(K1->Right)) + 1;
    K2->Height = Max(Height(K2->Right), K1->Height) + 1;
    return K2;
}
/////this function used when the problem is right of left /////////////
struct AVLnode* DoubleRotateWithLeft(struct AVLnode* K3) {
    K3->Left = SingleRotateWithRight(K3->Left);
    return SingleRotateWithLeft(K3);
}
/////this function used when the problem is left of right ///////////
struct AVLnode* DoubleRotateWithRight(struct AVLnode* K1) {
    K1->Right = SingleRotateWithLeft(K1->Right);
    return SingleRotateWithRight(K1);
}

//////create the tree and insert new node to the tree /////////////
struct AVLnode* Insert(char data[], struct AVLnode* T) {
    if (T == NULL) {
            ////malloc the tree in the memory ///////
        T = (struct AVLnode*)malloc(sizeof(struct AVLnode));
        if (T == NULL) {
            printf("Out of space!!!\n");

        } else {
            strcpy(T->word, data);
            T->frequency = 1;//////frequency start with one
            T->Height = 0;
            T->Left = T->Right = NULL;
        }
        ////comparing by using strcmp because we compare strings
    } else if (strcmp(data,T->word)<0) {
        T->Left = Insert(data, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2) {
            if (strcmp(data,T->Left->word)<0) {
                T = SingleRotateWithLeft(T);
            } else {
                T = DoubleRotateWithLeft(T);
            }
        }
    } else if (strcmp(data,T->word)>0) {
        T->Right = Insert(data, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2) {
            if (strcmp(data,T->Right->word)>0) {
                T = SingleRotateWithRight(T);
            } else {
                T = DoubleRotateWithRight(T);
            }
        }
        ////when the words the same we increase the frequency
    } else if (strcmp(data, T->word) == 0) {
        T->frequency++;
    }

    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;
    return T;
}
//////delete node from tree//////
struct AVLnode* Delete(char data[], struct AVLnode* T) {
    struct AVLnode* temp;

    if (T == NULL) {
        printf("Node not found\n");
        return NULL;
    }

    if (strcmp(data, T->word) < 0) {
        T->Left = Delete(data, T->Left);
    } else if (strcmp(data, T->word) > 0) {
        T->Right = Delete(data, T->Right);
    } else {
        if (T->Left == NULL) {
            temp = T->Right;
            free(T);
            return temp;
        } else if (T->Right == NULL) {
            temp = T->Left;
            free(T);
            return temp;
        }

        temp = minRight(T->Right);
        strcpy(T->word, temp->word);
        T->frequency = temp->frequency;
        T->Right = Delete(temp->word, T->Right);
    }

    // new height
    T->Height = 1 + Max(Height(T->Left), Height(T->Right));

    // balance the node
    int balance = Height(T->Left) - Height(T->Right);

    if (balance > 1) {
        if (Height(T->Left->Left) >= Height(T->Left->Right)) {
            T = SingleRotateWithLeft(T);
        } else {
            T = DoubleRotateWithLeft(T);
        }
    } else if (balance < -1) {
        if (Height(T->Right->Right) >= Height(T->Right->Left)) {
            T = SingleRotateWithRight(T);
        } else {
            T = DoubleRotateWithRight(T);
        }
    }

    return T;
}
////delete function if freq more than one reduce it
struct AVLnode* DeleteFrequency(char data[], struct AVLnode* T) {
    struct AVLnode* temp;

    if (T == NULL) {
        printf("Node not found\n");
        return NULL;
    }



    if (strcmp(data, T->word) < 0) {
        T->Left = DeleteFrequency(data, T->Left);
    } else if (strcmp(data, T->word) > 0) {
        T->Right = DeleteFrequency(data, T->Right);
    } else {
        if (T->frequency == 1) {
            T = Delete(data, T);
            printf("Word has been deleted...\n");
        } else {
            T->frequency--;
            printf("Frequency has been reduced...\n");
        }
    }

    return T;
}


////////print the tree//////////
void PrintInOrder(struct AVLnode* t) {
    if (t != NULL) {
        PrintInOrder(t->Left);
        printf("%s [%d]\t", t->word, t->frequency);
        PrintInOrder(t->Right);
    }
}

struct AVLnode* minRight(struct AVLnode* T){ // the smallest in the big size.
      while(T->Left !=NULL){
        T= T->Left;
      }
      return T;
}


/////start with hashing ///////
////hash function //////
int hashfunction(char *word,int tablesize){
   int hashvalue = 0;

   while(*word != '\0'){

    hashvalue +=*word++;
   }
    return (hashvalue % tablesize);

}

/////create the hash table
struct hash* createhashtable(int size) {
    //malloc the hash table
    struct hash* hashtable = (struct hash*)malloc(sizeof(struct hash));
    if (hashtable == NULL) {
        printf("Out of space...\n");
        return NULL;
    }

    hashtable->size = size;
    for (int i = 0; i < MAX_SIZE; i++) {
        hashtable->table[i] = NULL;
    }


    return hashtable;
}
////insert to the hash table
void inserttotable(struct hash* hashtable, char *word, int frequency) {
    int index = hashfunction(word, hashtable->size);
    struct node* current = hashtable->table[index];

    // check if the word already exists in the linked list
    while (current != NULL) {
        if (strcmp(current->word, word) == 0) {
            current->frequency += frequency;
            return NULL;//don't add another  node
        }
        current = current->next;
    }

    // if the word does not exist, create a new node
    struct node* newnode = (struct node*)malloc(sizeof(struct node));
    if (newnode == NULL) {
        printf("Out of space...\n");
        return NULL;
    }

    // initialize the new node
    strncpy(newnode->word, word, sizeof(newnode->word) - 1);

    newnode->word[sizeof(newnode->word) - 1] = '\0';
    newnode->frequency = frequency;

    newnode->next = hashtable->table[index];
    hashtable->table[index] = newnode;
}
/////traverse the AVL tree
void traverseAVLAndInsert(struct AVLnode* root, struct hash *hashtable) {
    if (root == NULL) {
           return NULL;

    }
    traverseAVLAndInsert(root->Left, hashtable);
    inserttotable(hashtable, root->word,root->frequency);
    traverseAVLAndInsert(root->Right, hashtable);
}
/////print hash table
void printStatistics(struct hash* hashtable, int threshold) {
   int unique = 0;
    struct node* mostfrequent = NULL;
    int countMoreThanThreshold = 0;
    int i;
    for ( i = 0; i < hashtable->size; i++) {
        struct node* tmp = hashtable->table[i];
        while (tmp != NULL) {
            unique++;
            if (mostfrequent == NULL || tmp->frequency > mostfrequent->frequency) {
                mostfrequent = tmp;
            }
            if (tmp->frequency > threshold) {
                countMoreThanThreshold++;
            }
            tmp = tmp->next;
        }
    }

    printf("Total number of unique words: %d\n", unique);
    if (mostfrequent != NULL) {
        printf("Most frequent word: %s (Frequency: %d)\n", mostfrequent->word, mostfrequent->frequency);
    }
    printf("Number of words repeated more than %d times: %d\n", threshold, countMoreThanThreshold);


    printf("Words repeated more than %d times:\n", threshold);
    int j;
    for (j = 0; j < hashtable->size; j++) {
        struct node* tmp = hashtable->table[j];
        while (tmp != NULL) {
            if (tmp->frequency > threshold) {
                printf("%s (Frequency: %d)\n", tmp->word, tmp->frequency);
            }
            tmp = tmp->next;
        }
    }
}
////delete word from the table

void deleteFromTable(struct hash* hashtable, char *word) {
    int index = hashfunction(word, hashtable->size);
    struct node* newnode = hashtable->table[index];
    struct node* tmp = NULL;

    // search for the word in the linked list
    while (newnode != NULL) {
        if (strcmp(newnode->word, word) == 0) {
            if (tmp == NULL) {

                hashtable->table[index] = newnode->next;
            } else {

                tmp->next = newnode->next;
            }
            free(newnode); // free the memory of the node
            printf("word '%s' deleted from the hash table...\n", word);
            return NULL;//to avoid print the two statements togother
        }
        tmp = newnode;
        newnode= newnode->next;
    }
    printf("word '%s' not found in the hash table.\n", word);
}
////search about a word from the hash table
void searchAndPrintFrequency(struct hash* hashtable, char *word) {
    int index = hashfunction(word, hashtable->size);
    struct node* newnode = hashtable->table[index];

    // search for the word in the linked list
    while (newnode != NULL) {
        if (strcmp(newnode->word, word) == 0) {
            printf("Frequency of [%s]: %d\n", word, newnode->frequency);
            return NULL;
        }
        newnode = newnode->next;
    }
    printf("word '%s' not found in the hash table.\n", word);
}



