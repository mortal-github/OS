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
	//Schedule_show();//���̵�����ʾ
	//DeadLock_show();//���м��㷨��ʾ
	//DynamicPartition_show();//��̬�����㷨��ʾ
	DiskSchedule_show();//���̵�����ʾ
}
