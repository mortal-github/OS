#include "PM.h"
#include <malloc.h>
#include <stdio.h>

Boolean PM_init(PM* pm)
{
	if (NULL == pm) {
		return FALSE;
    }
	if (FALSE == ArrayList_init(&(pm->creating))) {
		return FALSE;
	}
	if (FALSE == ArrayList_init(&(pm->runnable))) {
		return FALSE;
	}
	if (FALSE == ArrayList_init(&(pm->blocked))) {
		return FALSE;
	}
	if (FALSE == ArrayList_init(&(pm->terminated))) {
		return FALSE;
	}
	pm->run = NULL;
	pm->time = 0;
	pm->nextId = 1;
	return TRUE;
}

Boolean PM_updateTime(PM* pm, Long now)
{
	if (NULL == pm || now <= pm->time) {
		return FALSE;
	}
	if (NULL == pm->run) {//����
		return FALSE;
	}
	//���½��̿�����Ϣ��
	Long increment = now - pm->time;
	pm->time = now;
	if (FALSE == PCB_addRun(pm->run, (Int)increment)) {
		return FALSE;
	}
	return TRUE;
}
Boolean PM_removeRun(PM* pm) {
	if (NULL == pm) {
		return FALSE;
	}
	//�����β��
	if (FALSE == ArrayList_add(&(pm->runnable), pm->runnable.size, pm->run)) {
		return FALSE;
	}
	pm->run = NULL;
	return TRUE;
}

Boolean PM_setRun(PM* pm, PCB* pcb) {
	if (NULL == pm || NULL == pcb || RUNNABLE != pcb->status) {
		return FALSE;
	}
	pm->run = pcb;
	return TRUE;
}

PCB* PM_apply(PM* pm)
{
	if (NULL == pm) {
		return NULL;
	}
	PCB* pcb = malloc(sizeof(PCB));
	if (NULL == pcb) {
		return NULL;
	}
	//��ʼ��
	if (FALSE == PCB_init(pcb, pm->nextId)) {
		return NULL;
	}
	if (FALSE == ArrayList_add(&(pm->creating), pm->creating.size, pcb)) {
		free(pcb);
		return NULL;
	}
	pm->nextId++;
	return pcb;
}

Boolean PM_PCB_equals(PCB* left, PCB* right) {
	if (NULL == left || NULL == right) {
		return FALSE;
	}
	if (left->id == right->id) {
		return TRUE;
	}
	return FALSE;
}

Boolean PM_active(PM* pm, PCB* pcb) {
	if (NULL == pm || NULL == pcb || pcb == pm->run) {
		return FALSE;
	}
	//���ҽ��̶���
	ArrayList* list;
	if ( BLOCKED == pcb->status) {
		list = &(pm->blocked);
	}
	else if (CREATING == pcb->status) {
		list = &(pm->creating);
	}
	else {
		return FALSE;
	}
	if (NULL == list) {
		return FALSE;
	}
	//���ҽ���
	Int index = ArrayList_indexOf(list, pcb, PM_PCB_equals);
	if (-1 == index) {
		return FALSE;
	}
	PCB* deleted = ArrayList_remove(list, index);
	if (NULL == deleted) {
		return FALSE;
	}
	if (pcb != deleted) {//���ռ��
		ArrayList_add(list, index, pcb);
		return FALSE;
	}
	//�����������
	if (FALSE == ArrayList_add(&(pm->runnable), pm->runnable.size, pcb)) {
		ArrayList_add(list, index, pcb);
		return FALSE;
	}
	PCB_setStatus(pcb, RUNNABLE);
	return TRUE;
}

Boolean PM_block(PM* pm, PCB* pcb) {
	if (NULL == pm || NULL == pcb || pcb == pm->run || RUNNABLE != pcb->status) {
		return FALSE;
	}
	//������������
	Int index = ArrayList_indexOf(&(pm->runnable), pcb, PM_PCB_equals);
	if (-1 == index) {
		return FALSE;
	}
	PCB* runnable = ArrayList_remove(&(pm->runnable), index);
	if (NULL == runnable) {
		return FALSE;
	}
	if (pcb != runnable) {
		ArrayList_add(&(pm->runnable), index, pcb);
		return FALSE;
	}
	//������������
	if (FALSE == ArrayList_add(&(pm->blocked), pm->blocked.size, pcb)) {
		ArrayList_add(&(pm->runnable), index, pcb);
		return FALSE;
	}
	//����
	PCB_setStatus(pcb, BLOCKED);

	return TRUE;
}

Boolean PM_terminate(PM* pm, PCB* pcb) {
	if (NULL == pm || NULL == pcb || pcb == pm->run) {
		return FALSE;
	}

	Int index;
	ArrayList* list;
	switch (pcb->status) {
	case CREATING:
		list = &(pm->creating);
		break;
	case RUNNABLE:
		list = &(pm->runnable);
		break;
	case BLOCKED:
		list = &(pm->blocked);
		break;
	default:
		return FALSE;
	}
	//������ֹ����
	index = ArrayList_indexOf(list, pcb, PM_PCB_equals);
	if (-1 == index) {
		return FALSE;
	}
	PCB* terminated = ArrayList_remove(list, index);
	if (NULL == terminated) {
		return FALSE;
	}
	if (pcb != terminated) {//���ռ��
		ArrayList_add(list, index, pcb);
		return FALSE;
	}
	//������ֹ����
	if (FALSE == ArrayList_add(&(pm->terminated), pm->terminated.size, pcb)) {
		ArrayList_add(list, index, pcb);
		return FALSE;
	}
	PCB_setStatus(pcb, TERMINATED);
	PCB_setTerminate(pcb, pm->time);
	return TRUE;
}

void PM_PCB_print(PCB* pcb) {
	if (NULL == pcb) {
		return;
	}
	printf_s(" ���� ���� ���� ����\n");
	printf_s("|id        = %"PRId32"\n", pcb->id);
	printf_s("|name      = %s\n", pcb->name);
	printf_s("|arrive    = %"PRId64"\n", pcb->arrive);
	printf_s("|length    = %"PRId32"\n", pcb->length);
	printf_s("|run       = %"PRId32"\n", pcb->run);
	printf_s("|priori    = %f\n", pcb->priority);
	printf_s("|terminate = %"PRId64"\n", pcb->terminated);
	printf_s("|wake      = %"PRId64"\n", pcb->wake);
	printf_s("|ram       = %p\n", pcb->ram);
	printf_s("|size      = %d\n", pcb->size);
	printf_s(" ���� ���� ���� ����\n");
}

void PM_print(PM* pm) {
	//���н���
	PCB* pcb = pm->run;
	if (NULL != pcb) {
		printf_s("���н���:\n");
		PM_PCB_print(pcb);
	}
	//��������
	printf_s("��������:\n");
	for (Int i = 0; i < pm->runnable.size; i++) {
		pcb = ArrayList_get(&(pm->runnable), i);
		PM_PCB_print(pcb);
	}
	//��������
	printf_s("��������:\n");
	for (Int i = 0; i < pm->blocked.size; i++) {
		pcb = ArrayList_get(&(pm->blocked), i);
		PM_PCB_print(pcb);
	}
	//����
	printf_s("��ֹ����:\n");
	for (Int i = 0; i < pm->terminated.size; i++) {
		pcb = ArrayList_get(&(pm->terminated), i);
		PM_PCB_print(pcb);
	}
}