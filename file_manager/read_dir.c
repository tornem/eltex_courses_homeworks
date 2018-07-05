#define _DEFAULT_SOURCE
#include "common.h"

void ListDir (struct PmPanel* win)
{
	win->dir_num = scandir(win->start_dir, &(win->name_list), 
											NULL, alphasort);
		if (win->dir_num == -1) {
			perror("scandir");
			exit(EXIT_FAILURE);
		}

	win->y = 1;
	win->x = 1;
}

void RenderingListDir (struct PmPanel* win)
{	

	for (int cnt = 0; cnt < win->dir_num && cnt < 21; ++cnt) {
		mvwprintw(win->w_half, cnt+1, 1, "%s", win->name_list[cnt]->d_name);
	}
}