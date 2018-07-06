#include "common.h"

void Init_main_win(struct PmPanel* win, int max_y, int max_x, 
													int x_coor)  
{
	win->w_half = newwin(max_y - 1, max_x/2, 0, x_coor);
	wbkgd(win->w_half, COLOR_PAIR(1));
	box(win->w_half, '|', '-');

	win->w_dir = derwin(win->w_half, 1, max_x/2, 0, 0);
	wbkgd(win->w_dir, COLOR_PAIR(1));
}