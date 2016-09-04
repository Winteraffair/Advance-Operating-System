#include "myftp.h"

void insert_cap(struct capablity *node,int *order,char *data_name, char *privilege,char *name,int *group){
	struct tm *timeinfo;
	time_t t;
	char timestr[20];

	time(&t);
	timeinfo = localtime(&t);
	strftime(timestr,20,"%b %d %Y",timeinfo);

	while(node->next != NULL){
	  node = node->next;	
	}
	
	node->next = (struct capablity *)malloc(sizeof(struct capablity));
	node=node->next;

	memset(node,'\0',sizeof(struct capablity));
	//order of data
	node->order = *order;
	*order=*order+1;
	//copy name, dataname, privilege, group;
	strcpy(node->name,name);
	strcpy(node->data_name,data_name);
	strcpy(node->privilege,privilege);
	switch(*group){
	  case 1:
		strcpy(node->group,"AOS");
		break;
	  case 2:
		strcpy(node->group,"CSE");
		break;
	  case 3:
		strcpy(node->group,"OTHER");
		break;	
	}
	//time
	strcpy(node->time,timestr);
	//rw_stat initial
	node->rw_stat[0]=0;
	node->rw_stat[1]=0;
	//new data size = 0;
	node->size = 0;
	node->next = NULL;
	   
	

	return;
}
void print_cap(struct capablity *list,struct host *host_list,int host_num){
	int i,check[2]={0};
	struct capablity *start = list;
	printf("---------------------------------------\nCapability List State :\n");
	for(i=0;i<host_num;i++){
	  printf("%s",host_list[i].name);
	  while(list->next != NULL){
	    list = list->next; 
	    if(strcmp(list->name,host_list[i].name)==0){
	      if(list->privilege[0] == 'r'){check[0]=1;}
	      if(list->privilege[1] == 'w'){check[1]=1;}
	      if(check[0]==1 || check[1]==1){
		printf(" -> (%s,",list->data_name);
	        if(check[0]==1){printf("r");}
		if(check[1]==1){printf("w");}
		printf(")");
	      }
	    }
	    else if(strcmp(list->group,host_list[i].group)==0){
	      if(list->privilege[2] == 'r'){check[0]=1;}
	      if(list->privilege[3] == 'w'){check[1]=1;}
	      if(check[0]==1 || check[1]==1){
		printf("-> (%s,",list->data_name);
	        if(check[0]==1){printf("r");}
		if(check[1]==1){printf("w");}
		printf(")");
	      }
	    }
	    else{
	      if(list->privilege[4] == 'r'){check[0]=1;}
	      if(list->privilege[5] == 'w'){check[1]=1;}
	      if(check[0]==1 || check[1]==1){
		printf("-> (%s,",list->data_name);
	        if(check[0]==1){printf("r");}
		if(check[1]==1){printf("w");}
		printf(")");
	      }
	    }
	    check[0]=0;
	    check[1]=0;
	  }
	  printf("\n");
 	  list = start;
	}
	printf("---------------------------------------\n");
	fflush(stdout);
	return;
}
int check_routine(struct capablity *list,int socket ,int mode,char *databuf){
	switch(mode){
	  case 1:
		if(list->rw_stat[1] == 0){
		  list->rw_stat[0] =  list->rw_stat[0] + 1;
		  return 1;		
		}
		else{
		  memset(databuf,'\0',1000);
	    	  strcpy(databuf,"This File is being written !\n");
	   	  if(send(socket,databuf,1000,0)==-1){
	     	    perror("send");
	     	    exit(1);		
	  	  }
		  return 0;
		}
	  case 2:
		list->rw_stat[0] = list->rw_stat[0] - 1;
		return 1;		
	  case 3:
		if(list->rw_stat[0] == 0 && list->rw_stat[1] == 0 ){
	  	  list->rw_stat[1] = list->rw_stat[1] + 1;
		  return 1;		
		}
		else{		
		  memset(databuf,'\0',1000);
	    	  strcpy(databuf,"This File is being read or written!\n");
	   	  if(send(socket,databuf,1000,0)==-1){
	     	    perror("send");
	     	    exit(1);		
	  	  }
		  return 0;
		}
	  case 4:
	  	list->rw_stat[1] = list->rw_stat[1] - 1;
		return 1;
	}
}
//server side
void new_option(int socket,char *option,char *databuf,struct capablity *list,int *order,char *name,int *group){
	char delim[]=" \n\t";
	char data_name[20];
	char privilege[7];
	char *token;
	int first=0;
	FILE *in;
	
	

	strtok(option,delim);
	token = strtok(NULL,delim);
	strcpy(data_name,token);
	token = strtok(NULL,delim);
	strcpy(privilege,token);
	
	memset(databuf,'\0',1000);
	while(list->next != NULL){
	  list = list->next;
	  if(strcmp(list->data_name,data_name)==0){
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"NULL");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    return;
	  }
	  else{
	    if(first==0){
	      sprintf(databuf,"%s %s %s %d %s %s\n",list->privilege,list->name,list->group,list->size,list->time,list->data_name);
	      first++;
	    }
	    else
	      sprintf(databuf,"%s%s %s %s %d %s %s\n",databuf,list->privilege,list->name,list->group,list->size,list->time,list->data_name);
	  }	
	}
	if((in = fopen(data_name,"w+"))==NULL){
	  perror("fopen");
	  exit(1);	
	}
	fclose(in);
	insert_cap(list,order,data_name,privilege,name,group);
	list = list->next;
	sprintf(databuf,"%s%s %s %s %d %s %s\n",databuf,list->privilege,list->name,list->group,list->size,list->time,list->data_name);
	printf("\n\n[%s] use ""new"" command\n",name);

	if(send(socket,databuf,1000,0)==-1){
	  perror("send");
	  exit(1);		
	}
	
	
	return;
	
}
void read_option(int socket,char *option,char *databuf,struct capablity *list,char *name,int *group){
	char delim[]=" \n\t";
	char data_name[20],check[20],group_check[10];
	char privilege[7];
	char *token;
	int first=0,readsize=0,size,block=0;
	FILE *in;
	
	
	strtok(option,delim);
	token = strtok(NULL,delim);
	strcpy(data_name,token);

	//check data exist
	if(list->next == NULL){
	   memset(databuf,'\0',1000);
	   strcpy(databuf,"NULL");
	   if(send(socket,databuf,1000,0)==-1){
	     perror("send");
	     exit(1);		
	   }
	   return;
	}
	while(list->next != NULL){
	  list = list->next;
	  if(strcmp(list->data_name,data_name)==0){
	    break;
	  }
	  else if(list->next == NULL){
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"NULL");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    return;
	  }
	}
	switch(*group){
	  case 1:
		strcpy(group_check,"AOS");
		break;
	  case 2:
		strcpy(group_check,"CSE");
		break;
	  case 3:
		strcpy(group_check,"OTHER");
		break;	
	}

	//check user privilege
	while(1){
	if(strcmp(list->name,name)==0){
	  if(list->privilege[0]=='r'){
	    break;
	  }
	  else{
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"You don't have enough privilege!\n");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  return;
	}
	else if(strcmp(list->group,group_check)==0){
	  if(list->privilege[2]=='r'){
	    break;
	  }
	  else{
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"You don't have enough privilege!\n");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  return;
	}
	else{
	  if(list->privilege[4]=='r'){
	    break;
	  }
	  else{
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"You don't have enough privilege!\n");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  return;
	}
	}
	//check new data
	if(list->size == 0){
	  memset(databuf,'\0',1000);
	  strcpy(databuf,"Data size is 0 , no one write it!\n");
	  if(send(socket,databuf,1000,0)==-1){
	    perror("send");
	    exit(1);		
	  }
	  return;	
	}
	//check rw_stat
	if(check_routine(list,socket,1,databuf)==0){
	  return;	
	}
	//transmit start
	printf("\n\n[%s] use ""read"" command\n",name);
	printf("-------------------------------------\nTransmit Start : File [%s]\n",data_name);
	memset(databuf,'\0',1000);
	strcpy(databuf,"Data Tansmit Start!\n");
	if(send(socket,databuf,1000,0)==-1){
	   perror("send");
	   exit(1);		
	}
	memset(databuf,'\0',1000);
	
	if((in = fopen(data_name,"r"))==NULL){
	  perror("fopen");
	  exit(1);
	}
	
	while((readsize = fread(databuf,1,1000,in))>0){
	  if(block == 10){
	    if(recv(socket,check,20,0)==-1){
	      perror("recv");
	      exit(1);  	
	    }
	    block=0;
	  }
	  if(readsize < 1000){
	    if(send(socket,databuf,readsize,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    break;
	  }
	  else{
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  block++;
	}
	
	size = ftell(in);
	printf("Transmit End : File [%s] %d bytes\n-------------------------------------\n",data_name,size);
	//change rw_stat
	check_routine(list,socket,2,databuf);
	
	
	fclose(in);
	return;
	
}
void write_option(int socket,char *option,char *databuf,struct capablity *list,char *name,int *group){
	char delim[]=" \n\t";
	char data_name[20],check[20],group_check[10];
	char oa[5];
	char *token;
	int first=0,readsize=0,size,block=1;
	FILE *in;
	struct capablity *start=list;
	
	strtok(option,delim);
	token = strtok(NULL,delim);
	strcpy(data_name,token);
	token = strtok(NULL,delim);
	strcpy(oa,token);


	//check data exist
	if(list->next == NULL){
	   memset(databuf,'\0',1000);
	   strcpy(databuf,"NULL");
	   if(send(socket,databuf,1000,0)==-1){
	     perror("send");
	     exit(1);		
	   }
	   return;
	}
	while(list->next != NULL){
	  list = list->next;
	  if(strcmp(list->data_name,data_name)==0){
	    break;
	  }
	  else if(list->next == NULL){
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"NULL");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    return;
	  }
	}
	switch(*group){
	  case 1:
		strcpy(group_check,"AOS");
		break;
	  case 2:
		strcpy(group_check,"CSE");
		break;
	  case 3:
		strcpy(group_check,"OTHER");
		break;	
	}
	//check user privilege
	while(1){
	if(strcmp(list->name,name)==0){
	  if(list->privilege[1]=='w'){
	    break;
	  }
	  else{
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"You don't have enough privilege!\n");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  return;
	}
	else if(strcmp(list->group,group_check)==0){
	  if(list->privilege[3]=='w'){
	    break;
	  }
	  else{
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"You don't have enough privilege!\n");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  return;
	}
	else{
	  if(list->privilege[5]=='w'){
	    break;
	  }
	  else{
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"You don't have enough privilege!\n");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  return;
	}
	}
	//check o or a
	if(oa[0]=='o'){
	  if((in = fopen(data_name,"w"))==NULL){
	    perror("fopen");
	    exit(1);
	  }
	}
	else if(oa[0]=='a'){
	  if((in = fopen(data_name,"a"))==NULL){
	    perror("fopen");
	    exit(1);
	  }
	}
	else{
	  memset(databuf,'\0',1000);
	  strcpy(databuf,"Wrong Command !\n");
	  if(send(socket,databuf,1000,0)==-1){
	    perror("send");
	    exit(1);		
	  }	
	}
	// check rw_stat
	if(check_routine(list,socket,3,databuf)==0){
	  return;	
	}
	//ready to recv file
	memset(databuf,'\0',1000);
	strcpy(databuf,"Start Transmit File\n");
	if(send(socket,databuf,1000,0)==-1){
	  perror("send");
	  exit(1);		
	}
	printf("\n\n[%s] use ""write"" command\n",name);
	printf("-------------------------------------\nStart Receive File [%s]\n",data_name);
	
	while((readsize = recv(socket,databuf,1000,0))>0){
	  if(block==10){
	    strcpy(check,"block ok\n");
	    if(send(socket,check,20,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    block=0;
	  }
	  if(readsize < 1000){
	    fwrite(databuf,readsize,1,in);
	    break;
	  }
	  else{
	    fwrite(databuf,1000,1,in);
	  }
	  block++;	
	}
	fseek(in,0L,SEEK_END);
	size = ftell(in);
	list->size = size;
	printf("Recv file [%s] %d bytes\n-------------------------------------\n",data_name,size);
	
	check_routine(list,socket ,4,databuf);
	fclose(in);
	
	return;
}
void change_option(int socket,char *option,char *databuf,struct capablity *list,char *name,int *group){
	char delim[]=" \n\t";
	char data_name[20];
	char privilege[7];
	char *token;
	int first=0;
	struct capablity *start = list;

	strtok(option,delim);
	token = strtok(NULL,delim);
	strcpy(data_name,token);
	token = strtok(NULL,delim);
	strcpy(privilege,token);
	//check data exist
	if(list->next == NULL){
	   memset(databuf,'\0',1000);
	   strcpy(databuf,"NULL");
	   if(send(socket,databuf,1000,0)==-1){
	     perror("send");
	     exit(1);		
	   }
	}
	while(list->next != NULL){
	  list = list->next;
	  if(strcmp(list->data_name,data_name)==0){
	    break;
	  }
	  else if(list->next == NULL){
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"NULL");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    return;
	  }
	}
	//check privilege
	while(1){
	if(strcmp(list->name,name)==0){
	  /*if(list->privilege[0]=='r'){
	    break;
	  }
	  else{
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"You don't have enough privilege!\n");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  return;*/
	  break;
	}
	else{
	  memset(databuf,'\0',1000);
	  strcpy(databuf,"You don't have enough privilege!\n");
	  if(send(socket,databuf,1000,0)==-1){
	    perror("send");
	    exit(1);		
	  }	
	  return;
	}
	}
	strcpy(list->privilege,privilege);

	printf("\n\n[%s] use ""change"" command\n",name);
	//printf("---------------------------------------\nCapability List State :\n%s---------------------------------------\n",databuf);
	memset(databuf,'\0',1000);
	strcpy(databuf,"Change Success!\n");
	if(send(socket,databuf,1000,0)==-1){
	   perror("send");
	   exit(1);		
	}
	fflush(stdout);
	return;
}
void information_option(int socket,char *option,char *databuf,struct capablity *list,char *name,int *group){
	char delim[]=" \n\t";
	char data_name[20];
	char *token;
	
	
	strtok(option,delim);
	token = strtok(NULL,delim);
	strcpy(data_name,token);

	while(list->next != NULL){
	  list = list->next;
	  if(strcmp(list->data_name,data_name)==0){
	    break;
	  }
	  else if(list->next == NULL){
	    memset(databuf,'\0',1000);
	    strcpy(databuf,"NULL");
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    return;
	  }
	}
	
	memset(databuf,'\0',1000);
	sprintf(databuf,"%s %s %s %d %s %s\n",list->privilege,list->name,list->group,list->size,list->time,list->data_name);
	if(send(socket,databuf,1000,0)==-1){
	  perror("send");
	  exit(1);
	}
	return;	

}
//client side
void new_client(int socket,char *option,char *databuf){

	if(send(socket,option,50,0)==-1){
	  perror("send");
	  exit(1);		
	}
	if(recv(socket,databuf,1000,0)==-1){
	  perror("recv");
	  exit(1);  	
	}
	if(strcmp(databuf,"NULL")==0){
	  printf("\nThis file is already exist !\n");
	  return;	
	}
	

	printf("\n%s\n",databuf);
	fflush(stdout);
	return;
}
void read_client(int socket,char *option,char *databuf){
	int readsize=0,size,block=1;	
	FILE *out;
	char delim[]=" \n\t";
	char data_name[20],check[20];
	char *token;
	


	if(send(socket,option,50,0)==-1){
	  perror("send");
	  exit(1);		
	}
	if(recv(socket,databuf,1000,0)==-1){
	  perror("recv");
	  exit(1);  	
	}

	if(strcmp(databuf,"NULL")==0){
	  printf("This file isn't exist !\n");
	  return;	
	}
	else if(strcmp(databuf,"You don't have enough privilege!\n")==0){
	    printf("\n%s\n",databuf);
	    return;
	}
	else if(strcmp(databuf,"Data size is 0 , no one write it!\n")==0){
	    printf("\n%s\n",databuf);
	    return;
	}
	else if(strcmp(databuf,"This File is being written !\n")==0){
	    printf("\n%s\n",databuf);
	    return;
	}
	else
	  printf("\n%s",databuf);
	
	strtok(option,delim);
	token = strtok(NULL,delim);
	strcpy(data_name,token);

	//recv data
	if((out = fopen(data_name,"w+"))==NULL){
	  perror("fopen");
	  exit(1);	
	}
	while((readsize = recv(socket,databuf,1000,0))>0){
	  if(block==10){
	    strcpy(check,"block ok\n");
	    if(send(socket,check,20,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    block=0;
	  }
	  if(readsize < 1000){
	    fwrite(databuf,readsize,1,out);
	    break;
	  }
	  else{
	    fwrite(databuf,1000,1,out);
	  }
	  block++;	
	}
	fseek(out,0L,SEEK_END);
	size = ftell(out);
	printf("Recv file [%s] %d bytes\n\n",data_name,size);
	fclose(out);
	return;

}
void write_client(int socket,char *option,char *databuf){
	int readsize=0,size,block=0;	
	FILE *out;
	char delim[]=" \n\t";
	char data_name[20],copy_option[50],check[20];
	char *token;	
	struct stat st;

	strcpy(copy_option,option);
	strtok(copy_option,delim);
	token = strtok(NULL,delim);
	strcpy(data_name,token);


	if(stat(data_name,&st)==-1){
	  printf("This file isn't exist in Your side\n");
	  return;	
	}
	if(send(socket,option,50,0)==-1){
	  perror("send");
	  exit(1);		
	}

	if(recv(socket,databuf,1000,0)==-1){
	  perror("recv");
	  exit(1);  	
	}

	if(strcmp(databuf,"NULL")==0){
	  printf("\nThis file isn't exist in Server, use new command to create it !\n\n");
	  return;	
	}
	else if(strcmp(databuf,"You don't have enough privilege!\n")==0){
	    printf("\n%s\n",databuf);
	    return;
	}
	else if(strcmp(databuf,"Wrong Command !\n")==0){
	    printf("\n%s\n",databuf);
	    return;
	}
	else if(strcmp(databuf,"This File is being read or written!\n")==0){
	    printf("\n%s\n",databuf);
	    return;
	}
	else
	  printf("\n%s",databuf);

	//send data
	if((out = fopen(data_name,"r"))==NULL){
	  perror("fopen");
	  exit(1);	
	}
	
	while((readsize = fread(databuf,1,1000,out))>0){
	  if(block == 10){
	    if(recv(socket,check,20,0)==-1){
	      perror("recv");
	      exit(1);  	
	    }
	    block=0;
	  }
	  if(readsize < 1000){
	    if(send(socket,databuf,readsize,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	    break;
	  }
	  else{
	    if(send(socket,databuf,1000,0)==-1){
	      perror("send");
	      exit(1);		
	    }
	  }
	  block++;
	}

	size = ftell(out);
	printf("Transmit End : File [%s] %d bytes\n\n",data_name,size);
	fclose(out);
	return;
	
	
}
void change_client(int socket,char *option,char *databuf){
	if(send(socket,option,50,0)==-1){
	  perror("send");
	  exit(1);		
	}
	if(recv(socket,databuf,1000,0)==-1){
	  perror("recv");
	  exit(1);  	
	}
	if(strcmp(databuf,"NULL")==0){
	  printf("This file isn't exist !\n");
	  return;	
	}

	printf("\n%s\n",databuf);
	fflush(stdout);
	return;
}
void information_client(int socket,char *option,char *databuf){
	if(send(socket,option,50,0)==-1){
	  perror("send");
	  exit(1);		
	}
	if(recv(socket,databuf,1000,0)==-1){
	  perror("recv");
	  exit(1);  	
	}
	if(strcmp(databuf,"NULL")==0){
	  printf("This file isn't exist !\n");
	  return;	
	}

	printf("\n%s\n",databuf);
	fflush(stdout);
	return;
}

