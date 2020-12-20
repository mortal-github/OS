#include "LinkList.h"
#include <malloc.h>

Boolean LinkList_swap(LinkList* list, LinkNode* node1, LinkNode* node2)
{
	if (NULL == list || NULL == node1 || NULL == node2 || node1 == node2) {
		return FALSE;
	}

	//�����ڵ�
	LinkNode* ll = node1->previous;
	LinkNode* lr = node1->next;
	LinkNode* rl = node2->previous;
	LinkNode* rr = node2->next;

	if (lr == node2) {//ll->node1->node2->rr
		//lr==rl
		if (NULL != ll) {
			ll->next = node2;
		}

		node2->previous = ll;
		node2->next = node1;

		node1->previous = node2;
		node1->next = rr;

		if (NULL != rr) {
			rr->previous = node1;
		}
	}
	else if (rr == node1) {//rl->node2->node1->lr
		//rr==ll
		if (NULL != rl) {
			rl->next = node1;
		}

		node1->previous = rl;
		node1->next = node2;

		node2->previous = node1;
		node2->next = lr;

		if (NULL != lr) {
			lr->previous = node2;
		}
	}
	else {// node2->...->node1 �� node1->...->node2
		if (NULL != ll) {
			ll->next = node2;
		}
		node2->previous = ll;
		node2->next = lr;
		if (NULL != lr) {
			lr->previous = node2;
		}

		if (NULL != rl) {
			rl->next = node1;
		}
		node1->previous = rl;
		node1->next = rr;
		if (NULL != rr) {
			rr->previous = node1;
		}
	}

	//��������
	if (list->head == node1) {
		list->head = node2;
	}
	else if (list->head == node2) {
		list->head = node1;
	}

	if (list->tail == node1) {
		list->tail = node2;
	}
	else if (list->tail == node2) {
		list->tail = node1;
	}

	return TRUE;
}

Boolean LinkList_addPrevious(LinkList* list, LinkNode* node, Element element)
{
	if (NULL == list || NULL == node || NULL == element) {
		return FALSE;
	}
	//�����ڵ�
	LinkNode* new_node = (LinkNode*)malloc(sizeof(LinkNode));
	if (NULL == new_node) {
		return FALSE;
	}
	new_node->element = element;
	new_node->previous = node->previous;
	new_node->next = node;
	//����ڵ�ǰ
	if (NULL != node->previous) {
		node->previous->next = new_node;
	}
	node->previous = new_node;
	//������������
	list->size++;
	if (list->head == node) {
		list->head = new_node;
	}
	return TRUE;
}

Boolean LinkList_addAfter(LinkList* list, LinkNode* node, Element element)
{
	if (NULL == list || NULL == node || NULL == element) {
		return FALSE;
	}
	//�����ڵ�
	LinkNode* new_node = (LinkNode*)malloc(sizeof(LinkNode));
	if (NULL == new_node) {
		return FALSE;
	}
	new_node->element = element;
	new_node->previous = node;
	new_node->next = node->next;
	//����ڵ��
	if (NULL != node->next) {
		node->next->previous = new_node;
	}
	node->next = new_node;
	//������������
	list->size++;
	if (list->tail == node) {
		list->tail = new_node;
	}

	return TRUE;
}

Boolean LinkList_add(LinkList* list, Element element)
{
	if (NULL == list || NULL == element) {
		return FALSE;
	}
	if (0 == list->size) {
		//�����ڵ�
		LinkNode* new_node = (LinkNode*)malloc(sizeof(LinkNode));
		if (NULL == new_node) {
			return FALSE;
		}
		new_node->element = element;
		new_node->previous = NULL;
		new_node->next = NULL;
		//����ڵ�
		list->head = new_node;
		list->tail = new_node;
		list->size++;

		return TRUE;
	}

	return LinkList_addAfter(list, list->tail, element);
}

Element LinkList_remove(LinkList* list, LinkNode* node)
{
	if (NULL == list || NULL == node) {
		return NULL;
	}
	if (0 == list->size) {
		return NULL;//node����list�Ľڵ㡣
	}
	Element deleted = node->element;
	if (1 == list->size) {
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		free(node);
	}
	else {
		if (list->head == node) {
			list->head = node->next;
			list->head->previous = NULL;
			list->size--;
			free(node);
		}
		else if (list->tail == node) {
			list->tail = node->previous;
			list->tail->next = NULL;
			list->size--;
			free(node);
		}
		else {
			node->previous->next = node->next;
			node->next->previous = node->previous;
			list->size--;
			free(node);
		}
	}

	return deleted;
}

LinkNode* LinkList_node(LinkList* list, Int index)
{
	if (NULL == list || index < 0 || index >= list->size) {
		return NULL;
	}

	LinkNode* node;
	if (index <= list->size / 2) {//��ǰ�����ҡ�
		node = list->head;
		while (index > 0) {//assert NULL!=node
			node = node->next;
			index--;
		}
	}
	else {//�Ӻ���ǰ�ҡ�
		node = list->tail;
		while (index < list->size - 1) {//assert NULL!=node;
			node = node->previous;
			index++;
		}
	}

	return node;
}

Int LinkList_index(LinkList* list, LinkNode* node)
{
	if (NULL == list || NULL == node) {
		return -1;
	}
	if (0 == list->size) {
		return -1;//˵����node����list�Ľڵ㡣
	}
	Int head = 0;
	Int tail = list->size - 1;
	LinkNode* head_result = list->head;
	LinkNode* tail_result = list->tail;
	//����ͷͬʱ��
	while (node != head_result && node != tail_result) {
		//assert NULL!=head_result, NULL!=tail_result��
		if (NULL == head_result || NULL == tail_result) {
			return -1;//˵����node����list�Ľڵ㡣
		}
		head_result = head_result->next;
		head++;
		tail_result = tail_result->previous;
		tail--;
	}
	//���ؽ����
	if (node == head_result) {
		return head;
	}
	else {
		return tail;
	}
}

Boolean LinkList_init(LinkList* list)
{
	if (NULL == list) {
		return FALSE;
	}

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;

	return TRUE;
}

Boolean LinkList_destroy(LinkList* list)
{
	if (NULL == list) {
		return FALSE;
	}

	//�ͷ����ӽڵ㴢��ռ䡣
	LinkNode* node = list->head;
	LinkNode* next;
	while (NULL != node) {
		next = node->next;
		free(node);
		node = next;
	}

	//��ʼ��
	return LinkList_init(list);
}
