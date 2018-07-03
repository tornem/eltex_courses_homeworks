#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t cpid;
	pid_t cpid_1;
	pid_t cpid_2;
	pid_t cpid_3;
	pid_t cpid_4;
	int status_0;
	int status_1;
	int status_2;
	int ended_proc_id_0;
	int ended_proc_id_1;
	int ended_proc_id_2;
	int ended_proc_id_3;
	int ended_proc_id_4;

	printf("\n\t\t\t\t PARENT\n");
	printf("\t\t\t\tPID:%d\n", getpid());
	//printf("\t\t\t\tPPID:%d\n\n", getppid());

	cpid = fork();
	//create child_1 from PARENT
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (cpid == 0) {
		printf("\t\tchild_1\n");
		printf("\t\tPID:%d\n", getpid());
		printf("\t\tPPID:%d\n\n", getppid());
		
		cpid_1 = fork();
		if (cpid_1 == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		//create child_1_2 from child_1
		if (cpid_1 == 0) {
			printf("child_1_2\n");
			printf("PID:%d\n", getpid());
			printf("PPID:%d\n\n", getppid());
			sleep(1);
			//close child_1_2
			exit(EXIT_SUCCESS);
		}
		ended_proc_id_1 = wait(&status_1);
		printf("PID CLOSED:%d\n", ended_proc_id_1);
		//close child_1
		exit(EXIT_SUCCESS); 
	} else {
		cpid_2 = fork();
		//create child_2 from PARENT
		if (cpid_2 == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (cpid_2 == 0) {
			printf("\t\t\t\t\t\tchild_2\n");
			printf("\t\t\t\t\t\tPID:%d\n", getpid());
			printf("\t\t\t\t\t\tPPID:%d\n\n", getppid());

			cpid_3 = fork();
			//create child_2_1 from child_2
			if (cpid_3 == -1) {
				perror("fork");
				exit(EXIT_FAILURE);
			}
			if (cpid_3 == 0) {
				printf("\t\t\t\tchild_2_1\n");
				printf("\t\t\t\tPID:%d\n", getpid());
				printf("\t\t\t\tPPID:%d\n\n", getppid());
				sleep(1);
				//close child_2_1
				exit(EXIT_SUCCESS);
			} else {
				cpid_4 = fork();
				//create child_2_2 from child_2
				if (cpid_4 == -1) {
					perror("fork");
					exit(EXIT_FAILURE);
				}
				if (cpid_4 == 0) {
					printf("\t\t\t\t\t\t\t\tchild_2_2\n");
					printf("\t\t\t\t\t\t\t\tPID:%d\n", getpid());
					printf("\t\t\t\t\t\t\t\tPPID:%d\n\n", getppid());
					//close child_2_2
					sleep(1);
					exit(EXIT_SUCCESS);
				}
			}
			ended_proc_id_3 = waitpid(cpid_3, &status_2, 0);
			ended_proc_id_4 = waitpid(cpid_4, &status_2, 0);
			printf("PID CLOSED:%d\n", ended_proc_id_3);
			printf("PID CLOSED:%d\n", ended_proc_id_4);
			//close child_2
			exit(EXIT_SUCCESS);
		}
	}

	ended_proc_id_0 = waitpid(cpid,&status_0, 0);
	if (ended_proc_id_0 > 1 && !cpid) {
		printf("Error closing process ID:%d\n", cpid);
	}
	ended_proc_id_2 = waitpid(cpid_2, &status_1, 0);
	if (ended_proc_id_2 > 1 && !cpid_2) {
		printf("Error closing process ID:%d\n", cpid_2);
	}
	printf("PID CLOSED:%d\n", ended_proc_id_0);
	printf("PID CLOSED:%d\n", ended_proc_id_2);
	//close PARENT
	exit(EXIT_SUCCESS);
	
}	