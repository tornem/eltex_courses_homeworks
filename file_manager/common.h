#ifndef __MAIN__H__

#define __MAIN__H__

#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

struct PmPanel {
	
	WINDOW* w_half;
	WINDOW* w_dir;

	unsigned int y;
	unsigned int x;
	unsigned int current_obj;
	unsigned int list_begin;
	unsigned int list_end;

	unsigned int size;

	struct dirent** name_list;

	int dir_num; //number of directory entries

	char* current_dir;

	char* work_dir;

	bool active;

};

void Init_main_win(struct PmPanel* w_half, int max_y, int max_x, 
													int x_coor);

int ListDir (struct PmPanel* win, WINDOW* status);

void RenderingListDir (struct PmPanel* win);

void CoordControl (struct PmPanel* win);

int ScrOrDir (int scr, int num_dir);

#endif
