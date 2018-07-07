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
	char buff_curr_dir[PATH_MAX];

	WINDOW* help_bar;

	struct PmPanel left_win;
	struct PmPanel right_win;

	initscr();  //init ncurses 
	signal(SIGWINCH, sig_winch);  
	start_color();  //init colors set
	cbreak();  //disable buffering and kill 
	keypad(stdscr, TRUE);  //possible user use functional keys
	noecho();  //disable echoed charaster typed user
	curs_set(0); //invisible cursor

	getmaxyx(stdscr, max_y, max_x);  //get max nlines and ncolons by terminal size
	init_pair(1, COLOR_WHITE, COLOR_BLUE);  //init colors pair
	init_pair(2, COLOR_RED, COLOR_GREEN);

	//initialisation half-main windows
	Init_main_win(&left_win, max_y, max_x, 0);
	Init_main_win(&right_win, max_y, max_x, max_x/2);

	//init help_bar for help
	help_bar = subwin(stdscr, 1, max_x, max_y - 1, 0);
	wbkgd(help_bar, COLOR_PAIR(2));  //background color setting
	//wprintw(sub_win, "Press 'h' for help");
	//Default active left window
	left_win.active = TRUE; 
	right_win.active = FALSE;


	//scaning directory
	if (--argc == 0) {
		getcwd(buff_curr_dir, PATH_MAX);
		left_win.current_dir = buff_curr_dir;
		right_win.current_dir = getenv("HOME");
	} else {
		exit(EXIT_FAILURE);
		printf("Sorry, the introduction of additional arguments is not yet implemented\n");
		// left_win.current_dir = argv[1];
		// right_win.current_dir = getenv("HOME");
	}
	ListDir(&left_win, help_bar);
	ListDir(&right_win, help_bar);

	//rendering list objects within directory
	RenderingListDir(&left_win);
	RenderingListDir(&right_win);

	wnoutrefresh(stdscr);
	wnoutrefresh(left_win.w_half);
	wnoutrefresh(right_win.w_half);
	wnoutrefresh(help_bar);
	wnoutrefresh(left_win.w_dir);
	wnoutrefresh(right_win.w_dir);
	doupdate();
	
	mvwchgat(left_win.w_half, left_win.y, left_win.x, -1, A_BOLD, 1, NULL);
	mvwchgat(right_win.w_half, right_win.y, right_win.x, -1, A_DIM, 1, NULL);
	wrefresh(left_win.w_half);

	while ( (ch = getch()) != 'q' ){
		switch (ch) {
			case KEY_UP:
				if (left_win.active) {
					wchgat(left_win.w_half, -1, 0, 1, NULL);
					--left_win.y;
					--left_win.current_obj;
					CoordControl(&left_win);
					wclear(help_bar);
					wprintw(help_bar, "y=%d dir=%d le=%d ls=%d ws=%d co=%d", 
							left_win.y, left_win.dir_num, left_win.list_end, 
							left_win.list_begin, left_win.size, left_win.current_obj);

					wnoutrefresh(help_bar);
					mvwchgat(left_win.w_half, left_win.y, left_win.x, -1, A_BOLD, 1, NULL);
					wnoutrefresh(stdscr);
					wnoutrefresh(left_win.w_half);
				} else {
					wchgat(right_win.w_half, -1, 0, 1, NULL);
					--right_win.y;
					--right_win.current_obj;
					CoordControl(&right_win);
					wclear(help_bar);
					wprintw(help_bar, "y=%d dir=%d le=%d ls=%d ws=%d co=%d", 
							right_win.y, right_win.dir_num, right_win.list_end, 
							right_win.list_begin, right_win.size, right_win.current_obj);

					wnoutrefresh(help_bar);
					mvwchgat(right_win.w_half, right_win.y, right_win.x, -1, A_BOLD, 1, NULL);
					wnoutrefresh(stdscr);
					wnoutrefresh(right_win.w_half);
				}
				break;
			case KEY_DOWN:
				if (left_win.active) {
					wchgat(left_win.w_half, -1, 0, 1, NULL);
					++left_win.y;
					++left_win.current_obj;
					CoordControl(&left_win);
					wclear(help_bar);
					wprintw(help_bar, "y=%d dir=%d le=%d ls=%d ws=%d co=%d", 
							left_win.y, left_win.dir_num, left_win.list_end, 
							left_win.list_begin, left_win.size, left_win.current_obj);
					
					wnoutrefresh(help_bar);
					mvwchgat(left_win.w_half, left_win.y, left_win.x, -1, A_BOLD, 1, NULL);
					wnoutrefresh(stdscr);
					wnoutrefresh(left_win.w_half);
				} else {
					wchgat(right_win.w_half, -1, 0, 1, NULL);
					++right_win.y;
					++right_win.current_obj;
					CoordControl(&right_win);
					wclear(help_bar);
					wprintw(help_bar, "y=%d dir=%d le=%d ls=%d ws=%d co=%d", 
							right_win.y, right_win.dir_num, right_win.list_end, 
							right_win.list_begin, right_win.size, right_win.current_obj);

					wnoutrefresh(help_bar);
					mvwchgat(right_win.w_half, right_win.y, right_win.x, -1, A_BOLD, 1, NULL);
					wnoutrefresh(stdscr);
					wnoutrefresh(right_win.w_half);
				}
				break;
			case 9:  //9 - code for TAB symboladded the possibility of a scrollingadded the possibility of a scadded the possibility of a scrolling upadded the possibility of a scrolling upadded the possibility of a scrolling upadded the possibility of a scrolling uprolling up up
				if (left_win.active) {
					left_win.active = FALSE;
					wchgat(left_win.w_half, -1, A_DIM, 1, NULL);
					wchgat(right_win.w_half, -1, A_BOLD, 1, NULL);
					wnoutrefresh(stdscr);
					wnoutrefresh(left_win.w_half);
					wnoutrefresh(right_win.w_half);
				} else {
					left_win.active = TRUE;
					wchgat(right_win.w_half, -1, A_DIM, 1, NULL);
					wchgat(left_win.w_half, -1, A_BOLD, 1, NULL);
					wnoutrefresh(stdscr);
					wnoutrefresh(right_win.w_half);
					wnoutrefresh(left_win.w_half);
				}
				break;
			case 10:  //10 - code for ENTRE symbol.
				if (left_win.active) {
					left_win.current_dir = left_win.name_list[left_win.current_obj]->d_name;
					wclear(help_bar);
					if (ListDir(&left_win, help_bar) == -1) {
						break;
					} else {
						left_win.current_obj = 1;
					}
					RenderingListDir(&left_win);
					wnoutrefresh(stdscr);
					wnoutrefresh(help_bar);
					wnoutrefresh(left_win.w_half);
				} else {
					right_win.current_dir = right_win.name_list[right_win.current_obj]->d_name;
					wclear(help_bar);
					if (ListDir(&right_win, help_bar) == -1) {
						break;
					} else {
						right_win.current_obj = 1;
					}
					RenderingListDir(&right_win);
					wnoutrefresh(stdscr);
					wnoutrefresh(help_bar);
					wnoutrefresh(right_win.w_half);
				}
				break;
		}
		doupdate();
	}
	
	endwin();

	return 0;
}