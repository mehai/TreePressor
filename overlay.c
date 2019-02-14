#ifndef AUXILIARY
#define AUXILIARY
#include"auxiliary.h"
#endif

void overlaying(qtNode *node1, qtNode *node2, qtNode *node3)
{
	node3->red = (node1->red + node2->red) / 2;
	node3->green = (node1->green + node2->green) / 2;
	node3->blue = (node1->blue + node2->blue) / 2;
	/*daca ambii arboti se opresc in acest punct*/
	if(node1->v[0] == NULL && node2->v[0] == NULL)
	{
		node3->v[0] = NULL;
		node3->v[1] = NULL;
		node3->v[2] = NULL;
		node3->v[3] = NULL;
	}
	else
	{
		/*aloc memorie pentru fii*/
		node3->v[0] = (qtNode *)malloc(sizeof(qtNode));
		node3->v[1] = (qtNode *)malloc(sizeof(qtNode));
		node3->v[2] = (qtNode *)malloc(sizeof(qtNode));
		node3->v[3] = (qtNode *)malloc(sizeof(qtNode));
		/*daca niciunul din arbori nu se opreste aici*/
		if(node1->v[0] != NULL && node2->v[0] != NULL)
		{
			overlaying(node1->v[0], node2->v[0], node3->v[0]);
			overlaying(node1->v[1], node2->v[1], node3->v[1]);
			overlaying(node1->v[2], node2->v[2], node3->v[2]);
			overlaying(node1->v[3], node2->v[3], node3->v[3]);
		}
		/*daca arborele 1 se opreste aici*/
		else if(node1->v[0] == NULL)
		{
			overlaying(node1, node2->v[0], node3->v[0]);
			overlaying(node1, node2->v[1], node3->v[1]);
			overlaying(node1, node2->v[2], node3->v[2]);
			overlaying(node1, node2->v[3], node3->v[3]);
		}
		/*daca arborele 2 se opreste aici*/
		else
		{
			overlaying(node1->v[0], node2, node3->v[0]);
			overlaying(node1->v[1], node2, node3->v[1]);
			overlaying(node1->v[2], node2, node3->v[2]);
			overlaying(node1->v[3], node2, node3->v[3]);
		}
	}
}

void overlay(char *inputf1, char *inputf2, char *outputf, int factor)
{
	int i, size = 0, maxRGB = 0;
	uch **img;
	qtNode *root1 = NULL, *root2 = NULL;
	/*citesc datele imaginii 1*/
	img = imageRead(inputf1, &size, &maxRGB);
	/*creez arborele cuaternar de compresie 1*/
	buildQTree(&root1, img, size, factor);
	freeMatrix(img, size);
	/*citesc datele imaginii 2*/
	img = imageRead(inputf2, &size, &maxRGB);
	/*creez arborele cuaternar de compresie 2*/
	buildQTree(&root2, img, size, factor);
	/*realizez suprapunerea printr-un al 3-lea arbore*/
	qtNode *root3 = (qtNode *)malloc(sizeof(qtNode));
	overlaying(root1, root2, root3);
	/*creez imaginea pe baza arborelui 3*/
	Dimensions co;
	co.size = size;
	co.x = 0;
	co.y = 0;
	buildImage(img, root3, co);
	/*creez fisierul de output*/
	FILE *ppmfile = fopen(outputf, "wb");
	fprintf(ppmfile, "P6\n");
	fprintf(ppmfile, "%d %d\n", co.size, co.size);
	fprintf(ppmfile, "%d\n", 255);
	for(i = 0; i < co.size; i++)
		fwrite(img[i], sizeof(uch), co.size*3, ppmfile);
	fclose(ppmfile);
	/*eliberez memoria alcoata dinamic*/
	freeMatrix(img, size);
	freeQTree(root1);
	freeQTree(root2);

}