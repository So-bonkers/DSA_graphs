/*
There is a planet where there are N islands connected by X airways and Y waterways. 
This God of Planet is Doris and she lives on central island A(which is the First island). The ith airway connects island ai to bi which whose length is si. The ith waterway connects the central island 1 to island xi whose length is yi.
Recently the waterways became unsafe because of the increased activity of pirates, so Doris decided to close as many waterways as many possible under the certain condition:
The travel time from central island to any of the island must not change

NOTE
All the ways(airways and waterways) are bi-directional
It is guaranteed that there exists at least one way to reach any island from Central island.
There can be multiple airways between two islands.

Planet has a unique plane which is capable of travelling at the same speed in water and air and that plane is used for all the travelling

Input:
The first line of input contains three integers N, X, Y (2 ≤ N ≤ 2(10^5); 1 ≤ X ≤ 2(10^5); 1 ≤ Y ≤ 10^5).
Each of the next X lines of input contains three integers ai, bi, si (1 ≤ ai, bi ≤ N; ai != bi; 1 ≤ s ≤ 2*(10^9)).
Each of the next Y lines contains two integers xi and yi (2 ≤ xi ≤ N; 1 ≤ y ≤ 10^9).
OUTPUT: A single integer representing the maximum number of the waterways that can be closed

Sample Input

5 5 4
1 2 1
2 3 2
1 3 3
3 4 4
1 5 7

2 7
3 5
4 5
5 5

Output: 2
*/

//Step 1 : Create the graphs with the initial waterways, initialise the counter to 0
//Step 2 : Plot the airways
//Step 3 : Compare the length of the waterways and the airways; if l(airways) > l(waterways)
//         then if (there exists a path from central island to dest. island with path
//         length == l(waterway)), counter ++
//Step 4 : Loop for all the islands respectively

#include<stdio.h>
#include <stdlib.h>

typedef struct stGraph * Graph;
typedef struct stNode * Node;
typedef struct stSPTable * SPTable;
typedef int Vertex;

#define WHITE  1
#define GRAY   0
#define BLACK -1
#define INFINITY  10000
#define UNKNOWN -1
#define NOTVERTEX -1

Graph   CreateGraph(int iNumber_of_vertices);
void    InsertAirway(Graph G, Vertex u, Vertex v);
void    InserWaterway(Graph G, Vertex t);
void    BFS(Graph G, Vertex s);
void    DFS(Graph G, Vertex s);
SPTable Dijsktra(Graph G, float **c);

struct stGraph{
    int iN;
    Node *pVertex;
};

struct stNode{
    Vertex iVertexID;
    Node pNext;
};

struct stSPTable
{
    int *pKnown;
    float *pdistance_to_source;
    Vertex *pPrevious_vertex;
};

typedef struct stNodeq  * Queue;  // Normal Queue implementation
typedef struct stNodepq * PQueue; // Priority Queue using Min Heaps  or Fibonacci heaps
typedef struct stNodeq  * Nodeq;  // Node in Queue
typedef struct stNodepq * Nodepq; // Node in Priority Queue

typedef int Element;
typedef float Key;

Queue   CreateEmptyQueue();
PQueue  CreateEmptyPriorityQueue();
Element DeQueue(Queue Q);
void    Enqueue(Queue Q, Element u);
void    Priority_Enqueue(PQueue Q, Element u, Key k);
int     IsQueueEmpty(Queue Q);
int     IsPQueueEmpty(PQueue Q);
Element ExtractMin(PQueue Q);
void    DecreaseKey(PQueue Q, Element u, Key new_decreased_key);

struct stNodeq{
    Element n;
    Nodeq pNext;
};

struct stNodepq{
    Element n;
    Key k;
    Nodepq pNext;
};

Graph   CreateGraph(int iNumber_of_vertices)
{
    int i=0;
    Graph G;

    // First allocate memory 
    G = malloc(sizeof(struct stGraph));
    if( G == NULL)
    {
        exit(0);
    }
    G->iN       = iNumber_of_vertices;
    G->pVertex  = malloc(iNumber_of_vertices*sizeof(struct stNode));
    if( G->pVertex == NULL)
    {
        exit(0);
    }

    // Create Empty Graph
    for (i=0;i<iNumber_of_vertices;i++)
    {
        G->pVertex[i]->iVertexID    = i;
        G->pVertex[i]->pNext        = NULL;
    }
    return G;
}

void InserWaterway(Graph G, Vertex u, Vertex v)
{
    int i,found=0;
    Node temp;
    Vertex w;

    // First check if the edge already exists.
    temp    = G->pVertex[u];
    while(temp->pNext != NULL)
    {
        w = temp->pNext->iVertexID;
        if(w==v)
            found=1;
        temp = temp->pNext;
    }

    // If it is not already there.
    if(!found)
    {
        temp                    = (Node) malloc(sizeof(struct stNode));
        temp->iVertexID         = v;
        temp->pNext             = G->pVertex[u]->pNext;
        G->pVertex[u]->pNext    = temp;

    }
    return;
}
