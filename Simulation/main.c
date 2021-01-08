#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <time.h>
#include "IndexList.h"
#include "DynamicPartition.h"
#include "ArrayList.h"

Boolean Int_print(Int* element) {
	if (NULL == element) {
		return FALSE;
	}
	printf_s(", %d", *element);
	return TRUE;
}

int main() {
	
}
