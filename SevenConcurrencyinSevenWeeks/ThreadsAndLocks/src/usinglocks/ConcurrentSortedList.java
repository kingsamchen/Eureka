package usinglocks;

import java.util.concurrent.locks.ReentrantLock;

public class ConcurrentSortedList {
    static private class Node {
        int value;
        Node prev;
        Node next;
        final ReentrantLock lock = new ReentrantLock();

        Node() {}

        Node(int value, Node prev, Node next) {
            this.value = value;
            this.prev = prev;
            this.next = next;
        }
    }

    private final Node head;
    private final Node tail;

    public ConcurrentSortedList() {
        head = new Node();
        tail = new Node();
        head.next = tail;
        tail.prev = head;
    }

    public void insert(int new_value) {
        Node current = head;
        current.lock.lock();
        Node next = current.next;
        try {
            while (true) {
                next.lock.lock();
                try {
                    if (next == tail || next.value > new_value) {
                        Node new_node = new Node(new_value, current, next);
                        current.next = new_node;
                        next.prev = new_node;
                        return;
                    }
                } finally {
                    current.lock.unlock();
                }

                current = next;
                next = current.next;
            }
        } finally {
            next.lock.unlock();
        }
    }

    // Don't call this function while others using the list.
    public void dumpList() {
        for (Node node = head.next; node != tail; node = node.next) {
            System.out.println(node.value);
        }
    }
}
