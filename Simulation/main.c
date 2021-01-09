#include "Primitive.h"
#include "DynamicPartition.h"
#include <stdio.h>

Boolean Int_print(Int* element) {
	if (NULL == element) {
		return FALSE;
	}
	printf_s(", %d", *element);
	return TRUE;
}

int main() {
	//Schedule_show();
	//DeadLock_show();
	DynamicPartition_show();
}
