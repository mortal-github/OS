#ifndef _PRIMITIVE__H_
#define _PRIMITIVE__H_

#include <inttypes.h>

typedef int8_t Byte;
typedef int16_t Short;
typedef int32_t Int;
typedef int64_t Long;

typedef uint8_t UnsignedByte;
typedef uint16_t UnsignedShort;
typedef uint32_t UnsignedInt;
typedef uint64_t UnsignedLong;

typedef char Char;
typedef Int Boolean;

typedef void* Pointer;
typedef Pointer Element;

#define TRUE 1
#define FALSE 0

/*
 获得第index个元素的首地址,array是数组的首地址，unit数组元素的大小。
 输入条件：NULL!=array,unit>0, index>=0。
 正常处理：获得第index个元素的首地址。
 错误处理：若不满足输入条件，则返回NULL。
*/
Pointer Array_get(Pointer array, Int unit, Int index);
/*
 设置第index个元素,array是数组的首地址，unit数组元素的大小。
 输入条件：NULL!=array,unit>=0,index>=0,NULL!=element。
 正常处理：设置第index个元素为新值，element是新值内容的指针，返回TRUE。
 错误处理：若不满足输入条件，则返回FALSE。
*/
Boolean Array_set(Pointer array, Int unit, Int index, Pointer element);
/*
 把origin数组前length个元素复制到target数组的前length的位置，相当于跳跃式移动数据到新位置。
 若被复制空间与复制空间有所重叠的话，函数可以正确处理，保证复制后复制空间的值序列与复制前被复制空间的值序列相同。
 输入条件：NULL!=origin,NULL!=target,unit>0,length>0。
 正常处理：把origin数组前length个元素完整复制到target数组的前length的位置
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE。
*/
Boolean Array_jump(Pointer origin, Pointer target, Int unit, Int length);
/*
 遍历数组的每个元素，对每个元素调用Element_runable。
 输入条件：NULL!=array, unit>0, length>0,NULL!=Element_runable。
 正常处理：遍历数组的每个元素，对每个元素调用Element_runable，返回TRUE。
 错误处理：若不满足输入条件，则什么都不处理，返回FALSE。若调用Element_runable返回FALSE,停止遍历，返回FALSE。
*/
Boolean Array_forEach(Pointer array, Int unit, Int length, Boolean(*Element_runable)(Pointer element));

void Array_test();
#endif // !_PRIMITIVE__H_
