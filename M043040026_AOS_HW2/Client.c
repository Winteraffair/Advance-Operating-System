#include "myftp.h"

int main(int argc , char *argv[]){
	int client_socket,len,group=0,end=0;
	struct sockaddr_in server;
	char name[10],option[50],databuf[1000],buf[50];;

	getcwd(buf,sizeof(buf));
	sprintf(buf,"%s%s",buf,"/client");
	chdir(buf);

	if((client_socket=socket(AF_INET,SOCK_STREAM,0))==-1){
	  perror("socket");
	  exit(1);	
	}
	
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	len = sizeof(server);

	if(connect(client_socket,(struct sockaddr *)&server,len)==-1){
	  perror("connect");
	  exit(1);	
	}
	
	printf("Server Connect !\n");
	//get name
	printf("Please Enter Your Name:");
	memset(name,'\0',sizeof(name));
	gets(name);
	if(send(client_socket,name,sizeof(name),0)==-1){
	  perror("send");
	  exit(1);		
	}
	printf("get name\n");
	//wait reply
	if(recv(client_socket,&group,sizeof(group),0)==-1){
	  perror("recv");
	  exit(1);  	
	}	
	//get group
	printf("Please Choose Your Group:(1)AOS (2)CSE (3)OTHER:");
	scanf("%d",&group);
	while(group < 1 || group > 3){
	  printf("You Typed Wrong number , ");
	  printf("Please Choose Your Group:(1)AOS (2)CSE (3)OTHER:");
	  scanf("%d",&group);
	}

	if(send(client_socket,&group,sizeof(group),0)==-1){
	  perror("send");
	  exit(1);		
	}
	//wait reply server data
	if(recv(client_socket,databuf,sizeof(databuf),0)==-1){
	  perror("recv");
	  exit(1);  	
	}
	gets(option);
	printf("------------------Server Data --------------------\n%s--------------------------------------------------\n",databuf);
	//---------------start command---------------
	printf("commmand :\n(1)new <data_name> <privilege>\n(2)read <data_name>\n(3)write <data_name> o/a\n(4)change <data_name> <privilege>\n(5)information <data_name>\n(6)bye\n");
	printf("---------------------------------------------------\n");
	while(end==0){
	  printf("Please Enter Command :");
	  memset(option,'\0',sizeof(option));
	  gets(option);
	  switch(option[0]){
		case 'n':
			new_client(client_socket,option,databuf);
			break;
		case 'r':
			read_client(client_socket,option,databuf);
			break;
		case 'w':
			write_client(client_socket,option,databuf);
			break;
		case 'c':
			change_client(client_socket,option,databuf);
			break;
		case 'i':
			information_client(client_socket,option,databuf);
			break;
		case 'b':
			end=1;
			if(send(client_socket,option,sizeof(option),0)==-1){
	  		  perror("send");
	  		  exit(1);		
			}
			break;
		default:
			printf("Wrong command , Please type again !\n");
			break;
	  }
	}
	return 0;
}


