#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
Name : Zainab Abuawwad
ID : 1221410
Section : 2

*/

// Define structs for town and district
typedef struct Town {
    char name[50];
    int population;
    struct Town* next;
    struct Town* prev;
} Town;

typedef struct District {
    char name[50];
    int totalPopulation;
    struct Town* townHead;
    struct District* next;
    struct District* prev;
} District;
District *districts = NULL;
    District *sortedDistrict = NULL;


/////////////////////////////////////////////////////////////////////////////////////
//prototype for functions:
void insertTown(Town **head, char name[], int population);//adding town to district
void insertDistrict(District **head, char name[], Town *towns);//insert districts
void loadInputFile(District** head);//loading file and information to linked list
void printSortedData(District *districts);//print list
int isEmpty (District* l);//check if the list is empty or not
void deleteList(District* L);//delete list
void popMaxMin(District* p);//to find the population of palestine and the max , min population from all towns
void printDisPop(District* p);//to print the district name and its population without towns details
void deleteLTowns (District* head);//to delete all towns and there population without deleting towns
void addDistrict(District** head);//adding district to the list
District* MakeEmpty(District* p);//to make the list empty if its not
int FindMaxLength(District* L);//find the max length of districts
void addSpace(District* L);//add spaces to districts name to make their lengths equel
District* radixSort(District* p);//radix sort
void saveToOutPut(District* l);//save to output file
void addTowntoDistrict(District* district);//add new town with its population to the list
void sortTownsByPopulation(District* district);//to sort towns depending on population
void changeTownPopulation(District *head);//change the population of a certain town
void freeMemory(District* head);//free list from memory
/////////////////////////////////////////////////////////////////////

// Function to add a new town to a district
void insertTown(Town **head, char name[], int population) {
    Town *newTown = (Town *)malloc(sizeof(Town));
    if (newTown == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newTown->name, name);
    newTown->population = population;
    newTown->next = NULL;
    newTown->prev = NULL;

    if (*head == NULL) {
        *head = newTown;
    } else {
        Town *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newTown;
    }
}

// Function to add a new district or find an existing one
void insertDistrict(District **head, char name[], Town *towns) {
    District *newDistrict = (District *)malloc(sizeof(District));
    if (newDistrict == NULL) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newDistrict->name, name);
    newDistrict->totalPopulation = 0;
    newDistrict->townHead = towns;
    newDistrict->next = NULL;
    newDistrict->prev = NULL;

    Town *temp = towns;
    while (temp != NULL) {
        newDistrict->totalPopulation += temp->population;
        temp = temp->next;
    }

    if (*head == NULL) {
        *head = newDistrict;
    } else {
        District *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newDistrict;
    }
}


// Function to load input data from file
void loadInputFile(District** head) {
    FILE* input;

    input = fopen("districts.txt", "r");
    if (input == NULL) {
        printf("Error opening file...\n");
        return;
    }

    char array[50];
    char districtName[15], townName[15];
    int population;
    while (fgets(array, sizeof(array), input) != NULL) {
            printf("%s\n",array);
         if (sscanf(array, "%[^|]|%[^|]|%d", districtName, townName, &population) != 3) {
            printf("Error reading data from file.\n");
            fclose(input);
            return;
        }

        // Check if the district already exists in the list
        District *newDistrict = *head;
        while (newDistrict != NULL) {
            if (strcmp(newDistrict->name, districtName) == 0) {
                // District already exists, add town to its town list
                insertTown(&(newDistrict->townHead), townName, population);
                newDistrict->totalPopulation += population;
                break;
            }
            newDistrict = newDistrict->next;
        }

        // If district doesn't exist, create a new district node
        if (newDistrict == NULL) {
            Town *towns = NULL;
            insertTown(&towns, townName, population);
            insertDistrict(head, districtName, towns);
        }
    }
    fclose(input);
}

void printSortedData(District *districts) {
    District *currentDistrict = districts;
    Town *currentTown;

    printf("Sorted Information:\n");

    while (currentDistrict != NULL) {
        printf("%s District, Population = %d\n", currentDistrict->name, currentDistrict->totalPopulation);
        currentTown = currentDistrict->townHead;
        while (currentTown != NULL) {
            printf("%s, %d\n", currentTown->name, currentTown->population);
            currentTown = currentTown->next;
        }
        currentDistrict = currentDistrict->next;
    }
}

// function to check if the list is empty or not

int isEmpty (District* l){
  return l->next ==NULL && l->prev==NULL;
}

//to delete all list

void deleteList(District* L){

    if(L==NULL){
        printf("List Not Found..\n");
        return;
    }else if(isEmpty(L)){
        printf("List is empty, Nothing to delete..\n");
        return;
    }

    District* ptr=L;
    L=NULL;
    District* temp;

    while(ptr!=NULL){
        temp=ptr->next;
        free(ptr);
        ptr=temp;
    }


    printf("districts List has been deleted..\n");

}

//Calculate the population of Palestine, the max and min town population
void popMaxMin(District* p){

       int pop=0;
       int maxpop=0;
       int minpop = 10000000;

    District* newNode = p;//(struct District*)malloc(sizeof(struct District));
    //newNode = p->next;
    //newNode = p;

    while (newNode != NULL) {
        struct Town* newTown =newNode->townHead; //(struct Town*)malloc(sizeof(struct Town));

         //newTown= newNode->townHead;
           while(newTown != NULL){

            pop += newTown->population;


            if (newTown->population > maxpop) {
                maxpop = newTown->population;
            }

            if (newTown->population < minpop) {
                minpop = newTown->population;
            }
            newTown = newTown->next;

           }


        newNode = newNode->next;
    }

    printf("Total population =  %d\n", pop);
    printf("max town population =  %d\n", maxpop);
    printf("min town population =  %d\n", minpop);
}

//districts and their population with out town details
void printDisPop(District* p){

//delete towns and there population without delete districts name
    int pop=0;


  if(p == NULL){
    printf("list is empty \n");
  }


District* tmp=p;


    while(tmp != NULL){

      pop = tmp->totalPopulation;


        printf("%s\t ",tmp->name);




  printf("population= %d\n",pop);

  tmp=tmp->next;



    }

}
void deleteLTowns (District* head) {


    // Check if the p is not allocated
    if (head == NULL) {
        printf("List Not Found..\n");
        return;
    }

    // Assuming isEmpty() function checks if the p is empty
    if (isEmpty(head)) {
        printf("List is empty, Nothing to delete...\n");
        return;
    }

    // Traverse district list
    District* newDistrict = head;
    while (newDistrict != NULL) {
         Town* cTown =newDistrict->townHead;
        Town* preTown = NULL;

             while (cTown != NULL) {
            Town* nextTown = cTown->next; // Store the next town pointer before deletion

            // Delete the current town
            free(cTown);

             // Update pointers
            if (preTown == NULL) {
                newDistrict->townHead = nextTown;
            }else {

                preTown->next = nextTown;
            }
            cTown = nextTown;
             }
              newDistrict = newDistrict->next;
    }


    printf("towns List and there population has been deleted...\n");
}

//adding district to the list
void addDistrict(District** head) {

    char districtName[15];

    printf("Enter the name of the new district: ");
     //scanf("%99s", districtName);
    getchar();
     gets(districtName);


    District* currentDistrict = head;
    while (currentDistrict != NULL) {
        if (strncmp(currentDistrict->name, districtName,3) == 0) {
            printf("This district already exists.\n");
            return;
        }
        currentDistrict = currentDistrict->next;
    }

    // if the district does not exist, add it to the list
    insertDistrict(&head, districtName, NULL);

    printf("District '%s' added to the list.\n", districtName);
}
void addTowntoDistrict(District* head)  {
    if (head == NULL) {
        printf("No districts found\n");
        return;
    }

    char districtName[15];
    printf("Enter the name of the district: ");
    //scanf("%s", districtName);
    getchar();
    gets(districtName);
    District* currentDistrict ;//= (struct District*)malloc(sizeof(struct District));
    // search for the specified district
     currentDistrict = head;
    while (currentDistrict != NULL) {

         if (strncmp(currentDistrict->name,districtName,3)== 0 ){

            // district found
            char townName[15];


            printf("Enter the name of the new town: ");
            //scanf("%s", townName);
           getchar();
           gets(townName);

           int population;
            printf("Enter the population of the town: ");
            scanf("%d", &population);

            // insert the town into the district
            insertTown(&(currentDistrict->townHead), townName, population);
            currentDistrict->totalPopulation += population;

            printf("Town '%s' added to district '%s' with population %d.\n", townName, districtName, population);
            return;
         }
        currentDistrict = currentDistrict->next;
    }


    // district not found
    printf("District '%s' not found...\n", districtName);

}
//to sort the town depending on their population by using insertion sort);
void sortTownsByPopulation(District* district) {

    if (district == NULL || district->townHead == NULL || district->townHead->next == NULL) {
        // no need to sort if the district is empty or has only one town
        return;
    }

    Town *sorted = NULL; // initialize sorted list
    Town *current = district->townHead; // Traverse the original list

    while (current != NULL) {
        Town *next = current->next; // store the next node

        // insert current town into sorted list
        if (sorted == NULL || current->population <= sorted->population) {
            current->next = sorted;
            sorted = current;
        } else {
            Town *temp = sorted;
            while (temp->next != NULL && temp->next->population < current->population) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }

        current = next; // move to the next town in the original list
    }

    // update the head of the town list in the district to the sorted list
    district->townHead = sorted;
}
int FindMaxLength(District* L) {
    // check if the LL is empty
    if (isEmpty(L)) {
        return 0;
    }

    // the maximum length
    int maxL = 0;

    // pointer to the first node in the LL
    District* P = L;

    // find the maximum length
    while (P != NULL) {

        // Find the length of the current node's string
        int length =(strlen(P->name)-1);
//printf("%d\n",length);
        // Update the maximum length
        if (length > maxL) {

            maxL = length;

        }

        // Move to the next node
        P = P->next;

    }
// printf("%d\n",maxL);

    // maximum length
    return maxL;
}

//adds spaces to the strings , so that all the strings have the same length
void addSpace(District* L) {
    // find the maximum length of the Strings
    int maxL = FindMaxLength(L);

    // point to the first node in the list
    District* F_node = L;

    // add spaces to each node's string
    while (F_node != NULL) {
        // Get the length of the current node's string
        int num_str = (strlen(F_node->name)-1);
          //printf("%d\n",num_str);

        // Calculate the number of spaces to add
        int spaces_num = maxL - num_str;
                //printf("%d\n",spaces_num);
        // add spaces to the current node's string  ///strlen(F_node->name) - 2
        for (int i = 0; i < spaces_num; i++) {
            strncat(F_node->name, " ", sizeof(F_node->name) - num_str-1);

        }

        // move to the next node
        F_node = F_node->next;
    }
}

//radix sort uncompleted
District* radixSort(District* p) {
    // find max length
    int maxl = FindMaxLength(p);

    // add spaces to make all the strings the same length
    addSpace(p);

    District* letters[63]; // create a linked list for each character in the alphabet

    // initialize each element of the array
    for (int i = 0; i < 63; i++) {
        letters[i] = MakeEmpty(NULL);
    }

    // radix sort from right to left
    for (int j = maxl - 1; j >= 0; j--) { // j is the position
        District* current = p;
        while (current != NULL) {
            int index;
            // Check current character
            char currChar = current->name[j];
            if (currChar == ' ') {
                index = 0;
            } else if ('0' <= currChar && currChar <= '9') { // Numbers
                index = currChar - '0' + 1;
            } else if ('A' <= currChar && currChar <= 'Z') { // Uppercase letters
                index = currChar - 'A' + 11;
            } else if ('a' <= currChar && currChar <= 'z') { // Lowercase letters
                index = currChar - 'a' + 37;
            }
            // insert the district into the corresponding linked list
            insertDistrict(letters[index], current->name, NULL);
            // move to the next district in the original list
            current = current->next;
        }

        // make a new list and combine the array's nodes back into the first list
        District* newL= MakeEmpty(NULL);
        for (int i = 0; i < 63; i++) {
            District* current = letters[i]->next;
            while (current != NULL) {
                // insert the current node into the new list
                insertDistrict(newL, current->name, NULL);
                // move to the next node
                current = current->next;
            }
            // reset the array index to an empty list
            letters[i]->next = NULL;
        }

        // update the original list to point to the new list
        p->next = newL->next;
    }

    // free memory for nodes in the letters array
    for (int i = 0; i < 63; i++) {
        deleteList(letters[i]->next); // delete the nodes in the list
        free(letters[i]); // free the list itself
    }

    return p; // return the sorted list
}

// function to create an empty list
District* MakeEmpty(District* p) {
    if (p!= NULL)
        deleteList(p);
    p = (struct District*) malloc(sizeof(struct District));

    if (p == NULL)
        printf("Out of memory!\n");

    p->next = NULL;
    return p;
}


void changeTownPopulation(District *head) {
    char districtName[15];
    printf("Enter the name of the district: ");
    //scanf("%s", districtName);
    getchar();
    gets(districtName);

    District *current = head;
    while (current != NULL) {
        if (strncmp(current->name, districtName,3) == 0){

            // District found
            char townName[15];
            printf("Enter the name of the town: ");
            //scanf("%s", townName);
            getchar();
            gets(townName);

            // Search for the town within the district
            Town *newTown;
            newTown=current->townHead;
            while (newTown != NULL) {

                if (strncmp(newTown->name,townName,0) == 0){


                    // Town found
                    int newPopulation;
                    printf("Enter the new population of the town: ");
                    scanf("%d", &newPopulation);

                    // Update the population of the town
                    newTown->population = newPopulation;

                    printf("Population of town '%s' in district '%s' changed to %d.\n", townName, districtName, newPopulation);
                    return;
                }

                newTown = newTown->next;
            }
            printf("Town '%s' not found in district '%s'.\n", townName, districtName);
            return;
        }
        current = current->next;
    }
    printf("District '%s' not found.\n", districtName);
}


//save file
void saveToOutPut(District* l){
    // Open the output file ( write mode )
    FILE* output = fopen("sorted_districts.txt", "w");
    // Check if the file was opened
    if (output == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }


    // Write each district with its towns to the file
    District* currentDistrict = l;
    while (currentDistrict != NULL) {
        fprintf(output, " %s\n", currentDistrict->name);
        Town* currentTown = currentDistrict->townHead;
        while (currentTown != NULL) {
            fprintf(output, " %s,  %d\n", currentTown->name, currentTown->population);
            currentTown = currentTown->next;
        }
        fprintf(output, "\n"); // Add a newline to separate districts
        currentDistrict = currentDistrict->next;
    }

    printf("information has been saved...\n");
    fclose(output);


}
void freeMemory(District* head) {
    District* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}


int main() {
    //District *districts = NULL;
    //District *sortedDistrict = NULL;
    int choice;

    do {
        printf("\nMenu:\n");
        printf("1. Load the input file\n");
        printf("2. Print the loaded information before sorting\n");
        printf("3. Sort the districts alphabetically using Radix sort\n");
        printf("4. Sort the towns for each district based on population in ascending order\n");
        printf("5. Print the sorted information\n");
        printf("6. Add a new district to the list of sorted districts (and sort the list)\n");
        printf("7. Add a new town to a certain district\n");
        printf("8. Delete a town from a specific district\n");
        printf("9. Delete a complete district\n");
        printf("10. Calculate the population of Palestine, the max and min town population\n");
        printf("11. Print the districts and their total population (without towns details)\n");
        printf("12. Change the population of a town, after prompting the user to enter the district name\n");
        printf("13. Save to output file\n");
        printf("14. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                loadInputFile(&districts);

                break;
            case 2:
                  printSortedData(districts);

                break;
            case 3:
                    sortedDistrict=radixSort(districts);
                 printSortedData(sortedDistrict);


                break;
            case 4:
                   sortTownsByPopulation(districts);
                   printSortedData(districts);


                break;
            case 5:

                printSortedData(districts);
                break;
            case 6:
                   addDistrict(districts);
                   //printSortedData(districts);


                break;
            case 7:
                   addTowntoDistrict(districts);
                   //printSortedData(districts);

                break;
            case 8:
                   deleteLTowns(districts);

                break;
            case 9:
                   deleteList(districts);

                break;
            case 10:
                   popMaxMin(districts);

                break;
            case 11:
                   printDisPop(districts);

                break;
            case 12:changeTownPopulation(districts);
                   //printSortedData(districts);

                break;
            case 13:saveToOutPut(districts);

                break;
            case 14:freeMemory(districts);
                printf("Exiting the program.\n");

                return 0;
            default:
                printf("Invalid choice. Please enter a number between 1 and 14.\n");
        }
    } while (choice != 14);

    return 0;
}
