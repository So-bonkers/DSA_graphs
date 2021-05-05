#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INF -1000

struct MinHeapNode
{
    int v;
    int k_graph;
};

struct MinHeap
{
    int size;      // Number of heap nodes present currently
    int capacity;  // Capacity of min heap
    int *position; // This is needed for decreasek_graph()
    struct MinHeapNode **array;
};

typedef struct MinHeapNode *MinHeapNodePtr;

struct AdjListNode
{
    int destination;
    long long int weight;
    struct AdjListNode *next;
    bool condition;
};

typedef struct AdjListNode *AdjListNodePtr;

struct Graph
{
    int V;
    struct AdjList *array;
};

struct AdjList
{
    AdjListNodePtr head;
};

MinHeapNodePtr newMinHeapNode(int v, int k_graph)
{
    MinHeapNodePtr minHeapNode = (MinHeapNodePtr)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->k_graph = k_graph;
    return minHeapNode;
}

struct MinHeap *createMinHeap(int capacity)
{
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->position = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNodePtr *)malloc(capacity * sizeof(MinHeapNodePtr));
    return minHeap;
}

void swapMinHeapNode(MinHeapNodePtr *a, MinHeapNodePtr *b)
{
    MinHeapNodePtr t = *a;
    *a = *b;
    *b = t;
}

void heapify(struct MinHeap *minHeap, int index)
{
    int smallest, left, right;
    smallest = index;
    left = 2 * index + 1;
    right = 2 * index + 2;

    if (left < minHeap->size && minHeap->array[left]->k_graph < minHeap->array[smallest]->k_graph)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->k_graph < minHeap->array[smallest]->k_graph)
        smallest = right;

    if (smallest != index)
    {
        MinHeapNodePtr smallestNode = minHeap->array[smallest];
        MinHeapNodePtr indexNode = minHeap->array[index];

        minHeap->position[smallestNode->v] = index;
        minHeap->position[indexNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[index]);

        heapify(minHeap, smallest);
    }
}

int isEmpty(struct MinHeap *minHeap)
{
    return minHeap->size == 0;
}

MinHeapNodePtr extract(struct MinHeap *minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    MinHeapNodePtr root = minHeap->array[0];

    MinHeapNodePtr lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->position[root->v] = minHeap->size - 1;
    minHeap->position[lastNode->v] = 0;

    --minHeap->size;
    heapify(minHeap, 0);

    return root;
}

void decreasek_graph(struct MinHeap *minHeap, int v, int k_graph)
{
    int i = minHeap->position[v];

    minHeap->array[i]->k_graph = k_graph;

    while (i && minHeap->array[i]->k_graph < minHeap->array[(i - 1) / 2]->k_graph)
    {
        minHeap->position[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->position[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->position[v] < minHeap->size)
        return true;
    return false;
}

AdjListNodePtr newAdjListNode(int destination, long long int weight)
{
    AdjListNodePtr newNode = (AdjListNodePtr)malloc(sizeof(struct AdjListNode));
    newNode->destination = destination;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;

    graph->array = (struct AdjList *)malloc(V * sizeof(struct AdjList));

    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void add_Edge_graph(struct Graph *graph, int src, int destination, long long int weight, bool condition)
{
    AdjListNodePtr newNode = newAdjListNode(destination, weight);
    newNode->condition = condition;
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    AdjListNodePtr newNode1 = newAdjListNode(src, weight);
    newNode1->condition = condition;

    newNode1->next = graph->array[destination].head;
    graph->array[destination].head = newNode1;
}

int main()
{
    long long int count = 0;

    int n, m, a, b, w, aw;
    long long int s;
    scanf("%d %d %d", &n, &aw, &w);
    struct Graph *graph = createGraph(n);

    for (int i = 0; i < aw; i++)
    {
        scanf("%d %d %lld", &a, &b, &s);
        add_Edge_graph(graph, a - 1, b - 1, s, 1);
    }
    long long int array[n];
    for (int i = 0; i < n; i++)
    {
        array[i] = INF;
    }
    for (int i = 0; i < w; i++)
    {
        scanf("%d %lld", &b, &s);
        if (b == 1)
            count++;
        else if (array[b - 1] == INF)
        {
            array[b - 1] = s;
        }
        else if (array[b - 1] <= s)
            count++;
        else
        {
            array[b - 1] = s;
            count++;
        }
    }
    for (int i = 1; i < n; i++)
    {
        if (array[i] != INF)
            add_Edge_graph(graph, 0, i, array[i], 0);
    }

    while (1)
    {

        //dijkstra algo
        bool flag = false;
        long long int result = count;
        int V = graph->V;         // Get the number of vertices in graph
        long long int k_graph[V]; // k_graph values used to pick minimum weight edge in cut
        long long int A[V];

        struct MinHeap *minHeap = createMinHeap(V);

        for (int v = 0; v < V; ++v)
        {

            A[v] = k_graph[v] = _LONG_LONG_MAX_;
            minHeap->array[v] = newMinHeapNode(v, k_graph[v]);
            minHeap->position[v] = v;
        }

        // k_graph[0] = 0;
        minHeap->array[0] = newMinHeapNode(0, k_graph[0]);
        minHeap->position[0] = 0;
        k_graph[0] = 0;

        decreasek_graph(minHeap, 0, k_graph[0]);

        minHeap->size = V;

        while (!isEmpty(minHeap))
        {
            MinHeapNodePtr minHeapNode = extract(minHeap);
            int u = minHeapNode->v;
            // result += k_graph[u];
            AdjListNodePtr PC = graph->array[u].head;
            while (PC != NULL)
            {
                int v = PC->destination;

                if (isInMinHeap(minHeap, v) && k_graph[u] != _LONG_LONG_MAX_ && PC->weight + k_graph[u] <= k_graph[v])
                {
                    if (PC->condition == 1)
                    {
                        A[v] = PC->weight + k_graph[u];
                    }

                    k_graph[v] = PC->weight + k_graph[u];
                    // array[v] = u;
                    decreasek_graph(minHeap, v, k_graph[v]);
                }
                PC = PC->next;
            }
        }
        for (int i = 1; i < V; i++)
        {
            if (array[i] != INF && A[i] <= array[i])
                result++;
        }
        printf("%lld\n", result);
        if (!flag)
            break;
    }
    return 0;
}
