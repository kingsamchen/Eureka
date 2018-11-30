/*
 @ 0xCCCCCCCC
*/

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Assertions;

import java.time.Instant;
import java.util.ArrayList;

class MessageLoopTest {
    @Test
    void testRunAndQuit() {
        MessageLoop looper = new MessageLoop();
        looper.enqueueTask(new PendingTask(looper::quit, Instant.EPOCH));
        looper.run();
    }

    @Test
    void testTaskAndDelayedTask() {
        int[] count = {0};

        MessageLoop looper = new MessageLoop();

        looper.enqueueTask(new PendingTask(()-> ++count[0], Instant.EPOCH));
        looper.enqueueTask(new PendingTask(()-> {
            ++count[0];
            looper.quit();
        }, Instant.now().plusSeconds(2)));

        looper.run();

        Assertions.assertEquals(2, count[0]);
    }

    @Test
    void testRunTasksFromOtherThreads() {
        boolean[] flag = {false};

        MessageLoop looper = new MessageLoop();

        new Thread(()-> {
            looper.enqueueTask(new PendingTask(()-> flag[0] = true, Instant.EPOCH));
            looper.enqueueTask(new PendingTask(looper::quit, Instant.EPOCH));
        }).start();

        looper.run();

        Assertions.assertTrue(flag[0]);
    }

    @Test
    void testDelayedTaskOrder() {
        ArrayList<Integer> seq = new ArrayList<>();

        MessageLoop looper = new MessageLoop();

        looper.enqueueTask(new PendingTask(()-> seq.add(0), Instant.now().plusMillis(500)));
        looper.enqueueTask(new PendingTask(()-> seq.add(1), Instant.now().plusMillis(300)));
        looper.enqueueTask(new PendingTask(()-> seq.add(2), Instant.now().plusMillis(300)));
        looper.enqueueTask(new PendingTask(()-> seq.add(3), Instant.EPOCH));

        looper.enqueueTask(new PendingTask(looper::quit, Instant.now().plusMillis(500)));

        looper.run();

        Assertions.assertArrayEquals(new Integer[] {3, 1, 2, 0}, seq.toArray(new Integer[0]));
    }

    @Test
    void testStaticMethods() {
        MessageLoop.prepare();
        MessageLoop.current().enqueueTask(new PendingTask(MessageLoop.current()::quit, Instant.EPOCH));
        MessageLoop.loop();
    }
}
