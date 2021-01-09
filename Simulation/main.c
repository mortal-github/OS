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
	Schedule_show();//进程调度演示
	//DeadLock_show();//银行家算法演示
	//DynamicPartition_show();//动态分区算法演示
	//DiskSchedule_show();//磁盘调度演示
	//Computer_simulation();//课程设计
	system("pause");
}

void Computer_simulation() {
	printf_s("程序简介：课程设计————模拟操作系统\n");
	printf_s("	3118005434， 钟景文，2018级别信息安全2班，计算机学院，广东工业大学\n");
	printf_s("1. 提交一批作业（>=10），按先来先服选择一部分作业（最多5个）进入内存\n");
	printf_s("2. 为每个作业创建一个进程，并分配内存（用户内存：0—1024K，采用可变连续分配方式\n");
	printf_s("3. 进程调度功能（时间片轮转）\n");
	printf_s("4. 随机阻塞进程，并在一段时间后唤醒进程（选做）\n");
	printf_s("5. 显示相关信息：后备作业队列、内存分配情况、进程信息、完成作业情况\n");

	Int job = 0;
	printf_s("请输入后备作业数量：");
	scanf_s("%d", &job);
	Int total = job;
	//进程管理
	PM pm;
	PM_init(&pm);
	//储存管理
	LinkList ram;
	Int size = 1024 * 1024;
	//时间
	DynamicPartition_init(&ram, size);
	printf_s("\n\n");
	do {
		//1 .提交一批作业（ >= 10），按先来先服选择一部分作业（最多5个）进入内存。
		while(pm.runnable.size + pm.blocked.size < 5) {
			job--;
			PCB* pcb = PM_apply(&pm);
			PM_active(&pm, pcb);
		//2. 为每个作业创建一个进程，并分配内存（用户内存：0—1024K，采用可变连续分配方式\n
			Int need = 512 + rand() % (size / 5);
			pcb->size = need;
			Pointer address = DynamicPartition_mallocFirst(&ram, need);
			if (NULL == address) {
				PM_block(&pm, pcb);//分配不到内存故阻塞。
			}
			else {
				pcb->ram = address;
			}
		}
		//3. 进程调度功能（时间片轮转）
		PCB* run = RR(&(pm.runnable));
		PM_setRun(&pm, run);


		//5. 显示相关信息：后备作业队列、内存分配情况、进程信息、完成作业情况
		printf_s("**********************************************************************************\n");
		printf_s("现在的时间：%lld\n", pm.time);
		printf_s("后备队列尚有 %d 个作业，已经完成 %d 个作业\n", job, pm.terminated.size);
		printf_s("内存分配请况：进程内存请看进程信息的PCB.ram(内存始址),PCB.size(内存大小)\n");
		printf_s("=================================\n");
		printf_s("空闲分区链表：\n");
		DynamicPartition_print(&ram);
		printf_s("=================================\n");
		printf_s("进程信息：\n");
		PM_print(&pm);
		printf_s("\n");
		system("pause");//暂停


		//4. 随机阻塞进程，并在一段时间后唤醒进程（选做)
		if (NULL != run) {
			run->run++;//运行了一个时间片
			pm.time++;
			Int wake = rand() % 100;
			if (wake < 5) {//终止进程
				run->terminated = pm.time;
				PM_removeRun(&pm);
				PM_terminate(&pm, run);
				//释放内存
				DynamicPartition_freeFirst(&ram, run->ram);
				//为因为内存不足阻塞的进程分配内存。
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
			else if(wake < 50){//阻塞进程
				run->wake = pm.time + wake/20;
				PM_removeRun(&pm);
				PM_block(&pm, run);
			}
			else {//移入就绪队列
				PM_removeRun(&pm);
			}
		}
		for (Int i = 0; i < pm.blocked.size; i++) {//唤醒进程。
			PCB* pcb = ArrayList_get(&(pm.blocked), i);
			if (pcb->wake <= pm.time && NULL != pcb->ram) {
				pcb->wake = 0;
				PM_active(&pm, pcb);
				i--;
			}
		}
	} while (0 != job && 0 != pm.runnable.size + pm.blocked.size);//没有作业且没有可运行进程时候结束模拟	
}