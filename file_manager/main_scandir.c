#include "common.h"

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
	//int y, x;

	struct PmPanel left_win;
	struct PmPanel right_win;

	initscr();
	signal(SIGWINCH, sig_winch);
	start_color();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	getmaxyx(stdscr, max_y, max_x);
	init_pair(1, COLOR_WHITE, COLOR_BLUE);

	//initialisation half-main windows
	Init_main_win(&left_win, max_y, max_x, 0);
	//Default active left window
	left_win.active = TRUE;

	Init_main_win(&right_win, max_y, max_x, max_x/2);
	right_win.active = FALSE;

	//scaning directory
	if (--argc == 0) {
		left_win.start_dir = ".";
		right_win.start_dir = getenv("HOME");
	} else {
		left_win.start_dir = argv[1];
	}
	ListDir(&left_win);
	ListDir(&right_win);

	//rendering list objects within directory
	RenderingListDir(&left_win);
	RenderingListDir(&right_win);

	wnoutrefresh(stdscr);
	wnoutrefresh(left_win.w_half);
	wnoutrefresh(right_win.w_half);
	doupdate();
	
	// while ( (ch = getch()) != 'q' ){
	// 	switch (ch) {
	// 		case 9:
	// 			if (left_win.active) {
	// 				wmove(left_win.w_half, left_win.y, left_win.x);
	// 				wnoutrefresh(stdscr);
	// 				wnoutrefresh(left_win.w_half);
	// 				left_win.active = FALSE;
	// 			} else {
	// 				wmove(right_win.w_half, right_win.y, right_win.x);
	// 				wnoutrefresh(stdscr);
	// 				wnoutrefresh(right_win.w_half);
	// 				left_win.active = TRUE;
	// 			}
	// 	}

	// 	// if (ch == '1') {
	// 	// 	wmove(left_win.w_half, left_win.y, left_win.x);
	// 	// 	wnoutrefresh(stdscr);
	// 	// 	wnoutrefresh(left_win.w_half);
	// 	// } else if (ch == '2') {
	// 	// 	wmove(right_win.w_half, right_win.y, right_win.x);
	// 	// 	wnoutrefresh(stdscr);
	// 	// 	wnoutrefresh(right_win.w_half);
	// 	// }
	// 	doupdate();
	// }
	while ( (ch = getch()) != 'q' ){
		if (ch == '1') {
			wmove(left_win.w_half, left_win.y, left_win.x);
			wnoutrefresh(stdscr);
			wnoutrefresh(left_win.w_half);
		} else if (ch == '2') {
			wmove(right_win.w_half, right_win.y, right_win.x);
			wnoutrefresh(stdscr);
			wnoutrefresh(right_win.w_half);
		} else if (ch == KEY_UP) {
			wmove(left_win.w_half, --left_win.y, left_win.x);
			wnoutrefresh(stdscr);
			wnoutrefresh(left_win.w_half);
		}
		doupdate();
	}
	// wmove(left_win.w_half, left_win.y, left_win.x);
	// while ((ch = getch()) != 'q') {
	// 	if (left_win.active){
	// 		// getyx(left_win.w_half, y, x);
	// 		switch (ch){
	// 			case KEY_UP:
	// 				++left_win.y;
	// 				wmove(left_win.w_half, left_win.y, left_win.x);
	// 				break;
	// 			case KEY_DOWN:
	// 				--left_win.y;
	// 				wmove(left_win.w_half, left_win.y, left_win.x);
	// 				break;
	// 			case 9:
	// 				right_win.active = TRUE;
	// 				left_win.active = FALSE;
	// 				break;
	// 		}
	// 		wnoutrefresh(right_win.w_half);
	// 		doupdate();
	// 	} else {
	// 		//getyx(right_win.w_half, y, x);
	// 		switch (ch){
	// 			case KEY_UP:
	// 				++right_win.y;
	// 				wmove(right_win.w_half, right_win.y, right_win.x);
	// 				break;
	// 			case KEY_DOWN:
	// 				--right_win.y;
	// 				wmove(right_win.w_half, right_win.y, right_win.x);
	// 				break;
	// 			case 9:
	// 				left_win.active = TRUE;
	// 				right_win.active = FALSE;
	// 				break;
	// 		}
	// 		wnoutrefresh(right_win.w_half);
	// 		doupdate();
	// 	}
	// }

	getch();
	endwin();

	return 0;
}