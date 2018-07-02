#ifndef __INTERACTIVE_H__
#define __INTERACTIVE_H__

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

void sig_winch(int signo);

void WriteInWin(WINDOW** info_win, char* read_buf, int max_y, int max_x);

void SafeFile(int fd, char* read_buf);

void InitHelp(WINDOW** w_help, PANEL** panel_help);

void CallHelp(WINDOW** w_statusbar, PANEL** panel_help);
#endif