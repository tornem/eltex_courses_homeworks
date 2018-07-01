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

int main(int argc, char* argv[])
{
	int fd; 
	int y, x;
	int max_y, max_x;
	int key; 
	//bool cmd = TRUE;
	char read_buf[2400]; 
	char write_buf[2400];
	char cmd_mode[] = "Command mode";
	char ins_mode[] = "insert mode";
	WINDOW* w_statusbar;
	WINDOW* w_help;
	WINDOW* w_mode[2];
	//WINDOW* w_mode;
	PANEL* panel_help;
	PANEL* panel_mode[2];
	
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

	w_statusbar = subwin(stdscr, 1, max_x - strlen(ins_mode), max_y - 1, 0);
	//w_help = subwin(stdscr, 10, 20, max_y/2 - 5, max_x/2 - 10);
	init_pair(2, COLOR_BLACK, COLOR_BLUE);
	wbkgd(w_statusbar, COLOR_PAIR(0));
	wprintw(w_statusbar, "Press 'h' for help");

	//Initialisation window for help-panel
	InitHelp(&w_help, &panel_help);

	//Initialisation windows and panels for mode;
	init_pair(3, COLOR_RED, COLOR_BLACK);
	w_mode[0] = newwin(1, strlen(ins_mode), max_y - 1, max_x - strlen(ins_mode));
	// if (w_mode[0] == NULL){
	// 	printf("ERROR!!!\n");
	// }
	wbkgd(w_mode[0], COLOR_PAIR(3));
	wprintw(w_mode[0], "%s", cmd_mode);
	wrefresh(w_mode[0]);

	panel_mode[0] = new_panel(w_mode[0]);
	update_panels();
	doupdate();

	w_mode[1] = newwin(1, strlen(ins_mode), max_y - 1, max_x - strlen(ins_mode));
	// if (w_mode[1] == NULL){
	// 	printf("ERROR!!!\n");
	// }
	wbkgd(w_mode[1], COLOR_PAIR(3));
	wprintw(w_mode[1], "%s", ins_mode);
	wrefresh(w_mode[1]);

	panel_mode[1] = new_panel(w_mode[1]);

	top_panel(panel_mode[0]);
	update_panels();
	doupdate();	

	while ((key = getch()) != 'q'){
		if (key == 'i'){
			top_panel(panel_mode[1]);
			update_panels();
		} 
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
				WriteInWin(&w_statusbar, read_buf, max_y, max_x);
				break;
			case 's':
				SafeFile(fd, read_buf);
				break;
			case 'h':
				CallHelp(&w_statusbar, &panel_help);
				break;
		}
		top_panel(panel_mode[0]);
		update_panels();
		doupdate();
	}
	// FILE *fp;
	// fp = fdopen(fd, "w+");
	// putwin(stdscr, fp);
	endwin();


	return 0;
}