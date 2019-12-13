#include <stdlib.h>

#define NUM_VERTEX 10

struct Vertex {
	char name;
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
Vertex* new_vertex(char name) {
	Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
	vertex->name = name;
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
	printf ("\n%5d. %c", vertex->mark, vertex->name);
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
	
	// create a adjacency list
	Vertex *graph[NUM_VERTEX] = {
		new_vertex('A'), //[0]
		new_vertex('B'), //[1]
		new_vertex('C'), //[2]
		new_vertex('D'), //[3]
		new_vertex('E'), //[4]
		new_vertex('F'), //[5]
		new_vertex('G'), //[6]
		new_vertex('H'), //[7]
		new_vertex('I'), //[8]
		new_vertex('J')  //[9]
	};
	
	// connect the vertices
	connect(graph[0], graph[2]);
	connect(graph[0], graph[3]);
	connect(graph[0], graph[4]);
	connect(graph[1], graph[4]);
	connect(graph[1], graph[5]);
	connect(graph[2], graph[3]);
	connect(graph[2], graph[5]);
	connect(graph[4], graph[5]);
	connect(graph[6], graph[7]);
	connect(graph[6], graph[9]);
	connect(graph[7], graph[8]);
	connect(graph[8], graph[9]);
	
	// depth first search
	printf ("DFS: ");
	dfs (graph);
}