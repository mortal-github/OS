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
	/*可分配资源*/
	ArrayList resources;
	/*分配矩阵*/
	ArrayList allocations;
	/*资源总类数*/
	Int resource_count;
	/*进程总数*/
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
成功则分配，失败则不分配
*/
Boolean DeadLock_Banker(Banker* banker, Int index, ArrayList* request);

/*
 安全性算法：
 输入条件：
*/
Boolean DeadLock_security(Banker* banker);


//演示银行家算法

void DeadLock_printf(Banker* banker);
void DeadLock_run(Int count);
void DeadLock_show();
#endif // !_DEADLOCK__H_
