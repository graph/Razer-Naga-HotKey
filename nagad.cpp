// I took this http://www.thelinuxdaily.com/2010/05/grab-raw-keyboard-input-from-event-device-node-devinputevent/
// and modified to suit needs

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

void handler (int sig)
{
  printf ("\nexiting...(%d)\n", sig);
  exit (0);
}

void perror_exit (const char *error)
{
  perror (error);
  handler (9);
}

char basedir[1024];
void init(){
	memset(basedir, 0, sizeof(basedir));
	strcpy(basedir, getenv("HOME"));
	strcat(basedir, "/.config/naga");
	printf("Using directory %s\n", basedir);
	
	char xauth[2048];
	memset(xauth, 0, sizeof(xauth));
	sprintf(xauth, "%s/.Xauthority", getenv("HOME"));
	setenv("XAUTHORITY", xauth, 1);
	setenv("DISPLAY", ":0.0", 1);
	system("/usr/local/bin/naga_disablekbd.pl");
}


void runcmd(int button){
	// button 1-12 pressed on the naga 
	static char cmd[2048];
	
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "%s/button_%d 2>&1 >> %s/log &", basedir, button, basedir);
	system(cmd);
}

void docode(int code){
	static char map[256];
	static int init = 0;
	if(init == 0){
		memset(map, 0, sizeof(map));
		// when in num position
		map[79] = 1;
		map[80] = 2;
		map[81] = 3;
		
		map[75] = 4;
		map[76] = 5;
		map[77] = 6;
		
		map[71] = 7;
		map[72] = 8;
		map[73] = 9;
		
		map[82] = 10;
		map[74] = 11;
		map[78] = 12;

		// when in 123 position
		map[2] = 1;
		map[3] = 2;
		map[4] = 3;
		
		map[5] = 4;
		map[6] = 5;
		map[7] = 6;
		
		map[8] = 7;
		map[9] = 8;
		map[10] = 9;
		
		map[11] = 10;
		map[12] = 11;
		map[13] = 12;

	}
	if(code > 0 && code <= 255 && map[code])
		runcmd(map[code]);
	
}
int main (int argc, char *argv[])
{
  struct input_event ev[64];
  int fd, rd, value, size = sizeof (struct input_event);
  char name[256] = "Unknown";
  const char *device = NULL;
	if(daemon(0,0)){
		printf("Could not start as daemon\n");
		return -1;
	}

    device = "/dev/naga_keyboard";

  //Open Device
  if ((fd = open (device, O_RDONLY)) == -1){
    printf ("%s is not a vaild device.\n", device);
	return -1;
	}
  //Print Device Name
  ioctl (fd, EVIOCGNAME (sizeof (name)), name);
  printf ("Reading From : %s (%s)\n", device, name);
	init();
  while (1){
      if ((rd = read (fd, ev, size * 64)) < size)
          perror_exit ("read()");      

      value = ev[0].value;

      if (value != ' ' && ev[1].value == 1 && ev[1].type == 1){ // Only read the key press event
	   printf ("Code[%d]\n", (ev[1].code));
	   docode(ev[1].code);
      }
  }

  return 0;
}
