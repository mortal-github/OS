#include "PCB.h"

Boolean PCB_init(PCB* pcb, Int id)
{
	if (NULL == pcb || id < 0) {
		return FALSE;
	}
	pcb->id = id;
	pcb->name = NULL;
	pcb->status = CREATING;
	pcb->priority = 0;
	pcb->arrive = 0;
	pcb->terminated = 0;
	pcb->length = 0;
	pcb->run = 0;
	pcb->wake = 0;
	pcb->ram = 0;
	pcb->size = 0;
	return TRUE;
}

Boolean PCB_setName(PCB* pcb, Char* name)
{
	if (NULL == pcb) {
		return FALSE;
	}
	pcb->name = name;
	return TRUE;
}

Boolean PCB_setStatus(PCB* pcb, Byte status)
{
	if (NULL == pcb) {
		return FALSE;
	}
	//检验status
	switch (status) {
	case CREATING:
		break;
	case RUNNABLE:
		break;
	case BLOCKED:
		break;
	case TERMINATED:
		break;
	default:
		return FALSE;
	}
	//更改进程状态
	pcb->status = status;
	return TRUE;
}

Boolean PCB_setPriority(PCB* pcb, Int priority)
{
	if (NULL == pcb) {
		return FALSE;
	}
	pcb->priority = priority;
	return TRUE;
}

Boolean PCB_setArrive(PCB* pcb, Long arrive)
{
	if (NULL == pcb || arrive < 0) {
		return FALSE;
	}
	pcb->arrive = arrive;
	return TRUE;
}

Boolean PCB_setTerminate(PCB* pcb, Long terminate)
{
	if (NULL == pcb || terminate <= pcb->arrive) {
		return FALSE;
	}
	pcb->terminated = terminate;
	return TRUE;
}

Boolean PCB_setLength(PCB* pcb, Int length)
{
	if (NULL == pcb || length < 1) {
		return FALSE;
	}
	pcb->length = length;
	return TRUE;
}

Boolean PCB_addRun(PCB* pcb, Int time)
{
	if (NULL == pcb || time < 1) {
		return FALSE;
	}
	pcb->run += time;
	return TRUE;
}
