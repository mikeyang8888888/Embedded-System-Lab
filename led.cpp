#include <stdio.h>
#include "wiringPi.h"
#include "pthread.h"
#include <iostream>
using namespace std;

int range = 100; // random range [0, 100]
#define LED1_PIN 7
#define LED2_PIN 0
 
int TURN = 0; // lock
int FLAG[2];

int STOP = false; // let thread wait
int KILL = false; // stop thread

pthread_t thread1, thread2;

void *led1(void* argv);
void *led2(void* argv);

void init_LED()
{
    wiringPiSetup();
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
}

void *led1(void* argv)
{
    while(1){ 
        /* kill thread */
        if(KILL){
            if(TURN==0) TURN=1;
            printf("led1 thread stop.....\n");
            break;
        }

        /* wait signal */
        while(STOP){
            printf("led1 thread waiting.....\n");
            if(TURN==0) TURN=1; 
            delay(500);
        }

        FLAG[0]=true;
        TURN = 1;
        while(FLAG[0] && TURN==1);

        srand(time(NULL));
        int rand_num = rand()%range + 0;
        //int rand_num = 63; 

        if(rand_num%7==0){
            //FLAG[0]=true;
            //TURN = 1;
            //while(FLAG[0] && TURN==1);
            
            printf("led1 on | num: %d\n", rand_num);
            digitalWrite(LED1_PIN, HIGH);
            delay(1000);
            digitalWrite(LED1_PIN, LOW);
            delay(1000);
       
            //FLAG[0] = false;
            printf("------------------\n");
        }
        //else{
        //   printf("led1 off...\n");
        //}
        
        FLAG[0] = false;
        delay(500);
    }
    return NULL;
}


void *led2(void* argv)
{
    while(1){
        /* kill thread */
        if(KILL){
            if(TURN==1) TURN =0;
            printf("led2 thread stop...\n");
            break;
        }

        /* wait signal */
        while(STOP){
            printf("led2 thread waiting.....\n");
            if(TURN==1) TURN=0;
            delay(500);
        }

        FLAG[1]=true;
        TURN = 0;
        while(FLAG[1] && TURN==0);

        srand(time(NULL));
        int rand_num = rand()%range + 0;
        //int rand_num = 63;
        if(rand_num%9==0){
            //FLAG[1]=true;
            //TURN = 0;
            //while(FLAG[1] && TURN==0);
            
            printf("led2 on | num: %d\n", rand_num);
            digitalWrite(LED2_PIN, HIGH);
            delay(1000);
            digitalWrite(LED2_PIN, LOW);
            delay(1000);
            
            //FLAG[1]=false;
            printf("------------------\n");
        }
        //else{
        //   printf("led2 off...\n");
        //}

        FLAG[1] = false;
        delay(500);
    }
    return NULL;
}

void clear(){
    KILL=true;
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    pthread_join(thread1, NULL);
    delay(500);
    pthread_join(thread2, NULL);
    delay(500);
    printf("All thread kill...\n");
    return;
}

int main(void){

    init_LED();
    
    char SIG;

    //pthread_t thread1, thread2;
    
    
    int iret1 = pthread_create(&thread1, NULL, led1, NULL);
    if(iret1 != 0){
	    fprintf(stderr, "pthread_create thread1 failed!");
	    return 0;
    }
    
    int iret2 = pthread_create(&thread2, NULL, led2, NULL);
    if(iret2 != 0){
	    fprintf(stderr, "pthread_create thread2 failed!");
	    return 0;
    }
    printf("create led threads\n");
    

    /*      detect signal to change thread status       */
    while(1){
        while((SIG=getchar())!='\n' && SIG!=EOF){
            printf("recv sig.....\n");
            if(SIG=='1'){
                digitalWrite(LED1_PIN, LOW);
                digitalWrite(LED2_PIN, LOW);
                STOP=true;
            }
            if(SIG=='2') STOP=false;
            if(SIG=='3'){
                clear();
                printf("Finish lab....\n"); 
                return 0;
            }
            printf("!! STOP status: %d !!\n", STOP);
            delay(500);
        }
    }
    
    //printf("Finish lab....\n");
    return 0;
}
