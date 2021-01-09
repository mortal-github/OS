#include "Primitive.h"
#include <stdio.h>
#include <stdlib.h>
#include "PCB.h"
#include "PM.h"
#include "PS.h"
#include "Deadlock.h"
#include "DynamicPartition.h"
#include "DiskSchedule.h"

Boolean Int_print(Int* element) {
	if (NULL == element) {
		return FALSE;
	}
	printf_s(", %d", *element);
	return TRUE;
}

void Computer_simulation();
int main() {
	printf_s("����ϵͳʵ����γ���ƣ�\n");
	printf_s("	���ߣ��Ӿ���\n");
	printf_s("	ѧ�ţ�3118005434\n");
	printf_s("	�����Ϣ��ȫ��2����\n");
	printf_s("	�꼶��2018�꼶\n");
	printf_s("	ѧԺ�������ѧԺ\n");
	printf_s("	ѧУ���㶫��ҵ��ѧ\n");
	printf_s("\n\n");
	printf_s("�����飺\n");
	printf_s("�����򼯺����ĸ�ʵ���һ���γ̣���ͨ������ѡ��");
	printf_s("1 : ʵ��һ�����̵�����ʾ����\n");
	printf_s("2 : ʵ��������м��㷨��ʾ����\n");
	printf_s("3 : ʵ��������̬����������ʾ����\n");
	printf_s("4 : ʵ���ģ����̵����㷨��ʾ����\n");
	printf_s("5 : �γ���ƣ�����ϵͳ��ʾ����\n");

	Int select = 0;
	do {
		printf_s("���룺");
		scanf_s("%d", &select);
		if (select < 1 || select > 5) {
			continue;
		}
	} while (1 == 0);

	switch (select){
	case 1:Schedule_show();//���̵�����ʾ
		break;
	case 2:DeadLock_show();//���м��㷨��ʾ
		break;
	case 3:DynamicPartition_show();//��̬�����㷨��ʾ
		break;
	case 4:DiskSchedule_show();//���̵�����ʾ
		break;
	case 5:Computer_simulation();//�γ����
		break;
	default:
		break;
	}
	system("pause");
}

void Computer_simulation() {
	printf_s("�����飺�γ���ơ�������ģ�����ϵͳ\n");
	printf_s("	3118005434�� �Ӿ��ģ�2018������Ϣ��ȫ2�࣬�����ѧԺ���㶫��ҵ��ѧ\n");
	printf_s("1. �ύһ����ҵ��>=10�����������ȷ�ѡ��һ������ҵ�����5���������ڴ�\n");
	printf_s("2. Ϊÿ����ҵ����һ�����̣��������ڴ棨�û��ڴ棺0��1024K�����ÿɱ��������䷽ʽ\n");
	printf_s("3. ���̵��ȹ��ܣ�ʱ��Ƭ��ת��\n");
	printf_s("4. ����������̣�����һ��ʱ����ѽ��̣�ѡ����\n");
	printf_s("5. ��ʾ�����Ϣ������ҵ���С��ڴ���������������Ϣ�������ҵ���\n");

	Int job = 0;
	printf_s("���������ҵ������");
	scanf_s("%d", &job);
	Int total = job;
	//���̹���
	PM pm;
	PM_init(&pm);
	//�������
	LinkList ram;
	Int size = 1024 * 1024;
	//ʱ��
	DynamicPartition_init(&ram, size);
	printf_s("\n\n");
	do {
		//1 .�ύһ����ҵ�� >= 10�����������ȷ�ѡ��һ������ҵ�����5���������ڴ档
		while(pm.runnable.size + pm.blocked.size < 5) {
			job--;
			PCB* pcb = PM_apply(&pm);
			PM_active(&pm, pcb);
		//2. Ϊÿ����ҵ����һ�����̣��������ڴ棨�û��ڴ棺0��1024K�����ÿɱ��������䷽ʽ\n
			Int need = 512 + rand() % (size / 5);
			pcb->size = need;
			Pointer address = DynamicPartition_mallocFirst(&ram, need);
			if (NULL == address) {
				PM_block(&pm, pcb);//���䲻���ڴ��������
			}
			else {
				pcb->ram = address;
			}
		}
		//3. ���̵��ȹ��ܣ�ʱ��Ƭ��ת��
		PCB* run = RR(&(pm.runnable));
		PM_setRun(&pm, run);


		//5. ��ʾ�����Ϣ������ҵ���С��ڴ���������������Ϣ�������ҵ���
		printf_s("**********************************************************************************\n");
		printf_s("���ڵ�ʱ�䣺%lld\n", pm.time);
		printf_s("�󱸶������� %d ����ҵ���Ѿ���� %d ����ҵ\n", job, pm.terminated.size);
		printf_s("�ڴ��������������ڴ��뿴������Ϣ��PCB.ram(�ڴ�ʼַ),PCB.size(�ڴ��С)\n");
		printf_s("=================================\n");
		printf_s("���з�������\n");
		DynamicPartition_print(&ram);
		printf_s("=================================\n");
		printf_s("������Ϣ��\n");
		PM_print(&pm);
		printf_s("\n");
		system("pause");//��ͣ


		//4. ����������̣�����һ��ʱ����ѽ��̣�ѡ��)
		if (NULL != run) {
			run->run++;//������һ��ʱ��Ƭ
			pm.time++;
			Int wake = rand() % 100;
			if (wake < 5) {//��ֹ����
				run->terminated = pm.time;
				PM_removeRun(&pm);
				PM_terminate(&pm, run);
				//�ͷ��ڴ�
				DynamicPartition_freeFirst(&ram, run->ram);
				//Ϊ��Ϊ�ڴ治�������Ľ��̷����ڴ档
				for (Int i = 0; i < pm.blocked.size; i++) {
					PCB* pcb = ArrayList_get(&pm.blocked, i);
					if (NULL == pcb->ram) {
						pcb->ram = DynamicPartition_mallocFirst(&ram, pcb->size);
						if (NULL == pcb->ram) {
							PM_active(&pm, pcb);
						}
					}
				}
			}
			else if(wake < 50){//��������
				run->wake = pm.time + wake/20;
				PM_removeRun(&pm);
				PM_block(&pm, run);
			}
			else {//�����������
				PM_removeRun(&pm);
			}
		}
		for (Int i = 0; i < pm.blocked.size; i++) {//���ѽ��̡�
			PCB* pcb = ArrayList_get(&(pm.blocked), i);
			if (pcb->wake <= pm.time && NULL != pcb->ram) {
				pcb->wake = 0;
				PM_active(&pm, pcb);
				i--;
			}
		}
	} while (0 != job && 0 != pm.runnable.size + pm.blocked.size);//û����ҵ��û�п����н���ʱ�����ģ��	
}