#include "PS.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Int SPF_prior(PCB* left, PCB* right) {
	if (NULL == left || NULL == right) {
		return FALSE;
	}
	if (left->length < right->length) {
		return 1;
	}
	else if (left->length > right->length) {
		return -1;
	}
	else if(left->arrive < right->arrive){
		return 1;
	}
	else {
		return -1;
	}
}

PCB* SPF(ArrayList* list)
{
	if (NULL == list) {
		return NULL;
	}
	Heap heap;
	if (FALSE == Heap_init(&heap, list, 1, SPF_prior)) {
		return NULL;
	}
	if (FALSE == Heap_adjust(&heap)) {
		return NULL;
	}
	PCB* pcb = Heap_remove(&heap, 0);
	return pcb;
}

PCB* RR(ArrayList* list)
{
	return ArrayList_remove(list, 0);
}

PCB* HRRN(ArrayList* list, Long now_time)
{
	if (NULL == list) {
		return FALSE;
	}
	//������Ӧ��
	PCB* pcb;
	double last_ratio = 0;
	double ratio = 0;
	Int index = 0;
	PCB* scheduled = NULL;

	for (Int i = 0; i < list->size; i++) {
		pcb = ArrayList_get(list, i);
		if (NULL == pcb) {
			return FALSE;
		}
		//�Ƚϣ�ѡ�����Ӧ�ȡ�
		ratio = ((double)(now_time - pcb->arrive)) / pcb->length;
		pcb->priority = ratio;
		if (ratio > last_ratio) {
			scheduled = pcb;
			index = i;
			last_ratio = ratio;
		}
	}
	pcb = ArrayList_remove(list, index);
	if (NULL == pcb) {
		return NULL;
	}
	if (pcb != scheduled) {//���ռ��
		ArrayList_add(list, index, pcb);
		return NULL;
	}
	return pcb;
}

PCB* Schedule(PM* pm, Int method)
{
	if (NULL == pm) {
		return NULL;
	}
	switch (method)
	{
	case SPF_METHOD:
		return SPF(&(pm->runnable));
	case RR_METHOD:
		return RR(&(pm->runnable));
	case HRRN_METHOD:
		return HRRN(&(pm->runnable), pm->time);
	default:
		return NULL;
	}
}

Boolean runOnce(PM* pm, Int time, Int schedule) {

	//��Ȼ�ս����
	if (pm->run->run + time >= pm->run->length) {
		time = pm->run->length - pm->run->run;
	}

	//���¿�����Ϣ
	if (FALSE == PM_updateTime(pm, pm->time + time)) {
		return FALSE;
	}
	//�Ƴ����н���
	if (pm->run->run >= pm->run->length) {
		PCB_setStatus(pm->run, TERMINATED);
		ArrayList_add(&(pm->terminated), pm->terminated.size, pm->run);
		PCB_setTerminate(pm->run, pm->time);
		pm->run = NULL;
	}
	else if (FALSE == PM_removeRun(pm)) {//����������ж���
		return FALSE;
	}

	//���Ƚ���
	PCB* pcb = Schedule(pm, schedule);
	if (NULL == pcb) {
		return FALSE;
	}
	PM_setRun(pm, pcb);
	return TRUE;
}

Int random(Int min, Int max) {
	return min + (rand() % (max - min + 1));
}

ArrayList runOrder(Int srand_param, Int length_min, Int length_max, Int count) {
	ArrayList list;
	PCB* pcb = NULL;
	Int length = 0;
	Long time = 0;

	ArrayList_init(&list);
	srand(srand_param);

	for (Int i = 1; i <= count; i++) {
		length = random(length_min, length_max);
		pcb = malloc(sizeof(PCB));
		PCB_init(pcb, i);
		PCB_setLength(pcb, length);
		PCB_setArrive(pcb, time);
		PCB_setStatus(pcb, RUNNABLE);

		ArrayList_add(&list, list.size, pcb);

		time += random(1, length_max);//����һ��ʱ���
	}

	return list;
}

void runUntil(PM* pm, Int time_max, Int schedule, ArrayList* list) {
	PCB* pcb;
	Int once_time = 0;
	Long next_time = 0;
	Long old_time = 0;
	pm->run = ArrayList_remove(list, 0);
	if (list->size > 0) {
		pcb = ArrayList_get(list, 0);
		next_time = pcb->arrive;
	}
	
	do {
		//����һ��ʱ��
		once_time = pm->run->length - pm->run->run;
		once_time = time_max < once_time ? time_max : once_time;
		once_time = random(1, once_time);
		//����һ�����̡�
		if (list->size > 0 && (pm->time + once_time) >= next_time) {
			once_time = (Int)(next_time - pm->time);
			pcb = ArrayList_remove(list, 0);
			ArrayList_add(&pm->runnable, pm->runnable.size, pcb);
			
			if (list->size > 0) {
				pcb = ArrayList_get(list, 0);
				next_time = pcb->arrive;
			}
		}

		//ģ��һ�����С�
		PCB* old_run = pm->run;
		runOnce(pm, once_time, schedule);
		PCB* new_run = pm->run;
		if (old_run != new_run) {
			//��ӡ���н��
			printf_s("========================================\n");
			printf_s("ʱ�䣺%-4"PRId64", ���У�%-4"PRId64"\n", pm->time, pm->time - old_time);
			PM_print(pm);
			printf_s("\n");

			old_time = pm->time;
		}
	} while (NULL != pm->run);
}

void Schedule_show()
{
	Boolean loop;
	Int time_max = 1;
	Int schedule = 1;
	Int length_min = 1;
	Int length_max = 2;
	Int count = 3;
	PM pm;
	ArrayList list;
	Int srand_param = 0;
	PM_init(&pm);
	
	printf_s("�����飺\n");
	printf_s("	3118005434�� �Ӿ��ģ�2018������Ϣ��ȫ2�࣬�����ѧԺ���㶫��ҵ��ѧ\n");
	printf_s("	���̵�����ʾ����ѡ�������ָ����ʾ��ʽ\n");
	printf_s("��ͬ�����������������ͬ�Ľ�����\n");
	printf_s("�����㷨��\n");
	printf_s("	1: �̽��������㷨\n");
	printf_s("	2: ��ת�����㷨\n");
	printf_s("	3: ����Ӧ���㷨\n");
	do {
		printf_s("����������������,�����㷨,��С���̳���,�����̳���,��������\n");
		printf_s("���룺");
		scanf_s("%d,%d,%d,%d,%d", &srand_param, &schedule, &length_min, &length_max, &count);
		if (schedule < 1 || schedule >3 || length_min < 1 || length_max <= length_min || count < 3) {
			loop = TRUE;
			printf_s("�������\n");
		}
		else {
			loop = FALSE;
		}
	} while (TRUE == loop);

	switch (schedule){
	case SPF_METHOD:
		time_max = length_max;
		break;
	case RR_METHOD:
		time_max = 1;
		break;
	case HRRN_METHOD:
		time_max = 1;
		break;
	default:
		break;
	}

	list = runOrder(srand_param, length_min, length_max, count);
	runUntil(&pm, time_max, schedule, &list);

	//�����ƽ����תʱ��
	Long temp = 0;
	PCB* pcb;
	double avg1 = 0;
	double avg2 = 0;
	for (Int i = 0; i < pm.terminated.size; i++) {
		pcb = ArrayList_get(&(pm.terminated), i);
		temp = pcb->terminated - pcb->arrive;
		avg1 += temp;
		avg2 += ((double)temp) / ((double)pcb->length);
		
	}
	avg1 /= pm.terminated.size;
	avg2 /= pm.terminated.size;
	printf_s("ƽ����תʱ�䣺%f", avg1);
	printf_s("\nƽ����Ȩ��תʱ�䣺%f", avg2);
}
