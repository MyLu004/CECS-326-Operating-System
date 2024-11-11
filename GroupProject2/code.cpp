#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define PHILOSOPHER_AMOUNT 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0

int state[PHILOSOPHER_AMOUNT];  // Array to track each philosopher's state
pthread_mutex_t mutex;
pthread_cond_t cond_var[PHILOSOPHER_AMOUNT];

// Test if a philosopher can eat
int test(int philosopher_number) {
    if (state[(philosopher_number + 4) % PHILOSOPHER_AMOUNT] != EATING &&
        state[philosopher_number] == HUNGRY &&
        state[(philosopher_number + 1) % PHILOSOPHER_AMOUNT] != EATING) {
        
        state[philosopher_number] = EATING;
        printf("Philosopher %d starts eating.\n", philosopher_number);
        pthread_cond_signal(&cond_var[philosopher_number]);
        return 1;  // Philosopher can eat
    }
    return 0;  // Philosopher cannot eat
}

// Philosopher picks up forks
int pickup_forks(int philosopher_number) {
    pthread_mutex_lock(&mutex);
    state[philosopher_number] = HUNGRY;
    printf("Philosopher %d is hungry.\n", philosopher_number);
    
    int canEat = test(philosopher_number);  // Attempt to eat
    if (state[philosopher_number] != EATING) { //have to wait for the forks
        pthread_cond_wait(&cond_var[philosopher_number], &mutex);  // Wait if can't eat
        canEat = 0;  // Had to wait, so initially could not eat
    }
    
    //then eat and release
    pthread_mutex_unlock(&mutex); //before exist, unlock the mutex lock
    return canEat; //return 1
}

// Philosopher returns forks
int return_forks(int philosopher_number) {
    pthread_mutex_lock(&mutex);

    //reset the philosopher state back to THINKING
    state[philosopher_number] = THINKING;
    printf("Philosopher %d finishes eating and returns forks.\n", philosopher_number);
    
    // Signal neighbors to check if they can eat
    test((philosopher_number + 4) % PHILOSOPHER_AMOUNT); //signal their neighbor in the left
    test((philosopher_number + 1) % PHILOSOPHER_AMOUNT); //signal their neighbor in the right
    
    pthread_mutex_unlock(&mutex); //after finish signal, unlock the mutex
    return 1;  // Successful return
}

// Main function for philosopher routine
void* philosopherWay(void* arg) {
    int philosopher_number = *(int*)arg;
    while (true) {
        printf("Philosopher %d is thinking.\n", philosopher_number);
        sleep(rand() % 3 + 1);  // Think for a random period


        //if 1, TRUE
        if (pickup_forks(philosopher_number)) {  // Try to pick up forks and eat
            sleep(rand() % 3 + 1);  // Eat for a random period
            return_forks(philosopher_number);  // Put down forks
        }
    }
    return NULL;
}

int main() {
    printf("--- START ---\n");

    // Initialize the mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < PHILOSOPHER_AMOUNT; i++) {
        pthread_cond_init(&cond_var[i], NULL);
        state[i] = THINKING;  // Initialize all philosophers as THINKING
    }

    // Create philosopher threads
    pthread_t philosophers[PHILOSOPHER_AMOUNT];
    int philosopher_ids[PHILOSOPHER_AMOUNT];
    for (int i = 0; i < PHILOSOPHER_AMOUNT; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopherWay, &philosopher_ids[i]);
    }

    // Join philosopher threads (not strictly necessary in an infinite loop)
    for (int i = 0; i < PHILOSOPHER_AMOUNT; i++) {
        pthread_join(philosophers[i], NULL);
    }

    printf("--- END ---\n");
    
    // Clean up
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < PHILOSOPHER_AMOUNT; i++) {
        pthread_cond_destroy(&cond_var[i]);
    }

    return 0;
}
