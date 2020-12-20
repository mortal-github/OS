#ifndef _HEAP__H_
#define _HEAP__H_

#include "Collection.h"
#include "ArrayList.h"

typedef struct Heap {
	ArrayList* list;
	/*-1：小顶堆；0：大顶堆。1: 大顶堆*/
	Int tag;
	/*返回1表示left优先right,-1表示right优先left,0表示right等价left。*/
	Int(*prior)(Element left, Element right);
}Heap;

/*
 初始化堆。
 输入条件：NULL!=list,NULL!=list,tag=-1或tag=1,NULL!=prior。
 正常处理：初始化堆,返回TRUE。
 错误处理：若不满足输入条件或初始化失败，则什么都不处理，返回FALSE。
 隐式条件：堆初始化过,prior确实比较优先级并按要求返回-1，0，1。
*/
Boolean Heap_init(Heap* heap, ArrayList* list, Int tag, Int(*prior)(Element left, Element right));
/*
 销毁堆。
 输入条件：NULL!=list。
 正常处理：清空堆元信息，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE。
 隐式条件：堆是初始化过的，且只通过函数写堆。
*/
Boolean Heap_destroy(Heap* heap);
/*
 设置堆类型。
 输入条件：NULL!=list，-1=tag或1=tag。
 正常处理：设置堆类型，-1表示小顶堆，1表示大顶堆，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE。
 隐式条件：堆是初始化过的，且只通过函数写堆。
*/
Boolean Heap_setTag(Heap* heap, Int tag);
/*
 设置堆优先函数。
 输入条件：NULL!=list，NULL!=prior。
 正常处理：设置堆优先函数，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE。
 隐式条件：堆是初始化过的，且只通过函数写堆；prior确实比较优先级并按要求返回-1，0，1。。
*/
Boolean Heap_setPrior(Heap* heap, Int(*prior)(Element left, Element right));

/*
 筛选子堆，假设index索引的元素左右子树为堆，将以该元素为根的树筛选成堆。
 输入条件：NULL!=heap,index>=0,index<size。
 正常处理：假设根元素的两个子树皆为堆，将以该根元素为根的树调整为堆，返回TRUE。
 错误处理：若不满足输入条件，则返回FALSE。
 隐式条件：heap经过初始化，且只通过函数写heap。
*/
Boolean Heap_shiftDown(Heap* heap, Int index);
/*调整为堆。
输入条件：NULL!=heap。
正常处理：将heap内的元素排列成堆。
错误处理：若不满足输入条件，则什么都不做，返回FALSE。
隐式条件：Heap是初始化过的，且只通过函数处理Heap。*/
Boolean Heap_adjust(Heap* heap);
/*
 排序堆元素
 输入条件：NULL!=heap。
 正常处理：把堆内元素排序成有序，大顶堆升序，小顶堆降序。
 错误处理：若不满足输入条件，则什么都不做返回FALSE；。
 隐式条件：Heap是初始化过的，且只通过函数处理Heap，且heap内元素构成堆。
*/
Boolean Heap_sort(Heap* heap);

/*
 堆插入一个元素。
 输入条件：NULL!=list,NULL!=element。
 正常处理：堆指插入一个元素。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE;若插入元素失败，则什么都不做，返回FALSE。
 隐式条件：堆是初化过的，且只通过函数写堆。
*/
Boolean Heap_add(Heap* heap, Element element);
/*
 删除堆指定位置的一个元素。
 输入条件：NULL!=heap，index>=0, index<size。
 正常处理：删除堆指定位置的一个元素，返回该元素。
 错误处理：若不满足输入条件，则什么都不做，返回空引用；若删除失败，则什么都不做。
 隐式条件：堆是初始化过的，且只通过函数写堆。
*/
Element Heap_remove(Heap* heap, Int index);
/*
 更改堆指定位置的元素为新值。
 输入条件：NULL!=heap，index>=0, index<size, NULL!=element。
 正常处理：更改堆指定位置的元素为新值,返回旧值。
 错误处理：若不满足输入条件，则什么都不做，返回空引用。
 隐式条件：堆是初始化过的，且只通过函数写堆。
*/
Element Heap_set(Heap* heap, Int index, Element element);

#endif // !_HEAP__H_
