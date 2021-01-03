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
	//计算响应比
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
		//比较，选择高响应比。
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
	if (pcb != scheduled) {//保险检查
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

	//自然终结进程
	if (pm->run->run + time >= pm->run->length) {
		time = pm->run->length - pm->run->run;
	}

	//更新控制信息
	if (FALSE == PM_updateTime(pm, pm->time + time)) {
		return FALSE;
	}
	//移除运行进程
	if (pm->run->run >= pm->run->length) {
		PCB_setStatus(pm->run, TERMINATED);
		ArrayList_add(&(pm->terminated), pm->terminated.size, pm->run);
		PCB_setTerminate(pm->run, pm->time);
		pm->run = NULL;
	}
	else if (FALSE == PM_removeRun(pm)) {//移入就绪队列队首
		return FALSE;
	}

	//调度进程
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

		time += random(1, length_max);//经过一段时间后，
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
		//经过一段时间
		once_time = pm->run->length - pm->run->run;
		once_time = time_max < once_time ? time_max : once_time;
		once_time = random(1, once_time);
		//增加一个进程。
		if (list->size > 0 && (pm->time + once_time) >= next_time) {
			once_time = (Int)(next_time - pm->time);
			pcb = ArrayList_remove(list, 0);
			ArrayList_add(&pm->runnable, pm->runnable.size, pcb);
			
			if (list->size > 0) {
				pcb = ArrayList_get(list, 0);
				next_time = pcb->arrive;
			}
		}

		//模拟一次运行。
		PCB* old_run = pm->run;
		runOnce(pm, once_time, schedule);
		PCB* new_run = pm->run;
		if (old_run != new_run) {
			//打印运行结果
			printf_s("========================================\n");
			printf_s("时间：%-4"PRId64", 运行：%-4"PRId64"\n", pm->time, pm->time - old_time);
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
	
	printf_s("程序简介：\n");
	printf_s("	3118005434， 钟景文，2018级别信息安全2班，计算机学院，广东工业大学\n");
	printf_s("	进程调度演示程序：选择代码来指定演示方式\n");
	printf_s("相同的随机参数将产生相同的进程组\n");
	printf_s("调度算法：\n");
	printf_s("	1: 短进程优先算法\n");
	printf_s("	2: 轮转调度算法\n");
	printf_s("	3: 高响应比算法\n");
	do {
		printf_s("输入参数：随机参数,调度算法,最小进程长度,最大进程长度,进程总数\n");
		printf_s("输入：");
		scanf_s("%d,%d,%d,%d,%d", &srand_param, &schedule, &length_min, &length_max, &count);
		if (schedule < 1 || schedule >3 || length_min < 1 || length_max <= length_min || count < 3) {
			loop = TRUE;
			printf_s("输入错误！\n");
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

	//计算机平均周转时间
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
	printf_s("平均周转时间：%f", avg1);
	printf_s("\n平均带权周转时间：%f", avg2);
}
