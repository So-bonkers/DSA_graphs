#include <stdio.h>
#include <stdlib.h>

typedef long int lint;

#define MAX 100009
#define NIL -1

struct edge
{
    int ori;
    int dest;
    int weight;
    struct edge *link;
} *front = NULL;

void make_tree(struct edge tree[]);
void insert_pque(int i, int j, int wt);
void create_graph();

int n;
int main()
{
    lint A;
    scanf("%d %ld", &n, &A);
    int i;
    struct edge tree[n+100];
    long long int wt_tree = 0;

    create_graph(A,n);

    make_tree(tree);

    for (i = 1; i <= n - 1; i++)
    {
        wt_tree += tree[i].weight;
    }
    printf("%lld\n", wt_tree);

    return 0;
}

void make_tree(struct edge tree[])
{

    int x;
    struct edge *tmp;
    int v1, v2, root_v1, root_v2;
    int head[MAX];
    int i, count = 0;

    for (i = 0; i < n; i++)
        head[i] = NIL;

    if (front == NULL)
    {
        x = 0;
    }
    else
    {
        x = 1;
    }

    while (x == 1 && count < n - 1)
    {

        struct edge *tmp;
        tmp = front;
        front = front->link;

        v1 = tmp->ori;
        v2 = tmp->dest;

        while (v2 != NIL)
        {
            root_v2 = v2;
            v2 = head[v2];
        }

        while (v1 != NIL)
        {
            root_v1 = v1;
            v1 = head[v1];
        }

        if (root_v1 != root_v2)
        {
            count++;
            tree[count].ori = tmp->ori;
            tree[count].dest = tmp->dest;
            tree[count].weight = tmp->weight;
            head[root_v2] = root_v1;
        }
    }
}

void insert_pque(int i, int j, int wt)
{
    struct edge *tmp, *q;

    tmp = (struct edge *)malloc(sizeof(struct edge));
    tmp->weight = wt;
    tmp->dest = j;
    tmp->ori = i;

    if (front == NULL || tmp->weight < front->weight)
    {
        tmp->link = front;
        front = tmp;
    }
    else
    {
        q = front;
        while (q->link != NULL && q->link->weight <= tmp->weight)
            q = q->link;
        tmp->link = q->link;
        q->link = tmp;
        if (q->link == NULL)
            tmp->link = NULL;
    }
}

void create_graph(int A,int n)
{
    lint i, wt, origin, destin;

    // scanf("%d %ld", &n, &A);

    for (i = 1; i <= A; ++i)
    {
        scanf("%ld %ld %ld", &origin, &destin, &wt);
        insert_pque(origin, destin, wt);
    }
}
