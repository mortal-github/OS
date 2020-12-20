#include "Heap.h"

Boolean Heap_init(Heap* heap, ArrayList* list, Int tag, Int(*prior)(Element left, Element right))
{
    if (NULL == heap || NULL == list || (-1 != tag && 1 != tag) || NULL == prior) {
        return FALSE;
    }

    heap->list = list;
    heap->tag = tag;
    heap->prior = prior;
    return TRUE;
}

Boolean Heap_destroy(Heap* heap)
{
    if (NULL == heap) {
        return FALSE;
    }
    heap->list = NULL;
    heap->tag = 0;
    heap->prior = NULL;
    return TRUE;
}

Boolean Heap_setTag(Heap* heap, Int tag)
{
    if (NULL == heap || (-1 != tag && 1 != tag)) {
        return FALSE;
    }
    heap->tag = tag;
    return TRUE;
}

Boolean Heap_setPrior(Heap* heap, Int(*prior)(Element left, Element right))
{
    if (NULL == heap || NULL == prior) {
        return FALSE;
    }
    heap->prior = prior;
    return TRUE;
}

Boolean Heap_shiftDown(Heap* heap, Int index)
{
    if (NULL == heap || index < 0 || index >= heap->list->size) {
        return FALSE;
    }

    Int parent = index + 1;
    Int lchild;
    Int rchild;
    Int pchild;

    Element left;
    Element right;

    while (parent <= heap->list->size / 2) {//Ҷ�ӽڵ�ѭ������
        lchild = parent * 2;
        rchild = lchild + 1;

        //�ȽϺ���
        if (rchild > heap->list->size) {//û���Һ���
            pchild = lchild;
        }
        else {
            left = ArrayList_get(heap->list, lchild - 1);
            right = ArrayList_get(heap->list, rchild - 1);
            if (heap->tag == heap->prior(left, right)) {
                pchild = lchild;
            }
            else {
                pchild = rchild;
            }
        }

        //�Ƚϸ���
        left = *(heap->list->elements + parent - 1);
        right = *(heap->list->elements + pchild - 1);
        if (heap->tag == heap->prior(left, right)) {//�������ȣ�����ɸѡ
            break;
        }

        //��������
        ArrayList_swap(heap->list, pchild - 1, parent - 1);
        parent = pchild;
    }

    //ɸѡ�ɹ�
    return TRUE;
}

Boolean Heap_adjust(Heap* heap)
{
    if (NULL == heap) {
        return FALSE;
    }

    for (Int parent = heap->list->size / 2; parent > 0; parent--) {
        Heap_shiftDown(heap, parent - 1);
    }
    return TRUE;
}

Boolean Heap_sort(Heap* heap)
{
    if (NULL == heap) {
        return FALSE;
    }

    //������
    Int length = heap->list->size;
    while (heap->list->size > 0) {
        Heap_remove(heap, 0);
    }
    heap->list->size = length;

    return TRUE;
}

Boolean Heap_add(Heap* heap, Element element)
{
    if (NULL == heap || NULL == element) {
        return FALSE;
    }
    //����Ԫ��
    if (FALSE == ArrayList_add(heap->list, heap->list->size, element)) {
        return FALSE;
    }
    //������
    Int child = heap->list->size;
    Int parent = child / 2;
    Element left;
    Element right;

    while (child > 1) {
        left = ArrayList_get(heap->list, parent - 1);
        right = ArrayList_get(heap->list, child - 1);

        //�Ƚϸ���
        if (heap->tag == heap->prior(left, right)) {//�������ȣ���������
            break;
        }
        //��������
        ArrayList_swap(heap->list, parent - 1, child - 1);
        child = parent;
        parent = child / 2;
    }

    return TRUE;
}

Element Heap_remove(Heap* heap, Int index)
{
    if (NULL == heap || index < 0 || index >= heap->list->size) {
        return NULL;
    }

    //��β�ڵ㽻��
    ArrayList_swap(heap->list, index, heap->list->size - 1);
    //ɾ���ڵ�
    Element deleted = ArrayList_remove(heap->list, heap->list->size - 1);
    if (NULL == deleted) {
        return NULL;
    }
    //������
    if (heap->list->size > 1) {
        Heap_shiftDown(heap, index);
    }

    return deleted;
}

Element Heap_set(Heap* heap, Int index, Element element)
{
    if (NULL == heap) {
        return NULL;
    }

    //����Ԫ��
    Element deleted = ArrayList_set(heap->list, index, element);
    if (NULL == deleted) {
        return NULL;
    }
    //ɸѡ��
    Heap_shiftDown(heap, index);

    return deleted;
}

