#include "myftp.h"

void *client_thread(void *socket);

struct capablity *data_list,*data_pointer;
struct host host_list[10];
int cap_num=0,host_num=0;

int main(int argc , char *argv[]){

	int server_socket,client_socket,len;
	struct sockaddr_in server,client;
	pthread_t thread_id;
	char buf[50];
	
	if(argc < 2){
	  printf("Usage : ./Server [port]\n");
	  exit(0);	
	}
	if((server_socket = socket(AF_INET,SOCK_STREAM,0))==-1){
	  perror("socket");
	  exit(1);	
	}
	//change working directory
	getcwd(buf,sizeof(buf));
	sprintf(buf,"%s%s",buf,"/server");
	chdir(buf);

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	len = sizeof(server);	
	
	if(bind(server_socket,(struct sockaddr *)&server,len)==-1){
	  perror("socket");
	  exit(1);	
	}
	
	if(listen(server_socket,4)==-1){
	  perror("socket");
	  exit(1);	
	}
	printf("[ Server Initial Complete ]\n---------------------------------------\nWaiting Client\n");
	
	data_list = (struct capablity *)malloc(sizeof(struct capablity));
	data_list->order=0;
	data_list->next=NULL;
	data_pointer=data_list;	
	cap_num++;

	while(client_socket = accept(server_socket,(struct sockaddr *)&client,&len)){
	  
	  if(pthread_create(&thread_id,NULL,client_thread,(void *)&client_socket)==-1){
	    perror("socket");
	    exit(1);
	  }	
		
	}

	return 0;
}

void *client_thread(void *socket){
	int sockfd = *(int*)socket;
	int group=0,i,end=0,rcvsize=0;
	char name[10],option[50],databuf[1000];
	//get name
	if(recv(sockfd,name,sizeof(name),0)==-1){
	  perror("recv");
	  exit(1);  	
	}
	//send ok
	if(send(sockfd,&group,sizeof(group),0)==-1){
	  perror("send");
	  exit(1);		
	}
	//get group number
	if(recv(sockfd,&group,sizeof(group),0)==-1){
	  perror("recv");
	  exit(1);  	
	}
	switch(group){
	  case 1:
	    printf("Welcome [%s] in Group [AOS]\n",name);
	    strcpy(host_list[host_num].name,name);
	    strcpy(host_list[host_num].group,"AOS");
	    host_num++;
	    break;
	  case 2:
	    printf("Welcome [%s] in Group [CSE]",name);
	    strcpy(host_list[host_num].name,name);
	    strcpy(host_list[host_num].group,"CSE");
	    host_num++;
	    break;
	  case 3:
	    printf("Welcome [%s] in Group [OTHER]\n",name);
	    strcpy(host_list[host_num].name,name);
	    strcpy(host_list[host_num].group,"OTHER");
	    host_num++;
	    break;
	  default:
	    break;
	}
	fflush(stdout);
	//insert_cap(data_pointer,&cap_num,name,"rwrwrw");
	/*while(data_pointer->next != NULL){
	  data_pointer= data_pointer->next; 	
	}
	//printf("%d %s %s\n\n",data_list->next->order,data_list->next->data_name,data_list->next->privilege);
	//printf("%d %s %s\n\n",data_pointer->order,data_pointer->data_name,data_pointer->privilege);*/

	memset(databuf,'\0',sizeof(databuf));
	if(cap_num == 1){strcpy(databuf,"NULL\n");}
	else{
	  data_pointer = data_list->next;
	  sprintf(databuf,"%s : %s\n",data_pointer->data_name,data_pointer->privilege);
	  for(i=2;i < cap_num;i++){
	     data_pointer = data_pointer->next;
	     sprintf(databuf,"%s%s : %s\n",databuf,data_pointer->data_name,data_pointer->privilege);
	  }	
	}
	if(send(sockfd,databuf,sizeof(databuf),0)==-1){
	  perror("send");
	  exit(1);		
	}
	while(end==0){
	  //recv command
	  //printf("[%s] still Server\n",name);
	  memset(option,'\0',sizeof(option));
	  if((rcvsize = recv(sockfd,option,sizeof(option),0))>0){
  	
	  
	  switch(option[0]){
		case 'n':
			new_option(sockfd,option,databuf,data_list,&cap_num,name,&group);
			print_cap(data_list,host_list,host_num);
			break;
		case 'r':
			read_option(sockfd,option,databuf,data_list,name,&group);
			break;
		case 'w':
			write_option(sockfd,option,databuf,data_list,name,&group);
			print_cap(data_list,host_list,host_num);
			break;
		case 'c':
			change_option(sockfd,option,databuf,data_list,name,&group);
			print_cap(data_list,host_list,host_num);
			break;
		case 'i':
			information_option(sockfd,option,databuf,data_list,name,&group);
			break;
		case 'b':
			end=1;
			break;
		default:
			break;
	  }
	  }
	}

	printf("[%s] leave Server\n",name);

	pthread_exit(NULL);

}
