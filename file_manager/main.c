#include "main.h"

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int main()
{
	int max_y, max_x;
	WINDOW* w_main[2];

	initscr();
	signal(SIGWINCH, sig_winch);
	start_color();
	cbreak();
	//keypad();
	//noecho();
	getmaxyx(stdscr, max_y, max_x);
	init_pair(1, COLOR_BLACK, COLOR_BLUE);

	//initialisation half-main windows
	init_main_win(&w_main[0],&w_main[1], max_y, max_x);

	//draw stdscr and half-main windows
	wnoutrefresh(stdscr);
	wnoutrefresh(w_main[0]);
	wnoutrefresh(w_main[1]);
	doupdate();

	getch();
	endwin();

	return 0;
}