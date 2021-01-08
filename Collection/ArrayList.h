#ifndef _ARRAY_LIST__H_
#define _ARRAY_LIST__H_

#include "Primitive.h"

typedef struct ArrayList {
	Element* elements;
	Int capacity;
	Int size;
}ArrayList;

#define DEFAULT_CAPACITY 25
#define DEFAULT_INCREMENT 10
/*
 初始化数组链表。
 输入条件：NULL!=list, capacity>0。
 正常处理：初始化储存空间大小为capacity数组链表,返回TRUE。
 错误处理：若不满足输入条件或初始化失败，则什么都不处理，返回FALSE。
 隐式条件：无。
*/
Boolean ArrayList_initCapacity(ArrayList* list, Int capacity);
/*
 使储存空间足够大小。
 输入条件：NULL!=list，capacity>0。
 正常处理：确保数组链表在不重新分配储存空间的情况下就能够保存给定数量的元素，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE;若处理失败，则什么都不做，返回FALSE。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表。
*/
Boolean ArrayList_ensureCapacity(ArrayList* list, Int capacity);
/*
 将数组链表的储存容量缩减到当前尺寸。
 输入条件：NULL!=list。
 正常处理：将数组链表的储存容量缩减到当前尺寸，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE;若处理失败，则什么都不做，返回FALSE。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表。
*/
Boolean ArrayList_trimToSize(ArrayList* list);

/*
 初始化数组链表。
 输入条件：NULL!=list。
 正常处理：初始化数组链表,返回TRUE。
 错误处理：若不满足输入条件或初始化失败，则什么都不处理，返回FALSE。
 隐式条件：无。
*/
Boolean ArrayList_init(ArrayList* list);
/*
 销毁数组链表。
 输入条件：NULL!=list。
 正常处理：释放数组链表的储存空间，并清空数组链表元信息，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表。
*/
Boolean ArrayList_destroy(ArrayList* list);

/*
 在数组链表指定位置插入一个元素。
 输入条件：NULL!=list, index>=0, index<=size,NULL!=element。
 正常处理：在数组链表指定位置插入一个元素。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE;若插入元素失败，则什么都不做，返回FALSE。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表。
*/
Boolean ArrayList_add(ArrayList* list, Int index, Element element);
/*
 数组链表删除指定位置的一个元素。
 输入条件：NULL!=list，index>=0, index<size。
 正常处理：删除指定位置的一个元素，返回该元素。
 错误处理：若不满足输入条件，则什么都不做，返回空引用。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表。
*/
Element ArrayList_remove(ArrayList* list, Int index);
/*
 获取数组链表指定位置的一个元素。
 输入条件：NULL!=list，index>=0, index<size。
 正常处理：获取指定位置的一个元素。
 错误处理：若不满足输入条件，则什么都不做，返回空引用。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表。
*/
Element ArrayList_get(ArrayList* list, Int index);
/*
 更改数组链表指定位置的元素为新值。
 输入条件：NULL!=list，index>=0, index<size, NULL!=element。
 正常处理：更改数组链表指定位置的元素为新值,返回旧值。
 错误处理：若不满足输入条件，则什么都不做，返回空引用。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表。
*/
Element ArrayList_set(ArrayList* list, Int index, Element element);

/*
 交换两个位置的元素。
 输入条件：NULL!=list,index1>=0, index1<size,index2>=0, index2<size。
 正常处理：交换两个位置的元素。
 错误处理：若不满足输入条件，则什么都不做。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表。
*/
void ArrayList_swap(ArrayList* list, Int index1, Int index2);

/*
 获取等价于element的第一元素的索引。
 输入条件：NULL!=list,NULL!=element,NULL!=equals。
 正常处理：获取等价于element的第一元素的索引。
 错误处理：不满足输入条件或查找失败，返回-1。
 隐式条件：数组链表是初始化过的，且只通过函数写数组链表；equals确实比较element和list中的元素。
*/
Int ArrayList_indexOf(ArrayList* list, Element element, Boolean(*equals)(Element left, Element right));


#endif // !_ARRAY_LIST__H_
