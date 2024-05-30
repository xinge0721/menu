#ifndef __lian2_H
#define __lian2_H
#include <stdio.h>
#include <stdlib.h>

typedef struct menu{

    int data;
    struct menu* naxt;
}mn;

typedef struct line {
    struct line* prior;//指向直接前趋
    int line;
	int data;
    struct menu* up;
    struct line* next;//指向直接后继
}line;

line* line_Init(void);
void line_tail(line* li,int x,int y);

#endif
