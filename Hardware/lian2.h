#ifndef __lian2_H
#define __lian2_H
#include <stdio.h>
#include <stdlib.h>

typedef struct menu{
    int line;
    int data;
    struct line* Break;
    struct menu* next; //指向直接后继
    struct menu* prior;//指向直接前趋

}mn;

typedef struct line {
    struct line* prior;//指向直接前趋
    int line;
    int data;
    struct menu* up;
    struct line* next;//指向直接后继
}line;

line* line_Init(void);
mn* menu_Init(void);
void line_tail(line* li, int x, int y);
void menu_tail(mn* pr, line* li, int x, int y);

#endif
