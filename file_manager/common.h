#ifndef __MAIN__H__

#define __MAIN__H__

#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <dirent.h>

struct PmPanel {
	
	WINDOW* w_half;

	unsigned int y;

	unsigned int x;

	struct dirent** name_list;

	int dir_num; //number of directory entries

	char* start_dir;

	bool active;

};

void Init_main_win(struct PmPanel* w_half, int max_y, int max_x, 
													int x_coor);

void ListDir (struct PmPanel* win);

void RenderingListDir (struct PmPanel* win);

#endif
