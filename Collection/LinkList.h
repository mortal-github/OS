#ifndef _LINK_LIST__H_
#define _LINK_LIST__H_

#include "Collection.h"

typedef struct LinkNode {
	Element element;
	struct LinkNode* previous;
	struct LinkNode* next;
}LinkNode;

typedef struct LinkList {
	LinkNode* head;
	LinkNode* tail;
	Int size;
}LinkList;

/*
 ���������ڵ��λ�á�
 ����������NULL!=list, NULL!=node1, NULL!=node2, node1!=node2��
 �����������������ڵ��λ�ã�����TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ������list��ʼ��������ֻͨ������дlist��node1��list�Ľڵ㣬node2��list�Ľڵ㡣
*/
Boolean LinkList_swap(LinkList* list, LinkNode* node1, LinkNode* node2);
/*
 �ڽڵ�ǰ����һ����Ԫ�ء�
 ����������NULL!=list, NULL!=node, NULL!=element��
 ���������ڽڵ�ǰ����һ����Ԫ�أ�����TRUE��
 ��������������������������ʲô������������FALSE;����ʧ�ܷ���FALSE��
 ��ʽ������list��ʼ��������ֻͨ������дlist��node��list�Ľڵ㣬element�Ǻ���Ϸ��ġ�
*/
Boolean LinkList_addPrevious(LinkList* list, LinkNode* node, Element element);
/*
 �ڽڵ�����һ����Ԫ�ء�
 ����������NULL!=list, NULL!=node, NULL!=element��
 ���������ڽڵ�ǰ����һ����Ԫ�أ�����TRUE��
 ��������������������������ʲô������������FALSE;����ʧ�ܷ���FALSE��
 ��ʽ������list��ʼ��������ֻͨ������дlist��node��list�Ľڵ㣬element�Ǻ���Ϸ��ġ�
*/
Boolean LinkList_addAfter(LinkList* list, LinkNode* node, Element element);
/*
 ����һ����Ԫ�ء�
 ����������NULL!=list, NULL!=element��
 ������������һ����Ԫ�أ�����TRUE��
 ��������������������������ʲô������������FALSE;����ʧ�ܷ���FALSE��
 ��ʽ������list��ʼ��������ֻͨ������дlist��node��list�Ľڵ㣬element�Ǻ���Ϸ��ġ�
*/
Boolean LinkList_add(LinkList* list, Element element);
/*
 ɾ��Ԫ�ء�
 ����������NULL!=list, NULL!=node, NULL!=element��
 ��������ɾ��Ԫ��,����Ԫ�ء�
 ��������������������������ʲô������������NULL��
 ��ʽ������list��ʼ��������ֻͨ������дlist��node��list�Ľڵ㡣
*/
Element LinkList_remove(LinkList* list, LinkNode* node);

/*
 ��ȡ�����Ľڵ㡣
 ����������NULL!=list,index>=0, index<size��
 ������������������ָʾ�Ľڵ㡣
 ��������������������������ʲô������������NULL��
 ��ʽ������list��ʼ��������ֻͨ������дlist��
*/
LinkNode* LinkList_node(LinkList* list, Int index);
/*
 ��ȡ�ڵ��������
 ����������NULL!=list,NULL!=node��
 �����������ؽڵ��������
 ��������������������������ʲô������������-1��
 ��ʽ������list��ʼ��������ֻͨ������дlist��node��list�Ľڵ㡣
*/
Int LinkList_index(LinkList* list, LinkNode* node);

/*
 ��ʼ����������
 ����������NULL!=list��
 ����������ʼ����������TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ�������ޡ�
*/
Boolean LinkList_init(LinkList* list);
/*
 ������������
 ����������NULL!=list��
 �����������������ͷ����ӽڵ㴢��ռ䣬��ʼ��������������TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ������list��ʼ��������ֻͨ������дlist��
*/
Boolean LinkList_destroy(LinkList* list);

#endif // !_LINK_LIST__H_
