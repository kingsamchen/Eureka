package com.company.createthread;

public class Main {

    public static void main(String[] args) throws InterruptedException {
        Thread myThread = new Thread(() -> System.out.println("Hello world from new thread!"));
        myThread.start();
        Thread.yield();
        System.out.println("Hello world from main thread!");
        myThread.join();
    }
}
