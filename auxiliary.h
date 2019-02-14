#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

typedef struct QuadtreeNode {
	unsigned char blue, green, red;
	uint32_t area;
	int32_t top_left, top_right;
	int32_t bottom_left, bottom_right;
} __attribute__ (( packed)) QuadtreeNode;

typedef struct qtNode {
	unsigned char blue, green, red;
	uint32_t area;
	struct qtNode *v[4];
}qtNode;

typedef struct {
	/*x - indice linii; y - indice coloane*/
	int x, y, size;
}Dimensions;

typedef unsigned char uch;

void freeQTree(qtNode *node);
void freeMatrix(uch **img, int size);
uch mediumValue(uch **img, Dimensions co, int k);
uint64_t checkSimilarity(uch **img, Dimensions co);
void insert(qtNode *node, uch **img, Dimensions co, int factor);
void buildQTree(qtNode **root, uch **img, int size, int factor);
uch** imageRead(char *inp, int *size, int *maxRGB);
void buildImage(uch **img, qtNode *node, Dimensions co);
