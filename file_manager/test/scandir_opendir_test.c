#define _DEFAULT_SOURCE
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int ListDir(char dir, struct dirent** name_list)
{
	int num_dir; //number of directory entries

	num_dir = scandir(&dir, &name_list, NULL, alphasort);
	if (num_dir == -1) {
		perror("scandir");
		exit(EXIT_FAILURE);
	}

	while (num_dir--) {
		printf("%s\n", name_list[num_dir]->d_name);
		free(name_list[num_dir]);
	}
	free(name_list);

	exit(EXIT_SUCCESS);
}