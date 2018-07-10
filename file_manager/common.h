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
	
	WINDOW* w_half;  // panel of window
	WINDOW* w_dir;  // top-side window showing current directory

	unsigned int y;  
	unsigned int x;
	unsigned int selected_obj;  // number of the select object
	unsigned int list_begin;  // up-border of visible screen
	unsigned int list_end;  // bottom border
	unsigned int dir_num; // number of directory entries
	unsigned int size;  // useful panel of window size

	struct dirent** name_list;  // list of object inside directory from scandir()

	char* work_dir;  // working directory for the process

	bool active;

};

void Init_main_win(struct PmPanel* w_half, int max_y, int max_x, 
													int x_coor);

int ListDir (struct PmPanel* win, WINDOW* status);

void RenderingListDir (struct PmPanel* win);

void CoordControl (struct PmPanel* win);

int ScrOrDir (int scr, int num_dir);

#endif
