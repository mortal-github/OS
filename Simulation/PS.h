#ifndef _PS__H_
#define _PS__H_

#include "Primitive.h"
#include "PCB.h"
#include "ArrayList.h"
#include "Heap.h"
#include "PM.h"

/*
����ҵ�����㷨��
����������NULL!=pm,
������������������У�ѡ�������Ƚ��̣��Ƴ��þ������У����ظý��̡�
����������������������������NULL,ʧ�ܷ���NULL��
��ʽ������pm��ʼ�������ҽ�ͨ����������pm��
*/
PCB* SPF(ArrayList* list);
/*
 ��ת�����㷨��
 ����������NULL!=list, NULL!=pcb��
 ��������Ϊ��ת�㷨����ѡ������PCB,�Ƴ����У������ء�
 ���������������������������ʲô������������NULL��
 ��ʽ������list��Ԫ����PCB��
*/
PCB* RR(ArrayList* list);
/*
 ����Ӧ�����ȵ����㷨��
 ����������NULL!=list, NULL!=pcb, time>0��
 ��������Ϊ����Ӧ�����ȵ����㷨���ȳ�������½���,����TRUE��
 ���������������������������ʲô������������FALSE;���ʧ�ܣ���ʲô������������FALSE��
 ��ʽ������list��Ԫ����PCB��
*/
PCB* HRRN(ArrayList* list, Long now_time);


//��ʾ���ȳ���

#define SPF_METHOD 1
#define RR_METHOD  2
#define HRRN_METHOD  3
//ѡ���ض������㷨��ִ�С�
PCB* Schedule(PM* pm, Int method);
//��ʾ�ض������㷨��һ�ε��ȡ�
Boolean runOnce(PM* pm, Int time, Int schedule);
//����ָ����Χ�����
Int random(Int min, Int max);
//*�����ض���ʱ����ֵĽ��̡�
ArrayList runOrder(Int srand_param, Int length_min, Int length_max, Int count);
//��ʾ�ض������㷨�����е���
void runUntil(PM* pm, Int time_max, Int schedule, ArrayList* list);
//ѡ����ȷ�������ʾ���ȡ�
void Schedule_show();

#endif // !_PS__H_
