#include "common.h"

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

int main(int argc, char** argv)
{	
	int ch;  // input key
	int max_y, max_x;  
	char *buff_curr_dir = malloc(PATH_MAX);

	WINDOW* help_bar;  // bottom bar for help message 

	struct PmPanel left_win;
	struct PmPanel right_win;

	initscr();  //init ncurses 
	signal(SIGWINCH, sig_winch);  
	start_color();  //init colors set
	cbreak();  //disable buffering and kiw_hw_hll character-processing
	keypad(stdscr, TRUE);  //possible user use functional keys within programm
	noecho();  //disable echoed charaster typed user
	curs_set(0); //invisible cursor

	getmaxyx(stdscr, max_y, max_x);  //get max nlines and ncolons by terminal size
	init_pair(1, COLOR_WHITE, COLOR_BLUE);  //init colors pair
	init_pair(2, COLOR_RED, COLOR_GREEN);
	init_pair(3, COLOR_MAGENTA, COLOR_BLUE);

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


	// determine which directories to open at the first start
	if (--argc == 0) {  // non argumets 
		getcwd(buff_curr_dir, PATH_MAX);  // return absolute pathname that is 
										  // the current working directory
		left_win.work_dir = buff_curr_dir;  // left panel shows current user directory
		right_win.work_dir = getenv("HOME");  // right panel on default shows /home directory
	} else {
		printf("Sorry, the introduction of additional arguments is not yet implemented\n");
		exit(EXIT_FAILURE);
	}

	//rendering list objects within directory
	ListDir(&right_win, help_bar);
	RenderingListDir(&right_win);

	ListDir(&left_win, help_bar);
	RenderingListDir(&left_win);
	

	while (1) {
		if (left_win.active) {
			mvwchgat(left_win.w_half, left_win.y, left_win.x, -1, A_BOLD, 1, NULL);
			mvwchgat(right_win.w_half, right_win.y, right_win.x, -1, A_DIM, 1, NULL);
		} else {
			mvwchgat(left_win.w_half, left_win.y, left_win.x, -1, A_DIM, 1, NULL);
			mvwchgat(right_win.w_half, right_win.y, right_win.x, -1, A_BOLD, 1, NULL);
		}
		wnoutrefresh(stdscr);
		wnoutrefresh(left_win.w_half);
		wnoutrefresh(right_win.w_half);
		wnoutrefresh(help_bar);
		werase(help_bar);
		wnoutrefresh(left_win.w_dir);
		wnoutrefresh(right_win.w_dir);
		doupdate();

		ch = getch();

		if(ch == 'q' || ch == 'Q') {
			break;
		}
		switch (ch) {
			case KEY_UP:
				if (left_win.active) {
					if (left_win.name_list[left_win.selected_obj]->d_type == DT_EXEC) {
						wchgat(left_win.w_half, -1, A_BOLD, 3, NULL);						
					} else {
						wchgat(left_win.w_half, -1, 0, 1, NULL);
					}
					--left_win.y;
					--left_win.selected_obj;
					CoordControl(&left_win);
					
					wprintw(help_bar, "y=%d dir=%d le=%d ls=%d ws=%d co=%d", 
							left_win.y, left_win.dir_num, left_win.list_end, 
							left_win.list_begin, left_win.size, left_win.selected_obj);

					mvwchgat(left_win.w_half, left_win.y, left_win.x, -1, A_BOLD, 1, NULL);
				} else {
					if (right_win.name_list[right_win.selected_obj]->d_type == DT_EXEC) {
						wchgat(right_win.w_half, -1, A_BOLD, 3, NULL);						
					} else {
						wchgat(right_win.w_half, -1, 0, 1, NULL);
					}
					--right_win.y;
					--right_win.selected_obj;
					CoordControl(&right_win);
					wprintw(help_bar, "y=%d dir=%d le=%d ls=%d ws=%d co=%d", 
							right_win.y, right_win.dir_num, right_win.list_end, 
							right_win.list_begin, right_win.size, right_win.selected_obj);

					mvwchgat(right_win.w_half, right_win.y, right_win.x, -1, A_BOLD, 1, NULL);
				}
				break;
			case KEY_DOWN:
				if (left_win.active) {
					if (left_win.name_list[left_win.selected_obj]->d_type == DT_EXEC) {
						wchgat(left_win.w_half, -1, A_BOLD, 3, NULL);						
					} else {
						wchgat(left_win.w_half, -1, 0, 1, NULL);
					}
					++left_win.y;
					++left_win.selected_obj;
					CoordControl(&left_win);
					wprintw(help_bar, "y=%d dir=%d le=%d ls=%d ws=%d co=%d", 
							left_win.y, left_win.dir_num, left_win.list_end, 
							left_win.list_begin, left_win.size, left_win.selected_obj);
					
					mvwchgat(left_win.w_half, left_win.y, left_win.x, -1, A_BOLD, 1, NULL);
				} else {
					if (right_win.name_list[right_win.selected_obj]->d_type == DT_EXEC) {
						wchgat(right_win.w_half, -1, A_BOLD, 3, NULL);						
					} else {
						wchgat(right_win.w_half, -1, 0, 1, NULL);
					}
					++right_win.y;
					++right_win.selected_obj;
					CoordControl(&right_win);
					wprintw(help_bar, "y=%d dir=%d le=%d ls=%d ws=%d co=%d", 
							right_win.y, right_win.dir_num, right_win.list_end, 
							right_win.list_begin, right_win.size, right_win.selected_obj);

					mvwchgat(right_win.w_half, right_win.y, right_win.x, -1, A_BOLD, 1, NULL);
				}
				break;
			case 9:  //9 - code for TAB symboladded the possibility of a scrollingadded the possibility of a scadded the possibility of a scrolling upadded the possibility of a scrolling upadded the possibility of a scrolling upadded the possibility of a scrolling uprolling up up
				if (left_win.active) {
					left_win.active = FALSE;
					chdir(right_win.work_dir);
					wchgat(left_win.w_half, -1, A_DIM, 1, NULL);
					wchgat(right_win.w_half, -1, A_BOLD, 1, NULL);
				} else {
					left_win.active = TRUE;
					chdir(left_win.work_dir);
					wchgat(right_win.w_half, -1, A_DIM, 1, NULL);
					wchgat(left_win.w_half, -1, A_BOLD, 1, NULL);
				}
				break;
			case 10:  //10 - code for ENTER symbol.
				if (left_win.active) {
					if ((left_win.name_list[left_win.selected_obj]->d_type) == DT_EXEC) {
						if ((TryExec(left_win.name_list[left_win.selected_obj]->d_name)) == -1) {
							perror("TryExec");
						} 
					} else {
						left_win.work_dir = left_win.name_list[left_win.selected_obj]->d_name;
						if (ListDir(&left_win, help_bar) == -1) {
							break;
						} else {
							left_win.selected_obj = 1;
						}
					}
					RenderingListDir(&left_win);
				} else {
					right_win.work_dir = right_win.name_list[right_win.selected_obj]->d_name;
					if (ListDir(&right_win, help_bar) == -1) {
						break;
					} else {
						right_win.selected_obj = 1;
					}
					RenderingListDir(&right_win);
				}
				break;
		}
	}
	
	free(buff_curr_dir);

	// delwin(left_win.w_dir);
	// delwin(right_win.w_dir);
	// delwin(left_win.w_half);
	// delwin(right_win.w_half);
	// delwin(help_bar);

	endwin();

	return 0;
}
