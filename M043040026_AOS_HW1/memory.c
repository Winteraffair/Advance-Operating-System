#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>

int testcount=60000;
int number_set[101]={0};
int number_long=0;

int start_frame=10;
int end_frame = 60;
int save_frame = 0;
void getnumberset(void);
void FIFO(int *,int );
void optimal(int *,int );
void enhance_second_chance(int *,float,int);
void myself_range(int *,int);


int main(void){
  //0=page fault , 1=writing to disk , 2=interrupt
  int saveall[3][7]={0};
  char name[4][20]={"Page Fault","Writing Disk","Interrupt","Frames"};
  int count;
  float m,n;
  int randtest=0;
  int s1,s2;
  
  srand(time(NULL));
 
  
  while(randtest==0){
    printf("(0)FIFO (1)Optimal (2)Enhance-second-chance (3)Myself_range (4)Quit:");
    scanf("%d",&s1);
    switch(s1){
      case 0:
        printf("(0)Random (1)Set Array:");
        scanf("%d",&s2);
        switch(s2){
          case 0:
            //FIFO output
            printf("-------------------------------------------------------\n"); 
            FIFO(saveall,0);
            printf("Algorithm : FIFO\n");
            printf("Normal Random\n");
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            for(count=1;count<=6;count++){
              printf("[%s:%d]%10d%15d%15d\n",name[3],count*10,saveall[0][count],saveall[1][count],saveall[2][count]); 
            } 
   
            FIFO(saveall,1);
            printf("Locality Random\n");
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            for(count=1;count<=6;count++){
              printf("[%s:%d]%10d%15d%15d\n",name[3],count*10,saveall[0][count],saveall[1][count],saveall[2][count]); 
            }
            printf("-------------------------------------------------------\n"); 
            break;
          case 1:
            getnumberset();
            printf("-------------------------------------------------------\n");
            printf("Algorithm : FIFO\n");
            printf("Set Array Test\n"); 
            FIFO(saveall,2);
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            //for(count=1;count<=6;count++){
            printf("[%s:%d]%10d%15d%15d\n",name[3],save_frame,saveall[0][1],saveall[1][1],saveall[2][1]); 
           // }
            printf("-------------------------------------------------------\n");
            number_long=0;  
            break;
        }
        break;
      case 1:
        printf("(0)Random (1)Set Array:");
        scanf("%d",&s2);
        switch(s2){
          case 0:
            printf("-------------------------------------------------------\n"); 
            optimal(saveall,0);
            printf("Algorithm : optimal\n");
            printf("Normal Random\n");
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            for(count=1;count<=6;count++){
              printf("[%s:%d]%10d%15d%15d\n",name[3],count*10,saveall[0][count],saveall[1][count],saveall[2][count]); 
            }
  
            optimal(saveall,1);
            printf("Locality Random\n");
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            for(count=1;count<=6;count++){
              printf("[%s:%d]%10d%15d%15d\n",name[3],count*10,saveall[0][count],saveall[1][count],saveall[2][count]); 
            }
            printf("-------------------------------------------------------\n"); 
            break;
          case 1:
            getnumberset();
            printf("-------------------------------------------------------\n");
            printf("Algorithm : optimal\n");
            printf("Set Array Test\n"); 
            optimal(saveall,2);
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            printf("[%s:%d]%10d%15d%15d\n",name[3],save_frame,saveall[0][1],saveall[1][1],saveall[2][1]);
            printf("-------------------------------------------------------\n"); 
            number_long=0;  
            break;
        }
        break;
      case 2:
        printf("(0)Random (1)Set Array:");
        scanf("%d",&s2);
        switch(s2){
          case 0:
            printf("-------------------------------------------------------\n"); 
            printf("Algorithm : Enhance-second-chance\n");
            printf("Normal Random\n");
            for(m=0.05;m<=0.25;m=m+0.05){
              enhance_second_chance(saveall,m,0);
              printf("Modify set chance:%.2f\n",m);
              printf("%25s%17s%13s\n",name[0],name[1],name[2]);
              for(count=1;count<=6;count++){
                printf("[%s:%d]%10d%15d%15d\n",name[3],count*10,saveall[0][count],saveall[1][count],saveall[2][count]); 
              } 
            }    
            printf("Locality Random\n");
            for(m=0.05;m<=0.25;m=m+0.05){
              enhance_second_chance(saveall,m,1);
              printf("Modify set chance:%.2f\n",m);
              printf("%25s%17s%13s\n",name[0],name[1],name[2]);
              for(count=1;count<=6;count++){
                printf("[%s:%d]%10d%15d%15d\n",name[3],count*10,saveall[0][count],saveall[1][count],saveall[2][count]); 
              } 
            }
            printf("-------------------------------------------------------\n"); 
            break;
          case 1:
            getnumberset();
            printf("-------------------------------------------------------\n");
            printf("Algorithm : Enhance-second-chance\n");
            printf("Set Array Test\n"); 
            for(m=0.05;m<=0.25;m=m+0.05){
              enhance_second_chance(saveall,m,2);
              printf("Modify set chance:%.2f\n",m);
              printf("%25s%17s%13s\n",name[0],name[1],name[2]);
              printf("[%s:%d]%10d%15d%15d\n",name[3],save_frame,saveall[0][1],saveall[1][1],saveall[2][1]);
            }
              printf("-------------------------------------------------------\n");
              number_long=0; 
            break;
        }
        break;
      case 3:
        printf("(0)Random (1)Set Array:");
        scanf("%d",&s2);
        switch(s2){
          case 0:
            myself_range(saveall,0);
            printf("-------------------------------------------------------\n"); 
            printf("Algorithm : myself_range\n");
            printf("Normal Random\n");
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            for(count=1;count<=6;count++){
              printf("[%s:%d]%10d%15d%15d\n",name[3],count*10,saveall[0][count],saveall[1][count],saveall[2][count]); 
            }
  
            myself_range(saveall,1);
            printf("Locality Random\n");
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            for(count=1;count<=6;count++){
              printf("[%s:%d]%10d%15d%15d\n",name[3],count*10,saveall[0][count],saveall[1][count],saveall[2][count]); 
            }
            printf("-------------------------------------------------------\n");   
            break;
          case 1:
            getnumberset();
            myself_range(saveall,2);
            printf("-------------------------------------------------------\n"); 
            printf("Algorithm : myself_range\n");
            printf("Set Array Test\n");
            printf("%25s%17s%13s\n",name[0],name[1],name[2]);
            printf("[%s:%d]%10d%15d%15d\n",name[3],save_frame,saveall[0][1],saveall[1][1],saveall[2][1]);
            number_long=0;
            printf("-------------------------------------------------------\n");   
            break;
        }
        break;
      case 4:
        printf("Bye!!\n");
        exit(0);
        break;
      default:
        printf("Usage [the number you want to printf]\n");
        break;
    }
  }
 
}

void FIFO(int *saveall,int option){
  int page_fault=0,interrupt=0,writing_disk=0;
  int i,j,randnb,count=1,check=0,fullcount=1;
  int queue_array[61]={0}; 
  int randcount=1;
  
  for(i=start_frame;i<=end_frame;i=i+10){
    for(j=1;j<=testcount;j++){
      //random
      if(option==0){
        randnb = (rand()%600)+1;
      }

      else if(option==1){
        if(randcount<=50){
          randnb = (rand()%100)+1;
          randcount++;
        }
        else if(randcount<=100){
         randnb = (rand()%600)+1;
         randcount++;
        }
        else
        randcount=1;
      }
      
      else if(option ==2){
        testcount = number_long;
        randnb=number_set[randcount];
        randcount++;
      }
      
    
  
      //algorithm
      if(fullcount <= i){
        for(count=1;count<fullcount;count++){
          if(queue_array[count]==randnb){
            check = 1;
          }
        }
        if(check == 0){
          queue_array[fullcount]=randnb;
          page_fault++;
          fullcount++;
        }
        check=0;
      }
      else{
        count=1;
        while(count <= i && check == 0){
          if(queue_array[count]==randnb){
            check=1;
          }
          count++;
        }
        if(check==0){
	  page_fault++;
	  for(count=1;count <= i-1;count++){
            queue_array[count]=queue_array[count+1];
          }
	  queue_array[i]=randnb;
        }
      }
      check=0;
    }
     //save and reset
    /*if(i==10){
      if(testcount < i){
        first = page_fault+i;
      }
      else
        first = page_fault;
    }
    if(option !=2){
      *(saveall+(i/10)) = page_fault+i;
    }
    else if(testcount >= i){
      *(saveall+(i/10)) = first;
    }
    else{
      *(saveall+(i/10)) = first;
    }*/
    if(option==2){
      *(saveall+1) = page_fault;
      *(saveall+7+1) = writing_disk;
      *(saveall+14+1) =interrupt;
    }
    else{
      *(saveall+(i/10)) = page_fault;
      *(saveall+7+(i/10)) = writing_disk;
      *(saveall+14+(i/10)) =interrupt;
    }
    
    
    page_fault=0;
    interrupt=0;
    writing_disk=0;
    
    randcount=1;
    fullcount=1;
    for(count=1;count<=61;count++){
      queue_array[count]=0;
    }

  }
  start_frame=10;
  end_frame =60;
  testcount=60000;
  return;
}

void optimal(int *saveall,int option){
 int page_fault=0,interrupt=0,writing_disk=0;
  int i,j,randnb,count=0,check=0,findcount=0,findcheck=0,findbig,biggest,havefind=0,fullcount=1;
  int data_array[60001]={0};
  int save_array[61]={0};
  int place_array[61]={0};
  int randcount=1;
 
  //use time as seed
  //srand(time(NULL));
  //random , save into data_array
  if(option == 2){testcount = number_long;}
  for(i=1;i<=testcount;i++){
    if(option==0){
        data_array[i] = (rand()%600)+1;
    }
    if(option==1){
        if(randcount<=50){
          data_array[i] = (rand()%100)+1;
          randcount++;
         }
         else if(randcount<=100){
           data_array[i] = (rand()%600)+1;
           randcount++;
         }
         else
         randcount=1;
    }
    if(option==2){
      for(i=1;i<=testcount;i++){
        data_array[i]=number_set[i];
      }
    }
  }
  
 
 
  for(i=start_frame;i<=end_frame;i=i+10){
    for(j=1;j<=testcount;j++){     
      //algorithm
      if(fullcount <= i){
        for(count=1;count<fullcount;count++){
          if(save_array[count]==data_array[j]){
            check = 1;
          }
        }
        if(check == 0){
          page_fault++;
          save_array[fullcount]=data_array[j];
          fullcount++;
        }
        check=0;
      }
      else{
        count=1;
        //If the data is in the memory frames
        while(count <= i && check == 0){
          if(save_array[count]==data_array[j]){
            check=1;
          }
          count++;
        }
        if(check==0){
	  page_fault++;
          //calculate the place where they will be used 
          for(count=1;count<=i;count++){
            for(findcount=j+1;findcheck==0 && findcount<=testcount;findcount++){
              if(save_array[count] == data_array[findcount]){
                findcheck=1;
                place_array[count]=findcount;
              }
            }
	    if(findcheck==0){
              save_array[count]=data_array[j];
              count=i;
              havefind=1;
            }
            findcheck=0;        
	  }
	  if(havefind==0){
            for(findbig=1;findbig<=i;findbig++){
              if(findbig==1){
                biggest=1;
              }
              else if(place_array[findbig] > place_array[biggest]){
                biggest = findbig;
              }
            }
          save_array[biggest]=data_array[j];
          }       
        }
      }
      //reset
      count=0;
      check=0;
      findcheck=0;
      havefind=0;
    }
  
     //save and reset
    if(option==2){
      *(saveall+1) = page_fault;
      *(saveall+7+1) = writing_disk;
      *(saveall+14+1) =interrupt;
    }
    else{
      *(saveall+(i/10)) = page_fault;
      *(saveall+7+(i/10)) = writing_disk;
      *(saveall+14+(i/10)) =interrupt;
    }
    
    page_fault=0;
    interrupt=0;
    writing_disk=0;
    
    randcount=1;
    fullcount=1;
    for(count=1;count<=60;count++){
      save_array[count]=0;
    }

  }
  start_frame=10;
  end_frame =60;
  testcount=60000;
  return;
}
void enhance_second_chance(int *saveall,float m,int option){
  int page_fault=0,interrupt=0,writing_disk=0;
  int i,j,randnb,count=1,check=0,fullcount=1,findcheck=0,icount;
  int read_pointer=1;
  int save_array[61]={0};
  int check_array[61]={0}; 
  int reference_array[61]={0};
  int modify_array[61]={0};
  float number;
  int randcount=1;
  //srand(time(NULL));
  number = 100*m;
  if(option == 2){testcount = number_long;}
  for(i=start_frame;i<=end_frame;i=i+10){
    for(j=1;j<=testcount;j++){
      //random
      if(option==0){
        randnb = (rand()%600)+1;
      }

      else if(option==1){
        if(randcount<=50){
          randnb = (rand()%100)+1;
          randcount++;
        }
        else if(randcount<=100){
         randnb = (rand()%600)+1;
         randcount++;
        }
        else
        randcount=1;
      }
      
      else if(option ==2){
       
        randnb=number_set[randcount];
        randcount++;
      }
    
      
      //algorithm
      if(fullcount <= i){
        for(count=1;count<fullcount;count++){
          if(save_array[count]==randnb){
            check = 1;
            //if(fullcount==i){reference_array[fullcount]=1;}
          }
        }
        if(check == 0){
          if(number >= (rand()%100)+1){
            modify_array[fullcount]=1;
            //writing_disk++;
          }
          page_fault++;
          save_array[fullcount]=randnb;
          reference_array[fullcount]=1;
          fullcount++;
        }
        check=0;
      }
      else{
        count=1;
        while(count <= i && check == 0){
          if(save_array[count]==randnb){
            check=1;
            
            reference_array[count]=1;
          }
          count++;
        }
        if(check==0){
	  page_fault++;
          //interrupt++;
          for(count=1;count<=4 && findcheck==0;count++){
	    for(icount=1;icount<=i && findcheck==0;icount++){
              switch(count){
                case 1:
                  if(reference_array[read_pointer]==0 && modify_array[read_pointer]==0){
                    findcheck=1;
                    save_array[read_pointer]=randnb;
                    reference_array[read_pointer]=1;
                    check_array[read_pointer]=0;
                    if(number >= (rand()%100)+1){
                      modify_array[read_pointer]=1;
                      //writing_disk++;                        
                    }
                    check =0 ;
                    for(icount=1;icount<=i;icount++){
                      if(check_array[icount]==1){
                        check_array[icount]=0;
                        if(reference_array[icount]==1){
                          reference_array[icount]=0;
                          check =1;
                        }
                      }
                    }
                    if(check ==1){interrupt++;}
                    check =0;
                  }
                  else{
                    check_array[read_pointer]=1;
                  }
                  read_pointer++;
                  if(read_pointer>i){read_pointer=1;}
                  if(icount == i){
                    for(icount=1;icount<=i;icount++){
                      check_array[icount]=0;
                    }
                  }
                  break;
                case 2:
                   if(reference_array[read_pointer]==0 && modify_array[read_pointer]==1){
                     findcheck=1;
                     save_array[read_pointer]=randnb;
                     reference_array[read_pointer]=1;
                     check_array[read_pointer]=0;
                     writing_disk++; 
                     if(number >= (rand()%100)+1){
                       modify_array[read_pointer]=1;
                       //writing_disk++;                        
                     }
                     else{
                        modify_array[read_pointer]=0;
                     }
                     for(icount=1;icount<=i;icount++){
                       if(check_array[icount]==1){
                         check_array[icount]=0;
                         if(reference_array[icount]==1){
                           reference_array[icount]=0;
                           check =1;
                         }
                       }
                     }
                     if(check ==1){interrupt++;}
                     check =0; 
                   }
                   else{
                     check_array[read_pointer]=1;
                   }
                   read_pointer++;
                   if(read_pointer>i){read_pointer=1;}
                   if(icount == i){
                     for(icount=1;icount<=i;icount++){
                       check_array[icount]=0;
                     }
                   }
                   break;
                case 3:
                  if(reference_array[read_pointer]==1 && modify_array[read_pointer]==0){
                     findcheck=1;
                     save_array[read_pointer]=randnb;
                     reference_array[read_pointer]=1;
                     check_array[read_pointer]=0;
                     if(number >= (rand()%100)+1){
                       modify_array[read_pointer]=1;
                       //writing_disk++;                        
                     }
                     for(icount=1;icount<=i;icount++){
                       if(check_array[icount]==1){
                         check_array[icount]=0;
                         if(reference_array[icount]==1){
                           reference_array[icount]=0;
                           check =1;
                         }
                       }
                     }
                     if(check ==1){interrupt++;}
                     check =0; 
                  }
                  else{
                    check_array[read_pointer]=1;
                  }
                  read_pointer++;
                  if(read_pointer>i){read_pointer=1;}
                  if(icount == i){
                    for(icount=1;icount<=i;icount++){
                      check_array[icount]=0;
                    }
                  }
                  break;
                case 4:
                  if(reference_array[read_pointer]==1 && modify_array[read_pointer]==1){
                    findcheck=1;
                    save_array[read_pointer]=randnb;
                    reference_array[read_pointer]=1;
                    check_array[read_pointer]=0;
                    writing_disk++; 
                    if(number >= (rand()%100)+1){
                      modify_array[read_pointer]=1;
                      //writing_disk++;                        
                    } 
                    else{
                      modify_array[read_pointer]=0;
                    }
                     for(icount=1;icount<=i;icount++){
                       if(check_array[icount]==1){
                         check_array[icount]=0;
                         if(reference_array[icount]==1){
                           reference_array[icount]=0;
                           check =1;
                         }
                       }
                     }
                     if(check ==1){interrupt++;}
                     check =0;                  
                  }
                  else{
                    check_array[read_pointer]=1;
                  }
                  read_pointer++;
                  if(read_pointer>i){read_pointer=1;}
                  if(icount == i){
                    for(icount=1;icount<=i;icount++){
                      check_array[icount]=0;
                    }
                  }
                 break;
             }//swi             
           }//for
         }//for
         /*check =0 ;
         for(icount=1;icount<=i;icount++){
           if(check_array[icount]==1){
             check_array[icount]=0;
             if(reference_array[icount]==1){
               reference_array[icount]=0;
               check =1;
             }
           }
         }
         if(check ==1){interrupt++;} */    
       }//if      
     }//else
     check=0;
     findcheck=0;
    }//for j
     //save and reset
    if(option==2){
      *(saveall+1) = page_fault;
      *(saveall+7+1) = writing_disk;
      *(saveall+14+1) =interrupt;
    }
    else{
      *(saveall+(i/10)) = page_fault;
      *(saveall+7+(i/10)) = writing_disk;
      *(saveall+14+(i/10)) =interrupt;
    }
    
    page_fault=0;
    interrupt=0;
    writing_disk=0;
    read_pointer = 1;
    randcount=1;
    fullcount=1;
    for(count=1;count<=60;count++){
      save_array[count]=0;
    }
    for(count=1;count<=60;count++){
      reference_array[count]=0;
    }

    for(count=1;count<=60;count++){
      modify_array[count]=0;
    }


  }//for i
  if(m==0.25){ 
  start_frame=10;
  end_frame =60;
  testcount=60000;
  }
  return;
}

void myself_range(int *saveall,int option){
  int page_fault=0,interrupt=0,writing_disk=0;
  int i,j,randnb,count=1,check=0,fullcount=1,renew_count=0,icount,jcount,tmp,randcount=1;
  int read_pointer=1,rand_belong;
  int save_array[61]={0};  
  int range_count[6]={0};
  int range_sort[6]={0};
  
  if(option == 2){testcount = number_long;}
  for(count=0;count<6;count++){
    range_count[count]=0;
  }
  for(i=start_frame;i<=end_frame;i=i+10){
    for(j=1;j<=testcount;j++){
      //random
      if(option==0){
        randnb = (rand()%600)+1;
      }

      else if(option==1){
        if(randcount<=50){
          randnb = (rand()%100)+1;
          randcount++;
        }
        else if(randcount<=100){
         randnb = (rand()%600)+1;
         randcount++;
        }
        else
        randcount=1;
      }
      
      else if(option ==2){

        randnb=number_set[randcount];
        randcount++;
      }
 
      //record incoming page
      rand_belong = randnb/100;
      if(renew_count<=100){
        range_count[rand_belong]++;
        renew_count++;
      }
      if(renew_count >100){
        renew_count=1;
        //copy
        for(count=0;count<6;count++){
          range_sort[count]=range_count[count];
        }
        for(icount=0;icount<5;icount++){
          for(jcount=0;jcount<5;jcount++){
            if(range_sort[jcount]>range_sort[jcount+1]){
              tmp = range_sort[jcount];
              range_sort[jcount] = range_sort[jcount+1];
              range_sort[jcount+1]=tmp;
            }
          }
        }
        for(icount=0;icount<6;icount++){
          for(jcount=0;jcount<6;jcount++){
            if(range_sort[icount] == range_count[jcount]){
              range_sort[icount]=jcount;
            }
          }
        }
        for(count=0;count<6;count++){
          range_count[count]=0;
        }
        range_count[rand_belong]++;
        
      }
       
      
      //algorithm
      if(fullcount <= i){
        for(count=1;count<fullcount;count++){
          if(save_array[count]==randnb){
            check = 1;
          }
        }
        if(check == 0){
          save_array[fullcount]=randnb;
          page_fault++;
          fullcount++;
        }
        check=0;
      }
      else{
        count=1;
        while(count <= i && check == 0){
          if(save_array[count]==randnb){
            check=1;
          }
          count++;
        }
        if(check==0){
	  page_fault++;
          // j<=100 , use FIFO
	  if(j<=100){
            save_array[read_pointer]=randnb;
            read_pointer++;
            if(read_pointer > i){read_pointer=1;}
          }
          else{
            for(count=0;count<6 && check==0;count++){
              for(icount=1;icount<=i && check==0;icount++){
                switch(count){
                  case 0:
                    if(rand_belong == range_sort[count]){
                      save_array[read_pointer] = randnb;
                      check=1;
                    }
                    read_pointer++;
                    if(read_pointer > i){read_pointer=1;}
                    break;
                  case 1:
                    if(rand_belong == range_sort[count]){
                      save_array[read_pointer] = randnb;
                      check=1;
                    }
                    read_pointer++;
                    if(read_pointer > i){read_pointer=1;}
                    break;
                  case 2:
                    if(rand_belong == range_sort[count]){
                      save_array[read_pointer] = randnb;
                      check=1;
                    }
                    read_pointer++;
                    if(read_pointer > i){read_pointer=1;}
                    break;
                  case 3:
                    if(rand_belong == range_sort[count]){
                      save_array[read_pointer] = randnb;
                      check=1;
                    }
                    read_pointer++;
                    if(read_pointer > i){read_pointer=1;}
                    break;
                  case 4:
                    if(rand_belong == range_sort[count]){
                      save_array[read_pointer] = randnb;
                      check=1;
                    }
                    read_pointer++;
                    if(read_pointer > i){read_pointer=1;}
                    break;
                  case 5:
                    if(rand_belong == range_sort[count]){
                      save_array[read_pointer] = randnb;
                      check=1;
                    }
                    read_pointer++;
                    if(read_pointer > i){read_pointer=1;}
                    break;
                }
              }
            }
          }
        }
      }
      check=0;
    }
     //save and reset
    if(option==2){
      *(saveall+1) = page_fault;
      *(saveall+7+1) = writing_disk;
      *(saveall+14+1) =interrupt;
    }
    else{
      *(saveall+(i/10)) = page_fault;
      *(saveall+7+(i/10)) = writing_disk;
      *(saveall+14+(i/10)) =interrupt;
    }
    
    
    page_fault=0;
    interrupt=0;
    writing_disk=0;
    
    randcount=1;
    fullcount=1;
    for(count=1;count<=60;count++){
      save_array[count]=0;
    }

  }
  start_frame=10;
  end_frame =60;
  testcount=60000;
  return;

}

void getnumberset(void){
    int s1=0;
    int check=0;
    while(check == 0){
      printf("Please enter the number of frame :\n");
      scanf("%d",&s1);
      if(s1 <= 1 || s1 >=61){
         printf("Please enter a number >1 , < 61\n"); 
      }
      else{
        start_frame = s1;
        end_frame = s1;
        save_frame =s1;
        check =1;
      }
    }
    check=0;
    printf("Please enter number you want(no 0) , < -1 to stop  :\n");
    while(check==0){
      scanf("%d",&s1);
      if(s1 <= -1){
        check =1;
      }
      else if(s1 == 0){
        printf("Please don't type 0 , enter another number : \n");
      }
      else{
        number_set[number_long+1]=s1;
        number_long++;
        printf("[%d]: %d\n",number_long,number_set[number_long]);
      }
    }
    printf("You key in [%d] data\n",number_long);
}

