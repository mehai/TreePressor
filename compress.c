#ifndef AUXILIARY
#define AUXILIARY
#include"auxiliary.h"
#endif

#ifndef COMPRESS
#define COMPRESS
#include"compress.h"
#endif

void enqueue(Queue *q, qtNode *data, int *index)
{
	QueueNode *new = (QueueNode *)malloc(sizeof(QueueNode));
	new->data = data;
	new->index = *index;
	new->next = NULL;
	if(q->first == NULL)
		q->first = new;
	else
		q->last->next = new;
	q->last = new;
	*index += 1;
}

qtNode* dequeue(Queue *q)
{
	QueueNode *aux = q->first;
	qtNode *auxdata = q->first->data;
	q->first = q->first->next;
	free(aux);
	return auxdata;
}

void addNode(QuadtreeNode *v, Queue *q)
{
	int firstson = q->last->index + 1;
	QueueNode *qnode = q->first;
	int k = qnode->index;
	v[k].red = qnode->data->red;
	v[k].green = qnode->data->green;
	v[k].blue = qnode->data->blue;
	v[k].area = qnode->data->area;
	/*daca nodul mai are fii*/
	if(qnode->data->v[0] != NULL)
	{
		v[k].top_left = firstson;
		v[k].top_right = firstson + 1;
		v[k].bottom_right = firstson + 2;
		v[k].bottom_left = firstson + 3;
	}
	/*daca nodul nu mai are fii*/
	else
	{
		v[k].top_left = -1;
		v[k].top_right = -1;
		v[k].bottom_right = -1;
		v[k].bottom_left = -1;
	}
}

uint32_t countNodes(qtNode *root)
{
	int s = 0, i;
	if(root->v[0] == NULL)
		return 1;
	else
	{
		for(i = 0; i < 4; i++)
			s += countNodes(root->v[i]);
		return 1 + s;
	}
}

void compress(char *inputf, char *outputf, int factor)
{
	int i, size = 0, maxRGB = 0;
	uch **img;
	qtNode *root = NULL, *tmpnode = NULL;
	/*citesc datele imaginii*/
	img = imageRead(inputf, &size, &maxRGB);
	/*creez arborele cuaternar de compresie*/
	buildQTree(&root, img, size, factor);
	/*calculez numarul de noduri*/
	uint32_t nrNodes;
	nrNodes = countNodes(root);
	/*creez vectorul de structuri si o coada auxiliara pentru a parcurge
	 *arborele in latime
	 */
	QuadtreeNode *v = (QuadtreeNode *)malloc(nrNodes*sizeof(QuadtreeNode));
	Queue q;
	q.first = NULL;
	q.last = NULL;
	int index = 0;
	enqueue(&q, root, &index);
	/*cat timp coada nu e goala*/
	while(q.first != NULL)
	{
		addNode(v, &q);
		tmpnode = dequeue(&q);
		if(tmpnode->v[0] != NULL)
		{
			enqueue(&q, tmpnode->v[0], &index);
			enqueue(&q, tmpnode->v[1], &index);
			enqueue(&q, tmpnode->v[2], &index);
			enqueue(&q, tmpnode->v[3], &index);
		}
		free(tmpnode);
	}
	/*calculez numaruld e culori*/
	uint32_t nrColors = 0;
	for(i = 0; i < nrNodes; i++)
	{
		if(v[i].top_left == -1)
			nrColors++;
	}
	/*creez fisiserul comprimat*/
	FILE *compfile = fopen(outputf, "wb");
	fwrite(&nrColors, sizeof(uint32_t), 1, compfile);
	fwrite(&nrNodes, sizeof(uint32_t), 1, compfile);
	fwrite(v, sizeof(QuadtreeNode), nrNodes, compfile);
	fclose(compfile);
	/*eliberez memoria alocata dinamic*/
	freeMatrix(img, size);
	free(v);

}