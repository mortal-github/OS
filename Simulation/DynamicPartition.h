#ifndef _DYNAMIC_PARTITION__H_
#define _DYNAMIC_PARITITION__H_

#include "LinkList.h"

typedef struct Partition {
	Pointer address;
	Int allocated;
	Int size;
}Partition;
/*
 list�ǿ��з�������size�ǳ�ʼ��������С��
 ����������NULL!=list, size>0��
 ����������ʼ��������
 ��������������������������ʲô������������FALSE��
*/
Boolean DynamicPartition_init(LinkList* list, Int size);

/*
 ���״���Ӧ�㷨����size�ֽڵĿռ䡣
 ����������NULL!=list, size>=512��
 ������������size�ֽڵĿռ䡣
 ��������������������������ʲô������������NULL��
*/
Pointer DynamicPartition_mallocFirst(LinkList* list, Int size);
/*
 ���״���Ӧ�㷨�ͷ�address�ռ䡣
 ����������NULL!=list,NULL!=address��
 �����������״���Ӧ�㷨�ͷ�address�ռ䣬����TRUE��
 ��������������������������ʲô������������ַ���Ϸ�����ʲô������������FALSE.
*/
Boolean DynamicPartition_freeFirst(LinkList* list, Pointer address);


/*
 �������Ӧ�㷨����size�ֽڵĿռ䡣
 ����������NULL!=list, size>=512��
 ������������size�ֽڵĿռ䡣
 ��������������������������ʲô������������NULL��
*/
Pointer DynamicPartition_mallocMin(LinkList* list, Int size);
/*
 �������Ӧ�㷨�ͷ�address�ռ䡣
 ����������NULL!=list,NULL!=address��
 �����������״���Ӧ�㷨�ͷ�address�ռ䣬����TRUE��
 ��������������������������ʲô������������ַ���Ϸ�����ʲô������������FALSE.
*/
Boolean DynamicPartition_freeMin(LinkList* list, Pointer address);

Boolean DynamicPartition_print(LinkList* list);

void DynamicPartition_test();

void DynamicPartition_show();
#endif // !_DYNAMIC_PARTITION__H_
