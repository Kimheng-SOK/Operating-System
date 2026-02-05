import java.util.concurrent.*;

public class Main {
    public static void main(String[] args) throws Exception {
        ExecutorService single = Executors.newSingleThreadExecutor();
        ExecutorService cached = Executors.newCachedThreadPool();

        single.submit(() -> System.out.println("SingleThread: " + Thread.currentThread().getName()));
        cached.submit(() -> System.out.println("CachedThreadPool: " + Thread.currentThread().getName()));
        cached.submit(() -> System.out.println("CachedThreadPool: " + Thread.currentThread().getName()));

        single.shutdown();
        cached.shutdown();
        single.awaitTermination(1, TimeUnit.SECONDS);
        cached.awaitTermination(1, TimeUnit.SECONDS);

        ForkJoinPool fj = new ForkJoinPool();
        int result = fj.invoke(new SumTask(1, 100));
        System.out.println("ForkJoin result: " + result);
        fj.shutdown();
    }

    static class SumTask extends RecursiveTask<Integer> {
        final int start, end;
        SumTask(int start, int end) { this.start = start; this.end = end; }
        @Override
        protected Integer compute() {
            if (end - start <= 10) {
                int sum = 0;
                for (int i = start; i <= end; i++) sum += i;
                System.out.println("Range " + start + "-" + end + " by " + Thread.currentThread().getName());
                return sum;
            }
            int mid = (start + end) / 2;
            SumTask left = new SumTask(start, mid);
            SumTask right = new SumTask(mid + 1, end);
            left.fork();
            return right.compute() + left.join();
        }
    }
}