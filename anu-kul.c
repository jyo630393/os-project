#include<stdio.h>
#include<unistd.h>

#include<stdlib.h>

#define NEW 0
#define READY 1
#define WAITING 2
#define RUNNING 3
#define EXIT 4
int moneyearned=0;
int ongoingtime=0,time_till_last_ct=0,EXIT_STATE_ALL=0,CT_ALLOWED=1,n,i,j;
struct Process_structure 
{
	int pid;
	int state;
	int timeleft_to_exec;
	int at;
	int wt,tat,ct,exect;
	struct Process_structure *prev;
	struct Process_structure *next;
} *proc_arr;
int chpp;
struct Queue
{
	struct Process_structure *front ,*rear;
}*RQueue;

void enqueue(struct Process_structure *proc)
{
	if(RQueue->front==NULL)
	{
		RQueue->front=proc;
		RQueue->rear=proc;
		proc->next=NULL;
	}
        
	else
	{
		if(proc->timeleft_to_exec<RQueue->front->timeleft_to_exec)
		{
			proc->next=RQueue->front;
			RQueue->front->prev=proc;
			RQueue->front=proc;
		}
               else if(proc->timeleft_to_exec==RQueue->front->timeleft_to_exec)
		{
			proc->next=RQueue->front->next;
			proc->prev=RQueue->front;
			RQueue->front->next=proc;
                         if(proc->next!=NULL)
                      {
                        proc->next->prev=proc;
                      }
		}
		else if(proc->timeleft_to_exec>RQueue->rear->timeleft_to_exec)
		{
			proc->next=NULL;
			RQueue->rear->next=proc;
			proc->prev=RQueue->rear;
			RQueue->rear=proc;
		}
		else
		{
                   struct Process_structure *start=RQueue->front->next;
		   while(start->timeleft_to_exec<proc->timeleft_to_exec)
		   {
			   start=start->next;
		   }
                   if(start!=NULL&& proc->timeleft_to_exec==start->timeleft_to_exec)
                   {
                          proc->next=start->next;
                          start->next=proc;
                          proc->prev=start;
                      
                   }
                  else
               {
		   (start->prev)->next=proc;
		   proc->next=start;
		   proc->prev=start->prev;
		   start->prev=proc;
                }

		}
	}
}
struct Process_structure *  deQueue()
{
     if(RQueue->front==NULL)
     {
	     return NULL;
     }
     struct Process_structure * temp=RQueue->front;
     RQueue->front=RQueue->front->next;
     temp->next=NULL;
     if(RQueue->front==NULL)
     {
	     RQueue->rear=NULL;
     }
     return temp;
}
void CQ()
{
int cnt=0;
           for(i=0;i<n;i++)
	   {
                   
		   if(proc_arr[i].state == NEW && ongoingtime>=proc_arr[i].at)
		   {      
			   
			   enqueue(&proc_arr[i]);
			   proc_arr[i].state=READY;
			   
                    }
                  if(proc_arr[i].state==EXIT)
                  {
                    cnt++;
			}
	   }
	   if(cnt==n)
	   {
		  EXIT_STATE_ALL=1; 
		  
	   }
}
int main()
{


RQueue =(struct Queue*) malloc(sizeof(struct Queue));
	printf("            Please enter No of Passangers =");
	scanf("%d",&n);
	
	proc_arr=(struct Process_structure *)malloc(sizeof(struct Process_structure)*n);
	for(i=0;i<n;i++)
	{
             printf("\n\n        Enter Process Id For %d Passanger   =",(i+1));
	     scanf("%d",&(proc_arr[i].pid));
	     printf("\n         Enter arrival time For %d Passanger  =",(i+1));
	     scanf("%d",&(proc_arr[i].at));
	     printf("\n         Enter Distance to travel time For %d Passanger=",(i+1));
	     scanf("%d",&(proc_arr[i].timeleft_to_exec)); 
	     proc_arr[i].timeleft_to_exec*=2;
            proc_arr[i].exect=proc_arr[i].timeleft_to_exec;
	    proc_arr[i].state=NEW;
	        
	}
 struct Process_structure key; 

    for (i = 1; i < n; i++) { 
        key = proc_arr[i]; 
        j = i - 1; 
  
        while (j >= 0 && proc_arr[j].at > key.at) { 
            proc_arr[j + 1] = proc_arr[j]; 
            j = j - 1; 
        } 
        proc_arr[j + 1] = key; 
    } 

struct Process_structure *pr;
printf("Gannt Chart\n==========================================\n");
while(1)
{
        CQ();
        if(EXIT_STATE_ALL==1)
        {

                    break;      
        }
        
        if(RQueue->front!=NULL && CT_ALLOWED==1)
	{
             time_till_last_ct=1;
	     pr=deQueue();
	     moneyearned+=10;
	     printf(" %d| Passanger Id:%d [Drop+Return]|",ongoingtime,pr->pid);
             pr->state=RUNNING;
             pr->timeleft_to_exec--;
	     ongoingtime++;
             if(time_till_last_ct==pr->exect)
             {
                  CT_ALLOWED=1;
                  pr->state=EXIT;
                  pr->ct=ongoingtime;
                  pr->tat=ongoingtime-pr->at;
                  pr->wt=pr->tat-pr->exect;
             }
             else
            {
               CT_ALLOWED=0;
              }
	}
        else if(CT_ALLOWED==0&&pr!=NULL && pr->state==RUNNING )
	{
             if(pr->timeleft_to_exec==0)
             {
                  CT_ALLOWED=1;
                  pr->state=EXIT;
                  pr->ct=ongoingtime;
                  pr->tat=ongoingtime-(pr->at);
                  pr->wt=(pr->tat)-(pr->exect);
                  continue;
             }    
             time_till_last_ct++;
             pr->timeleft_to_exec--;
            ongoingtime++;

             if(pr->timeleft_to_exec==0)
             {
                  CT_ALLOWED=1;
                  pr->state=EXIT;
                  pr->ct=ongoingtime;
                  pr->tat=ongoingtime-(pr->at);
                  pr->wt=(pr->tat)-(pr->exect);
             }            
            else
          { 
              CT_ALLOWED=0;
	  }

          
             
          
	}
else
{
printf(" %d| IDLE |",ongoingtime);
ongoingtime++;
}
}
printf("%d|\n",ongoingtime);
printf("============================================================================================================\n");
int sumwt=0,sumtat=0;
for(i=0;i<n;i++)
	{
           printf("\n\nprocess pid=%d\tct=%d\ttat=%d\twt=%d",proc_arr[i].pid,proc_arr[i].ct,proc_arr[i].tat,proc_arr[i].wt);
           sumwt+=proc_arr[i].wt;
           sumtat+=proc_arr[i].tat;
           
	}
printf("\n\n Money Earned=%d \t Average WT=%f\n",moneyearned,(sumwt/(n*1.0)));
}

