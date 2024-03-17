/* ZARIOIU Denisa-Elena - grupa 314CD */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

// creeaza un arbore cuaternar
QuadTree createQuadTree(int x, int y, int size, int height) {
    QuadTree tree = malloc(sizeof(QuadTreeNode));
    tree->first = tree->second = tree->third = tree->fourth = NULL;
    tree->size = size;
    tree->x = x;
    tree->y = y;
    tree->height = height;
    return tree;
}

// dealoca memoria pentru un arbore cuaternar
void destroyQuadTree(QuadTree tree) {
    if(tree == NULL)
        return;
    destroyQuadTree(tree->first);
    destroyQuadTree(tree->second);
    destroyQuadTree(tree->third);
    destroyQuadTree(tree->fourth);
    free(tree);
}

// determina culorile medii pentru un arbore
void setTreeColors(QuadTree tree, PixelMatrix mat) {
	unsigned long long red = 0, green = 0, blue = 0;
	for(int i = tree->x; i < tree->x+tree->size; i++)
    	for(int j = tree->y; j < tree->y+tree->size; j++) {
    		red += mat[i][j].red;
    		green += mat[i][j].green;
            blue += mat[i][j].blue;
		}
	tree->red = red/(tree->size*tree->size);
    tree->green = green/(tree->size*tree->size);
    tree->blue = blue/(tree->size*tree->size);
}

// returneaza scorul de similitudine pentru o regiune din imagine
unsigned long long SimScore(QuadTree tree, PixelMatrix mat) {
	unsigned long long mean = 0;
	setTreeColors(tree, mat);
    for(int i = tree->x; i < tree->x+tree->size; i++)
        for(int j = tree->y; j < tree->y+tree->size; j++) {
            mean += (tree->red-mat[i][j].red)*(tree->red-mat[i][j].red) + 
                    (tree->green-mat[i][j].green)*(tree->green-mat[i][j].green) +
                    (tree->blue-mat[i][j].blue)*(tree->blue-mat[i][j].blue);        
        }
    mean /= (3*tree->size*tree->size);
    return mean;
}

// realizeaza comprimarea imaginii cu ajutorul arborelui cuaternar de compresie
void compression(QuadTree tree, PixelMatrix mat, FILE* fout) {
	if(tree == NULL)
		return ;
	Queue *q = createQueue(tree);
	fwrite(&tree->size, sizeof(unsigned int), 1, fout);
	while(!isQueueEmpty(q)) {
		tree = front(q);
		dequeue(q);
		unsigned char type;
		if(tree->first == NULL) { 
			// nod terminal
			type = 1;
			setTreeColors(tree, mat);
			fwrite(&type, sizeof(unsigned char), 1, fout);
			fwrite(&tree->red, sizeof(unsigned char), 1, fout);
			fwrite(&tree->green, sizeof(unsigned char), 1, fout);
			fwrite(&tree->blue, sizeof(unsigned char), 1, fout);
		} else {
			// nod intern
			type = 0;
			fwrite(&type, sizeof(unsigned char), 1, fout);
			enqueue(q, tree->first);
			enqueue(q, tree->second);
        	enqueue(q, tree->third);
        	enqueue(q, tree->fourth);
		}
	}
	destroyQueue(q);
}

// creeaza un nod din coada
QueueNode* createQueueNode(QuadTree elem) {
	QueueNode *node = malloc(sizeof(QueueNode));
	node->elem = elem;
	node->next = NULL;
	return node;
} 

// creeaza o coada cu un element
Queue* createQueue(QuadTree elem) {
	Queue *q = malloc(sizeof(Queue));
	q->front = createQueueNode(elem);
	q->rear = q->front;
	return q;
} 

// verifica daca o coada este goala sau nu
int isQueueEmpty(Queue *q) {
	return q == NULL || q->front == NULL;
}

// adauga un nod in coada
void enqueue(Queue *q, QuadTree elem) {
	if(q == NULL)
		return ;
	QueueNode *node = createQueueNode(elem);
	if(isQueueEmpty(q))
		q->front = node;
	else
		q->rear->next = node;
	q->rear = node;
}

// returneaza primul element din coada
QuadTree front(Queue* q) {
	if(isQueueEmpty(q))
		return 0;
	return q->front->elem;
}

// elimina un element din coada
void dequeue(Queue* q) {
	if(isQueueEmpty(q))
		return ;
	if(q->front == q->rear) {
		free(q->front);
		q->front = q->rear = NULL;
		return ;
	}
	QueueNode *tmp = q->front;
	q->front = q->front->next;
	free(tmp);
}

// dealoca memoria pentru coada
void destroyQueue(Queue *q) {
	if(q == NULL)
		return ;
	QueueNode *iter = q->front, *tmp;
	while(iter != NULL) {
		tmp = iter;
		iter = iter->next;
		free(tmp);
	}
	free(q);
}