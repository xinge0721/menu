#ifndef __lian2_H
#define __lian2_H
#include <stdio.h>
#include <stdlib.h>

typedef struct menu{
    int line;
    int data;
    struct line* Break;
    struct menu* next; //ָ��ֱ�Ӻ��
    struct menu* prior;//ָ��ֱ��ǰ��

}mn;

typedef struct line {
    struct line* prior;//ָ��ֱ��ǰ��
    int line;
    int data;
    struct menu* up;
    struct line* next;//ָ��ֱ�Ӻ��
}line;

line* line_Init(void);
mn* menu_Init(void);
void line_tail(line* li, int x, int y);
void menu_tail(mn* pr, line* li, int x, int y);

#endif
