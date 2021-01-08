#ifndef _INDEX_LIST__H_
#define _INDEX_LIST__H_

#include "Primitive.h"

/*
 索引表。
 size     ：有效元素的个数
 top      ：最后一个元素之后的位置。
 max      ：即indexs数组的长度。
 unit     ：elements数组中每个元素的大小。
 length   ：elements数组的大小，以元素大小为单位。
 indexs   ：记录了elements数组中顶标之前空闲位置索引。
 elements ：储存元素的空间。
*/
typedef struct IndexList {
	/*
	尺寸：有效元素的个数
	*/
	Int size;
	/*
	顶标：最后一个元素之后的位置。
	*/
	Int top;
	/*
	最大索引数量：即indexs数组的长度。
	*/
	Int max;
	/*
	元素单位：elements数组中每个元素的大小。
	*/
	Int unit;
	/*
	长度/元素：elements数组的大小，以元素大小为单位。
	*/
	Int length;
	/*
	索引数组：记录了elements数组中顶标之前空闲位置索引。
	*/
	Pointer indexs;
	/*
	元素数组：储存元素。
	*/
	Pointer elements;
}IndexList;

/*
 断言索引表，若索引表经过初始化，且之后只通过操作函数来操作索引表，则满足断言。
 输入条件：无。
 正常处理：断言索引表，若索引表经过初始化，且之后只通过操作函数来操作索引表，则满足断言。
 错误处理：无。
*/
void IndexList_assert(IndexList list);//测毕
/*
 计算索引表的索引数组所需空间大小。
 输入条件：max>0。
 正常处理：计算索引表的索引数组所需空间大小，返回值。
 错误处理：返回0。
*/
Int IndexList_sizeofIndexs(Int max);//不测
/*
 计算索引表的元素数组所需空间大小。
 输入条件：unit>0, length>0。
 正常处理：计算索引表的元素数组所需空间大小，返回值。
 错误处理：返回0。
*/
Int IndexList_sizeofElements(Int unit, Int length);//不测

/*
 初始化索引表。
 输入条件：NULL!=list,max>0,NULL!=indexs,unit>0,length>0,NULL!=elements。
 正常处理：初始化表，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE。
 注意：indexs储存空间至少要有max*sizeof(Int), elements储存空间大小至少要有unit*length。
*/
Boolean IndexList_init(IndexList* list, Int max, Pointer indexs, Int unit, Int length, Pointer elements);//测毕
/*
 把origin表克隆到target表中。
 输入条件：NULL!=origin,NULL!=target, target->size=0, target->max>=origin->top-origin->size, target->unit==origin->unit, target->length>=origin->top。
 正常处理：把origin表的克隆到target表中。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE。
 隐式条件：两个表是初始化过的，且只通过函数写表。
*/
Boolean IndexList_clone(IndexList* origin, IndexList* target);//测毕
/*
 调整索引表的元素位置，释放冗余的控制信息，即清空索引数组。
 输入条件：NULL!=list。
 正常处理：调整索引表的元素位置，释放冗余的控制信息，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE。
 隐式条件：表是初始化过的，且只通过函数写表。
 注意：调整后，部分元素的位置发生改变，如果以来索引或地址来确定元素，那么可能会出错。
 解决方法可以是重新使用次序来求新索引。
*/
Boolean IndexList_trim(IndexList* list);
/*
 获取元素在表中的次序。
 输入条件：NULL!=list,index>=0, index<list->top。
 正常处理：获取索引元素在表中的次序，返回序号。
 错误处理：若不满足输入条件，则什么都不做，返回-1;若索引位置是空闲的，返回-1。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Int IndexList_getOrderFromIndex(IndexList* list, Int index);//间接测毕
/*
 根据元素在表中次序获取其索引。
 输入条件：NULL!=list,order>=0, order<list->size。
 正常处理：根据索引元素在表中次序获取其索引，返回索引。
 错误处理：若不满足输入条件，则什么都不做，返回-1。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Int IndexList_getIndexFromOrder(IndexList* list, Int order);//间接测毕
/*
 计算表中索引对应指针。
 输入条件：NULL!=list,index>=0, index<list->top。
 正常处理：计算表中索引对应指针，返回指针。
 错误处理：若不满足输入条件，则什么都不做，返回NULL。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Pointer IndexList_getAddressFromIndex(IndexList* list, Int index);//不测
/*
 根据元素在表中的指针获取其索引。
 输入条件：NULL!=list, NULL!=element。
 正常处理：根据元素在表中的指针获取其索引，返回索引。
 错误处理：若不满足输入条件，则什么都不做，返回-1；若元素不在表中或指针没有指向元素首地址，返回-1。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Int IndexList_getIndexFromAddress(IndexList* list, Pointer element);//不测
/*
 返回一个距离index索引最近的空闲索引在indexs的位置，若它自己是空闲的，则返回自己的位置。
 输入条件：NULL!=list,index>=0,index<list->length。
 正常处理：返回一个距离index索引最近的空闲索引在indexs的位置，若它自己是空闲的，则返回自己的位置。
 错误处理：若不满足输入条件，则返回-1，若没有找到，则返回-1。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Int IndexList_getNearFree(IndexList* list, Int index);//间接测毕

/*
 获取元素位序获得元素的指针。
 输入条件：NULL!=list，order>=0, order<list->size。
 正常处理：获取元素位序获得元素的指针。
 错误处理：若不满足输入条件，则什么都不做，返回NULL。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Pointer IndexList_get(IndexList* list, Int order);//测毕
/*
 设置第order个元素为新值。
 输入条件：NULL!=list,order>=0,order<list->size, NULL!=element。
 正常处理：设置第order个元素为新值，element是新值内容的指针，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，则返回FALSE。
 隐式条件：表是初始化过的，且只通过函数写表；element是合理的。
 */
Boolean IndexList_set(IndexList* list, Int order, Pointer element);//测毕
/*
 在表order次序位置插入一个元素。
 输入条件：NULL!=list, order>=0, order<=list->size,NULL!=element。
 正常处理：在表order次序位置插入一个元素。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE;若插入元素失败，则什么都不做，返回FALSE。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Boolean IndexList_add(IndexList* list, Int order, Pointer element);//测毕
/*
 删除表中order次序位置的一个元素。
 输入条件：NULL!=list，order>=0, order<list->size。
 正常处理：删除order次序位置的一个元素，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Boolean IndexList_remove(IndexList* list, Int order);//测毕
/*
 从索引表中申请一个空闲元素单元，返回其索引。
 输入条件：NULL!=list, list->size<list->length。
 正常处理：从索引表中申请一个空闲元素单元，返回其索引。
 错误处理：若不满足输入条件，则什么都不做，则返回-1。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Int IndexList_malloc(IndexList* list);
/*
 释放表中index索引位置的元素空间。
 输入条件：NULL!=list，index>0, index<list->top。
 正常处理：释放表中index索引位置的元素空间，则返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE；若释放失败或索引本身是空闲的，返回FALSE。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Boolean IndexList_free(IndexList* list, Int index);
/*
 释放表全部元素空间。
 输入条件：NULL!=list。
 正常处理：释放表全部元素空间，则返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Boolean IndexList_freeAll(IndexList* list);

/*
 交换两个位置的元素。
 输入条件：NULL!=list,list->size<list->length,order1!=order2,order1>=0,order2>=0,order1<list->size,order2<list->szie。
 正常处理：交换两个位置的元素,返回TRUE。
 错误处理：若不满足输入条件，则什么都不做,返回FALSE。
 隐式条件：表是初始化过的，且只通过函数写表。
*/
Boolean IndexList_swap(IndexList* list, Int order1, Int order2);//测毕
/*
 遍历表的每个元素，对每个元素调用Element_runable。
 输入条件：NULL!=array,NULL!=Element_runable。
 正常处理：遍历表的每个元素，对每个元素调用Element_runable，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE。若调用Element_runable返回FALSE,停止遍历，返回FALSE。
 隐式条件：表是初始化过的，且只通过函数写表。
 */
Boolean IndexList_forEach(IndexList* list, Boolean(*Element_runable)(Pointer element));//测毕

void IndexList_test(Boolean(*Element_printf)(Int* element));
#endif // !_INDEX_LIST__H_
