#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>




void *cooker(void*);
void *customer(void*);
void *cashier(void *);

//A semaphore the controls the access to the rack. Only one person can put a burger into the rack and one cashier can take out from the rack.
sem_t rackAccess;

//The cashier sleeping and time to get ready
sem_t cashierWakeup;

// The customer places an order and the cashier waits for an order.
sem_t order;



int numCook;
int numCashier;
int numCust;
int rackSize;
int burgersInRack = 0;
int i,served;

// A random timer for the cook, each cook cooks in different speed
void wait() {
    sleep(5 + rand()%10);
}


int main(int argc,char *argv[])
{
    // Set the initial variables.
    srand(time(NULL));
    numCook = atoi(argv[1]);
    numCashier = atoi(argv[2]);
    numCust = atoi(argv[3]);
    rackSize = atoi(argv[4]);
    served = 0;
   

    // Initialize the semaphores
    sem_init(&rackAccess,0,1);
    sem_init(&cashierWakeup,0,0);
    sem_init(&order,0,1);
    

    // Entities
    pthread_t cid[numCook];
    pthread_t cashid[numCashier];
    pthread_t custid[numCust];
    
	// Create the cooks,cashiers and customers.
        for(i = 1; i <= numCook;i++)
        {
            pthread_create(&cid[i],NULL,cooker,i);  
        }

        for(i = 1; i <= numCashier;i++)
        {
            pthread_create(&cashid[i],NULL,cashier,i);
            sleep(1);
        }


        for(i = 1; i <= numCust;i++)
        {
            pthread_create(&custid[i],NULL,customer,i);
           sleep(3);
        }

	// Wait for each entity to finish
        for (i=1; i<=numCust; i++) {
            pthread_join(custid[i],NULL);
            sleep(1);
        }
         for (i=1; i<=numCashier; i++) {
            pthread_join(cashid[i],NULL);
            sleep(1);
        }

        for (i=1; i<=numCook; i++) {
            pthread_join(cid[i],NULL);
            sleep(1);
        }
    
    return 0;
}


void *cooker(void* i)
{
    while(served!=numCust)
    {
        int id = (int)i;
	//Cooker needs permission to put a burger into the rack
         sem_wait(&rackAccess);
        //If rack is still free put in burger.
        if(burgersInRack!=rackSize)
        {
            printf("Cook[%d] cooked a burger!\n",id);
            burgersInRack++;  
        }
        else
        {
            printf("Rack is full, Cook[%d] is waiting for rack to free up!\n",id);
        }
        sem_post(&rackAccess);
        wait();
    }
}

void *cashier(void* i)
{


    int id = (int) i;

 while(served!=numCust)
    {
    //The cashier is sleeping, waiting for customer to wake him up
    sem_wait(&cashierWakeup);
    printf("Cashier[%d] responds!\n",id);
    // The cashier is awake and waits for an order
    sem_wait(&order);
    
    // The cashier needs to take a burger out
    sem_wait(&rackAccess);
    // If it is empty, let go of access control and wait then access again untill there is a burger.
    while(burgersInRack==0)
    {
        sem_post(&rackAccess);
        sleep(2);
        sem_wait(&rackAccess);
    }
    burgersInRack--;
        printf("Cashier[%d] serves the customer a burger!\n",id);
    served++;
    sem_post(&rackAccess);
    
    sleep(5);
    }

}

void *customer(void *i)
{
    int id = (int) i;
    printf("Customer[%d] comes!\n",id);
    // Wake the cashier and place an order.
    sem_post(&cashierWakeup);
    sem_post(&order);
}
