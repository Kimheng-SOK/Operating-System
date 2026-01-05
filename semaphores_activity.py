from threading import Semaphore, Thread
import time
a = Semaphore(1)
b = Semaphore(0)
c = Semaphore(0)
def process1():
    global a, b
    while True:
        a.acquire()
        print('H')
        print('E')
        b.release()
        b.release()
        time.sleep(1)

def process2():
    global b, c
    while True:
        b.acquire()
        print('L')
        c.release()
        time.sleep(1)

def process3():
    global c, a
    while True:
        c.acquire()
        c.acquire()
        print('O')
        print(' ')
        time.sleep(1)

Thread(target=process1).start()
Thread(target=process2).start() 
Thread(target=process3).start()
