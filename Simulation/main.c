#include "Primitive.h"
#include <stdio.h>
#include <stdlib.h>
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

int main() {
	//Schedule_show();//进程调度演示
	//DeadLock_show();//银行家算法演示
	//DynamicPartition_show();//动态分区算法演示
	DiskSchedule_show();//磁盘调度演示
}
