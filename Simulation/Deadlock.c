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
	//�����Դ�ڵ�
	Resource* resource = malloc(sizeof(Resource));
	if (NULL == resource) {
		return FALSE;
	}
	resource->name = name;
	resource->count = count;
	//�����Դ
	if (FALSE == ArrayList_add(&(banker->resources), banker->resources.size, resource)) {
		free(resource);
		return FALSE;
	}
	banker->resource_count++;
	//���ӷ��������
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
	//Ѱ�ҽ�����
	Allocation* allocation;
	Int i = 0;
	for (i = 0; i < banker->process_count; i++) {
		allocation = ArrayList_get(&(banker->allocations), i);
		assert(NULL != allocation);

		if (allocation->pcb == pcb) {
			break;
		}
	}
	//û�ҵ�������
	if (i >= banker->process_count) {
		return FALSE;
	}
	assert(banker->process_count > 0);

	//ɾ��������
	allocation = ArrayList_remove(&(banker->allocations), i);
	assert(NULL != allocation);
	banker->process_count--;

	//��������
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
	//�����Դ
	for (Int i = 0; i < allocation->counter.size; i++) {
		counter = ArrayList_get(&(allocation->counter), i);
		resource = ArrayList_get(&(banker->resources), i);
		assert(NULL != counter && NULL != resource);

		if (counter->had < counter->max) {
			return FALSE;
		}
		assert(counter->had == counter->max);
	}
	//�ͷ���Դ
	for (Int i = 0; i < allocation->counter.size; i++) {
		counter = ArrayList_get(&(allocation->counter), i);
		resource = ArrayList_get(&(banker->resources), i);
		
		resource->count += counter->had;
		counter->had = 0;
	}
	return TRUE;
}


/*���ܷ�����Դ�����ͷ��Ѿ��������Դ��available
Ϊ���ں��������лָ�ԭ�������ı�allocation.had, �Ա�����㷨�ָ�ԭ�е���Դ���������*/
Boolean DeadLock_security_releaseIfEnable(Banker* banker, Int index) {
	if (NULL == banker || index < 0 || index >= banker->process_count) {
		return FALSE;
	}
	Allocation* allocation = ArrayList_get(&(banker->allocations), index);
	if (NULL == allocation) {
		return FALSE;
	}

	//�ͷ���Դ
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

		if (max < 0 || had < 0) {//����
			success = FALSE;
			break;
		}
		//�ɷ��䣬����ͷ�
		if (max <= had + resource->count) {
			resource->count += had;
		}
		else {
			success = FALSE;
			break;
		}
	}

	//�ָ�����
	if (FALSE == success) {
		i = i - 1;
		for (; i > -1; i--) {
			resource = ArrayList_get(&(banker->resources), i);
			counter = ArrayList_get(&(allocation->counter), i);
			assert(counter != NULL);
			had = counter->had;
			//�ָ�����
			resource->count -= had;
		}
	}

	return success;
}

Boolean DeadLock_security_recoverRelease(Banker* banker, Int index) {
	if (NULL == banker || index < 0 || index >= banker->process_count) {
		return FALSE;
	}
	//��÷�����
	Allocation* allocation = ArrayList_get(&(banker->allocations), index);
	if (NULL == allocation) {//����
		return FALSE;
	}
	//�ָ����䣬���ٷ���
	Resource* resource;
	AllocationCounter* counter;
	Int had = 0;
	for (Int i = 0; i < banker->resource_count; i++) {
		resource = ArrayList_get(&(banker->resources), i);
		if (NULL == resource) {//����
			return FALSE;
		}
		counter = ArrayList_get(&(allocation->counter), i);
		assert(NULL != counter);
		//�ָ�����
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
		//��飬request[i]<=need[i] = max-had
		req = (Int)ArrayList_get(request, i);
		counter = ArrayList_get(&(allocation->counter), i);
	
		if (counter->had + req > counter->max) {
			return FALSE;
		}

		//���request[i]<=available[i]
		resource = ArrayList_get(&(banker->resources), i);
		if (NULL == resource) {
			return FALSE;
		}
		if (req > resource->count) {
			return FALSE;
		}
	}

	//������Դ
	Int i = 0;
	
	for (; i < banker->resource_count; i++) {
		resource = ArrayList_get(&(banker->resources), i);
		assert(NULL != resource);
		
		req = (Int)ArrayList_get(request, i);
		//����
		resource->count -= req;
		counter = ArrayList_get(&(allocation->counter), i);
		counter->had += req;
	}

	//�ɹ������Ұ�ȫ���򷵻�true.
	if (TRUE == DeadLock_security(banker)) {
		return TRUE;
	}//�ָ�����ǰ
	else {
		i = i - 1;
		for (; i > -1; i--) {
			resource = ArrayList_get(&(banker->resources), i);
			if (NULL == resource) {
				break;
			}
			req = (Int)ArrayList_get(request, i);
			//�ָ�����
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
	
	Int order = 0;//ָʾһ�������ڰ�ȫ���е�λ��
	Allocation* allocation;//ָʾһ�����̼������

	//��ʼ��������Ϣ
	for (Int i = 0; i < banker->process_count; i++) {
		allocation = ArrayList_get(&(banker->allocations), i);
		allocation->order = 0;
	}

	Int i = 0;
	while (TRUE == TRUE) {//ֱ���ҵ���ȫ���е����һ�����̡�
		//�ҵ�һ�ΰ�ȫ����
		for (; i < banker->process_count;) {
			allocation = ArrayList_get(&(banker->allocations), i);
			//�Ѿ������밲ȫ����
			if (0 != allocation->order) {
				i++;//������һ�����̡�
			}
			else {
				//���밲ȫ����
				if (TRUE == DeadLock_security_releaseIfEnable(banker, i)) {
					order++;
					allocation->order = order;
					//������һ����ȫ���н��̡�
					i = 0;
				}
				else {
					i++;//������һ�����̡�
				}
			}
		}

		//û�а�ȫ����
		if (0 == order) {
			return FALSE;
		}//�ҵ���ȫ����
		else if(order == banker->process_count){
			//�ָ����з���
			for (Int j = 0; j < banker->process_count; j++) {
				DeadLock_security_recoverRelease(banker, j);
			}
			return TRUE;
		}//��ȫ���в���������ָ����һ�η��䣬����Ѱ���µİ�ȫ���С�
		else {
			for (Int j = 0; j < banker->process_count; j++) {
				allocation = ArrayList_get(&(banker->allocations), j);
				if (allocation->order == order) {
					allocation->order = 0;
					order--;
					//�ָ�����
					DeadLock_security_recoverRelease(banker, j);
					//����һ���������¿�ʼ����
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
	//�����������
	for (Int i = 0; i < count; i++) {
		pcb = PM_apply(&pm);
		assert(NULL != pcb);
		//�������м�
		DeadLock_addProcess(&banker, pcb);
		//������Դ���ֵ
		for (Int j = 0; j < banker.resource_count; j++) {
			resource = ArrayList_get(&(banker.resources), j);
			assert(NULL != resource);

			max = resource->count;
			max = rand() % (max + 1);

			DeadLock_setMax(&banker, i, j, max);
		}
		//�������
		PM_active(&pm, pcb);
	}
	//���ȵ�һ������
	pcb = RR(&(pm.runnable));
	PM_setRun(&pm, pcb);
	assert(NULL != pcb);
	//��ӡ��ʼ״̬
	printf_s("========================================\n");
	printf_s("ʱ�䣺%-4"PRId64", ���У�%-4d\n", pm.time, 0);
	PM_print(&pm);
	printf_s("�������\n");
	DeadLock_printf(&banker);

	Int i = 0;
	Int req = 0;
	Long now_time = 0;
	do {
		pcb = pm.run;
		//���������Դ
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

		//����һ��ʱ��Ƭ���������м��㷨��
		now_time++;
		PM_updateTime(&pm, now_time);

		//���м��㷨
		if (TRUE == DeadLock_Banker(&banker, i, &(allocation->request))) {
			//��������
			allocation->req = FALSE;
			ArrayList_destroy(&(allocation->request));

			//�ͷ���������
			while (pm.blocked.size > 0) {
				pcb = ArrayList_get(&(pm.blocked), 0);
				PM_active(&pm, pcb);
			}

			//�ս��ж�
			if (TRUE == DeadLock_releaseIfFull(&banker, i)) {
				pcb = pm.run;
				PM_removeRun(&pm);
				PM_terminate(&pm, pcb);
				DeadLock_removeProcess(&banker, pcb);
			}
			else {
				PM_removeRun(&pm);
			}
		}//����
		else {
			PM_removeRun(&pm);
			PM_block(&pm, allocation->pcb);
		}

		//�����½���
		pcb = RR(&(pm.runnable));
		if (NULL != pcb) {
			PM_setRun(&pm, pcb);
		}

		//��ӡ
		//���м��㷨������һ��ʱ��Ƭ
		printf_s("========================================\n");
		printf_s("ʱ�䣺%-4"PRId64", ���У�%-4d\n", pm.time, 1);
		PM_print(&pm);

		printf_s("�������\n");
		DeadLock_printf(&banker);

		system("pause");

	} while (NULL != pm.run);
}

void DeadLock_show()
{
	printf_s("�����飺\n");
	printf_s("	3118005434�� �Ӿ��ģ�2018������Ϣ��ȫ2�࣬�����ѧԺ���㶫��ҵ��ѧ\n");
	printf_s("	ÿ������һ��ʱ��Ƭ����ȣ�����ʱ�Ķ�����Ϣ��ӡ������\n");
	printf_s("	����ʱ�ķ�������ӡ����order����һ�ε���ʱ���м��㷨�����һ����ȫ���С�\n");
	printf_s("���������������\n");
	printf_s("���룺");
	Int count = 3;
	scanf_s("%d", &count);

	srand((unsigned int)time(NULL));
	DeadLock_run(count);
}
