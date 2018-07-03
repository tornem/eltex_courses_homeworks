#ifndef __MAIN__H__

#define __MAIN__H__

#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <dirent.h>

void Init_main_win(WINDOW** win_main_1, WINDOW** win_main_2, 
									  int max_y, int max_x);

int ListDir(char* dir, struct dirent*** name_list, int* cnt_dir);

void RenderingListDir(WINDOW** win, struct dirent** name_list, int cnt_dir);

#endif
