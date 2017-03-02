#include"stdafx.h"
#include"Queue.h"
#include "malloc.h"

LQueue * Init_LQueue()
{
	LQueue *q;
    QNode *p;
    q = (LQueue*)malloc(sizeof(LQueue)); /*申请链队指针*/
    p = (QNode*)malloc(sizeof(QNode));  /*申请头尾指针结点*/
    p->next =(struct node *)NULL;
    
    q->font = q->rear = p;
	q->lenth = 0;
    return q;
}


int Empty_LQueue(LQueue *q)
{
	if(q->font != q->rear)  
		return 0;
    else    
		return 1;
}


int Out_LQueue(LQueue *q, valuetype *x)
{
	if(Empty_LQueue(q)) 
		return 0;   /*空链队*/
    else
	{
		q->lenth--;
        QNode *p = q->font->next;
        *x = p->data;
        q->font->next = p->next;
        free(p);
        if (q->font->next == NULL)
            q->rear = q->font;
        return 1;
    }
}

void In_LQueue(LQueue *q, valuetype x)
{
	QNode *p = (QNode *)malloc(sizeof(QNode));  /*申请新结点*/
    p->data = x;
    p->next = (struct node *)NULL;
    q->rear->next = p;
    q->rear = p;
	q->lenth++;
}
