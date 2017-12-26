package usinglocks;

import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

public class Counting {
    static class Counter {
        private int count = 0;

        // Need synchronized here to enforce visibility.
        public synchronized int getCount() {
            return count;
        }

        // Use `synchronized` here to make the method mutual exclusive.
        public synchronized void increment() {
            ++count;
        }
    }

    void testCounting() throws InterruptedException {
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

    void testDownloader() throws InterruptedException, IOException {
        String url = "https://s3-img.meituan.net/v1/mss_3d027b52ec5a4d589e68050845611e68/ff/n0/0f/1f/eq_520021.jpg@596w_1l.jpg";
        Downloader imgDownloader = new Downloader(new URL(url), "ffimage.jpg");
        imgDownloader.addListener(p -> System.out.println("Image downloaded " + String.valueOf(p)));
        imgDownloader.run();
        imgDownloader.join();
    }

    void testConcurrentSortedListST() {
        ConcurrentSortedList slt = new ConcurrentSortedList();

    }

    public static void main(String[] args) throws InterruptedException, IOException {

    }
}
