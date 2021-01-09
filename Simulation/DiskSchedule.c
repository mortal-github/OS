#include "DiskSchedule.h"
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include "Heap.h"

double DiskSchedule_FCFS(Int last, Int now, ArrayList* request)
{
    if (last <= 0 || now <= 0 || NULL == request) {
        return FALSE;
    }

    if (request->size == 0) {
        return 0;
    }
    assert(request->size > 0);
   
    Int next;
    Int total = 0;
    for (Int i = 0; i < request->size; i++) {
        next = (Int)ArrayList_get(request, i);
        if (next >= now) {
            total += next - now;
        }
        else {
            total += now - next;
        }
        now = next;
    }
    return (double)total / (double)request->size;
}

double DiskSchedule_SSTF(Int last, Int now, ArrayList* request)
{
    if (last <= 0 || now <= 0 || NULL == request) {
        return -1;
   }
    if (request->size == 0) {
        return 0;
    }
    Int next;
    Int min;
    Int distance = 0;
    Int distance2 = 0;
    Int total = 0;
    for (Int i = 0; i < request->size; i++) {
        min = i;
        next = (Int)ArrayList_get(request, i);
        distance = abs(next - now);
        //�ҵ��뵱ǰ��̵�����
        for (Int j = i+1; j < request->size; j++) {
            next = (Int)ArrayList_get(request, j);
            distance2 = abs(next - now);
            if (distance2 < distance) {
                min = j;
                distance = distance2;
            }
        }
        next = (Int)ArrayList_remove(request, min);
        ArrayList_add(request, i, (Element)next);
        now = next;
        total += distance;
    }
    
    return (double)total / (double)request->size;
}

Int DiskSchedule_compare(Element left, Element right) {
    if (left < right) {
        return -1;
    }
    else if (left > right) {
        return 1;
    }
    else {
        return 0;
    }
}

double DiskSchedule_SCAN(Int last, Int now, ArrayList* request)
{
    if (last <= 0 || now <= 0 || last == now || NULL == request) {
        return -1;
    }
    Int tag;
    if (last > now) {//����
        tag = -1;
    }
    else {//����
        tag = 1;
    }
    //����
    Heap heap;
    Heap_init(&heap, request, tag, DiskSchedule_compare);
    Heap_adjust(&heap);
    Heap_sort(&heap);
    //�������ҵ����ߣ��������ҵ�С��
    Int i = 0;
    for (i; i < request->size; i++) {
        Int temp = (Int)ArrayList_get(request, i);
        if (tag == DiskSchedule_compare((Element)temp, (Element)now)) {
            break;
        }
    }
    //i֮ǰԪ�ز���size-j��λ�á�
    for (Int j = 0; j < i; j++) {
        Int temp = (Int)ArrayList_remove(request, 0);
        ArrayList_add(request, request->size - j, (Element)temp);
    }
    //����Ѱ������
    Int distance = 0;
    for (Int k = 0; k < request->size; k++) {
        Int temp = (Int)ArrayList_get(request, k);
        if (temp > now) {
            distance += temp - now;
        }
        else {
            distance += now - temp;
        }
        now = temp;
    }

    return (double)distance / (double)request->size;
}

double DiskSchedule_CSCAN(Int last, Int now, ArrayList* request)
{
    if (last <= 0 || now <= 0 || last == now || NULL == request) {
        return -1;
    }
    //����
    Heap heap;
    Heap_init(&heap, request, 1, DiskSchedule_compare);
    Heap_adjust(&heap);
    Heap_sort(&heap);
    //������һ��
    Int i = 0;
    for (i; i < request->size; i++) {
        Int temp = (Int)ArrayList_get(request, i);
        if (temp >= now) {
            break;
        }
    }
    //��i֮ǰ��ȫ���������
    for (Int j = 0; j < i; j++) {
        Int temp = (Int)ArrayList_remove(request, 0);
        ArrayList_add(request, request->size, (Element)temp);
    }
    //����Ѱ������
    Int distance = 0;
    for (Int k = 0; k < request->size; k++) {
        Int temp = (Int)ArrayList_get(request, k);
        if (temp > now) {
            distance += temp - now;
        }
        else {
            distance += now - temp;
        }
        now = temp;
    }

    return (double)distance / (double)request->size;
}

void DiskSchedule_test()
{
    ArrayList list;
    ArrayList_init(&list);
    Int last = 99;
    Int now = 100;
    ArrayList_add(&list, list.size, (Element)55);
    ArrayList_add(&list, list.size, (Element)58);
    ArrayList_add(&list, list.size, (Element)39);
    ArrayList_add(&list, list.size, (Element)18);
    ArrayList_add(&list, list.size, (Element)90);
    ArrayList_add(&list, list.size, (Element)160);
    ArrayList_add(&list, list.size, (Element)150);
    ArrayList_add(&list, list.size, (Element)38);
    ArrayList_add(&list, list.size, (Element)184);

    double avg = DiskSchedule_SCAN(now, last, &list);
    printf_s("avg = %f", avg);
    for (Int i = 0; i < list.size; i++) {
        Int temp = (Int)ArrayList_get(&list, i);
        printf_s(", %d", temp);
    }
}

Boolean DiskSchedule_show_print(Element element) {
    printf_s(", %d", (Int)element);
    return TRUE;
}
void DiskSchedule_show_clone(ArrayList* origin, ArrayList* copy) {
    copy->size = 0;
    for (Int i = 0; i < origin->size; i++) {
        Element element = ArrayList_get(origin, i);
        ArrayList_add(copy, copy->size, element);
    }
}
void DiskSchedule_show()
{
    printf_s("�����飺\n");
    printf_s("	3118005434�� �Ӿ��ģ�2018������Ϣ��ȫ2�࣬�����ѧԺ���㶫��ҵ��ѧ\n");
    printf_s("	������ִ��̵����㷨��\n");
    printf_s("1 �������ȷ����㷨\n");
    printf_s("2 : ���Ѱ���㷨\n");
    printf_s("3 : ɨ���㷨\n");
    printf_s("4 ��ѭ��ɨ���㷨\n");

    ArrayList request;
    ArrayList_init(&request);

    Int last = 0;
    Int now = 0;
    do {
        printf_s("�������ͷ��һ�����ڵ�λ�ã�");
        scanf_s("%d", &last);
        printf_s("�������ͷ��ǰ���ڵ�λ��  ��");
        scanf_s("%d", &now);
        if (last == now) {
            printf_s("����λ�ò�����ȣ�\n");
        }
        else {
            break;
        }
    } while (1 == 1);

    Int i = 1;
    Int req = 0;
    printf_s("�������������Ҫ�����ִ���0��0��ʾֹͣ����\n");
    do {
        printf_s("��% 2d������: ", i);
        scanf_s("%d", &req);
        if (0 == req) {
            break;
        }
        ArrayList_add(&request, request.size, (Element)req);
        i++;
    } while (1 == 1);

    double avg;
    ArrayList copy;
    ArrayList_init(&copy);
    //�����ȷ���
    DiskSchedule_show_clone(&request, &copy);
    avg  = DiskSchedule_FCFS(80, 100, &copy);
    printf_s("==============================\n");
    printf_s("�����ȷ���\n");
    printf_s("ƽ��Ѱ�����룺%f\n", avg);
    printf_s("Ѱ��˳��");
    ArrayList_forEach(&copy, DiskSchedule_show_print);
    printf_s("\n");

    //���Ѱ��ʱ�������㷨
    DiskSchedule_show_clone(&request, &copy);
    avg = DiskSchedule_SSTF(80, 100, &copy);
    printf_s("==============================\n");
    printf_s("���Ѱ��ʱ�������㷨��\n");
    printf_s("ƽ��Ѱ�����룺%f\n", avg);
    printf_s("Ѱ��˳��");
    ArrayList_forEach(&copy, DiskSchedule_show_print);
    printf_s("\n");
    

    //ɨ���㷨
    DiskSchedule_show_clone(&request, &copy);
    avg = DiskSchedule_SCAN(80, 100, &copy);
    printf_s("==============================\n");
    printf_s("ɨ���㷨��\n");
    printf_s("ƽ��Ѱ�����룺%f\n", avg);
    printf_s("Ѱ��˳��");
    ArrayList_forEach(&copy, DiskSchedule_show_print);
    printf_s("\n");


    //ѭ��ɨ���㷨
    DiskSchedule_show_clone(&request, &copy);
    avg = DiskSchedule_CSCAN(80, 100, &copy);
    printf_s("==============================\n");
    printf_s("ѭ��ɨ���㷨��\n");
    printf_s("ƽ��Ѱ�����룺%f\n", avg);
    printf_s("Ѱ��˳��");
    ArrayList_forEach(&copy, DiskSchedule_show_print);
    printf_s("\n");

    ArrayList_destroy(&request);
    ArrayList_destroy(&copy);
}
