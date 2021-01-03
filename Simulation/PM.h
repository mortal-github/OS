#ifndef _PM__H_
#define _PM__H_

#include "PCB.h"
#include "ArrayList.h"
#include "LinkList.h"

/*���̹���*/
typedef struct PM {
	/*��������*/
	ArrayList creating;
	/*��������*/
	ArrayList runnable;
	/*��������*/
	ArrayList blocked;
	/*��ֹ����*/
	ArrayList terminated;
	/*���н���*/
	PCB* run;

	/*����ʱ��*/
	Long time;
	/*����id*/
	Int nextId;
}PM;

/*
 ��ʼ�����̹���
 ����������NULL!=pm,NULL��=schedule��
 ����������ʼ��PM,����TRUE��
 ������������������������ʲô������������NULL;����ʼ��ʧ�ܣ���ʲô������������NULL��
 ��ʽ���������������Ϣ�Ѿ���Ч��������ܶ�ʧ��Ϣ��scheduleȷʵ�Ӿ��������Ƴ�һ�����̡�
*/
Boolean PM_init(PM* pm);
/*
 ���½���ʱ�������Ϣ��
 ����������NULL!=pm,now_time>pm->time��
 �����������½��̵�ʱ�������Ϣ������TRUE��
 ����������������������,��ʲô������������FALSE��
 ��ʽ������pm��ʼ��������ָͨ������д��
 */
Boolean PM_updateTime(PM* pm, Long now_time);
/*
 ����ǰ�����Ƴ�����״̬������������ж�β��
 ����������NULL!=pm��
 ������������ǰ�����Ƴ�����״̬������������ж�β������TRUE��
 ����������������������,��ʲô������������FALSE��
 ��ʽ������pm��ʼ��������ָͨ������д��
 */
Boolean PM_removeRun(PM* pm);
/*
 ���õ�ǰ���̡�
 ����������NULL!=pm��RUNNABLE=pcb->status��
 �����������õ�ǰ���̣�����TRUE��
 ����������������������,��ʲô������������FALSE��
 ��ʽ������pm��ʼ��������ָͨ������д��pcb�Ѿ��Ƴ��������У��Ҳ��ܴ������������С�
 */
Boolean PM_setRun(PM* pm, PCB* pcb);

/*
 ����հ�PCB��
 ����������NULL!=pm��
 ������������һ���հ׳�ʼ����PCB��
 ������������������������������ʧ�ܣ���ʲô������������NULL��
 ��ʽ������pm�Ѿ���ʼ��������ֻͨ����������pm��
*/
PCB* PM_apply(PM* pm);
/*������̡�
��������: NULL!=pm, NULL!=pcb,pcb!=pm->run,pcb��CREATING��BLOCKED��
����������һ��PCB���������������У�����TRUE��
�����������������������򼤻�ʧ�ܣ���ʲô������������FALSE��
��ʽ������pm�Ѿ���ʼ��������ֻͨ����������pm��pcb�Ѿ���ʼ��������ֻͨ����������pcb��
*/
Boolean PM_active(PM* pm, PCB* pcb);
/*�������̡�
��������: NULL!=pm, NULL!=pcb,pcb!=pm->run,pcb��RUNNABLE��
���������������̣��������������У�����TRUE��
����������������������������ʧ�ܣ���ʲô������������FALSE��
��ʽ������pm�Ѿ���ʼ��������ֻͨ����������pm��pcb�Ѿ���ʼ��������ֻͨ����������pcb��
*/
Boolean PM_block(PM* pm, PCB* pcb);
/*��ֹ���̡�
��������: NULL!=pm,NULL!=pcb,pcb!=pm->run,pcb����terminated��
����������ֹ���̣���������ֹ���У�����TRUE��
��������������������������ֹʧ�ܣ���ʲô������������FALSE��
��ʽ������pm�Ѿ���ʼ��������ֻͨ����������pm��pcb�Ѿ���ʼ��������ֻͨ����������pcb��
*/
Boolean PM_terminate(PM* pm, PCB* pcb);

void PM_print(PM* pm);
#endif // !_PM__H_
