#ifndef _DEADLOCK__H_
#define _DEADLOCK__H_

#include "ArrayList.h"
#include "PM.h"

typedef struct Resource {
	Char* name;
	Int count;
}Resource;

typedef struct AllocationCounter {
	Int max;
	Int had;
}AllocationCounter, Counter;

typedef struct Allocation {
	PCB* pcb;
	ArrayList counter;
	Int order;
	ArrayList request;
	Boolean req;
}Allocation;

typedef struct Banker {
	/*�ɷ�����Դ*/
	ArrayList resources;
	/*�������*/
	ArrayList allocations;
	/*��Դ������*/
	Int resource_count;
	/*��������*/
	Int process_count;
}Banker;

Boolean DeadLock_init(Banker* banker);

Boolean DeadLock_addResource(Banker* banker, Char* name, Int count);

Boolean DeadLock_addProcess(Banker* banker, PCB* pcb);

Boolean DeadLock_removeProcess(Banker* banker, PCB* pcb);



Boolean DeadLock_setMax(Banker* banker, Int process_index, Int resource_index, Int max);

Boolean DeadLock_initEmptyRequest(Banker* banker, ArrayList* request);

Boolean DeadLock_setRequest(ArrayList* request, Int resource_index, Int count);

Int DeadLock_getRequest(ArrayList* request, Int resource_index);

Int DeadLock_indexOf(Banker* banker, PCB* pcb);

Boolean DeadLock_releaseIfFull(Banker* banker, Int index);

/*
�ɹ�����䣬ʧ���򲻷���
*/
Boolean DeadLock_Banker(Banker* banker, Int index, ArrayList* request);

/*
 ��ȫ���㷨��
 ����������
*/
Boolean DeadLock_security(Banker* banker);


//��ʾ���м��㷨

void DeadLock_printf(Banker* banker);
void DeadLock_run(Int count);
void DeadLock_show();
#endif // !_DEADLOCK__H_
