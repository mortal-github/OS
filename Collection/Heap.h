#ifndef _HEAP__H_
#define _HEAP__H_

#include "Collection.h"
#include "ArrayList.h"

typedef struct Heap {
	ArrayList* list;
	/*-1��С���ѣ�0���󶥶ѡ�1: �󶥶�*/
	Int tag;
	/*����1��ʾleft����right,-1��ʾright����left,0��ʾright�ȼ�left��*/
	Int(*prior)(Element left, Element right);
}Heap;

/*
 ��ʼ���ѡ�
 ����������NULL!=list,NULL!=list,tag=-1��tag=1,NULL!=prior��
 ����������ʼ����,����TRUE��
 �������������������������ʼ��ʧ�ܣ���ʲô������������FALSE��
 ��ʽ�������ѳ�ʼ����,priorȷʵ�Ƚ����ȼ�����Ҫ�󷵻�-1��0��1��
*/
Boolean Heap_init(Heap* heap, ArrayList* list, Int tag, Int(*prior)(Element left, Element right));
/*
 ���ٶѡ�
 ����������NULL!=list��
 ����������ն�Ԫ��Ϣ������TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д�ѡ�
*/
Boolean Heap_destroy(Heap* heap);
/*
 ���ö����͡�
 ����������NULL!=list��-1=tag��1=tag��
 �����������ö����ͣ�-1��ʾС���ѣ�1��ʾ�󶥶ѣ�����TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д�ѡ�
*/
Boolean Heap_setTag(Heap* heap, Int tag);
/*
 ���ö����Ⱥ�����
 ����������NULL!=list��NULL!=prior��
 �����������ö����Ⱥ���������TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д�ѣ�priorȷʵ�Ƚ����ȼ�����Ҫ�󷵻�-1��0��1����
*/
Boolean Heap_setPrior(Heap* heap, Int(*prior)(Element left, Element right));

/*
 ɸѡ�Ӷѣ�����index������Ԫ����������Ϊ�ѣ����Ը�Ԫ��Ϊ������ɸѡ�ɶѡ�
 ����������NULL!=heap,index>=0,index<size��
 �������������Ԫ�ص�����������Ϊ�ѣ����Ըø�Ԫ��Ϊ����������Ϊ�ѣ�����TRUE��
 �������������������������򷵻�FALSE��
 ��ʽ������heap������ʼ������ֻͨ������дheap��
*/
Boolean Heap_shiftDown(Heap* heap, Int index);
/*����Ϊ�ѡ�
����������NULL!=heap��
����������heap�ڵ�Ԫ�����гɶѡ�
��������������������������ʲô������������FALSE��
��ʽ������Heap�ǳ�ʼ�����ģ���ֻͨ����������Heap��*/
Boolean Heap_adjust(Heap* heap);
/*
 �����Ԫ��
 ����������NULL!=heap��
 ���������Ѷ���Ԫ����������򣬴󶥶�����С���ѽ���
 ��������������������������ʲô����������FALSE����
 ��ʽ������Heap�ǳ�ʼ�����ģ���ֻͨ����������Heap����heap��Ԫ�ع��ɶѡ�
*/
Boolean Heap_sort(Heap* heap);

/*
 �Ѳ���һ��Ԫ�ء�
 ����������NULL!=list,NULL!=element��
 ����������ָ����һ��Ԫ�ء�
 ��������������������������ʲô������������FALSE;������Ԫ��ʧ�ܣ���ʲô������������FALSE��
 ��ʽ���������ǳ������ģ���ֻͨ������д�ѡ�
*/
Boolean Heap_add(Heap* heap, Element element);
/*
 ɾ����ָ��λ�õ�һ��Ԫ�ء�
 ����������NULL!=heap��index>=0, index<size��
 ��������ɾ����ָ��λ�õ�һ��Ԫ�أ����ظ�Ԫ�ء�
 ��������������������������ʲô�����������ؿ����ã���ɾ��ʧ�ܣ���ʲô��������
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д�ѡ�
*/
Element Heap_remove(Heap* heap, Int index);
/*
 ���Ķ�ָ��λ�õ�Ԫ��Ϊ��ֵ��
 ����������NULL!=heap��index>=0, index<size, NULL!=element��
 �����������Ķ�ָ��λ�õ�Ԫ��Ϊ��ֵ,���ؾ�ֵ��
 ��������������������������ʲô�����������ؿ����á�
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д�ѡ�
*/
Element Heap_set(Heap* heap, Int index, Element element);

#endif // !_HEAP__H_
