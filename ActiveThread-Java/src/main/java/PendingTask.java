/*
 @ 0xCCCCCCCC
*/

import java.time.Instant;
import java.util.Comparator;
import java.util.concurrent.atomic.AtomicLong;

class PendingTask {
    private static AtomicLong s_seqNumGenerator = new AtomicLong(0);

    private final long _seqNum;
    private final Instant _endTime;

    private Runnable _task;

    PendingTask(Runnable task, Instant endTime) {
        _seqNum = s_seqNumGenerator.getAndIncrement();
        _endTime = endTime;

        _task = task;
    }

    void run() {
        _task.run();
    }

    Instant endTime() {
        return _endTime;
    }

    long seqNum() {
        return _seqNum;
    }
}

class PendingTaskComparator implements Comparator<PendingTask> {
    @Override
    public int compare(PendingTask lhs, PendingTask rhs) {
        if (lhs.endTime().isBefore(rhs.endTime())) {
            return -1;
        }

        if (lhs.endTime().isAfter(rhs.endTime())) {
            return 1;
        }

        long diff = lhs.seqNum() - rhs.seqNum();

        if (diff < 0) {
            return -1;
        }

        if (diff > 0) {
            return 1;
        }

        return 0;
    }
}
