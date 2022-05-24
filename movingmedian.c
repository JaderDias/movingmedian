#include "movingmedian.h"
#include <stdlib.h>

//Customize for your data Item type
#define ItemLess(a,b)  ((a)<(b))
#define ItemMean(a,b)  (((a)+(b))/2)

/*--- Helper Functions ---*/

#define minCt(m) (((m)->ct-1)/2) //count of items in minheap
#define maxCt(m) (((m)->ct)/2)   //count of items in maxheap

//returns 1 if heap[i] < heap[j]
int mmless(Mediator* m, int i, int j)
{
   return ItemLess(m->data[m->heap[i]],m->data[m->heap[j]]);
}

//swaps items i&j in heap, maintains indexes
int mmexchange(Mediator* m, int i, int j)
{
   int t = m->heap[i];
   m->heap[i]=m->heap[j];
   m->heap[j]=t;
   m->pos[m->heap[i]]=i;
   m->pos[m->heap[j]]=j;
   return 1;
}

//swaps items i&j if i<j;  returns true if swapped
int mmCmpExch(Mediator* m, int i, int j)
{
   return (mmless(m,i,j) && mmexchange(m,i,j));
}

//maintains minheap property for all items below i/2.
void minSortDown(Mediator* m, int i)
{
   for (; i <= minCt(m); i*=2)
   {  if (i>1 && i < minCt(m) && mmless(m, i+1, i)) { ++i; }
      if (!mmCmpExch(m,i,i/2)) { break; }
   }
}

//maintains maxheap property for all items below i/2. (negative indexes)
void maxSortDown(Mediator* m, int i)
{
   for (; i >= -maxCt(m); i*=2)
   {  if (i<-1 && i > -maxCt(m) && mmless(m, i, i-1)) { --i; }
      if (!mmCmpExch(m,i/2,i)) { break; }
   }
}

//maintains minheap property for all items above i, including median
//returns true if median changed
int minSortUp(Mediator* m, int i)
{
   while (i>0 && mmCmpExch(m,i,i/2)) i/=2;
   return (i==0);
}

//maintains maxheap property for all items above i, including median
//returns true if median changed
int maxSortUp(Mediator* m, int i)
{
   while (i<0 && mmCmpExch(m,i/2,i))  i/=2;
   return (i==0);
}

/*--- Public Interface ---*/


//creates new Mediator: to calculate `nItems` running median.
//mallocs single block of memory, caller must free.
Mediator* MediatorNew(int nItems)
{
   int size = sizeof(Mediator)+nItems*(sizeof(Item)+sizeof(int)*2);
   Mediator* m=  malloc(size);
   m->data= (Item*)(m+1);
   m->pos = (int*) (m->data+nItems);
   m->heap = m->pos+nItems + (nItems/2); //points to middle of storage.
   m->N=nItems;
   m->ct = m->idx = 0;
   while (nItems--)  //set up initial heap fill pattern: median,max,min,max,...
   {  m->pos[nItems]= ((nItems+1)/2) * ((nItems&1)?-1:1);
      m->heap[m->pos[nItems]]=nItems;
   }
   return m;
}

 
//Inserts item, maintains median in O(lg nItems)
void MediatorInsert(Mediator* m, Item v)
{
   int isNew=(m->ct<m->N);
   int p = m->pos[m->idx];
   Item old = m->data[m->idx];
   m->data[m->idx]=v;
   m->idx = (m->idx+1) % m->N;
   m->ct+=isNew;
   if (p>0)         //new item is in minHeap
   {  if (!isNew && ItemLess(old,v)) { minSortDown(m,p*2);  }
      else if (minSortUp(m,p)) { maxSortDown(m,-1); }
   }
   else if (p<0)   //new item is in maxheap
   {  if (!isNew && ItemLess(v,old)) { maxSortDown(m,p*2); }
      else if (maxSortUp(m,p)) { minSortDown(m, 1); }
   }
   else            //new item is at median
   {  if (maxCt(m)) { maxSortDown(m,-1); }
      if (minCt(m)) { minSortDown(m, 1); }
   }
}

//returns median item (or average of 2 when item count is even)
Item MediatorMedian(Mediator* m)
{
   Item v= m->data[m->heap[0]];
   if ((m->ct&1)==0) { v= ItemMean(v,m->data[m->heap[-1]]); }
   return v;
}