#ifndef _ARRAY_LIST__H_
#define _ARRAY_LIST__H_

#include "Primitive.h"

typedef struct ArrayList {
	Element* elements;
	Int capacity;
	Int size;
}ArrayList;

#define DEFAULT_CAPACITY 25
#define DEFAULT_INCREMENT 10
/*
 ��ʼ����������
 ����������NULL!=list, capacity>0��
 ����������ʼ������ռ��СΪcapacity��������,����TRUE��
 �������������������������ʼ��ʧ�ܣ���ʲô������������FALSE��
 ��ʽ�������ޡ�
*/
Boolean ArrayList_initCapacity(ArrayList* list, Int capacity);
/*
 ʹ����ռ��㹻��С��
 ����������NULL!=list��capacity>0��
 ��������ȷ�����������ڲ����·��䴢��ռ������¾��ܹ��������������Ԫ�أ�����TRUE��
 ��������������������������ʲô������������FALSE;������ʧ�ܣ���ʲô������������FALSE��
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������
*/
Boolean ArrayList_ensureCapacity(ArrayList* list, Int capacity);
/*
 ����������Ĵ���������������ǰ�ߴ硣
 ����������NULL!=list��
 ������������������Ĵ���������������ǰ�ߴ磬����TRUE��
 ��������������������������ʲô������������FALSE;������ʧ�ܣ���ʲô������������FALSE��
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������
*/
Boolean ArrayList_trimToSize(ArrayList* list);

/*
 ��ʼ����������
 ����������NULL!=list��
 ����������ʼ����������,����TRUE��
 �������������������������ʼ��ʧ�ܣ���ʲô������������FALSE��
 ��ʽ�������ޡ�
*/
Boolean ArrayList_init(ArrayList* list);
/*
 ������������
 ����������NULL!=list��
 ���������ͷ���������Ĵ���ռ䣬�������������Ԫ��Ϣ������TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������
*/
Boolean ArrayList_destroy(ArrayList* list);

/*
 ����������ָ��λ�ò���һ��Ԫ�ء�
 ����������NULL!=list, index>=0, index<=size,NULL!=element��
 ������������������ָ��λ�ò���һ��Ԫ�ء�
 ��������������������������ʲô������������FALSE;������Ԫ��ʧ�ܣ���ʲô������������FALSE��
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������
*/
Boolean ArrayList_add(ArrayList* list, Int index, Element element);
/*
 ��������ɾ��ָ��λ�õ�һ��Ԫ�ء�
 ����������NULL!=list��index>=0, index<size��
 ��������ɾ��ָ��λ�õ�һ��Ԫ�أ����ظ�Ԫ�ء�
 ��������������������������ʲô�����������ؿ����á�
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������
*/
Element ArrayList_remove(ArrayList* list, Int index);
/*
 ��ȡ��������ָ��λ�õ�һ��Ԫ�ء�
 ����������NULL!=list��index>=0, index<size��
 ����������ȡָ��λ�õ�һ��Ԫ�ء�
 ��������������������������ʲô�����������ؿ����á�
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������
*/
Element ArrayList_get(ArrayList* list, Int index);
/*
 ������������ָ��λ�õ�Ԫ��Ϊ��ֵ��
 ����������NULL!=list��index>=0, index<size, NULL!=element��
 ��������������������ָ��λ�õ�Ԫ��Ϊ��ֵ,���ؾ�ֵ��
 ��������������������������ʲô�����������ؿ����á�
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������
*/
Element ArrayList_set(ArrayList* list, Int index, Element element);

/*
 ��������λ�õ�Ԫ�ء�
 ����������NULL!=list,index1>=0, index1<size,index2>=0, index2<size��
 ����������������λ�õ�Ԫ�ء�
 ��������������������������ʲô��������
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������
*/
void ArrayList_swap(ArrayList* list, Int index1, Int index2);

/*
 ��ȡ�ȼ���element�ĵ�һԪ�ص�������
 ����������NULL!=list,NULL!=element,NULL!=equals��
 ����������ȡ�ȼ���element�ĵ�һԪ�ص�������
 �������������������������ʧ�ܣ�����-1��
 ��ʽ���������������ǳ�ʼ�����ģ���ֻͨ������д��������equalsȷʵ�Ƚ�element��list�е�Ԫ�ء�
*/
Int ArrayList_indexOf(ArrayList* list, Element element, Boolean(*equals)(Element left, Element right));


#endif // !_ARRAY_LIST__H_
