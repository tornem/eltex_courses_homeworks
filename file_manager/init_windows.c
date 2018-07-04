#include "common.h"

void Init_main_win(WINDOW** win_main_1, WINDOW** win_main_2, 
											int max_y, int max_x)  
{
	*win_main_1 = newwin(max_y - 1, max_x/2, 1, 0);
	wbkgd(*win_main_1, COLOR_PAIR(1));
	box(*win_main_1, '|', '-');
	*win_main_2 = newwin(max_y - 1, max_x/2, 1, max_x/2);
	wbkgd(*win_main_2, COLOR_PAIR(1));
	box(*win_main_2, '|', '-');
}