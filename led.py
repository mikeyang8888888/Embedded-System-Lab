from gpiozero import LED
from time import sleep
import threading
import random
import time

led1 = LED("GPIO4")
led2 = LED("GPIO17")
lock = threading.Lock()

def led1_blink(stop):
    global lock
    while(1):
        if stop():
            print("led1 stop....")
            break
        #lock.acquire()
        cond = random.randint(0, 100)
        sleep(0.5)
        # cond = 63                   
        if(cond%7==0):
            #cnt+=1
            lock.acquire()
            print('job1: '+str(cond)+'| led1 on....')
            led1.on()
            sleep(1)
            led1.off()
            sleep(1)
            print("---------")
            lock.release()
        #else:
        #    print('led1 off: '+str(cond))

def led2_blink(stop):
    global lock
    while(1):
        if stop():
            print("led2 stop....")
            break
        #lock.acquire()
        cond = random.randint(0, 100)
        sleep(0.5)
        if(cond%9==0):
            lock.acquire()
            print('job2: '+str(cond)+'| led2 on....')
            led2.on()
            sleep(1)
            led2.off()
            sleep(1)
            print("---------")
            lock.release()

def run():
    STOP = False
    thread1 = threading.Thread(target = led1_blink, args=(lambda: STOP,))
    thread2 = threading.Thread(target = led2_blink, args=(lambda: STOP,))
    thread1.daemon = True
    thread2.daemon = True
    thread1.start()
    thread2.start()
    
    if(input()=="s"):
        STOP = True
        thread1.join()
        thread2.join()
        
    while(STOP):
        print("waiting.....")
        sleep(0.5)
        if(input()=="c"):
            STOP = False
            run()
            """
            thread1 = threading.Thread(target = led1_blink, args=(lambda: STOP,))
            thread2 = threading.Thread(target = led2_blink, args=(lambda: STOP,))
            thread1.daemon = True
            thread2.daemon = True
            thread1.start()
            thread2.start()
            """

    thread1.join()
    thread2.join()


def main():
    run()
    
if __name__=="__main__":
    main()
