#include <stdio.h>
#include "arithmetic.h"

int main()
{	
	char buff[12];
	int first_number;
	int second_number;
	int command_num;
	int result = 0;
	double result_f = 0;
	int* result_ptr = &result;
	double* result_f_ptr = &result_f;
	int catcher = 0;
	int end = 1;

	while(end){
		printf("Enter two numbers:\n");

		fgets(buff, 12, stdin);
		first_number = atoi(buff);
		fgets(buff, 12, stdin);
		second_number = atoi(buff);

		printf("Enter the command number for further action: \n");
		printf("(1) Addition\n");
		printf("(2) Subtraction\n");
		printf("(3) Multiplication\n");
		printf("(4) Division\n");
		printf("(0) Quit\n");

		fgets(buff, 2, stdin);
		command_num = buff[0] - 48;
		
		switch(command_num){
			case 1:
				catcher = Add(first_number, second_number, result_ptr);
				if (catcher == -1){
					printf("\nError: type overflow\n");
					catcher = 0;
				}else{
					printf("Resultate operation = %d\n", result);
				}
				break;
			case 2:
				catcher = Sub(first_number, second_number, result_ptr);
				if (catcher == -1){
					catcher = 0;
				}else{
					printf("Resultate operation = %d\n", result);
				}
				break;
			case 3:
				catcher = Mult(first_number, second_number, result_ptr);
				if (catcher == -1){
					catcher = 0;
				}else{
					printf("Resultate operation = %d\n", result);
				}
				break;
			case 4:
				catcher = Div(first_number, second_number, result_f_ptr);
				if (catcher == -1){
					printf("\nError: division by zero\n");
					catcher = 0;
				}else{
					printf("Resultate operation = %.1f\n", result_f);
				}
				break;
			case 0:
				end = 0;
				break;
		}

	}
	
}