#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *computation(void *arg);


int main(){
    printf("hello Kinoko\n");

    pthread_t thread1;

    pthread_create(&thread1, NULL, computation, NULL);

    pthread_join(thread1, NULL);

    printf("IT END HERE");
    return 0;
}

void *computation(void *arg){
    printf("the defination of Computation\n");
    return NULL;
}



/*
monitor DiningPhilosophers{
    enum { THINKING; HUNGRY, EATING) state [5] ;
    condition self [5];
    
    void pickup (int i) {
        state[i] = HUNGRY;
        test(i);
        if (state[i] != EATING) self[i].wait;
    }


    void putdown (int i) {
        state[i] = THINKING;

        // test left and right neighbors

        test((i + 4) % 5);
        test((i + 1) % 5);
    }

    void test (int i) {
        if ((state[(i + 4) % 5] != EATING) &&
        (state[i] == HUNGRY) &&
        (state[(i + 1) % 5] != EATING) ) {
            state[i] = EATING ;
            self[i].signal () ;
        }
    }

    initialization_code() {
        for (int i = 0; i < 5; i++)
        state[i] = THINKING;
    }
}


*/
