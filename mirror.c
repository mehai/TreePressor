#ifndef AUXILIARY
#define AUXILIARY
#include"auxiliary.h"
#endif

void swap(qtNode *node, int i, int j)
{
	qtNode *tmp;
	tmp = node->v[i];
	node->v[i] = node->v[j];
	node->v[j] = tmp;
}

void mirroringH(qtNode *node)
{
	/*daca nodul nu mare are fii*/
	if(node->v[0] == NULL)
		return;
	/*inversarea fiilor top_left - top_right*/
	swap(node, 0, 1);
	/*inversarea fiilor bottom-right - bottom_left*/
	swap(node, 2, 3);
	mirroringH(node->v[0]);
	mirroringH(node->v[1]);
	mirroringH(node->v[2]);
	mirroringH(node->v[3]);
}

void mirroringV(qtNode *node)
{
	/*daca nodul nu mare are fii*/
	if(node->v[0] == NULL)
		return;
	/*inversarea fiilor top_left - bottom_left*/
	swap(node, 0, 3);
	/*inversarea fiilor top_right - bottom_right*/
	swap(node, 1, 2);
	mirroringV(node->v[0]);
	mirroringV(node->v[1]);
	mirroringV(node->v[2]);
	mirroringV(node->v[3]);
}

void mirror(char *inputf, char *outputf, char type, int factor)
{
	int i, size = 0, maxRGB = 0;
	uch **img;
	qtNode *root = NULL;
	/*citesc datele imaginii*/
	img = imageRead(inputf, &size, &maxRGB);
	/*creez arborele cuaternar de compresie*/
	buildQTree(&root, img, size, factor);
	/*realizez oglindirea*/
	if(type == 'h')
		mirroringH(root);
	else if(type == 'v')
		mirroringV(root);
	else
		printf("incorrect type!\n");
	/*creez matricea*/
	Dimensions co;
	co.size = size;
	co.x = 0;
	co.y = 0;
	buildImage(img, root, co);
	/*creez fisierul ppm*/
	FILE *ppmfile = fopen(outputf, "wb");
	fprintf(ppmfile, "P6\n");
	fprintf(ppmfile, "%d %d\n", co.size, co.size);
	fprintf(ppmfile, "%d\n", 255);
	for(i = 0; i < co.size; i++)
		fwrite(img[i], sizeof(uch), co.size*3, ppmfile);
	fclose(ppmfile);
	/*eliberez memoria alocata dinamic*/
	freeMatrix(img, size);

}