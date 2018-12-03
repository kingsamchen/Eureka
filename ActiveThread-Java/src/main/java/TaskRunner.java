/*
 @ 0xCCCCCCCC
*/

// Our MessageLoop doesn't provide a way to complete pending tasks when it is asked to quit.
// Therefore our post-task methods don't care about if a task was submitted.

import java.time.Instant;

public class TaskRunner {
    private final MessageLoop _loop;

    // Uses current thread's owning MessageLoop.
    // Throws an exception if the thread has no bound MessageLoop.
    public TaskRunner() {
        _loop = MessageLoop.current();
    }

    public TaskRunner(MessageLoop loop) {
        _loop = loop;
    }

    public MessageLoop getMessageLoop() {
        return _loop;
    }

    public void postTask(Runnable r) {
        _loop.enqueueTask(new PendingTask(r, Instant.EPOCH));
    }

    public void postTaskAt(Runnable r, long uptimeMillis) {
        _loop.enqueueTask(new PendingTask(r, Instant.ofEpochMilli(uptimeMillis)));
    }

    public void postDelayedTask(Runnable r, long delayMillis) {
        _loop.enqueueTask(new PendingTask(r, Instant.now().plusMillis(delayMillis)));
    }

    public void runTask(Runnable r) {
        if (_loop.belongsToCurrentThread()) {
            r.run();
        } else {
            postTask(r);
        }
    }
}
