#include "IndexList.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void IndexList_assert(IndexList list)
{
	assert(list.size >= 0);
	assert(list.top >= list.size);
	assert(list.max > 0);
	assert(list.max >= list.top - list.size);
	assert(list.unit > 0);
	assert(list.length > 0);
	assert(list.length >= list.top);
	assert(NULL != list.indexs);
	assert(NULL != list.elements);
	if (0 == list.size) {
		assert(0 == list.top);
	}
}

Int IndexList_sizeofIndexs(Int max)
{
	if (max < 0) {
		max = 0;
	}
	return max * sizeof(Int);
}

Int IndexList_sizeofElements(Int unit, Int length)
{
	if (unit < 0) {
		unit = 0;
	}
	if (length < 0) {
		length = 0;
	}
	return unit * length;
}

Boolean IndexList_init(IndexList* list, Int max, Pointer indexs, Int unit, Int length, Pointer elements)
{
	if (NULL == list || max <= 0 || NULL == indexs
		|| unit <= 0 || length <= 0 || NULL == elements) {
		return FALSE;
	}

	list->size = 0;
	list->top = 0;
	list->max = max;
	list->unit = unit;
	list->length = length;
	list->indexs = indexs;
	list->elements = elements;

	return TRUE;
}

Boolean IndexList_clone(IndexList* origin, IndexList* target)
{
	if (NULL == origin || NULL == target || 0 != target->size 
		|| target->max < origin->top - origin->size
		|| target->unit != origin->unit
		|| target->length < origin->top) {
		return FALSE;
	}
	IndexList_assert(*origin);
	IndexList_assert(*target);
	
	//���Ʊ�
	Array_jump(origin->indexs, target->indexs, sizeof(Int), origin->top - origin->size);
	Array_jump(origin->elements, target->elements, origin->unit, origin->top);
	target->size = origin->size;
	target->top = origin->top;
	//���ؽ��
	return TRUE;
}

Boolean IndexList_trim(IndexList* list)
{
	if (NULL == list) {
		return FALSE;
	}
	IndexList_assert(*list);
	//�����
	if (list->size == list->top) {
		return TRUE;
	}
	Int begin;
	Int end;
	Pointer origin;
	Pointer target;
	//����Ԫ�ء�
	Int i = 0;
	for (; i < list->top - list->size - 1; i++) {
		begin = *((Int*)Array_get(list->indexs, sizeof(Int), i));
		end = *((Int*)Array_get(list->indexs, sizeof(Int), i + 1));

		origin = Array_get(list->elements, list->unit, begin + 1);
		target = Array_get(list->elements, list->unit, begin - i);
		Array_jump(origin, target, list->unit, end - begin - 1);
	}
	begin = *((Int*)Array_get(list->indexs, sizeof(Int), i));
	
	origin = Array_get(list->elements, list->unit, begin + 1);
	target = Array_get(list->elements, list->unit, begin - i);
	Array_jump(origin, target, list->unit, list->top - begin - 1);
	
	//������
	list->top = list->size;
	return TRUE;
}

Int IndexList_getOrderFromIndex(IndexList* list, Int index)
{
	if (NULL == list || index < 0 || index >= list->top) {
		return -1;
	}
	IndexList_assert(*list);

	Int free;
	Int i = 0;
	for (; i < list->top - list->size; i++) {
		free = *((Int*)Array_get(list->indexs, sizeof(Int), i));
		if (free == index) {//index�����ǿ��еġ�
			return -1;
		}
		if (free > index) {//��һ������index�Ŀ�����������indexǰ��i������Ԫ�ء�
			break;
		}
	}
	//���ؽ��
	return index - i;
}

Int IndexList_getIndexFromOrder(IndexList* list, Int order)
{
	if (NULL == list || order < 0 || order >= list->size) {
		return -1;
	}
	IndexList_assert(*list);
	if (order == list->size) {
		return list->top;
	}

	Int free;
	Int i = 0;
	for (; i < list->top - list->size; i++) {
		free = *((Int*)Array_get(list->indexs, sizeof(Int), i));
		if (free >= order + i + 1) {//�ҵ���һ��ǰ��û��i+1��������������
			//����һ������order+iǰ����i������������
			break;
		}
	}
	//���ؽ��
	return order + i;
}

Pointer IndexList_getAddressFromIndex(IndexList* list, Int index)
{
	if (NULL == list || index < 0 || index >= list->top) {
		return NULL;
	}
	IndexList_assert(*list);

	Byte* address = (Byte*)list->elements;
	address += list->unit * index;
	return (Pointer)address;
}

Int IndexList_getIndexFromAddress(IndexList* list, Pointer element)
{
	if (NULL == list || NULL == element) {
		return -1;
	}
	IndexList_assert(*list);

	Byte* address = list->elements;
	if (((Byte*)element) < address) {
		return -1;
	}

	Int index = ((Byte*)element) - address;
	if (0 != index % list->unit) {
		return -1;
	}

	index /= list->unit;
	if (index >= list->top) {
		return -1;
	}

	return index;
}

Int IndexList_getNearFree(IndexList* list, Int index)
{
	if (NULL == list || index < 0 || index >=list->length) {
		return -1;
	}
	Int free;
	Int i ;
	Int distance;
	Int max ;
	//û�п�������
	if (list->top == list->size) {
		return -1;
	}
	//indexǰ��û�п�����������ֱ�ӷ��ص�һ��
	free = *((Int*)Array_get(list->indexs, sizeof(Int), 0));
	if (free >= index) {
		return 0;
	}
	//indexǰ���п���������
	max = index - free;
	i = 1;
	for (; i < list->top - list->size; i++) {
		free = *((Int*)Array_get(list->indexs, sizeof(Int), i));
		
		distance = index - free;
		if (distance < 0) {
			distance = -distance;
		}
		if (distance < max) {//��������˾ͼ���ѭ����
			max = distance;
		}
		else {
			break;
		}
	}
	//���ؽ��
	return i - 1;
}

Pointer IndexList_get(IndexList* list, Int order)
{
	if (NULL == list || order < 0 || order >= list->size) {
		return NULL;
	}
	IndexList_assert(*list);

	Int index = IndexList_getIndexFromOrder(list, order);
	assert(-1 != index);

	return Array_get(list->elements, list->unit, index);
}

Boolean IndexList_set(IndexList* list, Int order, Pointer element)
{
	if (NULL == list || order < 0 || order >= list->size || NULL == element) {
		return FALSE;
	}
	IndexList_assert(*list);

	Int index = IndexList_getIndexFromOrder(list, order);
	assert(-1 != index);

	return Array_set(list->elements, list->unit, index, element);
}

Boolean IndexList_add(IndexList* list, Int order, Pointer element)
{
	if (NULL == list || order < 0 || order > list->size|| NULL == element) {
		return FALSE;
	}
	IndexList_assert(*list);

	//����ʧ��
	if (list->size == list->length) {
		return FALSE;
	}

	Pointer origin;
	Pointer target;
	
	if (list->size == list->top) {//��ͨ����, û�п���������
		//����һλ��
		origin = Array_get(list->elements, list->unit, order);
		target = Array_get(list->elements, list->unit, order + 1);
		Array_jump(origin, target, list->unit, list->size - order);
		//����Ԫ��
		Array_set(list->elements, list->unit, order, element);
		list->size++;
		list->top++;
	}
	else if (order == list->size) {//����ĩβ
		Int free_index = *((Int*)Array_get(list->indexs, sizeof(Int), list->top - list->size));
		//����
		origin = Array_get(list->elements, list->unit, free_index + 1);
		target = Array_get(list->elements, list->unit, free_index);
		Array_jump(origin, target, list->unit, list->top - free_index - 1);
		//����Ԫ��
		Array_set(list->elements, list->unit, list->top - 1, element);
		list->size++;
	}
	else{//�ƿղ���
		Int index = IndexList_getIndexFromOrder(list, order);
		assert(-1 != index);
		Int free_index = IndexList_getNearFree(list, index);

		Int free = *((Int*)Array_get(list->indexs, sizeof(Int), free_index));
		//ɾ����������
		origin = Array_get(list->indexs, sizeof(Int), free_index + 1);
		target = Array_get(list->indexs, sizeof(Int), free_index);
		Array_jump(origin, target, sizeof(Int), list->top - list->size - 1);
		//�ƶ�Ԫ��
		if (free > index) {//����
			origin = Array_get(list->elements, list->unit, index);
			target = Array_get(list->elements, list->unit, index + 1);
			Array_jump(origin, target, list->unit, free - index);
		}
		else if (free < index) {//����
			origin = Array_get(list->elements, list->unit, free + 1);
			target = Array_get(list->elements, list->unit, free);
			Array_jump(origin, target, list->unit, index - free);
			index--;//�����ƶ�ǰindexָԪ�ص�ǰ�������Ʋ����������Ƽ�һ��
		}
		else {
			assert(free == index);
		}
		//����Ԫ��
		Array_set(list->elements, list->unit, index, element);
		list->size++;
	}

	return TRUE;
}

Boolean IndexList_remove(IndexList* list, Int order)
{	
	if (NULL == list || order < 0 || order >= list->size) {
		return FALSE;
	}
	IndexList_assert(*list);

	//����ֻ��һ��Ԫ�ء�
	if (1 == list->size ) {
		list->size = 0;
		list->top = 0;
		return TRUE;
	}
	//ɾ�����һ��Ԫ�ء�
	if (order == list->size -1) {
		list->size--;
		list->top--;
		return TRUE;
	}
	//ɾ��ʧ�ܣ�0��Ԫ�ػ�û�п����������䴢�����������
	if (0 == list->size || list->max == list->top - list->size) {
		return FALSE;
	}

	Int index = IndexList_getIndexFromOrder(list, order);
	assert(-1 != index);

	Int free;
	Int i = 0;
	for (; i < list->top - list->size; i++) {
		free = *((Int*)Array_get(list->indexs, sizeof(Int), i));
		if (free > index) {//�ҵ���һ����index��Ŀ����������ʲ���˴���
			break;
		}
	}
	//���ƿ�������
	Pointer origin = Array_get(list->indexs, sizeof(Int), i);
	Pointer target = Array_get(list->indexs, sizeof(Int), i + 1);
	Array_jump(origin, target, sizeof(Int), list->top - list->size - i);
	//�����������
	Array_set(list->indexs, sizeof(Int), i, &index);
	//ɾ��Ԫ��
	list->size--;
	return TRUE;
}

Int IndexList_malloc(IndexList* list)
{
	if (NULL == list || list->size == list->length) {
		return -1;
	}
	IndexList_assert(*list);

	//����ռ�
	Int index;
	if (list->top > list->size) {
		index = *((Int*)Array_get(list->indexs, sizeof(Int), list->top - list->size - 1));
	}
	else {
		index = list->top;
		list->top++;
	}
	list->size++;
	//���ؽ��
	return index;
}

Boolean IndexList_free(IndexList* list, Int index)
{
	if (NULL == list || index < 0 || index >= list->top) {
		return FALSE;
	}
	//��鲢ת������
	Int order = IndexList_getOrderFromIndex(list, index);
	if (-1 == order) {
		return FALSE;
	}
	//�ͷſռ䡣
	return IndexList_remove(list, order);
}

Boolean IndexList_freeAll(IndexList* list)
{
	if (NULL == list) {
		return FALSE;
	}
	IndexList_assert(*list);

	list->size = 0;
	list->top = 0;
	return TRUE;
}

Boolean IndexList_swap(IndexList* list, Int order1, Int order2)
{
	if (NULL == list || order1 < 0 || order2 < 0 || order1 == order2
		|| list->size == list->length || order1 >= list->size || order2 >= list->size) {
		return FALSE;
	}
	IndexList_assert(*list);

	//��ȡ����Ԫ��
	Int index1 = IndexList_getIndexFromOrder(list, order1);
	Int index2 = IndexList_getIndexFromOrder(list, order2);
	assert(-1 != index2);
	assert(-1 != index2);

	Pointer element1 = Array_get(list->elements, list->unit, index1);
	Pointer element2 = Array_get(list->elements, list->unit, index2);
	if (NULL == element1 || NULL == element2) {
		return FALSE;
	}
	//��ȡ�����ռ�
	Int free;
	if (list->top == list->length) {
		free = *((Int*)Array_get(list->indexs, sizeof(Int), 0));
	}
	else {
		free = list->top;
	}
	Pointer temp = Array_get(list->elements, list->unit, free);
	//����
	Array_set(temp, list->unit, 0, element1);
	Array_set(element1, list->unit, 0, element2);
	Array_set(element2, list->unit, 0, temp);
	//���ؽ��
	return TRUE;
}

Boolean IndexList_forEach(IndexList* list, Boolean(*Element_runable)(Pointer element))
{
	if (NULL == list || NULL == Element_runable) {
		return FALSE;
	}
	IndexList_assert(*list);

	Int free;
	Int index = 0;
	Boolean success;
	Pointer element;
	for (Int i = 0; i < list->top - list->size; i++) {
		free = *((Int*)Array_get(list->indexs, sizeof(Int), i));
		while (index != free) {
			element = Array_get(list->elements, list->unit, index);
			success = Element_runable(element);
			if (FALSE == success) {
				return FALSE;
			}
			index++;
		}
		assert(index == free);
		printf_s(", --%d--", index);
		index++;
	}

	while (index < list->top) {
		element = Array_get(list->elements, list->unit, index);
		success = Element_runable(element);
		if (FALSE == success) {
			return FALSE;
		}
		index++;
	}

	return TRUE;
}

void IndexList_test(Boolean(*Element_printf)(Int* element)) {
	Int temp = 0;
	Int max = 10;
	Int unit = sizeof(Int);
	Int length = 10;
	Pointer indexs = malloc(max * sizeof(Int));
	Pointer elements = malloc(length * unit);
	assert(NULL != indexs);
	assert(NULL != elements);

	IndexList list;
	IndexList_init(&list, max, indexs, unit, length, elements);
	IndexList_forEach(&list, Element_printf); printf_s("\n\n");
	//add����
	for (Int i = 0; i < length; i++) {
		IndexList_add(&list, list.size / 2, &i);
		IndexList_forEach(&list, Element_printf); printf_s("\n");
	}
	//remove
	for (Int i = 0; i < length; i++) {
		IndexList_remove(&list, list.size / 2);
		IndexList_forEach(&list, Element_printf); printf_s("\n");
	}

	//add + remove
	srand((unsigned int)time(NULL));
	Int random;
	Int order;
	Int index;
	Boolean success;
	for (Long i = 0; i < 100; i++) {
		random = rand() % 10001;
		order = rand() % list.length;

		index = IndexList_getIndexFromOrder(&list, order);
		printf_s("index=%d, order=%d, ", index, order);

		if (random < 100 * 75) {
			success = IndexList_add(&list, order, &random);
			printf_s("add = ");
		}
		else {
			success = IndexList_remove(&list, order);
			printf_s("remove = ");
		}

		if (TRUE == success) {
			printf_s("TRUE :");
		}
		else {
			printf_s("FALSE :");
		}
		IndexList_forEach(&list, Element_printf); printf_s("\n");
	}
	//malloc + free��
	srand((unsigned int)time(NULL));
	random;
	order;
	index;
	success;
	for (Long i = 0; i < 100; i++) {
		random = rand() % 10001;
		
		if (random < 100 * 50) {
			index = IndexList_malloc(&list);
			success = Array_set(list.elements, list.unit, index, &index);
			printf_s("malloc = ");
		}
		else {
			index = rand() % (list.top+1);
			success = IndexList_free(&list, index);
			printf_s("free== = ");
		}

		if (TRUE == success) {
			printf_s("TRUE= ,");
		}
		else {
			printf_s("FALSE ,");
		}
		order = IndexList_getOrderFromIndex(&list, index);
		printf_s("index=%d, order=%d :", index, order);

		IndexList_forEach(&list, Element_printf); printf_s("\n");
		temp++;
	}
	//get,set
	printf_s("\n\n\n");
	Int element;
	for (Int i = 0; i < list.size; i++) {
		element = *((Int*)IndexList_get(&list, i));
		printf_s(", %d", element);

		element *= 10;
		IndexList_set(&list, i, &element);
	}
	printf_s("\n");
	IndexList_forEach(&list, Element_printf); printf_s("\n");

	//swap
	IndexList_swap(&list, 0, 1);
	IndexList_forEach(&list, Element_printf); printf_s("\n");
	IndexList_swap(&list, 0, list.size - 1);
	IndexList_forEach(&list, Element_printf); printf_s("\n");
	IndexList_swap(&list, 1, list.size - 1);
	IndexList_forEach(&list, Element_printf); printf_s("\n");
	IndexList_swap(&list, 2, list.size / 2);
	IndexList_forEach(&list, Element_printf); printf_s("\n");

	//clone
	printf_s("\n\n\n\n\n");
	max = list.top - list.size;
	length = list.top - 1;
	Pointer indexs2 = malloc(max * sizeof(Int));
	Pointer elements2 = malloc(length * unit);
	IndexList list2;
	success = IndexList_init(&list2, max, indexs2, unit, length, elements2);

	if (TRUE == success) {
		IndexList_clone(&list, &list2);
		IndexList_forEach(&list, Element_printf); printf_s("\n");
		IndexList_forEach(&list2, Element_printf); printf_s("\n");
		success = FALSE;
	}

	if (list.top - list.size >= 7) {
		printf_s("\n\n\n\n\n\n");
		IndexList_forEach(&list, Element_printf); printf_s("\n");
		IndexList_trim(&list);
		IndexList_forEach(&list, Element_printf); printf_s("\n");
		temp++;
	}
	

	free(indexs);
	free(elements);
	free(indexs2);
	free(elements2);
}
