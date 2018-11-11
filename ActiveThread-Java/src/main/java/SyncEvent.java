/*
 @ 0xCCCCCCCC
*/

import java.time.Instant;
import java.util.Date;
import java.util.concurrent.locks.Condition;
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

// This SyncEvent is auto-reset, i.e. after being signaled, its state automatically
// transitions into non-signaled.
class SyncEvent {
    private AutoReentrantLock _lock = new AutoReentrantLock();
    private Condition _cond = _lock.newCondition();

    private boolean _signaled = false;

    SyncEvent() {}

    // Wait indefinitely for the event to be signaled.
    void await() {
        try (AutoCloseableLock lock = _lock.lockAsAuto()) {
            while (!_signaled) {
                try {
                    _cond.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }

            _signaled = false;
        }
    }

    // Guarantee no short-wait caused by spurious-wakeup will happen.
    boolean awaitUntil(Instant endTime) {
        boolean causedBySignaled = false;
        try (AutoCloseableLock lock = _lock.lockAsAuto()) {
            while (!_signaled && System.currentTimeMillis() < endTime.toEpochMilli()) {
                try {
                    causedBySignaled = _cond.awaitUntil(Date.from(endTime));
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }

            _signaled = false;
        }

        return causedBySignaled;
    }

    // Signal the event.
    void signal() {
        try (AutoCloseableLock lock = _lock.lockAsAuto()) {
            _signaled = true;
            _cond.signal();
        }
    }
}
