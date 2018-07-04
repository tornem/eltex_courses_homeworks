#include "common.h"

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

// struct PMPANEL - захуячить

int main(int argc, char** argv)
{	
	int ch;
	int max_y, max_x;
	int cnt_dir = 0; //number of directory entries
	int cnt_dir_2 = 0; //number of directory entries	
	WINDOW* w_main_1;
	WINDOW* w_main_2;
	struct dirent** name_list;
	struct dirent** name_list_2;
	char* home = getenv("HOME");

	initscr();
	signal(SIGWINCH, sig_winch);
	start_color();
	cbreak();
	//keypad();
	//noecho();
	getmaxyx(stdscr, max_y, max_x);
	init_pair(1, COLOR_WHITE, COLOR_BLUE);

	//initialisation half-main windows
	Init_main_win(&w_main_1,&w_main_2, max_y, max_x);


	//draw stdscr and half-main windows
	// wnoutrefresh(stdscr);
	// wnoutrefresh(w_main_1);
	// wnoutrefresh(w_main_2);
	// doupdate();

	//open dir !!! вынеси в отдельную функцию отрисовку!!!!!!!
	if (--argc == 0) {
		argv[1] = ".";
	}
	ListDir(argv[1], &name_list, &cnt_dir);
	ListDir(home, &name_list_2, &cnt_dir_2);

	//rendering list objects within directory
	RenderingListDir(w_main_1, name_list, cnt_dir);
	RenderingListDir(w_main_2, name_list_2, cnt_dir_2);

	wnoutrefresh(stdscr);
	wnoutrefresh(w_main_1);
	wnoutrefresh(w_main_2);
	doupdate();

	getch();
	endwin();

	return 0;
}