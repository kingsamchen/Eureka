package usinglocks;

public class Counting {
    static class Counter {
        private int count = 0;

        public int getCount() {
            return count;
        }

        // Use `synchronized` here to make the method mutual exclusive.
        public synchronized void increment() {
            ++count;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        final Counter counter = new Counter();

        class CounterThread extends Thread {
            @Override
            public void run() {
                for (int i = 0; i < 10000; ++i) {
                    counter.increment();
                }
            }
        }

        CounterThread t1 = new CounterThread();
        CounterThread t2 = new CounterThread();
        t1.start();
        t2.start();
        t1.join();
        t2.join();

        System.out.println("Value of counter is: " + String.valueOf(counter.getCount()));
    }
}
