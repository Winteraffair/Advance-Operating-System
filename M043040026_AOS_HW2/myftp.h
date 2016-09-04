

#include	<stdio.h>
#include	<sys/socket.h>
#include	<string.h>
#include	<arpa/inet.h>
#include	<stdlib.h>
#include	<netdb.h>
#include	<unistd.h>
#include	<netinet/in.h>
#include	<net/if.h>
#include	<linux/sockios.h>
#include	<time.h>
#include	<sys/times.h>
#include	<errno.h>
#include	<signal.h>
#include	<sys/select.h>
#include	<sys/stat.h>
#include	<sys/ioctl.h>
#include	<pthread.h>
#include	<unistd.h>


struct capablity{
  int order;
  char name[10];
  char group[10];
  char data_name[20];
  char privilege[7];
  int size;
  char time[20];
  int rw_stat[2];
  struct capablity *next;
};

struct host{
  char name[10];
  char group[10];
};

void insert_cap(struct capablity *node,int *order,char *data_name, char *privilege,char *name,int *group);
void print_cap(struct capablity *list,struct host *host_list,int host_num);

int  check_routine(struct capablity *list,int socket ,int mode,char *databuf);
void new_option(int socket,char *option,char *databuf,struct capablity *list,int *order,char *name,int *group);
void read_option(int socket,char *option,char *databuf,struct capablity *list,char *name,int *group);
void write_option(int socket,char *option,char *databuf,struct capablity *list,char *name,int *group);
void change_option(int socket,char *option,char *databuf,struct capablity *list,char *name,int *group);
void information_option(int socket,char *option,char *databuf,struct capablity *list,char *name,int *group);

void new_client(int socket,char *option,char *databuf);
void read_client(int socket,char *option,char *databuf);
void write_client(int socket,char *option,char *databuf);
void change_client(int socket,char *option,char *databuf);
void information_client(int socket,char *option,char *databuf);




