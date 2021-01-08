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
 ��õ�index��Ԫ�ص��׵�ַ,array��������׵�ַ��unit����Ԫ�صĴ�С��
 ����������NULL!=array,unit>0, index>=0��
 ����������õ�index��Ԫ�ص��׵�ַ��
 �������������������������򷵻�NULL��
*/
Pointer Array_get(Pointer array, Int unit, Int index);
/*
 ���õ�index��Ԫ��,array��������׵�ַ��unit����Ԫ�صĴ�С��
 ����������NULL!=array,unit>=0,index>=0,NULL!=element��
 �����������õ�index��Ԫ��Ϊ��ֵ��element����ֵ���ݵ�ָ�룬����TRUE��
 �������������������������򷵻�FALSE��
*/
Boolean Array_set(Pointer array, Int unit, Int index, Pointer element);
/*
 ��origin����ǰlength��Ԫ�ظ��Ƶ�target�����ǰlength��λ�ã��൱����Ծʽ�ƶ����ݵ���λ�á�
 �������ƿռ��븴�ƿռ������ص��Ļ�������������ȷ������֤���ƺ��ƿռ��ֵ�����븴��ǰ�����ƿռ��ֵ������ͬ��
 ����������NULL!=origin,NULL!=target,unit>0,length>0��
 ����������origin����ǰlength��Ԫ���������Ƶ�target�����ǰlength��λ��
 ��������������������������ʲô������������FALSE��
*/
Boolean Array_jump(Pointer origin, Pointer target, Int unit, Int length);
/*
 ���������ÿ��Ԫ�أ���ÿ��Ԫ�ص���Element_runable��
 ����������NULL!=array, unit>0, length>0,NULL!=Element_runable��
 �����������������ÿ��Ԫ�أ���ÿ��Ԫ�ص���Element_runable������TRUE��
 ��������������������������ʲô������������FALSE��������Element_runable����FALSE,ֹͣ����������FALSE��
*/
Boolean Array_forEach(Pointer array, Int unit, Int length, Boolean(*Element_runable)(Pointer element));

void Array_test();
#endif // !_PRIMITIVE__H_
