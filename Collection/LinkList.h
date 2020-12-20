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
 交换两个节点的位置。
 输入条件：NULL!=list, NULL!=node1, NULL!=node2, node1!=node2。
 正常处理：交换两个节点的位置，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE。
 隐式条件：list初始化过，且只通过函数写list；node1是list的节点，node2是list的节点。
*/
Boolean LinkList_swap(LinkList* list, LinkNode* node1, LinkNode* node2);
/*
 在节点前插入一个新元素。
 输入条件：NULL!=list, NULL!=node, NULL!=element。
 正常处理：在节点前插入一个新元素，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE;插入失败返回FALSE。
 隐式条件：list初始化过，且只通过函数写list；node是list的节点，element是合理合法的。
*/
Boolean LinkList_addPrevious(LinkList* list, LinkNode* node, Element element);
/*
 在节点后插入一个新元素。
 输入条件：NULL!=list, NULL!=node, NULL!=element。
 正常处理：在节点前插入一个新元素，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE;插入失败返回FALSE。
 隐式条件：list初始化过，且只通过函数写list；node是list的节点，element是合理合法的。
*/
Boolean LinkList_addAfter(LinkList* list, LinkNode* node, Element element);
/*
 插入一个新元素。
 输入条件：NULL!=list, NULL!=element。
 正常处理：插入一个新元素，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE;插入失败返回FALSE。
 隐式条件：list初始化过，且只通过函数写list；node是list的节点，element是合理合法的。
*/
Boolean LinkList_add(LinkList* list, Element element);
/*
 删除元素。
 输入条件：NULL!=list, NULL!=node, NULL!=element。
 正常处理：删除元素,返回元素。
 错误处理：若不满足输入条件，则什么都不做，返回NULL。
 隐式条件：list初始化过，且只通过函数写list；node是list的节点。
*/
Element LinkList_remove(LinkList* list, LinkNode* node);

/*
 获取索引的节点。
 输入条件：NULL!=list,index>=0, index<size。
 正常处理：返回索引所指示的节点。
 错误处理：若不满足输入条件，则什么都不做，返回NULL。
 隐式条件：list初始化过，且只通过函数写list。
*/
LinkNode* LinkList_node(LinkList* list, Int index);
/*
 获取节点的索引。
 输入条件：NULL!=list,NULL!=node。
 正常处理：返回节点的索引。
 错误处理：若不满足输入条件，则什么都不做，返回-1。
 隐式条件：list初始化过，且只通过函数写list；node是list的节点。
*/
Int LinkList_index(LinkList* list, LinkNode* node);

/*
 初始化链接链表。
 输入条件：NULL!=list。
 正常处理：初始化链表，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE。
 隐式条件：无。
*/
Boolean LinkList_init(LinkList* list);
/*
 销毁链接链表。
 输入条件：NULL!=list。
 正常处理：销毁链表：释放链接节点储存空间，初始化链接链表，返回TRUE。
 错误处理：若不满足输入条件，则什么都不做，返回FALSE。
 隐式条件：list初始化过，且只通过函数写list。
*/
Boolean LinkList_destroy(LinkList* list);

#endif // !_LINK_LIST__H_
