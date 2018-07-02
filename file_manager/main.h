#ifndef __MAIN__H__
#define __MAIN__H__

#include <curses.h>
//#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>


void init_main_win(WINDOW** win_main_1, WINDOW** win_main_2, 
											int max_y, int max_x);
#endif