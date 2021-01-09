#ifndef _DYNAMIC_PARTITION__H_
#define _DYNAMIC_PARITITION__H_

#include "LinkList.h"

typedef struct Partition {
	Pointer address;
	Int allocated;
	Int size;
}Partition;
/*
 list是空闲分区链，size是初始储存区大小。
 输入条件：NULL!=list, size>0。
 正常处理：初始化分区。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE。
*/
Boolean DynamicPartition_init(LinkList* list, Int size);

/*
 按首次适应算法申请size字节的空间。
 输入条件：NULL!=list, size>=512。
 正常处理：申请size字节的空间。
 错误处理：若不满足输入条件，则什么都不做，返回NULL。
*/
Pointer DynamicPartition_mallocFirst(LinkList* list, Int size);
/*
 按首次适应算法释放address空间。
 输入条件：NULL!=list,NULL!=address。
 正常处理：按首次适应算法释放address空间，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，若地址不合法，则什么都不做，返回FALSE.
*/
Boolean DynamicPartition_freeFirst(LinkList* list, Pointer address);


/*
 按最佳适应算法申请size字节的空间。
 输入条件：NULL!=list, size>=512。
 正常处理：申请size字节的空间。
 错误处理：若不满足输入条件，则什么都不做，返回NULL。
*/
Pointer DynamicPartition_mallocMin(LinkList* list, Int size);
/*
 按最佳适应算法释放address空间。
 输入条件：NULL!=list,NULL!=address。
 正常处理：按首次适应算法释放address空间，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，若地址不合法，则什么都不做，返回FALSE.
*/
Boolean DynamicPartition_freeMin(LinkList* list, Pointer address);

Boolean DynamicPartition_print(LinkList* list);

void DynamicPartition_test();

void DynamicPartition_show();
#endif // !_DYNAMIC_PARTITION__H_
