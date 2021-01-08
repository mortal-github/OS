#ifndef _INDEX_LIST__H_
#define _INDEX_LIST__H_

#include "Primitive.h"

/*
 ������
 size     ����ЧԪ�صĸ���
 top      �����һ��Ԫ��֮���λ�á�
 max      ����indexs����ĳ��ȡ�
 unit     ��elements������ÿ��Ԫ�صĴ�С��
 length   ��elements����Ĵ�С����Ԫ�ش�СΪ��λ��
 indexs   ����¼��elements�����ж���֮ǰ����λ��������
 elements ������Ԫ�صĿռ䡣
*/
typedef struct IndexList {
	/*
	�ߴ磺��ЧԪ�صĸ���
	*/
	Int size;
	/*
	���꣺���һ��Ԫ��֮���λ�á�
	*/
	Int top;
	/*
	���������������indexs����ĳ��ȡ�
	*/
	Int max;
	/*
	Ԫ�ص�λ��elements������ÿ��Ԫ�صĴ�С��
	*/
	Int unit;
	/*
	����/Ԫ�أ�elements����Ĵ�С����Ԫ�ش�СΪ��λ��
	*/
	Int length;
	/*
	�������飺��¼��elements�����ж���֮ǰ����λ��������
	*/
	Pointer indexs;
	/*
	Ԫ�����飺����Ԫ�ء�
	*/
	Pointer elements;
}IndexList;

/*
 ����������������������ʼ������֮��ֻͨ������������������������������ԡ�
 �����������ޡ�
 ������������������������������ʼ������֮��ֻͨ������������������������������ԡ�
 �������ޡ�
*/
void IndexList_assert(IndexList list);//���
/*
 �����������������������ռ��С��
 ����������max>0��
 �������������������������������ռ��С������ֵ��
 ����������0��
*/
Int IndexList_sizeofIndexs(Int max);//����
/*
 �����������Ԫ����������ռ��С��
 ����������unit>0, length>0��
 �������������������Ԫ����������ռ��С������ֵ��
 ����������0��
*/
Int IndexList_sizeofElements(Int unit, Int length);//����

/*
 ��ʼ��������
 ����������NULL!=list,max>0,NULL!=indexs,unit>0,length>0,NULL!=elements��
 ����������ʼ��������TRUE��
 ��������������������������ʲô������������FALSE��
 ע�⣺indexs����ռ�����Ҫ��max*sizeof(Int), elements����ռ��С����Ҫ��unit*length��
*/
Boolean IndexList_init(IndexList* list, Int max, Pointer indexs, Int unit, Int length, Pointer elements);//���
/*
 ��origin���¡��target���С�
 ����������NULL!=origin,NULL!=target, target->size=0, target->max>=origin->top-origin->size, target->unit==origin->unit, target->length>=origin->top��
 ����������origin��Ŀ�¡��target���С�
 ��������������������������ʲô������������FALSE��
 ��ʽ�������������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Boolean IndexList_clone(IndexList* origin, IndexList* target);//���
/*
 �����������Ԫ��λ�ã��ͷ�����Ŀ�����Ϣ��������������顣
 ����������NULL!=list��
 �������������������Ԫ��λ�ã��ͷ�����Ŀ�����Ϣ������TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
 ע�⣺�����󣬲���Ԫ�ص�λ�÷����ı䣬��������������ַ��ȷ��Ԫ�أ���ô���ܻ����
 �����������������ʹ�ô���������������
*/
Boolean IndexList_trim(IndexList* list);
/*
 ��ȡԪ���ڱ��еĴ���
 ����������NULL!=list,index>=0, index<list->top��
 ����������ȡ����Ԫ���ڱ��еĴ��򣬷�����š�
 ��������������������������ʲô������������-1;������λ���ǿ��еģ�����-1��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Int IndexList_getOrderFromIndex(IndexList* list, Int index);//��Ӳ��
/*
 ����Ԫ���ڱ��д����ȡ��������
 ����������NULL!=list,order>=0, order<list->size��
 ����������������Ԫ���ڱ��д����ȡ������������������
 ��������������������������ʲô������������-1��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Int IndexList_getIndexFromOrder(IndexList* list, Int order);//��Ӳ��
/*
 �������������Ӧָ�롣
 ����������NULL!=list,index>=0, index<list->top��
 ���������������������Ӧָ�룬����ָ�롣
 ��������������������������ʲô������������NULL��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Pointer IndexList_getAddressFromIndex(IndexList* list, Int index);//����
/*
 ����Ԫ���ڱ��е�ָ���ȡ��������
 ����������NULL!=list, NULL!=element��
 ������������Ԫ���ڱ��е�ָ���ȡ������������������
 ��������������������������ʲô������������-1����Ԫ�ز��ڱ��л�ָ��û��ָ��Ԫ���׵�ַ������-1��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Int IndexList_getIndexFromAddress(IndexList* list, Pointer element);//����
/*
 ����һ������index��������Ŀ���������indexs��λ�ã������Լ��ǿ��еģ��򷵻��Լ���λ�á�
 ����������NULL!=list,index>=0,index<list->length��
 ������������һ������index��������Ŀ���������indexs��λ�ã������Լ��ǿ��еģ��򷵻��Լ���λ�á�
 �������������������������򷵻�-1����û���ҵ����򷵻�-1��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Int IndexList_getNearFree(IndexList* list, Int index);//��Ӳ��

/*
 ��ȡԪ��λ����Ԫ�ص�ָ�롣
 ����������NULL!=list��order>=0, order<list->size��
 ����������ȡԪ��λ����Ԫ�ص�ָ�롣
 ��������������������������ʲô������������NULL��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Pointer IndexList_get(IndexList* list, Int order);//���
/*
 ���õ�order��Ԫ��Ϊ��ֵ��
 ����������NULL!=list,order>=0,order<list->size, NULL!=element��
 �����������õ�order��Ԫ��Ϊ��ֵ��element����ֵ���ݵ�ָ�룬����TRUE��
 ��������������������������ʲô���������򷵻�FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��element�Ǻ���ġ�
 */
Boolean IndexList_set(IndexList* list, Int order, Pointer element);//���
/*
 �ڱ�order����λ�ò���һ��Ԫ�ء�
 ����������NULL!=list, order>=0, order<=list->size,NULL!=element��
 ���������ڱ�order����λ�ò���һ��Ԫ�ء�
 ��������������������������ʲô������������FALSE;������Ԫ��ʧ�ܣ���ʲô������������FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Boolean IndexList_add(IndexList* list, Int order, Pointer element);//���
/*
 ɾ������order����λ�õ�һ��Ԫ�ء�
 ����������NULL!=list��order>=0, order<list->size��
 ��������ɾ��order����λ�õ�һ��Ԫ�أ�����TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Boolean IndexList_remove(IndexList* list, Int order);//���
/*
 ��������������һ������Ԫ�ص�Ԫ��������������
 ����������NULL!=list, list->size<list->length��
 ����������������������һ������Ԫ�ص�Ԫ��������������
 ��������������������������ʲô���������򷵻�-1��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Int IndexList_malloc(IndexList* list);
/*
 �ͷű���index����λ�õ�Ԫ�ؿռ䡣
 ����������NULL!=list��index>0, index<list->top��
 ���������ͷű���index����λ�õ�Ԫ�ؿռ䣬�򷵻�TRUE��
 ��������������������������ʲô������������FALSE�����ͷ�ʧ�ܻ����������ǿ��еģ�����FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Boolean IndexList_free(IndexList* list, Int index);
/*
 �ͷű�ȫ��Ԫ�ؿռ䡣
 ����������NULL!=list��
 ���������ͷű�ȫ��Ԫ�ؿռ䣬�򷵻�TRUE��
 ��������������������������ʲô������������FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Boolean IndexList_freeAll(IndexList* list);

/*
 ��������λ�õ�Ԫ�ء�
 ����������NULL!=list,list->size<list->length,order1!=order2,order1>=0,order2>=0,order1<list->size,order2<list->szie��
 ����������������λ�õ�Ԫ��,����TRUE��
 ��������������������������ʲô������,����FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
*/
Boolean IndexList_swap(IndexList* list, Int order1, Int order2);//���
/*
 �������ÿ��Ԫ�أ���ÿ��Ԫ�ص���Element_runable��
 ����������NULL!=array,NULL!=Element_runable��
 ���������������ÿ��Ԫ�أ���ÿ��Ԫ�ص���Element_runable������TRUE��
 ��������������������������ʲô������������FALSE��������Element_runable����FALSE,ֹͣ����������FALSE��
 ��ʽ���������ǳ�ʼ�����ģ���ֻͨ������д��
 */
Boolean IndexList_forEach(IndexList* list, Boolean(*Element_runable)(Pointer element));//���

void IndexList_test(Boolean(*Element_printf)(Int* element));
#endif // !_INDEX_LIST__H_
