//============================================================================
// Name        : usb_daq_user.cpp
// Author      : zhuce
// Version     :
// Copyright   : esss
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


#define IOCTL_BLK_DAQ_START_WRITE				_IOWR('x', 0, int)
#define IOCTL_BLK_DAQ_STOP_WRITE				_IOWR('x', 1, int)

int main()
{
	int i = 0;
	int rt = 0;
	int fd = 0;
	int fd2 = 0;
	char* buf = new char[1024*8];
	unsigned long wr_size = 2048;

	fd = open("/home/zhuce/stm32/reborn.txt", O_CREAT | O_RDWR | O_SYNC);
	if (fd == -1) {
		printf("Cann't open file /home/zhuce/stm32/reborn.txt error: %d\n", errno);
		return -1;
	}

	memset(buf, 0x0f, 2048);

	lseek(fd, 0, SEEK_SET);
	rt = write(fd, buf, 2048);

	if(rt == -1)
	{
		printf("write error: %d, Mesg: %s\n", errno, strerror(errno));
		return -1;
	}

	memset(buf, 0, 8*1024);

	lseek(fd, 0, SEEK_SET);
	rt = read(fd, buf, 8*1024);

	if(rt == -1)
	{
		printf("read error: %d, Mesg: %s\n", errno, strerror(errno));
		return -1;
	}

	close(fd);

	fd = open("/home/zhuce/stm32/reborn2.txt", O_CREAT | O_RDWR | O_SYNC);
	if (fd == -1) {
		printf("Cann't open file /home/zhuce/stm32/reborn2.txt error: %d\n", errno);
		return -1;
	}

	fd2 = open("/dev/usb_daq0", O_RDWR);
	if (fd2 == -1) {
		printf("Cann't open file /dev/usb_daq0 error: %d\n", errno);
		return -1;
	}

	wr_size = 2048;
	rt = ioctl(fd2, IOCTL_BLK_DAQ_START_WRITE, &wr_size);
	if (rt == 0) {
		printf("IOCTL_BLK_DAQ_START_WRITE error !\n");
		printf("errno: %d, msg: %s\n", errno, strerror(errno));
		return -1;
	}

	memset(buf, i, 2048);
	lseek(fd, 0, SEEK_SET);
	rt = write(fd, buf, 2048);

	if(rt == -1)
	{
		printf("write error: %d, Mesg: %s\n", errno, strerror(errno));
		return -1;
	}

	memset(buf, 0, 8*1024);
	lseek(fd, 0, SEEK_SET);
	rt = read(fd, buf, 8*1024);

	if(rt == -1)
	{
		printf("read error: %d, Mesg: %s\n", errno, strerror(errno));
		return -1;
	}

	close(fd2);
	close(fd);

	delete buf;
	return 0;
}

