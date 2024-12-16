#include<signal.h>
#include<stdio.h>
#include<strings.h>
#include<fcntl.h>
#include<time.h>
#include<sys/ioctl.h>
#include<errno.h>

#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#include "asm-arm/arch-pxa/lib/def.h"
#include "creator_lib.h"
#define SIZE 4
typedef struct point{
    int x,y;
}point;
point cube[SIZE][SIZE];

void init(int fd){
    int i,j;
    int width = 10;
    int height = 4;
    for(i = 0 ; i<SIZE ; i++){
        for(j = 0 ; j<SIZE ; j++){
            cube[i][j].x = j*width+width/2;
            cube[i][j].y = i*height+height/2;
        }
    }

}


void show_game(int fd){
    int i,j;
    int width = 10;
    int height = 4;
    for(i = 0 ; i<SIZE ; i++){
        for(j = 0 ; j<SIZE ; j++){
            LCD_Cursor(fd, i*width, j*height);
            LCD_put_string(fd,"[  ]");
            cube[i][j].x = j*width+width/2;
            cube[i][j].y = i*height+height/2;
        }
    }

}

int main(){
    int fd;
    fd = open("/dev/lcd", O_RDWR);
    if(fd<0){
        printf("open /dev/lcd error\n");
        return (-1);
    }
    LCD_SetHandle(fd_lcd);
    LCD_ClearScreen(fd);
    init();
    show_game();
//    while(1){
//    
//    }
//
    close(fd);
    return (0);
}
