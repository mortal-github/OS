#include "Deadlock.h"
#include <malloc.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "PS.h"

Boolean DeadLock_init(Banker* banker)
{
	if (NULL == banker) {
		return FALSE;
	}
	if (FALSE == ArrayList_init(&(banker->resources))) {
		return FALSE;
	}
	if (FALSE == ArrayList_init(&(banker->allocations))) {
		return FALSE;
	}
	banker->resource_count = 0;
	banker->process_count = 0;
	return TRUE;
}

Boolean DeadLock_addResource(Banker* banker, Char* name, Int count)
{
	if (NULL == banker || count < 0) {
		return FALSE;
	}
	//获得资源节点
	Resource* resource = malloc(sizeof(Resource));
	if (NULL == resource) {
		return FALSE;
	}
	resource->name = name;
	resource->count = count;
	//添加资源
	if (FALSE == ArrayList_add(&(banker->resources), banker->resources.size, resource)) {
		free(resource);
		return FALSE;
	}
	banker->resource_count++;
	//增加分配矩阵列
	Allocation* allocation;
	AllocationCounter* counter;
	for (Int i = 0; i < banker->process_count; i++) {
		allocation = ArrayList_get(&(banker->allocations), i);
		counter = malloc(sizeof(AllocationCounter));
		if (NULL == counter) {
			return FALSE;
		}
		counter->max = 0;
		counter->had = 0;

		ArrayList_add(&(allocation->counter), allocation->counter.size, counter);
	}
	return TRUE;
}

Boolean DeadLock_addProcess(Banker* banker, PCB* pcb)
{
	if (NULL == banker || NULL == pcb) {
		return FALSE;
	}

	Allocation* allocation = malloc(sizeof(Allocation));
	if (NULL == allocation) {
		return FALSE;
	}
	allocation->pcb = pcb;
	allocation->order = 0;
	allocation->req = FALSE;
	ArrayList_init(&(allocation->counter));
	AllocationCounter* counter;

	for (Int i = 0; i < banker->resource_count; i++) {
		counter = malloc(sizeof(AllocationCounter));
		if (NULL == counter) {
			return FALSE;
		}
		counter->max = 0;
		counter->had = 0;
		ArrayList_add(&(allocation->counter), allocation->counter.size, counter);
	}

	ArrayList_add(&(banker->allocations), banker->allocations.size, allocation);
	banker->process_count++;

	return TRUE;
}

Boolean DeadLock_removeProcess(Banker* banker, PCB* pcb)
{
	if (NULL == banker || NULL == pcb) {
		return FALSE;
	}
	//寻找进程行
	Allocation* allocation;
	Int i = 0;
	for (i = 0; i < banker->process_count; i++) {
		allocation = ArrayList_get(&(banker->allocations), i);
		assert(NULL != allocation);

		if (allocation->pcb == pcb) {
			break;
		}
	}
	//没找到进程行
	if (i >= banker->process_count) {
		return FALSE;
	}
	assert(banker->process_count > 0);

	//删除进程行
	allocation = ArrayList_remove(&(banker->allocations), i);
	assert(NULL != allocation);
	banker->process_count--;

	//销毁数据
	AllocationCounter* counter;
	for (Int i = 0; i < allocation->counter.size; i++) {
		counter = ArrayList_get(&(allocation->counter), i);
		assert(NULL != counter);
		free(counter);
	}
	ArrayList_destroy(&(allocation->counter));
	ArrayList_destroy(&(allocation->request));
	free(allocation);

	return TRUE;
	
}

Boolean DeadLock_setMax(Banker* banker, Int process_index, Int resource_index, Int max)
{
	if (NULL == banker || process_index < 0 || process_index >= banker->process_count
		|| resource_index < 0 || resource_index >= banker->resource_count || max < 0) {
		return FALSE;
	}

	Allocation* allocation = ArrayList_get(&(banker->allocations), process_index);
	assert(NULL != allocation);

	AllocationCounter* counter = ArrayList_get(&(allocation->counter), resource_index);
	assert(NULL != counter);

	counter->max = max;
	return TRUE;
}

Boolean DeadLock_initEmptyRequest(Banker* banker, ArrayList* request)
{
	if (NULL == banker || NULL == request) {
		return FALSE;
	}
	if (FALSE == ArrayList_initCapacity(request, banker->resource_count)) {
		return FALSE;
	}
	request->size = banker->resource_count;

	Boolean success;
	for (Int i = 0; i < banker->resource_count; i++) {
		success = DeadLock_setRequest(request, i, 0);
		assert(TRUE == success);
	}

	return TRUE;
}

Boolean DeadLock_setRequest(ArrayList* request, Int resource_index, Int count) {
	if (NULL == request || resource_index < 0 || resource_index >= request->size || count < 0) {
		return FALSE;
	}

	*(request->elements + resource_index) = (void*)count;
	return TRUE;
}

Int DeadLock_getRequest(ArrayList* request, Int resource_index) {
	if (NULL == request || resource_index < 0 || resource_index >= request->size ) {
		return -1;
	}

	return (Int)*(request->elements + resource_index);
}

Int DeadLock_indexOf(Banker* banker, PCB* pcb)
{
	if (NULL == banker || NULL == pcb) {
		return -1;
	}
	Allocation* allocation;
	for (Int i = 0; i < banker->process_count; i++) {
		allocation = ArrayList_get(&(banker->allocations), i);
		assert(NULL != allocation);

		if (pcb == allocation->pcb) {
			return i;
		}
	}

	return -1;
}

Boolean DeadLock_releaseIfFull(Banker* banker, Int index)
{
	if (NULL == banker || index <0 || index >banker->process_count) {
		return FALSE;
	}
	Allocation* allocation = ArrayList_get(&(banker->allocations), index);
	assert(NULL != allocation);

	AllocationCounter* counter;
	Resource* resource;
	//检查资源
	for (Int i = 0; i < allocation->counter.size; i++) {
		counter = ArrayList_get(&(allocation->counter), i);
		resource = ArrayList_get(&(banker->resources), i);
		assert(NULL != counter && NULL != resource);

		if (counter->had < counter->max) {
			return FALSE;
		}
		assert(counter->had == counter->max);
	}
	//释放资源
	for (Int i = 0; i < allocation->counter.size; i++) {
		counter = ArrayList_get(&(allocation->counter), i);
		resource = ArrayList_get(&(banker->resources), i);
		
		resource->count += counter->had;
		counter->had = 0;
	}
	return TRUE;
}


/*若能分配资源，则释放已经分配的资源给available
为了在后续操作中恢复原样，不改变allocation.had, 以便后续算法恢复原有的资源分配情况。*/
Boolean DeadLock_security_releaseIfEnable(Banker* banker, Int index) {
	if (NULL == banker || index < 0 || index >= banker->process_count) {
		return FALSE;
	}
	Allocation* allocation = ArrayList_get(&(banker->allocations), index);
	if (NULL == allocation) {
		return FALSE;
	}

	//释放资源
	Int i = 0;
	Boolean success = TRUE;
	Resource* resource;
	AllocationCounter* counter;
	Int max = 0;
	Int had = 0;
	for (; i < banker->resource_count; i++) {
		resource = ArrayList_get(&(banker->resources), i);
		assert(NULL != resource);

		counter = ArrayList_get(&(allocation->counter), i);
		assert(counter != NULL);

		max = counter->max;
		had = counter->had;

		if (max < 0 || had < 0) {//断言
			success = FALSE;
			break;
		}
		//可分配，则可释放
		if (max <= had + resource->count) {
			resource->count += had;
		}
		else {
			success = FALSE;
			break;
		}
	}

	//恢复分配
	if (FALSE == success) {
		i = i - 1;
		for (; i > -1; i--) {
			resource = ArrayList_get(&(banker->resources), i);
			counter = ArrayList_get(&(allocation->counter), i);
			assert(counter != NULL);
			had = counter->had;
			//恢复分配
			resource->count -= had;
		}
	}

	return success;
}

Boolean DeadLock_security_recoverRelease(Banker* banker, Int index) {
	if (NULL == banker || index < 0 || index >= banker->process_count) {
		return FALSE;
	}
	//获得分配行
	Allocation* allocation = ArrayList_get(&(banker->allocations), index);
	if (NULL == allocation) {//断言
		return FALSE;
	}
	//恢复分配，即再分配
	Resource* resource;
	AllocationCounter* counter;
	Int had = 0;
	for (Int i = 0; i < banker->resource_count; i++) {
		resource = ArrayList_get(&(banker->resources), i);
		if (NULL == resource) {//断言
			return FALSE;
		}
		counter = ArrayList_get(&(allocation->counter), i);
		assert(NULL != counter);
		//恢复分配
		resource->count -= counter->had;
	}

	return TRUE;
}

Boolean DeadLock_Banker(Banker* banker, Int index, ArrayList* request)
{
	if (NULL == banker || index < 0 || index >= banker->process_count || NULL == request) {
		return FALSE;
	}

	Allocation* allocation = ArrayList_get(&(banker->allocations), index);
	if (NULL == allocation) {
		return FALSE;
	}

	Int max = 0;
	Int had = 0;
	Int req = 0;
	Resource* resource;
	AllocationCounter* counter;
	for (Int i = 0; i < banker->resource_count; i++) {
		//检查，request[i]<=need[i] = max-had
		req = (Int)ArrayList_get(request, i);
		counter = ArrayList_get(&(allocation->counter), i);
	
		if (counter->had + req > counter->max) {
			return FALSE;
		}

		//检查request[i]<=available[i]
		resource = ArrayList_get(&(banker->resources), i);
		if (NULL == resource) {
			return FALSE;
		}
		if (req > resource->count) {
			return FALSE;
		}
	}

	//分配资源
	Int i = 0;
	
	for (; i < banker->resource_count; i++) {
		resource = ArrayList_get(&(banker->resources), i);
		assert(NULL != resource);
		
		req = (Int)ArrayList_get(request, i);
		//分配
		resource->count -= req;
		counter = ArrayList_get(&(allocation->counter), i);
		counter->had += req;
	}

	//成功分配且安全，则返回true.
	if (TRUE == DeadLock_security(banker)) {
		return TRUE;
	}//恢复分配前
	else {
		i = i - 1;
		for (; i > -1; i--) {
			resource = ArrayList_get(&(banker->resources), i);
			if (NULL == resource) {
				break;
			}
			req = (Int)ArrayList_get(request, i);
			//恢复分配
			resource->count += req;
			counter = ArrayList_get(&(allocation->counter), i);
			counter->had -=req;
		}
		return FALSE;
	}
}

Boolean DeadLock_security(Banker* banker)
{
	if (NULL == banker) {
		return FALSE;
	}
	
	Int order = 0;//指示一个进程在安全序列的位置
	Allocation* allocation;//指示一个进程及其分配

	//初始化控制信息
	for (Int i = 0; i < banker->process_count; i++) {
		allocation = ArrayList_get(&(banker->allocations), i);
		allocation->order = 0;
	}

	Int i = 0;
	while (TRUE == TRUE) {//直到找到安全序列的最后一个进程。
		//找到一段安全序列
		for (; i < banker->process_count;) {
			allocation = ArrayList_get(&(banker->allocations), i);
			//已经被计入安全序列
			if (0 != allocation->order) {
				i++;//计算下一个进程。
			}
			else {
				//计入安全序列
				if (TRUE == DeadLock_security_releaseIfEnable(banker, i)) {
					order++;
					allocation->order = order;
					//查找下一个安全序列进程。
					i = 0;
				}
				else {
					i++;//计算下一个进程。
				}
			}
		}

		//没有安全序列
		if (0 == order) {
			return FALSE;
		}//找到安全序列
		else if(order == banker->process_count){
			//恢复所有分配
			for (Int j = 0; j < banker->process_count; j++) {
				DeadLock_security_recoverRelease(banker, j);
			}
			return TRUE;
		}//安全序列不完整，则恢复最后一次分配，重新寻找新的安全序列。
		else {
			for (Int j = 0; j < banker->process_count; j++) {
				allocation = ArrayList_get(&(banker->allocations), j);
				if (allocation->order == order) {
					allocation->order = 0;
					order--;
					//恢复分配
					DeadLock_security_recoverRelease(banker, j);
					//从下一个进程重新开始计算
					i = j + 1;
					break;
				}//end if
			}//end for
		}//end else
	}
}

void DeadLock_printf(Banker* banker)
{
	if (NULL == banker) {
		return;
	}
	Resource* resource;
	printf_s("|res:%2d", banker->resource_count);
	printf_s(", proce:%2d|", banker->process_count);
	for (Int i = 0; i < banker->resource_count; i++) {
		resource = ArrayList_get(&(banker->resources), i);
		assert(NULL != resource);
		printf_s(",|name=%1s,count=%2d|", resource->name,resource->count);
	}
	printf_s("\n");


	Allocation* allocation;
	AllocationCounter* counter;
	for (Int i = 0; i < banker->process_count; i++) {
		allocation = ArrayList_get(&(banker->allocations), i);
		assert(NULL != allocation);

		printf_s("|PCB:%2d", allocation->pcb->id);
		printf_s(", order:%2d|", allocation->order);
		for (Int j = 0; j < banker->resource_count; j++) {
			counter = ArrayList_get(&(allocation->counter), j);
			assert(NULL != allocation);

			printf_s(",| max=%2d, had=%2d|", counter->max, counter->had);
		}
		printf_s("\n");
	}
}

void DeadLock_run(Int count) {
	Boolean success;
	PCB* pcb;
	PM pm;
	PM_init(&pm);

	Banker banker;
	DeadLock_init(&banker);
	DeadLock_addResource(&banker, "A", 10);
	DeadLock_addResource(&banker, "B", 15);
	DeadLock_addResource(&banker, "C", 12);

	Int index = 0;
	Allocation* allocation;
	AllocationCounter* counter;
	Resource* resource;
	Int max = 0;
	//生成随机进程
	for (Int i = 0; i < count; i++) {
		pcb = PM_apply(&pm);
		assert(NULL != pcb);
		//加入银行家
		DeadLock_addProcess(&banker, pcb);
		//设置资源最大值
		for (Int j = 0; j < banker.resource_count; j++) {
			resource = ArrayList_get(&(banker.resources), j);
			assert(NULL != resource);

			max = resource->count;
			max = rand() % (max + 1);

			DeadLock_setMax(&banker, i, j, max);
		}
		//激活进程
		PM_active(&pm, pcb);
	}
	//调度第一个进程
	pcb = RR(&(pm.runnable));
	PM_setRun(&pm, pcb);
	assert(NULL != pcb);
	//打印初始状态
	printf_s("========================================\n");
	printf_s("时间：%-4"PRId64", 运行：%-4d\n", pm.time, 0);
	PM_print(&pm);
	printf_s("分配矩阵：\n");
	DeadLock_printf(&banker);

	Int i = 0;
	Int req = 0;
	Long now_time = 0;
	do {
		pcb = pm.run;
		//随机申请资源
		i = DeadLock_indexOf(&banker, pcb);
		allocation = ArrayList_get(&(banker.allocations), i);
		assert(NULL != allocation);
		if (FALSE == allocation->req) {
			success = DeadLock_initEmptyRequest(&banker, &(allocation->request));
			assert(FALSE != success);

			for (Int j = 0; j < banker.resource_count; j++) {
				counter = ArrayList_get(&(allocation->counter), j);
				req = counter->max;
				req -= counter->had;
				req = rand() % (req + 1);

				DeadLock_setRequest(&(allocation->request), j, req);
			}

			allocation->req = TRUE;
		}

		//运行一个时间片来调度银行家算法。
		now_time++;
		PM_updateTime(&pm, now_time);

		//银行家算法
		if (TRUE == DeadLock_Banker(&banker, i, &(allocation->request))) {
			//重置请求
			allocation->req = FALSE;
			ArrayList_destroy(&(allocation->request));

			//释放阻塞进程
			while (pm.blocked.size > 0) {
				pcb = ArrayList_get(&(pm.blocked), 0);
				PM_active(&pm, pcb);
			}

			//终结判断
			if (TRUE == DeadLock_releaseIfFull(&banker, i)) {
				pcb = pm.run;
				PM_removeRun(&pm);
				PM_terminate(&pm, pcb);
				DeadLock_removeProcess(&banker, pcb);
			}
			else {
				PM_removeRun(&pm);
			}
		}//阻塞
		else {
			PM_removeRun(&pm);
			PM_block(&pm, allocation->pcb);
		}

		//调度新进程
		pcb = RR(&(pm.runnable));
		if (NULL != pcb) {
			PM_setRun(&pm, pcb);
		}

		//打印
		//银行家算法运行了一个时间片
		printf_s("========================================\n");
		printf_s("时间：%-4"PRId64", 运行：%-4d\n", pm.time, 1);
		PM_print(&pm);

		printf_s("分配矩阵：\n");
		DeadLock_printf(&banker);

		system("pause");

	} while (NULL != pm.run);
}

void DeadLock_show()
{
	printf_s("程序简介：\n");
	printf_s("	3118005434， 钟景文，2018级别信息安全2班，计算机学院，广东工业大学\n");
	printf_s("	每次运行一个时间片后调度，将此时的队列信息打印出来。\n");
	printf_s("	将此时的分配矩阵打印处理。order是上一次调度时银行家算法算出的一个安全序列。\n");
	printf_s("输入随机进程数量\n");
	printf_s("输入：");
	Int count = 3;
	scanf_s("%d", &count);

	srand((unsigned int)time(NULL));
	DeadLock_run(count);
}
