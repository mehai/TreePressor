typedef struct queue {
	qtNode *data;
	int index;
	struct queue *next;
}QueueNode;

typedef struct{
	QueueNode *first;
	QueueNode *last;
}Queue;

void enqueue(Queue *q, qtNode *data, int *index);
qtNode* dequeue(Queue *q);
void addNode(QuadtreeNode *v, Queue *q);
uint32_t countNodes(qtNode *root);
void compress(char *inputf, char *outputf, int factor);
