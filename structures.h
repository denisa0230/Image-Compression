/* ZARIOIU Denisa-Elena - grupa 314CD */

#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

// structura pentru arborele cuaternar de compresie
typedef struct node {
    int x, y; // indicii corespunzatori din matricea de pixeli
    int size; // dimensiunea zonei aferente din matricea de pixeli
    int height; // inaltimea nodului din arbore
    unsigned char red, green, blue; // culorile medii ale zonei aferente din matricea de pixeli
    // cele 4 subdiviziuni in care se imparte zona aferenta nodului din matricea de pixeli
    struct node *first;
    struct node *second;
    struct node *third;
    struct node *fourth;
} QuadTreeNode, *QuadTree;

// structura unui pixel
typedef struct pixel {
    unsigned char red, green, blue; // canalele de culori
} Pixel, **PixelMatrix;

// structura pentru un nod din coada
typedef struct QueueNode {
	QuadTree elem;
	struct QueueNode *next;
} QueueNode;

// structura pentru coada
typedef struct Queue {
	QueueNode *front;
	QueueNode *rear;
} Queue;

void setTreeColors(QuadTree tree, PixelMatrix mat);
unsigned long long SimScore(QuadTree tree, PixelMatrix mat);
QuadTree createQuadTree(int x, int y, int size, int height);
void destroyQuadTree(QuadTree tree);
Queue* createQueue(QuadTree elem);
int isQueueEmpty(Queue *q);
void enqueue(Queue *q, QuadTree elem);
QuadTree front(Queue* q);
void dequeue(Queue* q);
void destroyQueue(Queue *q);
void compression(QuadTree tree, PixelMatrix mat, FILE* fout);

#endif