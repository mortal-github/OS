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
	//Ѱ�ҿ�������
	LinkNode* node = list->head;
	LinkNode* next = list->head;
	assert(NULL != list->head);
	Partition* partition;
	do {
		node = next;
		partition = (Partition*)node->element;
		next = node->next;
	} while (((partition->size) < size || (1 == partition->allocated)) && (NULL != next));
	//û�п�������
	if (partition->size < size || (1 == partition->allocated)) {
		return NULL;
	}
	//�п�����
	assert(partition->size >= size && 0 == partition->allocated);
	//ʣ��ռ��С�����������ռ�
	if (partition->size - size <= 512) {
		partition->allocated = 1;
		return partition->address;
	}
	//��Ҫ�и����
	assert(partition->size - size > 512);
	Partition* allocated = malloc(sizeof(Partition));
	assert(NULL != allocated);

	allocated->address = partition->address;
	allocated->allocated = 1;
	allocated->size = size;
	partition->address = (Byte*)allocated->address + allocated->size;
	partition->size = partition->size - allocated->size;
	//ǰ���������ԭ����ǰ�档
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
	//��ַ���Ϸ���
	if (address != partition->address || 0 == partition->allocated) {
		return FALSE;
	}
	Partition* other;
	//�ͷſռ�
	partition->allocated = 0;
	//����ǰ���ڷ���
	if (NULL != node->previous ) {
		other = (Partition*)(node->previous->element);
		//����������ϲ�
		if (0 == other->allocated) {
			assert(((Byte*)other->address + other->size) == partition->address);
			other->size += partition->size;
			
			node = node->previous;
			LinkList_remove(list, node->next);
			free(partition);
			partition = (Partition*)node->element;
		}
	}
	//��������ڷ�����
	if (NULL != node->next) {
		other = (Partition*)node->next->element;
		if (0 == other->allocated) {//�ϲ������ڷ���
			assert(((Byte*)partition->address + partition->size) == other->address);
			partition->size += other->size;
			LinkList_remove(list, node->next);
			free(other);
		}
	}
	//���ؽ��
	return TRUE;
}

Pointer DynamicPartition_mallocMin(LinkList* list, Int size)
{
	if (NULL == list || size < 512) {
		return NULL;
	}
	//Ѱ�ҿ�������
	LinkNode* node = list->head;
	LinkNode* next = list->head;
	assert(NULL != list->head);
	Partition* partition;
	do {
		node = next;
		partition = (Partition*)node->element;
		next = node->next;
	} while (((partition->size) < size || (1 == partition->allocated)) && (NULL != next));
	//û�п�������
	if (partition->size < size || (1 == partition->allocated)) {
		return NULL;
	}
	//�п�����
	assert(partition->size >= size && 0 == partition->allocated);
	//ʣ��ռ��С�����������ռ�
	if (partition->size - size <= 512) {
		partition->allocated = 1;
		return partition->address;
	}
	//��Ҫ�и����
	assert(partition->size - size > 512);
	Partition* allocated = malloc(sizeof(Partition));
	assert(NULL != allocated);

	allocated->address = partition->address;
	allocated->allocated = 1;
	allocated->size = size;
	partition->address = (Byte*)allocated->address + allocated->size;
	partition->size = partition->size - allocated->size;
	//���������ԭ����ǰ�档
	LinkNode* target = LinkList_addPrevious(list, node, allocated);
	assert(NULL != target);

	LinkNode* other_node;
	Partition* other;
	//�������
	while (NULL != node->previous) {
		other_node = node->previous;
		other = (Partition*)other_node->element;
		if (other->size > partition->size || 1 == other->allocated) {//�����С��ǰ�ƣ������ѷ��������
			Boolean success = LinkList_swap(list, other_node, node);
			assert(TRUE == success);
		}
		else {
			break;//���������
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
	//��ַ���Ϸ���
	if (address != partition->address || 0 == partition->allocated) {
		return FALSE;
	}
	//�ͷſռ�
	partition->allocated = 0;

	//����ǰ���ڷ���
	next = list->head;
	Partition* other;
	LinkNode* other_node;
	do {
		other_node = next;
		other = (Partition*)other_node->element;
		next = other_node->next;
	} while (((Byte*)other->address + other->size != partition->address) && (NULL != next));

	if ((Byte*)other->address + other->size == partition->address) {
		//����������ϲ�
		if (0 == other->allocated) {
			other->size += partition->size;
			LinkList_remove(list, node);
			free(partition);

			node = other_node;
			partition = (Partition*)node->element;
		}
	}
	
	//��������ڷ�����
	next = list->head;
	do {
		other_node = next;
		other = (Partition*)other_node->element;
		next = other_node->next;
	} while (((Byte*)partition->address + partition->size != other->address) && (NULL != next));
	
	if (((Byte*)partition->address + partition->size) == other->address) {
		if (0 == other->allocated) {//�ϲ������ڷ���
			partition->size += other->size;
			LinkList_remove(list, other_node);
			free(other);
		}
	}
	
	//������������������ź���ģ��ǿ��������ܼ��ڿ����м䡣
	//����ÿ�������Ƚ������Ƶ�������֮�⡣
	while (NULL != node->next) {
		other_node = node->next;
		other = (Partition*)other_node->element;
		Boolean success = LinkList_swap(list, other_node, node);
		assert(TRUE == success);
	}
	while (NULL != node->previous) {
		other_node = node->previous;
		other = (Partition*)other_node->element;
		if (other->size > partition->size || 1 == other->allocated) {//�����С�����ƣ�������������
			Boolean success = LinkList_swap(list, node, other_node);
			assert(TRUE == success);
		}
		else {
			break;
		}
	}
	//���ؽ��
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


//��̬�����㷨��ʾ
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
	case 1://�����ڴ�
	{
		Pointer address = Dmalloc(list, size);
		if (address != NULL) {
			Partition* partition = DynamicPartition_show_getPartitionFromAddress(list, address);
			assert(partition->size == size);
			ArrayList_add(job, job->size, partition);

			printf_s("==================================================\n");
			printf_s("��ҵ %d ���� %d �ֽڣ����������\n", i, size);
			ArrayList_forEach(job, DynmicPartitio_show_print_element);
			printf_s("==================================================\n");
			printf_s("���з�����״̬��\n");
			DynamicPartition_print(list);
		}
		else {
			printf_s("��ҵ %d ���� %d �ֽڣ�����ʧ�ܣ�\n", i, size);
		}
		

	}break;
	case 2://�ͷ��ڴ�
	{
		Partition* partition = DynamicPartition_show_removePartitionFromSize(job, size);
		if (NULL != partition) {
			Dfree(list, partition->address);

			printf_s("==================================================\n");
			printf_s("��ҵ %d �ͷ� %d �ֽڣ����������\n", i, size);
			ArrayList_forEach(job, DynmicPartitio_show_print_element);
			printf_s("==================================================\n");
			printf_s("���з�����״̬��\n");
			DynamicPartition_print(list);
		}
		printf_s("��ҵ %d �ͷ� %d �ֽڣ��ͷ�ʧ�ܣ�\n", i, size);
	}break;
	default:
		break;
	}

	printf_s("\n");
}
void DynamicPartition_show()
{
	printf_s("�����飺\n");
	printf_s("	3118005434�� �Ӿ��ģ�2018������Ϣ��ȫ2�࣬�����ѧԺ���㶫��ҵ��ѧ\n");
	printf_s("	��̬�������䷽ʽ��ģ�⣬����������ѡ��̬�����㷨��\n");
	printf_s("����ѡ��\n");
	printf_s("1 ���״���Ӧ�㷨\n");
	printf_s("2 : �����Ӧ�㷨\n");
	Int select = 0;
	do {
		printf_s("���룺");
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
	//���з�����
	LinkList list;
	DynamicPartition_init(&list, 640 * 1024);
	//��ҵ
	ArrayList jobs; ArrayList_init(&jobs);
	for (Int i = 0; i < 8; i++) {
		ArrayList* job = malloc(sizeof(ArrayList));
		ArrayList_init(job);
		ArrayList_add(&jobs, jobs.size, job);
	}
	Int i;
	Int size;
	Int type;
	//��ҵ1����130.
	i = 1;
	size = 130 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//��ҵ2����60.
	i = 2;
	size = 60 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//��ҵ3����100.
	i = 3;
	size = 100 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//��ҵ2�ͷ�60.
	i = 2;
	size = 60 * 1024;
	type = 2;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);
	
	//��ҵ4����200.
	i = 4;
	size = 200 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//��ҵ3�ͷ�100.
	i = 3;
	size = 100 * 1024;
	type = 2;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//��ҵ5����140.
	i = 5;
	size = 140 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//��ҵ6����60.
	i = 6;
	size = 60 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//��ҵ7����50.
	i = 7;
	size = 50 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);

	//��ҵ8����60.
	i = 8;
	size = 60 * 1024;
	type = 1;
	DynamicPartition_show_do(&list, &jobs, i, size, type, Dmalloc, Dfree);
}