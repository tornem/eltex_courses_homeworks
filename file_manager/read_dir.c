#define _DEFAULT_SOURCE
#include "common.h"

void ListDir (char* dir_name, struct dirent*** name_list, int* cnt_dir)
{
	*cnt_dir = scandir(dir_name, name_list, NULL, alphasort);
		if (*cnt_dir == -1) {
			perror("scandir");
			exit(EXIT_FAILURE);
		}
}

void RenderingListDir (WINDOW* win, struct dirent** name_list, 
													int cnt_dir)
{
	for (int cnt = 0; cnt < cnt_dir; ++cnt) {
		mvwprintw(win, cnt+1, 1, "%s", name_list[cnt]->d_name);
	}
}