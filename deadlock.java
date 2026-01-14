import java.util.concurrent.Semaphore;

class Account {
    String name;
    int balance;
    Semaphore lock = new Semaphore(1);
    int id; // Add unique ID for ordering

    Account(String name, int balance, int id) {
        this.name = name;
        this.balance = balance;
        this.id = id;
    }
}


class Transfer {

    static void transfer(Account from, Account to, int amount) {
        // Determine lock order based on account ID to prevent deadlock
        Account first, second;
        if (from.id < to.id) {
            first = from;
            second = to;
        } else {
            first = to;
            second = from;
        }

        try {
            System.out.println(Thread.currentThread().getName() +
                    " trying to lock FIRST " + first.name);
            first.lock.acquire();
            System.out.println(Thread.currentThread().getName() +
                    " locked FIRST " + first.name);

            // Delay to increase deadlock chance (won't deadlock now)
            Thread.sleep(100);

            System.out.println(Thread.currentThread().getName() +
                    " trying to lock SECOND " + second.name);
            second.lock.acquire();
            System.out.println(Thread.currentThread().getName() +
                    " locked SECOND " + second.name);

            // Critical section
            from.balance -= amount;
            to.balance += amount;

            System.out.println(Thread.currentThread().getName() +
                    " transfer completed: " + from.name + " -> " + to.name + " : " + amount);

            second.lock.release();
            first.lock.release();

        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}


public class deadlock {
    public static void main(String[] args) {

        Account account1 = new Account("Account-1", 1000, 1);
        Account account2 = new Account("Account-2", 1000, 2);

        Thread t1 = new Thread(() ->
                Transfer.transfer(account1, account2, 100),
                "Thread-1"
        );

        Thread t2 = new Thread(() ->
                Transfer.transfer(account2, account1, 200),
                "Thread-2"
        );

        t1.start();
        t2.start();
    }
}