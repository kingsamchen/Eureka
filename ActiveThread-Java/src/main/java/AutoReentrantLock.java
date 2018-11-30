/*
 @ 0xCCCCCCCC
*/

import java.util.concurrent.locks.ReentrantLock;

interface AutoCloseableLock extends AutoCloseable {
    @Override
    void close();
}

class AutoReentrantLock extends ReentrantLock {
    AutoCloseableLock lockAsAuto() {
        lock();
        return this::unlock;
    }
}
