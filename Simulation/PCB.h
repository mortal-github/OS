#ifndef _PCB__H_
#define _PCB__H_

#include "Primitive.h"

typedef struct PCB {
	//���̱�ʶ��
	Int id;
	//struct PCB* parent;
	//ArrayList children;
	Char* name;

	//�����״̬
	//Long ir;
	//Long pswr;
	//Long* regs;
	//Short regs_size;

	//���̵�����Ϣ
	Byte status;
	double priority;
	Long arrive;
	Long terminated;
	Int length;
	Int run;

	//���̿�����Ϣ
	//Long ram;
	//Long swap;
	//Long disk;
	//ͬ����ͨ�Ż��ơ�
	//��Դ�嵥��
}PCB;

#define CREATING 0
#define RUNNABLE 1
#define BLOCKED 2
#define TERMINATED 3

/*
 ��ʼ��PCB��
 ����������NULL!=pcbm, id>=0��
 ����������ʼ��PCB��
 ����������������������������FALSE��
 ��ʽ����:
 */
Boolean PCB_init(PCB* pcb, Int id);
/*
 ���ý�������
 ����������NULL!=pcbm��
 �����������ý�����������TRUE��
 ����������������������������FALSE��
 ��ʽ����: 
 */
Boolean PCB_setName(PCB* pcb, Char* name);

/*
 ���ý���״̬��
 ����������NULL!=pcbm��status�ǺϷ���ֵ��
 �����������ý���״̬����TRUE��
 ����������������������������FALSE;ת��ʧ�ܷ���FALSE��
 ��ʽ����:
 */
Boolean PCB_setStatus(PCB* pcb, Byte status);
/*
 ���ý�����������
 ����������NULL!=pcbm��
 �����������ý���������������TRUE��
 ����������������������������FALSE��
 ��ʽ����:
 */
Boolean PCB_setPriority(PCB* pcb, Int priority);
/*
 ���ý��̵���ʱ�䡣
 ����������NULL!=pcbm, arrive>=0��
 �����������ý��̵���ʱ�䣬����TRUE��
 ����������������������������FALSE��
 ��ʽ����:
 */
Boolean PCB_setArrive(PCB* pcb, Long arrive);
/*
 ���ý��̽���ʱ�䡣
 ����������NULL!=pcbm, arrive>=0��
 �����������ý��̽���ʱ�䣬����TRUE��
 ����������������������������FALSE��
 ��ʽ����:
 */
Boolean PCB_setTerminate(PCB* pcb, Long terminate);
/*
 ���ý��̳��ȡ�
 ����������NULL!=pcbm, length>0��
 �����������ý��̳��ȣ�����TRUE��
 ����������������������������FALSE��
 ��ʽ����:
 */
Boolean PCB_setLength(PCB* pcb, Int length);
/*
 ��������ʱ�������
 ����������NULL!=pcbm, add_time>0��
 ����������������ʱ�����,����TRUE��
 ����������������������������FALSE��
 ��ʽ����:
 */
Boolean PCB_addRun(PCB* pcb, Int time);
#endif // !_PCB__H_
