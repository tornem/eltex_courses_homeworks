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
}