#ifndef QUEUE
#define QUEUE

#define NULL ((void *)0) 
//FIFO

typedef struct TMessage{
	
} TMessage;

typedef TMessage valuetype;


typedef struct node
{
    valuetype data;
    struct node *next;
}QNode;

typedef struct
{
    QNode *font, *rear;
    unsigned int lenth;
}LQueue;


LQueue * Init_LQueue(void);
int Empty_LQueue(LQueue *q);
void In_LQueue(LQueue *q, valuetype x);
int Out_LQueue(LQueue *q, valuetype *x);


#endif