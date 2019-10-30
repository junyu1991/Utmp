#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <pwd.h>
#include <string.h>

int open_file(char *pathname)
{
	int open_fd;
	open_fd = open(pathname, O_RDONLY);
//	printf("Open %d\n", open_fd);
	return open_fd;
}

/**
int read_from_file(int fd)
{
	struct utmp entry;
	system("echi before adding entry:;who");
	entry.ut_type = USER_PROCESS;
	entry.ut_pid = getpid();
	strcpy(entry.ut_line, ttyname(STDIN_FILENO) + strlen("/dev/"));
	//only correct fot ptys named /dev/tty[pqr][0-9a-z] 
	strcpy(entry.ut_id, ttyname(STDIN_FILENO) + strlen("/dev/tty"));
	time(&entry.ut_time);
	strcpy(entry.ut_user, getpwuid(getuid())->pw_name);
	memset(entry.ut_host, 0, UT_HOSTSIZE);
	entry.ut_addr = 0;
	setutent();
	pututline(&entry);
	system("echo after adding entry:;who");
	entry.ut_type = DEAD_PEOCESS;
	memset(entry.ut_line, 0, UT_LINESIZE);
	entry.ut_time = 0;
	memset(entry.ut_user, 0, UT_NAMESIZE);
	setutent();
	pututline(&entry);
	system("echo after removing entry:;who");
	endutent();
	exit(EXIT_SUCCESS);
}
**/
void read_wtmp()
{
	int fd;
	int read_size;
	int read_result;
	struct utmp *read_utmp;
	fd = open_file("/var/log/wtmp");
	read_size = sizeof(struct utmp);
//	printf("read_size:%d\n", read_size);
	read_utmp = malloc(read_size);
	read_result = read(fd, read_utmp, read_size);
//	printf("Read %d byte\n", read_result);
//	printf("user\ttty\thost\tdate\n");
	while(read_result > 0) {
		if(strlen(read_utmp->ut_user)!=0 && strlen(read_utmp->ut_host)!=0)
			printf("%s\t%s\t%s\t\n", read_utmp->ut_user, read_utmp->ut_line, read_utmp->ut_host);
		read_result = read(fd, read_utmp, read_size);
	}
}

int write_utmp()
{
	int write_fd, read_fd, size, read_result;
	struct utmp* operat_utmp;
	read_fd = open_file("/var/log/wtmp");
	write_fd = open("./test_wtmp", O_WRONLY|O_CREAT);
	if(read_fd < 0)
		return read_fd;
	size = sizeof(struct utmp);
	operat_utmp = malloc(size);
	read_result = read(read_fd, operat_utmp, size);
	while(read_result > 0) {
		read_result = write(write_fd, operat_utmp, read_result);
		if(read_result < 0){
			printf("write file fail\n");
			return read_result;
		}
		read_result = read(read_fd, operat_utmp, size);
	}
	printf("copy file finished");
	return 0;
}

int main()
{
	read_wtmp();
	write_utmp();
	exit(EXIT_SUCCESS);
}
