#define MAX_BUILDINGS 50
#define MAX_NAME_LEN 50
#define MAX_HEAP_SIZE 100
#define MAX_COURSES 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
name : zainab abu awwad
id : 1221410
section : 2
*/

// Graph represented by adjacency list
typedef struct Node {
    char building[MAX_NAME_LEN];
    int distance;
    struct Node* next;
} Node;

typedef struct Building {
    char name[MAX_NAME_LEN];
    Node* adjList;
} Building;

// Struct for the heap (min_priority queue)
typedef struct MinHeap {
    int maxSize;
    int size;
    int* heap;        // array of vertex indices
    int* vertexPos;   // position of vertices in the heap
    int* distances;
} MinHeap;

Building buildings[MAX_BUILDINGS];
int buildingCount = 0;
////start with topological sort and its graph////
typedef struct node {
    char course[MAX_NAME_LEN];
    struct node* next;
} node;

typedef struct Course {
    char name[MAX_NAME_LEN];
    node* adjList; // Courses that depend on this course
    int inDegree;  // Number of prerequisites
} Course;

Course courses[MAX_COURSES];
int CoursesCount = 0;
////here is the queue
typedef struct QueueNode {
    int courseIndex;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;


int topologicalOrder[MAX_COURSES];
int topologicalIndex = 0;

int main()
{
     int choice;


    do {
        printf("\nMenu:\n");
        printf("1. Load the two files\n");
        printf("2. Calculate the shortest distance between two buildings\n");
        printf("3. Print the shortest route between two buildings and the total distance\n");
        printf("4. Sort the courses using the topological sort\n");
        printf("5. Print the sorted courses\n");
        printf("6. Exit the application\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:////read the data from the files
            printf("buildings file has been loaded...\n");
            loadGraph();
            printf("______________________________\n");
            printf("courses file has been loaded...\n");
            loadCourses();

            break;
        case 2:{////calculate the shortest path///////
            char src[MAX_NAME_LEN], dest[MAX_NAME_LEN];
    printf("Enter the source building: ");
    scanf("%s", src);
    printf("Enter the destination building: ");
    scanf("%s", dest);

    int srcIndex = findBuildingIndex(src);
    int destIndex = findBuildingIndex(dest);

    if (srcIndex == -1 || destIndex == -1) {
        printf("One or both buildings not found.\n");
        return 1;
    }

    int distances[buildingCount];
    int previous[buildingCount];

    dijkstra(srcIndex,destIndex, distances, previous);
    printf("Shortest path has been calculated...\n");

        }

            break;
        case 3:{/////print the shortest path with its total
            char src[MAX_NAME_LEN], dest[MAX_NAME_LEN];

    int srcIndex = findBuildingIndex(src);
    int destIndex = findBuildingIndex(dest);

    if (srcIndex == -1 || destIndex == -1) {
        printf("One or both buildings not found.\n");
        return 1;
    }

    int distances[buildingCount];
    int previous[buildingCount];

    dijkstra(srcIndex,destIndex, distances, previous);



    printShortestPath(srcIndex,destIndex, distances, previous);

        }
               break;

        case 4:topologicalSort();
               printf("courses has been sorted...\n");
    break;


        case 5:printSortedCourses();

            break;


        case 6:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);

    return 0;
}
//////functions prototypes///////////
///we will start with the Dijkstra ///
void loadGraph();
int findBuildingIndex(char* name);
void addEdge(char* from, char* to, int distance);
MinHeap* createMinHeap(int maxsize);
int parent(int pos);
int leftChild(int pos);
int rightChild(int pos);
int isLeaf(MinHeap *minHeap, int pos);
void swap(MinHeap *minHeap, int fpos, int spos);
void minHeapify(MinHeap *minHeap, int pos);
void insert(MinHeap *minHeap, int vertex, int distance);
int removeMin(MinHeap *minHeap);
void decreaseKey(MinHeap *minHeap, int vertex, int distance);
void dijkstra(int srcIndex, int destIndex, int* distances, int* previous);
void printShortestPath(int srcIndex, int destIndex, int* distances, int* previous);
///prototypes for the Topological functions ///
void initQueue(Queue* q);
int isQueueEmpty(Queue* q);
void enqueue(Queue* q, int courseIndex);
int dequeue(Queue* q);
int findCourseIndex(char* name);
void addEdgeTopo(char* course, char* prerequisite);
void loadCourses();
void topologicalSort();
void printSortedCourses();
////read the Dijkstra file  /////

void loadGraph() {
    FILE *input;
    input = fopen("input_buildings.txt", "r");
    if (input == NULL) {
        printf("Error opening file...\n");
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), input)) {
        char from[MAX_NAME_LEN], to[MAX_NAME_LEN];
        int distance;

        char *token = strtok(line, "#");
        if (token != NULL) {
            strcpy(from, token);
        }

        token = strtok(NULL, "#");
        if (token != NULL) {
            strcpy(to, token);
        }

        token = strtok(NULL, "#");
        if (token != NULL) {
            distance = atoi(token);
        }

        addEdge(from, to, distance);
    }

    fclose(input);
}

int findBuildingIndex(char* name) {
    for (int i = 0; i < buildingCount; i++) {
        if (strcmp(buildings[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void addEdge(char* from, char* to, int distance) {
    int fromIndex = findBuildingIndex(from);
    int toIndex = findBuildingIndex(to);

    if (fromIndex == -1) {
        fromIndex = buildingCount++;
        strcpy(buildings[fromIndex].name, from);
        buildings[fromIndex].adjList = NULL;
    }

    if (toIndex == -1) {
        toIndex = buildingCount++;
        strcpy(buildings[toIndex].name, to);
        buildings[toIndex].adjList = NULL;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->building, to);
    newNode->distance = distance;
    newNode->next = buildings[fromIndex].adjList;
    buildings[fromIndex].adjList = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->building, from);
    newNode->distance = distance;
    newNode->next = buildings[toIndex].adjList;
    buildings[toIndex].adjList = newNode;
}

MinHeap* createMinHeap(int maxsize) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->maxSize = maxsize;
    minHeap->size = 0;
    minHeap->heap = (int*)malloc((maxsize + 1) * sizeof(int));
    minHeap->vertexPos = (int*)malloc(maxsize * sizeof(int));
    minHeap->distances = (int*)malloc(maxsize * sizeof(int));
    minHeap->heap[0] = -1;
    return minHeap;
}

int parent(int pos) {
    return pos / 2;
}

int leftChild(int pos) {
    return 2 * pos;
}

int rightChild(int pos) {
    return (2 * pos) + 1;
}

int isLeaf(MinHeap *minHeap, int pos) {
    return pos > minHeap->size / 2 && pos <= minHeap->size;
}

void swap(MinHeap *minHeap, int fpos, int spos) {
    int tmp = minHeap->heap[fpos];
    minHeap->heap[fpos] = minHeap->heap[spos];
    minHeap->heap[spos] = tmp;

    // Update the vertex positions
    minHeap->vertexPos[minHeap->heap[fpos]] = fpos;
    minHeap->vertexPos[minHeap->heap[spos]] = spos;
}

void minHeapify(MinHeap *minHeap, int pos) {
    if (!isLeaf(minHeap, pos)) {
        int left = leftChild(pos);
        int right = rightChild(pos);

        if (left <= minHeap->size && right <= minHeap->size &&
            (minHeap->distances[minHeap->heap[pos]] > minHeap->distances[minHeap->heap[left]] ||
             minHeap->distances[minHeap->heap[pos]] > minHeap->distances[minHeap->heap[right]])) {
            if (minHeap->distances[minHeap->heap[left]] < minHeap->distances[minHeap->heap[right]]) {
                swap(minHeap, pos, left);
                minHeapify(minHeap, left);
            } else {
                swap(minHeap, pos, right);
                minHeapify(minHeap, right);
            }
        } else if (left <= minHeap->size && minHeap->distances[minHeap->heap[pos]] > minHeap->distances[minHeap->heap[left]]) {
            swap(minHeap, pos, left);
            minHeapify(minHeap, left);
        }
    }
}

void insert(MinHeap *minHeap, int vertex, int distance) {
    if (minHeap->size >= minHeap->maxSize) {
        printf("Heap is full\n");
        return NULL;
    }
    minHeap->heap[++minHeap->size] = vertex;
    minHeap->vertexPos[vertex] = minHeap->size;
    minHeap->distances[vertex] = distance;

    int current = minHeap->size;
    while (current > 1 && minHeap->distances[minHeap->heap[current]] < minHeap->distances[minHeap->heap[parent(current)]]) {
        swap(minHeap, current, parent(current));
        current = parent(current);
    }
}

int removeMin(MinHeap *minHeap) {
    int popped = minHeap->heap[1];
    minHeap->heap[1] = minHeap->heap[minHeap->size--];
    minHeapify(minHeap, 1);
    return popped;
}

void decreaseKey(MinHeap *minHeap, int vertex, int distance) {
    int i = minHeap->vertexPos[vertex];  // Position of the vertex in the heap array
    minHeap->distances[vertex] = distance;

    while (i > 1 && minHeap->distances[vertex] < minHeap->distances[minHeap->heap[parent(i)]]) {
        swap(minHeap, i, parent(i));
        i = parent(i);
    }
}

void dijkstra(int srcIndex, int destIndex, int* distances, int* previous) {
    MinHeap* minHeap = createMinHeap(buildingCount);

    for (int i = 0; i < buildingCount; i++) {
        distances[i] = INT_MAX;
        previous[i] = -1;
        insert(minHeap, i, distances[i]);
    }

    decreaseKey(minHeap, srcIndex, 0);
    distances[srcIndex] = 0;

    while (minHeap->size > 0) {
        int u = removeMin(minHeap);

        Node* node = buildings[u].adjList;
        while (node != NULL) {
            int v = findBuildingIndex(node->building);
            if (v != -1 && distances[u] != INT_MAX && node->distance + distances[u] < distances[v]) {
                distances[v] = node->distance + distances[u];
                previous[v] = u;
                decreaseKey(minHeap, v, distances[v]);
            }
            node = node->next;
        }
    }

    free(minHeap->heap);
    free(minHeap->vertexPos);
    free(minHeap->distances);
    free(minHeap);
}

void printShortestPath(int srcIndex, int destIndex, int* distances, int* previous) {
    if (distances[destIndex] == INT_MAX) {
        printf("There is no path between %s and %s.\n", buildings[srcIndex].name, buildings[destIndex].name);
        return NULL;
    }

    printf("Shortest path from %s to %s is %d units.\n", buildings[srcIndex].name, buildings[destIndex].name, distances[destIndex]);
    printf("Path: ");

    int path[MAX_BUILDINGS];
    int pathIndex = 0;
    for (int i = destIndex; i != -1; i = previous[i]) {
        path[pathIndex++] = i;
    }

    for (int i = pathIndex - 1; i > 0; i--) {
        printf("%s -> ", buildings[path[i]].name);
    }
    printf("%s\n", buildings[path[0]].name);
}



////////////////////////////////
////queue functions ///////

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}

void enqueue(Queue* q, int courseIndex) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->courseIndex = courseIndex;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

int dequeue(Queue* q) {
    if (isQueueEmpty(q)) {
        return -1;
    }
    QueueNode* temp = q->front;
    int courseIndex = temp->courseIndex;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return courseIndex;
}

int findCourseIndex(char* name) {
    for (int i = 0; i < CoursesCount; i++) {
        if (strcmp(courses[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}
/////add vertex to graph/////
void addEdgeTopo(char* course, char* prerequisite) {
    int courseIndex = findCourseIndex(course);
    int prerequisiteIndex = findCourseIndex(prerequisite);

    if (prerequisiteIndex == -1) {
        prerequisiteIndex = CoursesCount++;
        strcpy(courses[prerequisiteIndex].name, prerequisite);
        courses[prerequisiteIndex].adjList = NULL;
        courses[prerequisiteIndex].inDegree = 0;
    }

    if (courseIndex == -1) {
        courseIndex = CoursesCount++;

        strcpy(courses[courseIndex].name, course);
        courses[courseIndex].adjList = NULL;

        courses[courseIndex].inDegree = 0;
    }

    node* newNode = (node*)malloc(sizeof(node));
    strcpy(newNode->course, course);


    newNode->next = courses[prerequisiteIndex].adjList;
    courses[prerequisiteIndex].adjList = newNode;


    courses[courseIndex].inDegree++;
}
void loadCourses() {
    FILE *input = fopen("input_courses.txt", "r");
    if (input == NULL) {
        printf("Error opening file...\n");
        return NULL;
    }

    char line[512];
    while (fgets(line, sizeof(line), input)) {
        char Course[MAX_NAME_LEN];
        char Prerequisites[MAX_NAME_LEN * 10];

        // use strtok to tokenize the string
        char *token = strtok(line, "#");
        if (token != NULL) {
            strncpy(Course, token, MAX_NAME_LEN);
            Course[MAX_NAME_LEN - 1] = '\0';
        }

        token = strtok(NULL, "\n");
        if (token != NULL) {
            strncpy(Prerequisites, token, sizeof(Prerequisites) - 1);
            Prerequisites[sizeof(Prerequisites) - 1] = '\0';
        }

        //  multiple prerequisites
        token = strtok(Prerequisites, "#");
        while (token != NULL) {
            addEdgeTopo(Course, token);
            token = strtok(NULL, "#");
        }
    }

    fclose(input);
}
void topologicalSort() {
    Queue q;
    initQueue(&q);

     topologicalIndex = 0;

    // enqueue all courses with in degree = 0
    for (int i = 0; i < CoursesCount; i++) {
        if (courses[i].inDegree == 0) {
            enqueue(&q, i);
        }
    }

    while (!isQueueEmpty(&q)) {
        int courseIndex = dequeue(&q);//so dequeue the zero in degree nodes
        topologicalOrder[topologicalIndex++] = courseIndex;

        node* adjList = courses[courseIndex].adjList;
        while (adjList != NULL) {
            int adjIndex = findCourseIndex(adjList->course);
            courses[adjIndex].inDegree--;//decrease the in degree for every nodes
            if (courses[adjIndex].inDegree == 0) {
                enqueue(&q, adjIndex);
            }
            adjList = adjList->next;
        }
    }

    if (topologicalIndex != CoursesCount) {
        printf("there exists a cycle in the graph...\n");
        return NULL;
    }


}

//print the topological sort /////
void printSortedCourses() {
    if (topologicalIndex != CoursesCount) {
        printf("There exists a cycle in the graph\n");
        return NULL;
    }

    printf("Topological Sort of Courses:\n");

    for (int i = 0; i < topologicalIndex; i++) {

        printf("%s\n", courses[topologicalOrder[i]].name);
    }
}
