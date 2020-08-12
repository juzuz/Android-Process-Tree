#include <sys/syscall.h>
#include <unistd.h>	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __NR_ptreecall 356


struct prinfo {
pid_t parent_pid;
/* process id of parent, set 0 if it has no parent*/
pid_t pid;
/* process id */
pid_t first_child_pid;
/* pid of youngest child, set 0 if it has no child */
pid_t next_sibling_pid; /* pid of older sibling, set 0 if it has no sibling*/
long state;
/* current state of process */
long uid;
/* user id of process owner */
char comm[64];
/* name of program executed */
};


void printTree(struct prinfo *buf, int *nr)
{
int pidArray[1000] = {0};
int tab = 0;   

printf("There are %d processes!\n", *nr);
printf("%s,%d,%ld,%d,%d,%d,%d\n", buf[0].comm, buf[0].pid, buf[0].state,
buf[0].parent_pid, buf[0].first_child_pid, buf[0].next_sibling_pid, buf[0].uid);

int i=1;

while(i < *nr)
{
    if(buf[i].parent_pid == buf[i-1].pid)
    {
        tab++;
    }
    else if (buf[i].parent_pid != buf[i-1].parent_pid) 
        {
            int backtrack = buf[i-1].parent_pid;
            tab--;
            while(buf[i].parent_pid!=buf[pidArray[backtrack]].parent_pid) 
            {
                backtrack = buf[pidArray[backtrack]].parent_pid; 
                tab--;
            }
        }
    // else if(buf[i].parent_pid != buf[i-1].parent_pid)
    // {
        
    //     int backtrack = buf[i-1].parent_pid;
    //     int cur = buf[i].parent_pid;
    //     //tab--;
    //     while(cur !=  buf[pidArray[backtrack]].parent_pid)
    //     {
    //         backtrack = buf[pidArray[backtrack]].parent_pid;
    //         tab--;
    //     }
    // }

    pidArray[buf[i].pid] = i;

    int j = 0;
    while(j < tab)
    {
        printf("\t");
        j++;
    }

    printf("%s,%d,%ld,%d,%d,%d,%ld\n", buf[i].comm, buf[i].pid, buf[i].state,
    buf[i].parent_pid, buf[i].first_child_pid, buf[i].next_sibling_pid, buf[i].uid);

    i++;
}
   
}

int main()
{
struct prinfo *buf = malloc(1000 * sizeof(struct prinfo));
	int *nr = malloc(sizeof(int));
	if (buf == NULL || nr == NULL) {
		printf("Allocation error!\n");
		return 0;
	}

    syscall(__NR_ptreecall, buf, nr);
	
    printTree(buf,nr);

    free(buf);
    free(nr);

    return 0;
}
