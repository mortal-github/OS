#include "ArrayList.h"
#include <malloc.h>
#include "Heap.h"

Boolean ArrayList_initCapacity(ArrayList* list, Int capacity)
{
	if (NULL == list || capacity < 1) {
		return FALSE;
	}

	Element* elements = malloc(capacity * sizeof(Element));
	if (NULL == elements) {
		return FALSE;
	}

	list->elements = elements;
	list->capacity = capacity;
	list->size = 0;
	return TRUE;
}

Boolean ArrayList_ensureCapacity(ArrayList* list, Int capacity)
{
	if (NULL == list || capacity < 1) {
		return FALSE;
	}
	if (list->capacity < capacity) {
		Element* elements = realloc(list->elements, capacity * sizeof(Element));
		if (NULL == elements) {
			return FALSE;
		}
		list->elements = elements;
		list->capacity = capacity;
	}

	return TRUE;
}

Boolean ArrayList_trimToSize(ArrayList* list)
{
	if (NULL == list) {
		return FALSE;
	}
	if (list->capacity != list->size) {
		Element* elements = realloc(list->elements, list->size * sizeof(Element));
		if (NULL == elements) {
			return FALSE;
		}
		list->elements = elements;
		list->capacity = list->size;
	}

	return TRUE;
}

Boolean ArrayList_init(ArrayList* list)
{
	return ArrayList_initCapacity(list, DEFAULT_CAPACITY);
}

Boolean ArrayList_destroy(ArrayList* list)
{
	if (NULL == list) {
		return FALSE;
	}
	free(list->elements);
	list->elements = NULL;
	list->capacity = 0;
	list->size = 0;
	return TRUE;
}

Boolean ArrayList_add(ArrayList* list, Int index, Element element)
{
	if (NULL == list || NULL == element) {
		return FALSE;
	}
	//扩容
	if (list->size == list->capacity) {
		Int capacity = list->capacity + DEFAULT_INCREMENT;
		Element* elements = realloc(list->elements, capacity * sizeof(Element));
		if (NULL == elements) {
			return FALSE;
		}
		list->elements = elements;
		list->capacity = capacity;
	}
	//插入元素
	for (Int i = list->size; i > index; i--) {
		*(list->elements + i) = *(list->elements + i - 1);
	}
	*(list->elements + index) = element;
	list->size++;
	//返回结果
	return TRUE;
}

Element ArrayList_remove(ArrayList* list, Int index)
{
	if (NULL == list || index < 0 || index >= list->size) {
		return NULL;
	}

	//删除指定元素
	Element deleted = *(list->elements + index);
	for (Int i = index + 1; i < list->size; i++) {
		*(list->elements + i - 1) = *(list->elements + i);
	}
	list->size--;

	return deleted;
}

Element ArrayList_get(ArrayList* list, Int index)
{
	if (NULL == list || index < 0 || index >= list->size) {
		return NULL;
	}
	return *(list->elements + index);
}

Element ArrayList_set(ArrayList* list, Int index, Element element)
{
	if (NULL == list || index < 0 || index >= list->size || NULL == element) {
		return NULL;
	}

	Element deleted = *(list->elements + index);
	*(list->elements + index) = element;
	return deleted;
}

void ArrayList_swap(ArrayList* list, Int index1, Int index2) {
	Element element = ArrayList_get(list, index1);
	element = ArrayList_set(list, index2, element);
	ArrayList_set(list, index1, element);
}

Int ArrayList_indexOf(ArrayList* list, Element element, Boolean(*equals)(Element left, Element right))
{
	if (NULL == list || NULL == element || NULL == equals) {
		return -1;
	}
	for (Int i = 0; i < list->size; i++) {
		if (TRUE == equals(element, *(list->elements + i))) {
			return i;
		}
	}
	return -1;
}


Boolean ArrayList_forEach(ArrayList* list, Boolean(*Element_runable)(Element element))
{
	if (NULL == list || NULL == Element_runable) {
		return FALSE;
	}
	Boolean success = TRUE;
	Element element;
	for (Int i = 0; i < list->size; i++) {
		element = ArrayList_get(list, i);
		success = Element_runable(element);
		if (FALSE == success) {
			return success;
		}
	}

	return success;
}
