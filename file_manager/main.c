#include "main.h"

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int main(int argc, char** argv)
{	
	int ch;
	int max_y, max_x;
	int cnt_dir = 0; //number of directory entries
	WINDOW* w_main_1;
	WINDOW* w_main_2;
	struct dirent** name_list;

	initscr();
	signal(SIGWINCH, sig_winch);
	start_color();
	cbreak();
	//keypad();
	//noecho();
	getmaxyx(stdscr, max_y, max_x);
	init_pair(1, COLOR_BLACK, COLOR_BLUE);

	//initialisation half-main windows
	Init_main_win(&w_main_1,&w_main_2, max_y, max_x);


	//draw stdscr and half-main windows
	wnoutrefresh(stdscr);
	wnoutrefresh(w_main_1);
	wnoutrefresh(w_main_2);
	doupdate();

	//open dir !!! вынеси в отдельную функцию отрисовку!!!!!!!
	if (--argc == 0) {
		argv[1] = ".";
	}
	// if (ListDir(argv[1], &name_list, &cnt_dir)) { //send addr addresa struct
	// 	perror("ListDir");
	// 	exit(EXIT_FAILURE);
	// }
	cnt_dir = scandir(argv[1], &name_list, NULL, alphasort);
	if (cnt_dir == -1) {
		perror("scandir");
		exit(EXIT_FAILURE);
	}
	int i, k = 0;
	while (cnt_dir--) {
		mvwaddstr(w_main_1, cnt_dir+1, 3, name_list[cnt_dir]->d_name);
		//wnoutrefresh(w_main_1);
		free(name_list[cnt_dir]);
		i++;
	}
	free(name_list);

	//rendering list objects within directory
	//RenderingListDir(&w_main_1, &name_list, cnt_dir);
	wnoutrefresh(stdscr);
	wnoutrefresh(w_main_1);
	doupdate();

	while ((ch = getch()) != 'q'){
		switch (ch){
			case KEY_UP:
			
		}
	}

	getch();
	endwin();

	return 0;
}