 #include <stdio.h>
 #include <unistd.h>
 #include <signal.h>
 #include <stdlib.h>
int points=0;
int a=1, n=4, s=2;
int child1pid = -1;
int child2pid = -1;

static struct sigaction sigact;

//the signal handler structure is fed into the sigaction and manages the signal
void signalHandler(int sig, siginfo_t *si,void *context)
{
	int sender_pid=si->si_pid;
	
	if(sender_pid==getppid())
	{
		points+=a;
	}
	else if(sender_pid==child1pid || sender_pid==child2pid)
	{
		points-=s;
	}
	else if(sender_pid-getpid()==1 || sender_pid-getpid()==-1)
	{
		points-=s/2;
	}
	
	printf("points are %d", points);
	
	if(points==0)
	{
		printf("exiting");
		exit(0);
	}
	printf("Pid : %d\n",getpid());
    	printf ("ppid : %d\n",getppid());
    	printf ("Children : %d , %d\n",child1pid, child2pid);
	printf("Signal received from : %d\n",sender_pid);
	
}
void main()
{
	points=n;
	int lvl=(n+1)/2;
	for(int i=0; i<lvl;i++)
	{
		child1pid=0;
		child2pid=0;
		if((child1pid=fork())==0)
		{
			break;
		}
		if(i == lvl -1 && n%2 != 0)
        	{	
        		break;
        	}
        	if((child2pid=fork())==0)
        	{
        		continue;
        	}
        	break;	
	}
	//sigact is the signal action structure. Specifies how to handle a signal. here we are specifiying the signal handler and the flags.
	sigact.sa_sigaction = *signalHandler;
    	sigact.sa_flags |= SA_SIGINFO;
    	
    	
    	if( sigaction(SIGCHLD, &sigact, NULL) != 0)
    	{
		printf(" Error in applying signal handler\n");
		exit(0); 
    	}
    
    	int pid=getpid();
    	int ppid= getppid();
    	
    	for(int i=ppid-n; i<=pid+n;i++)
    	{
    		kill(i, SIGCHLD);
    		sleep(2);
    	}
	
		
	
	
	
	
	
}
