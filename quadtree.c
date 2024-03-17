/* ZARIOIU Denisa-Elena - grupa 314CD */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structures.h"

int main(int argc, char** argv) {
    // deschiderea fisierelor de input si output
    FILE *fin = fopen(argv[argc-2], "rb");
    if(fin == NULL)
        return -1;
    FILE *fout = fopen(argv[argc-1], "wb");

    // interpretarea argumentelor din linia de comanda
    if(strcmp(argv[1], "-d") != 0) {
        // task 1 sau task 2

        // determining the compression factor
        unsigned long long factor = strtol(argv[2], NULL, 10);

        unsigned int size;
        fseek(fin, 3, SEEK_SET);
        // imaginea este patratica, deci dimensiunile sunt egale
        fscanf(fin, "%d", &size);
        fscanf(fin, "%d", &size);
        fseek(fin, 5, SEEK_CUR);

        // alocarea memoriei pentru matricea de pixeli
        PixelMatrix mat = malloc(size * sizeof(Pixel *));
        for(int i = 0; i < size; i++)
            mat[i] = malloc(size * sizeof(Pixel));

        for(int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
                fread(mat[i]+j, sizeof(Pixel), 1, fin);
        fclose(fin);

        // constructia arborelui de compresie
        QuadTree quadtree = createQuadTree(0, 0, size, 1), tree;
        Queue *q = createQueue(quadtree);
        unsigned long long mean;
        int terminal_nodes = 0, max_size = 0, levels = 0;
        while(!isQueueEmpty(q)) {
            tree = front(q);
            dequeue(q);
            mean = SimScore(tree, mat);
            if(mean <= factor) {
                terminal_nodes++;
                if(tree->size > max_size)
                    max_size = tree->size;
                if(tree->height > levels)
                    levels = tree->height;
            } else {
                // divizarea imaginii
                tree->first = createQuadTree(tree->x, tree->y, tree->size/2, tree->height+1);
                tree->second = createQuadTree(tree->x, tree->y + tree->size/2, tree->size/2, tree->height+1);
                tree->third = createQuadTree(tree->x + tree->size/2, tree->y + tree->size/2, tree->size/2, tree->height+1);
                tree->fourth = createQuadTree(tree->x + tree->size/2, tree->y, tree->size/2, tree->height+1);
                enqueue(q, tree->first);
                enqueue(q, tree->second);
                enqueue(q, tree->third);
                enqueue(q, tree->fourth);    
            }
        }
        // dealocarea memoriei utilizate pentru coada
        destroyQueue(q);

        if(strcmp(argv[1], "-c1") == 0) {
            // task 1 => afisarea statisticilor
            fprintf(fout, "%d\n%d\n%d\n", levels, terminal_nodes, max_size);
        } else {
            // task 2 => comprimarea imaginii 
            compression(quadtree, mat, fout);
        }
        // dealocarea memoriei utilizate pentru arbore
        destroyQuadTree(quadtree);
        // dealocarea memoriei utilizate pentru matricea de pixeli
        for(int i = 0; i < size; i++)
            free(mat[i]);
        free(mat);
    } else {
        // task 3
        unsigned int size;
        unsigned char type;
        fread(&size, sizeof(unsigned int), 1, fin);

        // alocarea matricii de pixeli pentru decompresie
        PixelMatrix mat = malloc(size * sizeof(Pixel *));
        for(int i = 0; i < size; i++)
            mat[i] = malloc(size * sizeof(Pixel));

        // reconstituirea arborelui de compresie
        QuadTree quadtree = createQuadTree(0, 0, size, 1), tree;
        Queue *q = createQueue(quadtree);
        while(!isQueueEmpty(q)) {
            tree = front(q);
            dequeue(q);
            fread(&type, sizeof(unsigned char), 1, fin);
            if(!type) {
                // nod intern
                tree->first = createQuadTree(tree->x, tree->y, tree->size/2, tree->height+1);
                tree->second = createQuadTree(tree->x, tree->y + tree->size/2, tree->size/2, tree->height+1);
                tree->third = createQuadTree(tree->x + tree->size/2, tree->y + tree->size/2, tree->size/2, tree->height+1);
                tree->fourth = createQuadTree(tree->x + tree->size/2, tree->y, tree->size/2, tree->height+1);
                enqueue(q, tree->first);
                enqueue(q, tree->second);
                enqueue(q, tree->third);
                enqueue(q, tree->fourth); 
            } else {
                // nod terminal
                fread(&tree->red, sizeof(unsigned char), 1, fin);
                fread(&tree->green, sizeof(unsigned char), 1, fin);
                fread(&tree->blue, sizeof(unsigned char), 1, fin);
                for(int i = tree->x; i < tree->x + tree->size; i++)
                    for(int j = tree->y; j < tree->y + tree->size; j++) {
                        mat[i][j].red = tree->red;
                        mat[i][j].green = tree->green;
                        mat[i][j].blue = tree->blue;
                    }
            }
        }
        fclose(fin);
        // dealocarea memoriei pentru arbore si coada
        destroyQuadTree(quadtree);
        destroyQueue(q);
        // construirea imaginii decomprimate
        fprintf(fout, "P6\n%d %d\n255\n", size, size);
        for(int i = 0; i < size; i++)
            for(int j = 0; j < size; j++)
                fwrite(&mat[i][j], sizeof(Pixel), 1, fout);
        // dealocarea memoriei pentru matricea de pixeli
        for(int i = 0; i < size; i++)
            free(mat[i]);
        free(mat);
    }
    return 0;
}