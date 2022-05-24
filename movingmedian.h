#ifndef MOVINGMEDIAN_H
#define MOVINGMEDIAN_H

typedef double Item;

typedef struct Mediator_t
{
   Item* data;  //circular queue of values
   int*  pos;   //index into `heap` for each value
   int*  heap;  //max/median/min heap holding indexes into `data`.
   int   N;     //allocated size.
   int   idx;   //position in circular queue
   int   ct;    //count of items in queue
} Mediator;

Mediator* MediatorNew(int nItems);

void MediatorInsert(Mediator* m, Item v);

Item MediatorMedian(Mediator* m);

#endif // !MOVINGMEDIAN_H