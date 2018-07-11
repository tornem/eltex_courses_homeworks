#define _DEFAULT_SOURCE
#include "common.h"
#include <string.h>

int ListDir (struct PmPanel* win, WINDOW* status)
{
	
	if ( !(strcmp(win->work_dir, "..")) ) {  // if user select ".."
		char *buff = malloc(PATH_MAX);  
		chdir("..");  // climb the hierarchy above
		getcwd(buff, PATH_MAX);  // write working directory abs addr in buf
		strcpy(win->work_dir, buff);
		// win->work_dir = buff; 
		free(buff);
	}

	int temp_dir_num = scandir(win->work_dir, &(win->name_list), 
											NULL, alphasort); 
		if (temp_dir_num == -1) {
			wprintw(status, "Cant open this file/directory %s", win->name_list[win->selected_obj]->d_name);
			wnoutrefresh(status);
			
			doupdate();
			return -1;
		} else {
			chdir(win->work_dir);  // change the working directory

			win->dir_num = temp_dir_num;
			win->size = getmaxy(win->w_half) - 2;
			win->list_begin = 0;
			win->list_end = win->size; 
			win->y = 1;
			win->x = 1;
			win->selected_obj = 1;	
		}
	return 0;
}

void RenderingListDir (struct PmPanel* win)
{	
    char buf[PATH_MAX];
	werase(win->w_half);

	int i = 0;
	for (int cnt = win->list_begin; cnt < win->dir_num; ++cnt) {
		mvwprintw(win->w_half, i, 1, "%s", win->name_list[cnt]->d_name);
		i++;
	}
	box(win->w_half, '|', '-');
	werase(win->w_dir);
	wattron(win->w_dir, A_UNDERLINE);
    getcwd(buf, PATH_MAX);
	wprintw(win->w_dir, "%s" ,buf);
	wchgat(win->w_dir, -1, A_UNDERLINE, 1, NULL);
}

void CoordControl (struct PmPanel* win) 
{ 
	if ((win->dir_num) > (win->size)) {  
	//часть директории не поместилась на экране
		if ((win->y) >= (win->size)) {
		//достигнут нижний край экрана
			win->list_begin++;  //увеличение верхней границы буфера отображения файлов в директории
			win->list_end++;  //тоже только с нижней границей
			win->y = win->size - 1;  //установка курсора в конец окна
			if (win->selected_obj >= win->dir_num) {  
			//объекты в директории закончились
				win->y = 1;  //установка курсора в начало окна
				win->selected_obj = 1;  //drop 
				win->list_begin = 0;  //установка нижней границы буфера
				win->list_end = win->size;  //установка верхней границы
			}
			RenderingListDir(win);
		} else if ((win->y) < 1) {
		//достигнут верхний край экрана
			if ((win->selected_obj) > 0) {
			//и это не первый файл директории
			//то осуществляем скрол вверх
				--win->list_begin;
				--win->list_end;
				win->y = 1;
			} else {
			//и это первый файл в директории
				win->y = win->size - 1;  //установка курсора на самый последний объект в директории
                win->list_begin = win->dir_num - win->size;  //сдвиг нижней границы буфера на размер окна
				win->list_end = win->dir_num - 1;  //установка верхней границы буфера 
				win->selected_obj = win->dir_num - 1;  //drop co in end
			}
			RenderingListDir(win);  //переотрисовка окна
		}
	} else {  
	//вся директория на экране
		if ((win->y) < 1) {  
		//достигнут верхний край экрана
			win->y = win->dir_num - 1;
			win->selected_obj = win->y;
		} else if ((win->y) >= (win->dir_num)) {
		//достигнут последний файл в директории
			win->y = 1;  //переводим курсов на первый файл
			win->list_end = win->dir_num;  //проверить!!!!!!!!!
			win->selected_obj = 1;  //drop
		}
	}
}

