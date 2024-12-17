#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
#include "asm-arm/arch-pxa/lib/def.h"
#include "creator_lib.h"

#define SIZE 4
#define LCD_MAX_WIDTH 16 // LCD 的列數（假設解析度為 16x2）
#define LCD_MAX_HEIGHT 4 // LCD 的行數

typedef struct point {
    int x, y; // 方塊中心座標
} point;

point cube[SIZE][SIZE]; // 儲存方塊中心位置
int width = 16 / SIZE;  // 計算每個方塊的寬度
int height = 4 / SIZE;  // 計算每個方塊的高度

void init() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            cube[i][j].x = j * width + width / 2;
            cube[i][j].y = i * height + height / 2;
        }
    }
}

void show_game(int fd) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            // 移動到方塊的中心座標
            LCD_Cursor(fd, cube[i][j].x, cube[i][j].y);
            LCD_put_string(fd, "[ ]"); // 繪製方塊
        }
    }
}

int main() {
    int fd;
    // 開啟 LCD 設備
    fd = open("/dev/lcd", O_RDWR);
    if (fd < 0) {
        printf("open /dev/lcd error\n");
        return -1;
    }

    LCD_SetHandle(fd);
    LCD_ClearScreen();

    init();         // 初始化方塊中心座標
    show_game(fd);  // 顯示方塊

    // 保持程式運行（可選）
    while (1) {
        sleep(1);
    }

    close(fd);
    return 0;
}
