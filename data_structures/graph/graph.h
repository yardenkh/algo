#ifndef BASICGRAPH_H
#define BASICGRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include "../dynarray/dynarray.h" // You must have dynarray.h in the include path

// ---------------------- Vertex Definition ------------------------
typedef struct V
{
    int id;
    DynArray neighbors; // Use dynamic array for neighbors
} V;

// ---------------------- Edge Definition --------------------------
typedef struct E
{
    int weight;
    V *eVertices[2]; // eVertices[0] = start, eVertices[1] = end
} E;

// ---------------------- Graph Definition -------------------------
typedef struct G
{
    DynArray Vertices; // Dynamic array of V*
    DynArray Edges;    // Dynamic array of E*
} G;

// ---------------------- Vertex Functions -------------------------

// Creates a new vertex with a given id
static inline V *newV(int id)
{
    V *newVertex = (V *)malloc(sizeof(V));
    if (!newVertex)
    {
        fprintf(stderr, "newV: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    newVertex->id = id;
    dynarray_init(&newVertex->neighbors, 0); // initialize with default capacity
    return newVertex;
}

// Connects two vertices (adds 'end' as neighbor to 'start')
static inline void connectV(V *start, V *end)
{
    dynarray_push(&start->neighbors, end);
}

// ---------------------- Edge Functions ---------------------------

// Creates a new edge and connects startv to endv
static inline E *newE(V *startv, V *endv, int weight)
{
    E *edge = (E *)malloc(sizeof(E));
    if (!edge)
    {
        fprintf(stderr, "newE: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    edge->eVertices[0] = startv;
    edge->eVertices[1] = endv;
    connectV(startv, endv); // add the neighbor link
    edge->weight = weight;
    return edge;
}

// ---------------------- Graph Functions --------------------------

// Initializes a new empty graph
static inline void graph_init(G *graph)
{
    dynarray_init(&graph->Vertices, 0);
    dynarray_init(&graph->Edges, 0);
}

// Adds a vertex to the graph
static inline void graph_add_vertex(G *graph, V *vertex)
{
    dynarray_push(&graph->Vertices, vertex);
}

// Adds an edge to the graph
static inline void graph_add_edge(G *graph, E *edge)
{
    dynarray_push(&graph->Edges, edge);
}

// Frees all memory used by the graph (does not free vertices/edges themselves)
static inline void graph_free(G *graph)
{
    dynarray_free(&graph->Vertices);
    dynarray_free(&graph->Edges);
}

#endif // BASICGRAPH_H