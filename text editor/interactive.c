#include "interactive.h"

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

void WriteInWin(WINDOW** info_win, char* read_buf, int max_y, int max_x)
{	
	int key;
	int y1, x1;
	
	while ((key = getch()) != 27){
		switch (key){
			case KEY_BACKSPACE:
				getyx(stdscr, y1, x1);
				if (x1 == 0){
					mvdelch(y1, x1);
					read_buf[y1*max_y + x1] = ' ';
					break;
				} 
				addch(' ');
				read_buf[y1*max_y + x1] = ' ';
				move(y1, x1-1);
				break;
			default:
				getyx(stdscr, y1, x1);
				read_buf[y1*max_y + x1] = (char)key;
				addch(key);
		}
	}
}

void SafeFile(int fd, char* read_buf)
{
	if(lseek(fd, 0, SEEK_SET) == -1){
		perror("Error the offset of file discriptor");
	} else {
		//printf("Offseting complite!\n");
	}

	//printf("%ld", strlen(read_buf));
	if ((write(fd, read_buf, strlen(read_buf))) == -1){
		perror("Fail write in file");
	} else {
		//printf("Write complite!\n");
	}
	
}

void InitHelp(WINDOW** w_help, PANEL** panel_help)
{
	int height = 9, wight = 40;
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	*w_help = newwin(height, wight, max_y/2 - height/2, max_x/2 - wight/2);
	wbkgd(*w_help, COLOR_PAIR(2));
	wclear(*w_help);
	mvwaddstr(*w_help, 0, 2, "Use next keys for manage text editor:");
	mvwaddstr(*w_help, 2, 1, "If you in comand mode:");
	mvwaddstr(*w_help, 3, 0, "i - insert mode");
	mvwaddstr(*w_help, 4, 0, "s - save file");
	mvwaddstr(*w_help, 5, 0, "q - quite");
	mvwaddstr(*w_help, 6, 1, "If you in insert mode:");
	mvwaddstr(*w_help, 7, 0, "esc - return in comand mode");

	wrefresh(*w_help);

	*panel_help = new_panel(*w_help);
	update_panels();
	doupdate();
	hide_panel(*panel_help);
}

void CallHelp(WINDOW** w_statusbar, PANEL** panel_help)
{
	int key;
	//WINDOW* bind_win = panel_window(panel_help);
	curs_set(FALSE);
	show_panel(*panel_help);
	update_panels();
	doupdate();
	wclear(*w_statusbar);
	wprintw(*w_statusbar, "Press h again for close help");
	wrefresh(*w_statusbar);
	if ((key = getch()) == 'h'){
		hide_panel(*panel_help);
		update_panels();
		doupdate();
		wclear(*w_statusbar);
		wprintw(*w_statusbar, "Press 'h' for help");
		wrefresh(*w_statusbar);
	}
	curs_set(TRUE);
}
