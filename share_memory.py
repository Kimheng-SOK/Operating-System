from threading import Semaphore, Thread
import time

# Shared resources
buffer = [100]
S = Semaphore(0)
space = Semaphore(50)
lock = Semaphore(1)

def producer():
    pair_id = 0
    while True:
        p1 = f"P1-{pair_id}"
        p2 = f"P2-{pair_id}"

        space.acquire()
        lock.acquire()
        buffer.append(p1)
        buffer.append(p2)

        print(f"Produced pair {pair_id}: {p1}, {p2}")
        S.release()
        lock.release()
        pair_id += 1
        time.sleep(0.5)

def consumer():
    while True:
        S.acquire()
        # lock.acquire()
        p1 = buffer.pop()
        p2 = buffer.pop()
        space.release()
        # lock.release()
        print(f"Consumed pair: {p1}, {p2}")
        time.sleep(0.5)

if __name__ == "__main__":
    Thread(target=producer, daemon=True).start()
    Thread(target=consumer, daemon=True).start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Stopped")