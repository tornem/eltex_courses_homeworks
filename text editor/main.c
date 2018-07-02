#include "interactive.h"

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
	curs_set(2); 
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
	move(0,0);
	while ((key = getch()) != 'q'){
		if (key == 'i'){
			top_panel(panel_mode[1]);
			update_panels();
			doupdate();
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
		 // top_panel(panel_mode[0]);
		 // update_panels();
		 // doupdate();

		 // move(y, x);
	}
	// FILE *fp;
	// fp = fdopen(fd, "w+");
	// putwin(stdscr, fp);
	endwin();


	return 0;
}