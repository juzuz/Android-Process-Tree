#include <unistd.h>  //for execl(),fork(),getpid()
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
	pid_t pid = fork();	
	
	if (pid < 0)
		printf("Error!");
	else if (pid == 0)
  {
		printf("518030990041 Child, %d\n", getpid());
        printf("Child's parent ID, %d\n",getppid());	
		execl("/data/misc/ptreeARM", "ptreeARM", NULL);	// execute ptree in child process	
	}
	else
    {
		printf("518030990041 Parent, %d\n", getpid());	
        printf("Parent's parent ID, %d\n",getppid());
        execl("/data/misc/ptreeARM", "ptreeARM", NULL);
    }
	return 0;
}