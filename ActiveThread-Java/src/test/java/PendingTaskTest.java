/*
 @ 0xCCCCCCCC
*/

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Assertions;

import java.time.Instant;

class PendingTaskTest {
    @Test
    void testFundamental() {
        final int[] hit = {0, 0};

        PendingTask task1 = new PendingTask(()-> hit[0]++, Instant.EPOCH);
        PendingTask task2 = new PendingTask(()-> hit[1] = 1024, Instant.EPOCH);

        // Seq-numbers
        Assertions.assertEquals(0, task1.seqNum());
        Assertions.assertEquals(1, task2.seqNum());

        // Run tasks

        Assertions.assertEquals(0, hit[0]);
        task1.run();
        Assertions.assertEquals(1, hit[0]);

        Assertions.assertEquals(0, hit[1]);
        task2.run();
        Assertions.assertEquals(1024, hit[1]);
    }

    @Test
    void testTaskOrder() {
        PendingTaskComparator cmp = new PendingTaskComparator();

        // ts = 0
        PendingTask task1 = new PendingTask(()-> {}, Instant.EPOCH);

        // ts = now + 3s
        PendingTask delayedTask1 = new PendingTask(()-> {}, Instant.now().plusSeconds(3));
        PendingTask delayedTask2 = new PendingTask(()-> {}, Instant.now().plusSeconds(3));

        // ts = now - 3s
        PendingTask delayedTask3 = new PendingTask(()-> {}, Instant.now().minusSeconds(3));

        // epoch is the earliest
        Assertions.assertTrue(cmp.compare(task1, delayedTask1) < 0);
        Assertions.assertTrue(cmp.compare(task1, delayedTask3) < 0);

        // before -> now -> after
        Assertions.assertTrue(cmp.compare(delayedTask3, delayedTask1) < 0);

        // Same end-time but earlier task has less seq-num.
        Assertions.assertEquals(delayedTask1.endTime(), delayedTask2.endTime());
        Assertions.assertTrue(cmp.compare(delayedTask1, delayedTask2) < 0);
    }
}
