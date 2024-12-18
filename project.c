#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include<stdlib.h>
#include<time.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#include "asm-arm/arch-pxa/lib/def.h"
#include "creator_lib.h"

#define SIZE 4
#define width 4
#define height 5
#define LCD_MAX_HEIGHT 4 // LCD 的行數

typedef struct point {
    int x, y; // cube mid point
} point;

point cube[SIZE][SIZE]; // cube x,y point
point number[SIZE][SIZE]; // generate number x,y point
lcd_write_info_t display;
int ret;

void init() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            cube[i][j].x = j*weidth;
            cube[i][j].y = i * height;
            number[i][j].x = j*weidth+1;
            number[i][j].y = j*weidth;
        }
    }
}

void show_game(int fd) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            LCD_Cursor(fd, cube[i][j].x, cube[i][j].y);
            display.Count = sprintf((char*)display.Msg, "[ ]");
            ret = ioctl(fd,LCD_IOCTL_WRITE,&display);
        }
    }
}

int delay_time = 4000;
void generate_number(int fd){
    time_t t;
    srand((unsigned) time(&t));
    int num = rand()%9;
    int i = rand()%SIZE;
    int j = rand()%SIZE;
    LCD_Cursor(fd, number[i][j].x, number[i][j].y);
    display.Count = sprintf((char*)display.Msg, (char)num);
    ret = ioctl(fd,LCD_IOCTL_WRITE,&display);
    Delay(delay_time);
    LCD_printf("");

}


int main() {
    int fd;
    // open file
    fd = open("/dev/lcd", O_RDWR);
    if (fd < 0) {
        printf("open /dev/lcd error\n");
        return -1;
    }

    LCD_SetHandle(fd);
    LCD_ClearScreen();

    init();         // 
    show_game(fd);  // display cube

    // main loop
    while (1) {
        generate_number(fd);
    }

    close(fd);
    return 0;
}
