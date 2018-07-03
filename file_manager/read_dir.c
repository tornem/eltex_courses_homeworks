#include "main.h"

int ListDir(char* dir, struct dirent*** name_list, int* cnt_dir)
{

	*cnt_dir = scandir(dir, name_list, NULL, alphasort);
	if (*cnt_dir == -1) {
		perror("scandir");
		exit(EXIT_FAILURE);
	}

	return 0;
}

void RenderingListDir(WINDOW** win, struct dirent** name_list, int cnt_dir)
{
	printf("%d\n", cnt_dir);
	while (cnt_dir--) {
		printf("%s\n", name_list[cnt_dir]->d_name);
		//free(name_list[cnt_dir]);
	}
	//free(name_list);
	printf("%d\n", cnt_dir);
}