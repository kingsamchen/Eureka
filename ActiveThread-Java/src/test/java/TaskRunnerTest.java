/*
 @ 0xCCCCCCCC
*/

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Assertions;

class TaskRunnerTest {
    @Test
    void testPostTasks() {
        boolean[] flags = {false, false};

        MessageLoop.prepare();

        TaskRunner runner = new TaskRunner();
        runner.postTask(()-> {
            flags[0] = true;
            runner.postDelayedTask(()-> {
                flags[1] = true;
                runner.postTask(runner.getMessageLoop()::quit);
            }, 50);
        });

        MessageLoop.loop();
        MessageLoop.reset();

        Assertions.assertTrue(flags[0]);
        Assertions.assertTrue(flags[1]);
    }

    @Test
    void testRunTask() {
        final long tid = Thread.currentThread().getId();

        MessageLoop.prepare();

        TaskRunner runner = new TaskRunner();

        new Thread(()-> {
            Assertions.assertNotEquals(tid, Thread.currentThread().getId());
            runner.runTask(()-> Assertions.assertEquals(tid, Thread.currentThread().getId()));
            runner.postTask(runner.getMessageLoop()::quit);
        }).start();

        MessageLoop.loop();
        MessageLoop.reset();
    }
}
