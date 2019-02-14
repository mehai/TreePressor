#ifndef AUXILIARY
#define AUXILIARY
#include"auxiliary.h"
#endif

/*FUNCTII PENTRU ELIBERAREA MEMORIEI*/
void freeQTree(qtNode *node)
{
	if(node == NULL)
		return;
	freeQTree(node->v[0]);
	freeQTree(node->v[1]);
	freeQTree(node->v[2]);
	freeQTree(node->v[3]);
	free(node);
}

void freeMatrix(uch **img, int size)
{
	int i;
	for(i = 0; i < size; i++)
	{
		free(img[i]);
	}
	free(img);
}

/*FUNCTII FOLOSITE PENTRU CONSTRUIREA ARBORELUI*/
uch mediumValue(uch **img, Dimensions co, int k)
{
	/*OBS: k = 0/1/2 pt red/green/blue*/
	uint64_t sum = 0, i, j;
	for(i = co.x; i < co.x + co.size; i++)
		for(j = co.y + k; j < co.y + 3*co.size; j += 3)
			sum += img[i][j];
	sum /= co.size * co.size;
	return (uch)sum;
}

uint64_t checkSimilarity(uch **img, Dimensions co)
{
	int i, j;
	uint64_t mean = 0;
	uch red, green, blue;
	/*calculez valorile medii*/
	red = mediumValue(img, co, 0);
	green = mediumValue(img, co, 1);
	blue = mediumValue(img, co, 2);
	/*calculez scorul de similaritate*/
	for(i = co.x; i < co.x + co.size; i++)
		for(j = co.y; j < co.y + 3*co.size; j += 3)
			{
				mean += (red - img[i][j]) * (red - img[i][j]);
				mean += (green - img[i][j+1]) * (green - img[i][j+1]);
				mean += (blue - img[i][j+2]) * (blue - img[i][j+2]);
			}
	mean /= 3 * co.size * co.size;
	return mean;
}

void insert(qtNode *node, uch **img, Dimensions co, int factor)
{
	uint64_t scor;
	int initsize = co.size;
	scor = checkSimilarity(img, co);
	node->area = co.size * co.size;
	/*se completeaza cu valorile medii ale blocului*/
	node->red = mediumValue(img, co, 0);
	node->green = mediumValue(img, co, 1);
	node->blue = mediumValue(img, co, 2);
	/*daca e nod frunza*/
	if(scor <= factor)
	{	
		node->v[0] = NULL; 
		node->v[1] = NULL;
		node->v[2] = NULL; 
		node->v[3] = NULL;
	}
	/*daca e nevoie de divizare in continuare*/
	else
	{
		/*aloc memorie pentru fii*/
		node->v[0] = (qtNode *)malloc(sizeof(qtNode));
		node->v[1] = (qtNode *)malloc(sizeof(qtNode));
		node->v[2] = (qtNode *)malloc(sizeof(qtNode));
		node->v[3] = (qtNode *)malloc(sizeof(qtNode));
		/*inserez nodul pentru patratul top_left*/
		co.size = initsize / 2;
		insert(node->v[0], img, co, factor);
		/*inserez nodul pentru patratul top_right*/
		co.y += initsize*3 / 2;
		insert(node->v[1], img, co, factor);
		/*inserez nodul pentru patratul bottom_right*/
		co.x += initsize / 2;
		insert(node->v[2], img, co, factor);
		/*inserez nodul pentru patratul bottom_left*/
		co.y -= initsize*3 / 2 ;
		insert(node->v[3], img, co, factor);

	}
}

void buildQTree(qtNode **root, uch **img, int size, int factor)
{
	/*stabilesc coordonatele initiale*/
	Dimensions co;
	co.x = 0;
	co.y = 0;
	co.size = size;
	/*aloc memorie pentru nodul radacina*/
	*root = (qtNode *)malloc(sizeof(qtNode));
	/*incep inserarea*/
	insert(*root, img, co, factor);
}

/*FUNCTII DE CITIRE FISIER PPM*/
uch** imageRead(char *inp, int *size, int *maxRGB)
{
	uch **img;
	char type[3];
	int i, w, h;
	/*citesc tipul fisierului*/
	FILE *f = fopen(inp, "rb");
	fread(&type, sizeof(type), 1, f);
	type[2] = '\0';
	if(strcmp(type, "P6") != 0)
	{
		printf("Unexpected format!\n");
		exit(1);
	}
	/*citesc dimensiunile*/
	fscanf(f, "%d %d", &w, &h);
	if(w != h)
	{
		printf("Height and width must be the same!\n");
		exit(1);
	}
	/*citesc val max RGB*/
	fscanf(f, "%d", maxRGB);
	if(*maxRGB != 255)
	{
		printf("maxRGB must be 255!\n");
		exit(1);
	}
	/*sar peste \n*/
	fseek(f, 1, SEEK_CUR);
	/*aloc memorie pentru matricea de pixeli*/
	img = (uch **)malloc(h * sizeof(uch *));
	for(i = 0; i < h; i++)
	{
		img[i] = (uch *)malloc(3*w * sizeof(uch));
		/*citesc linie cu linie*/
		fread(img[i], sizeof(uch), 3*w, f);
	}
	fclose(f);
	*size = h;
	return img;
}

/*FUNCTIE CONSTRUIRE IMAGINE*/
void buildImage(uch **img, qtNode *node, Dimensions co)
{
	int i, j, initsize;
	initsize = co.size;
	/*daca e frunza*/
	if(node->v[0] == NULL)
	{
		/*introduc valorile in matrice*/
		for(i = co.x; i < co.x + co.size; i++)
			for(j = co.y; j < co.y + 3*co.size; j += 3)
				{
					img[i][j] = node->red;
					img[i][j+1] = node->green;
					img[i][j+2] = node->blue;
				}
	}
	/*daca e nod intern*/
	else
	{
		/*merg pe subpatratul 1*/
		co.size = initsize / 2;
		buildImage(img, node->v[0], co);
		/*merg pe subpatratul 2*/
		co.y += initsize*3 / 2;
		buildImage(img, node->v[1], co);
		/*merg pe subpatratul 3*/
		co.x += initsize / 2;
		buildImage(img, node->v[2], co);
		/*merg pe subpatratul 4*/
		co.y -= initsize*3 / 2 ;
		buildImage(img, node->v[3], co);
	}
	/*eliberez memoria alocata pentru nod*/
	free(node);
}