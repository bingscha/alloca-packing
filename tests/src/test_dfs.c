#include <stdlib.h>
#include <stdio.h>

#define NUM_VERTEX 10000

struct Vertex {
	int mark;
	struct Node* list;
};
struct Node {
	struct Vertex* vertex;
	struct Node* next;
};
typedef struct Vertex Vertex;
typedef struct Node Node;

// creates a new vertex and return pointer to it
Vertex* new_vertex() {
	Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
	vertex->list = 0;
	return vertex;
}

// connect the vertex a to vertex b
// by adding b to a's linked list!
void do_connect(Vertex* a, Vertex* b) {
	Node* node = (Node*)malloc(sizeof(Node));
	node->vertex = b;
	node->next = a->list;
	a->list = node;
}

// connects a to b and b to a
void connect(Vertex* a, Vertex* b) {
	do_connect (a, b);
	do_connect (b, a);
}

// visit function
void visit(Vertex* vertex) {
	printf ("\n%5d.", vertex->mark);
}

// do a depth first search
void do_dfs(Vertex *vertex, int* count) {
	Node* p = vertex->list;
	vertex->mark = ++(*count);
	visit (vertex);
	while (p != 0) {
		if (!p->vertex->mark) {
			do_dfs (p->vertex, count);
		}
		p = p->next;
	}
}
void dfs(Vertex *graph[]) {
	int i;
	int count = 0;
	// set all to unvisited
	for (i = 0; i < NUM_VERTEX; i ++) {
		graph[i]->mark = 0;
	}
	// each vertex dfs it
	for (i = 0; i < NUM_VERTEX; i ++) {
		if (graph[i]->mark == 0) {
			do_dfs (graph[i], &count);
		}
	}
}

// main function - their example, should replace with random graph
int main() {
	srand(0);
	// create a adjacency list
	Vertex *graph[NUM_VERTEX];

	for (int i = 0; i < NUM_VERTEX; ++i) {
		graph[i] = new_vertex();
	}

	for (int i = 0; i < NUM_VERTEX; ++i) {
		for (int j = i + 1; j < NUM_VERTEX; ++j) {
			if (rand() > 0.5) {
				connect(graph[i], graph[j]);
			}
		}
	}
	
	// depth first search
	printf ("DFS: ");
	dfs (graph);
}