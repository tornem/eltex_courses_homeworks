#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <panel.h>

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

void WriteInWin(WINDOW* info_win, char* read_buf, int max_y, int max_x)
{	
	char mode[] = "Edition mode ON";
	int key;
	int y1, x1;
	
	wprintw(info_win, "Edition mode ON!");
	wrefresh(info_win);

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

void InitHelp(WINDOW* w_help, PANEL* panel_help)
{
	w_help = newwin(10, 20, 2, 10);
	wbkgd(w_help, COLOR_PAIR(2));
	wclear(w_help);
	wrefresh(w_help);

	panel_help = new_panel(w_help);
	update_panels();
	doupdate();
	hide_panel(panel_help);
}

void CallHelp(WINDOW* w_statusbar, PANEL* panel_help)
{
	int key;
	//WINDOW* bind_win = panel_window(panel_help);
	curs_set(FALSE);
	show_panel(panel_help);
	update_panels();
	doupdate();
	wclear(w_statusbar);
	wprintw(w_statusbar, "Press h again for close help");
	wrefresh(w_statusbar);
	if ((key = getch()) == 'h'){
		hide_panel(panel_help);
		update_panels();
		doupdate();
		wclear(w_statusbar);
		wprintw(w_statusbar, "Press 'h' for help");
		wrefresh(w_statusbar);
	}
	curs_set(TRUE);
}


int main(int argc, char* argv[])
{
	int fd; 
	int y, x;
	int max_y, max_x;
	int key; 
	char read_buf[2400]; 
	char write_buf[2400];
	WINDOW* w_statusbar;
	WINDOW* w_help;
	PANEL* panel_help;
	
	if (--argc == 0){
		//вставить справку
		printf("Please, enter filename what your need open/creat.\n");
		exit(1);
	} else {
		fd = open(argv[1], O_RDWR | O_CREAT, 0666);
		if (fd  == -1){
			perror("Error opening/creating the file");
		}
	}
	
	if (read(fd, read_buf, 2400) == -1){
		perror("Error reading file");
	} else {
		printf("file readed!\n");
	}

	initscr();
	signal(SIGWINCH, sig_winch);
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	bkgd(COLOR_PAIR(1));
	curs_set(TRUE); 
	printw("%s", read_buf);
	noecho();
	cbreak();
	keypad(stdscr, TRUE); // позволяет обходить проверку системы на esc последовательности
	//raw();
	getmaxyx(stdscr, max_y, max_x);

	w_statusbar = subwin(stdscr, 1, max_x, max_y - 1, 0);
	//w_help = subwin(stdscr, 10, 20, max_y/2 - 5, max_x/2 - 10);
	init_pair(2, COLOR_BLACK, COLOR_BLUE);
	wbkgd(w_statusbar, COLOR_PAIR(0));
	wprintw(w_statusbar, "Press 'h' for help");

	//Initialisation window for help-panel
	// InitHelp(w_help, panel_help);
	w_help = newwin(10, 20, 2, 10);
	wbkgd(w_help, COLOR_PAIR(2));
	wclear(w_help);
	wrefresh(w_help);
	mvwaddstr(w_help, 0, 0, "Use next keys for manage text editor:");


	panel_help = new_panel(w_help);
	update_panels();
	doupdate();
	hide_panel(panel_help);

	while ((key = getch()) != 'q'){
		getyx(stdscr, y, x);
		switch (key){
			case KEY_UP:
				move(--y,x);
				break;
			case KEY_DOWN:
				move(++y,x);
				break;
			case KEY_RIGHT:
				move(y,++x);
				break;
			case KEY_LEFT:
				move(y,--x);
				break;
			case 'i':
				WriteInWin(w_statusbar, read_buf, max_y, max_x);
				break;
			case 's':
				SafeFile(fd, read_buf);
			case 'h':
				CallHelp(w_statusbar, panel_help);
				break;
		}
	}
	// FILE *fp;
	// fp = fdopen(fd, "w+");
	// putwin(stdscr, fp);
	endwin();


	return 0;
}