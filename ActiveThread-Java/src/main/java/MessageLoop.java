/*
 @ 0xCCCCCCCC
*/

import java.time.Instant;
import java.util.ArrayDeque;
import java.util.PriorityQueue;
import java.util.Queue;

public class MessageLoop implements MessagePump.Delegate {
    private static ThreadLocal<MessageLoop> tls_loopInThread = new ThreadLocal<>();

    private final long _ownerThreadID = Thread.currentThread().getId();

    private AutoReentrantLock _incomingTaskLock = new AutoReentrantLock();
    private Queue<PendingTask> _incomingTasks = new ArrayDeque<>();

    private Queue<PendingTask> _outgoingTasks = new ArrayDeque<>();

    private PriorityQueue<PendingTask> _delayedTasks = new PriorityQueue<>(new PendingTaskComparator());
    private Instant _recentTime = Instant.EPOCH;

    private MessagePump _pump = new MessagePumpDefault();

    // Can be called only once on a thread.
    public static void prepare() {
        if (tls_loopInThread.get() != null) {
            throw new RuntimeException("Current thread is already bound with a MessageLoop");
        }

        tls_loopInThread.set(new MessageLoop());
    }

    public static MessageLoop current() {
        if (tls_loopInThread.get() == null) {
            throw new RuntimeException("Current thread is not bound with a MessageLoop");
        }

        return tls_loopInThread.get();
    }

    // Run the message-loop bound with the thread.
    public static void loop() {
        MessageLoop.current().run();
    }

    public void run() {
        _pump.run(this);
    }

    // Thread-safe.
    public void quit() {
        if (belongsToCurrentThread()) {
            _pump.quit();
        } else {
            enqueueTask(new PendingTask(_pump::quit, Instant.EPOCH));
        }
    }

    // Thread-safe
    public void wakeup() {
        _pump.wakeup();
    }

    public void enqueueTask(PendingTask task) {
        try (AutoCloseableLock lock = _incomingTaskLock.lockAsAuto()) {
            _incomingTasks.add(task);
        }

        if (!belongsToCurrentThread()) {
            wakeup();
        }
    }

    public boolean belongsToCurrentThread() {
        return _ownerThreadID == Thread.currentThread().getId();
    }

    @Override
    public boolean doWork() {
        for (PendingTask nextTask = takeTask(); nextTask != null; nextTask = takeTask()) {
            if (Instant.EPOCH.equals(nextTask.endTime())) {
                nextTask.run();
                return true;
            }

            _delayedTasks.add(nextTask);
        }

        return false;
    }

    @Override
    public boolean doDelayedWork() {
        PendingTask nextTask = _delayedTasks.peek();
        if (nextTask == null) {
            return false;
        }

        Instant nextRunTime = nextTask.endTime();
        if (nextRunTime.isAfter(_recentTime)) {
            // Calibrate only when the last view of 'now' has fallen behind.
            _recentTime = Instant.now();
            if (nextRunTime.isAfter(_recentTime)) {
                return false;
            }
        }

        _delayedTasks.poll();

        nextTask.run();

        return true;
    }

    @Override
    public Instant nextDelayedWorkExpiration() {
        PendingTask top = _delayedTasks.peek();
        return top == null ? Instant.EPOCH : top.endTime();
    }

    // Retrieve a pending task.
    // Returns null when no pending task.
    private PendingTask takeTask() {
        if (_outgoingTasks.isEmpty()) {
            try (AutoCloseableLock lock = _incomingTaskLock.lockAsAuto()) {
                _outgoingTasks.addAll(_incomingTasks);
                _incomingTasks.clear();
            }
        }

        return _outgoingTasks.poll();
    }
}
