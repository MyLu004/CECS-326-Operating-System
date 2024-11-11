//PROJECT 2 : SYNCHRONIZATION
//CECS 326 - OPERATING SYSTEM

//My Lu : 029895591
//Fozhan Babaeiyan : 029701865 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //for thread
#include <unistd.h> //for sleep function


#define PHILOSOPHER_AMOUNT 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0

int state[PHILOSOPHER_AMOUNT]; //array to keep track philosopher state
// int *PhiloArray = new int[PHILOSOPHER_AMOUNT];
pthread_mutex_t mutex; //mutex lock
pthread_cond_t cond_var[PHILOSOPHER_AMOUNT];


//test condition for philosopher, check if they have to forks available
int testingCondition(int philosopher_number){
    if (state[(philosopher_number + 4) % PHILOSOPHER_AMOUNT] != EATING &&
        state[philosopher_number] == HUNGRY &&
        state[(philosopher_number + 1) % PHILOSOPHER_AMOUNT] != EATING) {
        
        state[philosopher_number] = EATING;
        printf("Philosopher %d starts eating.\n", philosopher_number);
        pthread_cond_signal(&cond_var[philosopher_number]);
        return 1;  // Philosopher can eat
    }
    return 0; // Philosopher cannot eat
}




 
//philosopher pick up and start eatng
int pickup_forks(int philosopher_number){
    //mutex lock : 
    pthread_mutex_lock(&mutex);
    
    //change state into THINKING
    state[philosopher_number] = HUNGRY;
    printf("Philosopher %d is HUNGRY.\n", philosopher_number);

    //before eat, checking if they able to eat
    int EatToggle = testingCondition(philosopher_number);

    //if the fork are not available, they have to wait
    if( EatToggle == 0){
        pthread_cond_wait(&cond_var[philosopher_number], &mutex);
        EatToggle = 0;
    }

    //after eatch or checking, unlock the mutex
    pthread_mutex_unlock(&mutex);

    //return the EatToggle for the philosopher state
    return EatToggle;
    
}


//Task: put down the fork and signal their neighbor the fork is avaible
int return_forks(int philosopher_number){
    //call when the philosopher finish eating

    //lock the mutex
    pthread_mutex_lock(&mutex);

    //change state into THINKING
    state[philosopher_number] = THINKING;
    // printf("THINKING STATE");

    printf("philosopher %d finishes eating and return forks.\n", philosopher_number);
 
    testingCondition((philosopher_number+4)% PHILOSOPHER_AMOUNT); //on the left
    testingCondition((philosopher_number+1)% PHILOSOPHER_AMOUNT); //on the right

    pthread_mutex_unlock(&mutex); //then unlock the mutex lock

    //successful return
    return 1;
    //signal their neighbor to check if they can eat
    
}


//PHILOSOPHER ROUTINE
void* philosopherAction(void* arg){
    int philosopher_Number = *(int*)arg;

   
    // printf("it went here, philo ACTION, \n");
    while (true) {
        printf("The philosopher %d is thinking \n", philosopher_Number);
        sleep(3); //sleep for 3 second

        if(pickup_forks(philosopher_Number)){
            sleep(3); //sleep 3 seconds;
            return_forks(philosopher_Number); //put down the fork, finished execution
        }
    }
    return NULL;

}



int main(){
    printf ("--- START --- \n");
    
    //init the pthread
    pthread_mutex_init(&mutex, NULL);

    //initial the condition var for each thread
    for (int i = 0; i < PHILOSOPHER_AMOUNT ; i++){
        pthread_cond_init(&cond_var[i], NULL);
        state[i] = THINKING;
    };

   
    /*
    CREATING PTHREAD
    int pthread_create(pthread_t *restrict thread,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);
    
    */

   //CREATING THREAD
   pthread_t philosophers[PHILOSOPHER_AMOUNT];
   int philosophers_ID[PHILOSOPHER_AMOUNT]; //aray to store id
   for (int i = 0; i < PHILOSOPHER_AMOUNT; i++)
   {
    /* code */
    philosophers_ID[i] = i;
    pthread_create(&philosophers[i], NULL, philosopherAction, &philosophers_ID[i]);
   }

   //JOIN THREAD AT THE END
   for (int i = 0; i < PHILOSOPHER_AMOUNT; i++)
   {
    /* code */
    pthread_join(philosophers[i], NULL);
   }
   

    printf ("\n --- IT END HERE ---");
    return 0;


};