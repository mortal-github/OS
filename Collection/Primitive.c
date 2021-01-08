#include "Primitive.h"
#include <stdio.h>

Pointer Array_get(Pointer array, Int unit, Int index)
{
	if (NULL == array || unit <= 0 || index < 0) {
		return NULL;
	}
	Byte* address = (Byte*)array;
	address += unit * index;
	return address;
}

Boolean Array_set(Pointer array, Int unit, Int index, Pointer element)
{
	if (NULL == array || unit <=0 || index < 0 || NULL == element) {
		return FALSE;
	}

	Byte* target = (Byte*)Array_get(array, unit, index);
	Byte* origin = (Byte*)element;
	for (Int i = 0; i < unit; i++) {
		*(target + i) = *(origin + i);
	}

	return TRUE;
}

Boolean Array_jump(Pointer origin, Pointer target, Int unit, Int length)
{
	if (NULL == origin || NULL == target || unit <=0 || length <=0) {
		return FALSE;
	}
	if (origin == target) {
		return TRUE;
	}
	//¸´ÖÆ
	Pointer element;
	if (origin < target) {//ÌøÔ¾Ê½ÓÒÒÆ
		for (Int i = length - 1; i > -1; i--) {
			element = Array_get(origin, unit, i);
			Array_set(target, unit, i, element);
		}
	}
	else {//ÌøÔ¾Ê½×óÒÆ
		for (Int i = 0; i < length; i++) {
			element = Array_get(origin, unit, i);
			Array_set(target, unit, i, element);
		}
	}

	return TRUE;
}

Boolean Array_forEach(Pointer array, Int unit, Int length, Boolean(*Element_runable)(Pointer element))
{
	if (NULL == array || unit <= 0 || length <= 0 || NULL == Element_runable) {
		return FALSE;
	}

	Pointer element;
	Boolean success = TRUE;
	for (Int i = 0; i < length; i++) {
		element = Array_get(array, unit, i);
		success = Element_runable(element);
		if (FALSE == success) {
			break;
		}
	}

	return success;
}

Boolean Array_test_element_print_int_3118005434(Int* element) {
	if (NULL == element) {
		return FALSE;
	}
	printf_s(", %d", *element);
	return TRUE;
}

void Array_test()
{
	Int ints[10];
	Int other[10];
	Int unit = sizeof(Int);
	Int length = 10;
	Int value;

	Array_forEach(ints, unit, length, Array_test_element_print_int_3118005434);
	printf_s("\n\n");

	for (Int i = 0; i < length; i++) {
		value = i;
		Array_set(ints, unit, i, &value);
	}
	Array_forEach(ints, unit, length, Array_test_element_print_int_3118005434);
	printf_s("\n\n");

	Int origin_begin = 4;
	Int other_begin = 3;
	Int copy_length = origin_begin > other_begin ? origin_begin : other_begin;
	copy_length = length - copy_length;

	Array_jump(ints + origin_begin, other + other_begin, unit, copy_length);
	Array_forEach(ints, unit, length, Array_test_element_print_int_3118005434);
	printf_s("\n");
	Array_forEach(other, unit, length, Array_test_element_print_int_3118005434);
	printf_s("\n\n");


	origin_begin = 9;
	other_begin = 0;
	copy_length = origin_begin > other_begin ? origin_begin : other_begin;
	copy_length = length - copy_length;

	Array_jump(ints + origin_begin, ints + other_begin, unit, copy_length);
	Array_forEach(ints, unit, length, Array_test_element_print_int_3118005434);
	printf_s("\n");
}
