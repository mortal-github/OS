#include "DynamicPartition.h"
#include "IndexList.h"
#include <math.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include "ArrayList.h"
#include <time.h>
#include <stdlib.h>

Boolean DynamicPartition_init(LinkList* list, Int size)
{
	if (NULL == list || size <= 0) {
		return FALSE;
	}
	Boolean success;
	success = LinkList_init(list);
	assert(TRUE == success);

	Partition* first = malloc(sizeof(Partition));
	assert(NULL != first);
	first->address = (Pointer)1;
	first->allocated = 0;
	first->size = size;
	
	success = LinkList_add(list, first);
	assert(TRUE == success);

	return TRUE;
}

Pointer DynamicPartition_mallocFirst(LinkList* list, Int size)
{
	if (NULL == list || size < 512) {
		return NULL;
	}
	//寻找可用区。
	LinkNode* node = list->head;
	LinkNode* next = list->head;
	assert(NULL != list->head);
	Partition* partition;
	do {
		node = next;
		partition = (Partition*)node->element;
		next = node->next;
	} while (((partition->size) < size || (1 == partition->allocated)) && (NULL != next));
	//没有可用区。
	if (partition->size < size || (1 == partition->allocated)) {
		return NULL;
	}
	//有可用区
	assert(partition->size >= size && 0 == partition->allocated);
	//剩余空间过小，分配整个空间
	if (partition->size - size <= 512) {
		partition->allocated = 1;
		return partition->address;
	}
	//需要切割分区
	assert(partition->size - size > 512);
	Partition* allocated = malloc(sizeof(Partition));
	assert(NULL != allocated);

	allocated->address = partition->address;
	allocated->allocated = 1;
	allocated->size = size;
	partition->address = (Byte*)allocated->address + allocated->size;
	partition->size = partition->size - allocated->size;
	//前割分区插入原分区前面。
	LinkNode* new_node =  LinkList_addPrevious(list, node, allocated);
	assert(NULL != new_node);

	return allocated->address;
}

Boolean DynamicPartition_freeFirst(LinkList* list, Pointer address)
{
	if (NULL == list || NULL == address) {
		return FALSE;
	}
	LinkNode* next = list->head;
	LinkNode* node;
	assert(NULL != list->head);
	Partition* partition;
	do {
		node = next;
		partition = (Partition*)node->element;
		next = node->next;
	} while ((address != partition->address) && (NULL != next));
	//地址不合法。
	if (address != partition->address || 0 == partition->allocated) {
		return FALSE;
	}
	Partition* other;
	//释放空间
	partition->allocated = 0;
	//处理前相邻分区
	if (NULL != node->previous ) {
		other = (Partition*)(node->previous->element);
		//空闲相邻则合并
		if (0 == other->allocated) {
			assert(((Byte*)other->address + other->size) == partition->address);
			other->size += partition->size;
			
			node = node->previous;
			LinkList_remove(list, node->next);
			free(partition);
			partition = (Partition*)node->element;
		}
	}
	//处理后相邻分区。
	if (NULL != node->next) {
		other = (Partition*)node->next->element;
		if (0 == other->allocated) {//合并后相邻分区
			assert(((Byte*)partition->address + partition->size) == other->address);
			partition->size += other->size;
			LinkList_remove(list, node->next);
			free(other);
		}
	}
	//返回结果
	return TRUE;
}

Pointer DynamicPartition_mallocMin(LinkList* list, Int size)
{
	if (NULL == list || size < 512) {
		return NULL;
	}
	//寻找可用区。
	LinkNode* node = list->head;
	LinkNode* next = list->head;
	assert(NULL != list->head);
	Partition* partition;
	do {
		node = next;
		partition = (Partition*)node->element;
		next = node->next;
	} while (((partition->size) < size || (1 == partition->allocated)) && (NULL != next));
	//没有可用区。
	if (partition->size < size || (1 == partition->allocated)) {
		return NULL;
	}
	//有可用区
	assert(partition->size >= size && 0 == partition->allocated);
	//剩余空间过小，分配整个空间
	if (partition->size - size <= 512) {
		partition->allocated = 1;
		return partition->address;
	}
	//需要切割分区
	assert(partition->size - size > 512);
	Partition* allocated = malloc(sizeof(Partition));
	assert(NULL != allocated);

	allocated->address = partition->address;
	allocated->allocated = 1;
	allocated->size = size;
	partition->address = (Byte*)allocated->address + allocated->size;
	partition->size = partition->size - allocated->size;
	//割分区插入原分区前面。
	LinkNode* target = LinkList_addPrevious(list, node, allocated);
	assert(NULL != target);

	LinkNode* other_node;
	Partition* other;
	//排序分区
	while (NULL != node->previous) {
		other_node = node->previous;
		other = (Partition*)other_node->element;
		if (other->size > partition->size || 1 == other->allocated) {//比左边小则前移，跳过已分配分区。
			Boolean success = LinkList_swap(list, other_node, node);
			assert(TRUE == success);
		}
		else {
			break;//排序结束。
		}
	}

	return allocated->address;
}

Boolean DynamicPartition_freeMin(LinkList* list, Pointer address)
{
	if (NULL == list || NULL == address) {
		return FALSE;
	}
	LinkNode* next = list->head;
	LinkNode* node;
	assert(NULL != list->head);
	Partition* partition;
	do {
		node = next;
		partition = (Partition*)node->element;
		next = node->next;
	} while ((address != partition->address) && (NULL != next));
	//地址不合法。
	if (address != partition->address || 0 == partition->allocated) {
		return FALSE;
	}
	//释放空间
	partition->allocated = 0;

	//处理前相邻分区
	next = list->head;
	Partition* other;
	LinkNode* other_node;
	do {
		other_node = next;
		other = (Partition*)other_node->element;
		next = other_node->next;
	} while (((Byte*)other->address + other->size != partition->address) && (NULL != next));

	if ((Byte*)other->address + other->size == partition->address) {
		//空闲相邻则合并
		if (0 == other->allocated) {
			other->size += partition->size;
			LinkList_remove(list, node);
			free(partition);

			node = other_node;
			partition = (Partition*)node->element;
		}
	}
	
	//处理后相邻分区。
	next = list->head;
	do {
		other_node = next;
		other = (Partition*)other_node->element;
		next = other_node->next;
	} while (((Byte*)partition->address + partition->size != other->address) && (NULL != next));
	
	if (((Byte*)partition->address + partition->size) == other->address) {
		if (0 == other->allocated) {//合并后相邻分区
			partition->size += other->size;
			LinkList_remove(list, other_node);
			free(other);
		}
	}
	
	//排序分区，空闲区是排好序的，非空闲区可能夹在空闲中间。
	//所以每次排序，先将它右移到空闲区之外。
	while (NULL != node->next) {
		other_node = node->next;
		other = (Partition*)other_node->element;
		Boolean success = LinkList_swap(list, other_node, node);
		assert(TRUE == success);
	}
	while (NULL != node->previous) {
		other_node = node->previous;
		other = (Partition*)other_node->element;
		if (other->size > partition->size || 1 == other->allocated) {//比左边小则左移，跳过分配区。
			Boolean success = LinkList_swap(list, node, other_node);
			assert(TRUE == success);
		}
		else {
			break;
		}
	}
	//返回结果
	return TRUE;
}

Boolean DynamicPartition_print(LinkList* list)
{
	if (NULL == list) {
		return FALSE;
	}

	LinkNode* node = list->head;
	assert(NULL != node);
	Partition* partition;
	do {
		partition = (Partition*)node->element;
		node = node->next;

		if (partition->allocated == 0) {
			printf_s("-------------------------\n");
			printf_s("address%2s = %p\n", " ", partition->address);
			printf_s("size%5s = %d\n", " ", partition->size);
			printf_s("allocated = ");
			if (0 == partition->allocated) {
				printf_s("FALSE\n");
			}
			else {
				printf_s("TRUE\n");
			}
		}
	} while (NULL != node);
	return TRUE;
}

void DynamicPartition_test()
{
	LinkList list;
	Int size = 1024 * 1024;
	Pointer address;
	Int need = 1024;
	Int random;
	DynamicPartition_init(&list, size);
	srand((unsigned int)time(NULL));
	address = NULL;
	Boolean success;
	ArrayList allocated;
	ArrayList_init(&allocated);
	Int index;
	do {
		random = rand() % 100 * 100;
		printf_s("======================================================\n");
		if (random > 100 * 60) {

			need = 512 + rand() % size;
			address = NULL;
			address = DynamicPartition_mallocFirst(&list, need);
			ArrayList_add(&allocated, allocated.size, address);

			DynamicPartition_print(&list);
			printf_s("malloc %d = %p\n", need, address);
			printf_s("\n");
		}
		else {

			if (allocated.size == 0) {
				continue;
			}
			else {
				index = rand() % allocated.size;
			}

			address = ArrayList_remove(&allocated, index);

			success = DynamicPartition_freeFirst(&list, address);
			DynamicPartition_print(&list);
			printf_s("free = %p", address);
			if (TRUE == success) {
				printf_s(", TRUE\n");
			}
			else {
				printf_s(", FALSE\n");
			}
			printf_s("\n");
		}
	} while (1);
}


//动态分配算法演示
Boolean DynmicPartitio_show_print_element(Partition* partition) {
	printf_s("-------------------------\n");
	printf_s("address%2s = %p\n", " ", partition->address);
	printf_s("size%5s = %d\n", " ", partition->size);
	printf_s("allocated = ");
	if (0 == partition->allocated) {
		printf_s("FALSE\n");
	}
	else {
		printf_s("TRUE\n");
	}
	return TRUE;
}
Partition* DynamicPartition_show_getPartitionFromAddress(LinkList* list, Pointer address) {

	LinkNode* node = list->head;
	while (NULL != node) {
		Partition* partition = (Partition*)node->element;
		if (partition->address == address) {
			return partition;
		}
		node = node->next;
	}

	return NULL;
}
Partition* DynamicPartition_show_removePartitionFromSize(ArrayList* list, Int size) {
	for (Int i = 0; i < list->size; i++) {
		Partition* partition = (Partition*)ArrayList_get(list, i);
		if (partition->size == size) {
			return ArrayList_remove(list, i);
		}
	}
	return NULL;
}
void DynamicPartition_show_do(LinkList* list, ArrayList* jobs, Int i, Int size, Int type,
	Pointer(*Dmalloc)(LinkList* list, Int size), Boolean(*Dfree)(LinkList* list, Pointer address)) {

	printf_s("***************************************************************************\n");
	ArrayList* job = ArrayList_get(jobs, i-1);
	switch (type) {
	case 1://申请内存
	{
		Pointer address = Dmalloc(list, size);
		if (address != NULL) {
			Partition* partition = DynamicPartition_show_getPartitionFromAddress(list, address);
			assert(partition->size == size);
			ArrayList_add(job, job->size, partition);

			printf_s("==================================================\n");
			printf_s("作业 %d 申请 %d 字节，分配请况：\n", i, size);
			ArrayList_forEach(job, DynmicPartitio_show_print_element);
			printf_s("==================================================\n");
			printf_s("空闲分区链状态：\n");
			DynamicPartition_print(list);
		}
		else {
			printf_s("作业 %d 申请 %d 字节，分配失败：\n", i, size);
		}
		

	}break;
	case 2://释放内存
	{
		Partition* partition = DynamicPartition_show_removePartitionFromSize(job, size);
		if (NULL != partition) {
			Dfree(list, partition->address);

			printf_s("==================================================\n");
			printf_s("作业 %d 释放 %d 字节，分配请况：\n", i, size);
			ArrayList_forEach(job, DynmicPartitio_show_print_element);
			printf_s("==================================================\n");
			printf_s("空闲分区链状态：\n");
			DynamicPartition_print(list);
		}
		printf_s("作业 %d 释放 %d 字节，释放失败：\n", i, size);
	}break;
	default:
		break;
	}

	printf_s("\n");
}
void DynamicPartition_show()
{
	printf_s("程序简介：\n");
	printf_s("	3118005434， 钟景文，2018级别信息安全2班，计算机学院，广东工业大学\n");
	printf_s("	动态分区分配方式的模拟，请输入数字选择动态分区算法。\n");
	printf_s("输入选择：\n");
	printf_s("1 ：首次适应算法\n");
	printf_s("2 : 最佳适应算法\n");
	Int select = 0;
	do {
		printf_s("输入：");
		scanf_s("%d", &select);
		if (select != 1 && select != 2) {
			continue;
		}
	} while (1 == 0);
	
	Pointer(*Dmalloc)(LinkList * list, Int size);
	Boolean(*Dfree)(LinkList * list, Pointer address);
	if (1 == select) {
		Dmalloc = DynamicPartition_mallocFirst;
		Dfree = DynamicPartition_freeFirst;
	}
	else {
		Dmalloc = DynamicPartition_mallocMin;
		Dfree = DynamicPartition_freeMin;
	}
	//空闲分区表
	LinkList list;
	DynamicPartition_init(&list, 640 * 1024);
	//作业
	ArrayList jobs; ArrayList_init(&jobs);
	for (Int i = 0; i < 8; i++) {
		ArrayList* job = malloc(sizeof(ArrayList));
		ArrayList_init(job);
		ArrayList_add(&jobs, jobs.size, job);
	}
	Int i;
	Int size;
	Int type;
	//作业1申请130.
	i = 1;
	size = 130 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//作业2申请60.
	i = 2;
	size = 60 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//作业3申请100.
	i = 3;
	size = 100 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//作业2释放60.
	i = 2;
	size = 60 * 1024;
	type = 2;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);
	
	//作业4申请200.
	i = 4;
	size = 200 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//作业3释放100.
	i = 3;
	size = 100 * 1024;
	type = 2;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//作业5申请140.
	i = 5;
	size = 140 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//作业6申请60.
	i = 6;
	size = 60 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//作业7申请50.
	i = 7;
	size = 50 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//作业8申请60.
	i = 8;
	size = 60 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);
}