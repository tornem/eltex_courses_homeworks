#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <form.h>

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

void WriteInWin(char* read_buf, int max_y, int max_x)
{
	int key;
	int y1, x1;
	while ((key = getch()) != 27){
		switch (key){
			case KEY_BACKSPACE:
				getyx(stdscr, y1, x1);
				if (x1 == 0){
					mvaddch(y1, x1, EOF);
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

int main(int argc, char* argv[])
{
	FIELD *field;
	int fd;
	int y, x;
	int max_y, max_x;
	int key;
	char read_buf[2400];
	char write_buf[2400];

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
	curs_set(1); 
	printw("%s", read_buf);
	noecho();
	cbreak();
	keypad(stdscr, TRUE); // позволяет обходить проверку системы на esc последовательности
	//raw();
	getmaxyx(stdscr, max_y, max_x);

	field = new_field(1, max_x, max_y, 0, 0, 0);

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
				WriteInWin(read_buf, max_y, max_x);
				break;
			case 's':
				SafeFile(fd, read_buf);

		}
	}
	// FILE *fp;
	// fp = fdopen(fd, "w+");
	// putwin(stdscr, fp);
	endwin();


	return 0;
}