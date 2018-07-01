#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int fd = open("test.txt", O_RDWR | O_CREAT, 0666);
	char write_buf[] = "Hello, i`m birning";
	ssize_t nByte;
	char read_buf[255];
	int test_read = 0;

	if (fd == -1){
		perror("Error opening/creating the file");
	} else {
		printf("File is open/creat\n");
	}
	if ((nByte = write(fd, write_buf, 18)) == -1){
		perror("Fail write in file");
	} else {
		printf("Write complite!\n");
	}
	if(lseek(fd, 0, SEEK_SET) == -1){
		perror("Error the offset of file discriptor");
	} else {
		printf("Offseting complite!\n");
	}
	if (test_read = read(fd, read_buf, 255) == -1){
		perror("Error reading the file");
	} else {
		printf("File reading is success!\n");
		//printf("%d\n", test_read);
		printf("%s\n", read_buf);
	}
	return 0;
}