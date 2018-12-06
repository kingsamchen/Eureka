/*
 @ 0xCCCCCCCC
*/

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Assertions;

class ActiveThreadTest {
    @Test
    void testRunActiveThread() {
        ActiveThread worker = new ActiveThread("Worker");
        Assertions.assertNull(worker.getMessageLoop());
        Assertions.assertFalse(worker.isAlive());

        worker.start();

        // Block on init.
        TaskRunner runner = new TaskRunner(worker.getMessageLoop());

        Assertions.assertTrue(worker.isAlive());

        SyncEvent event = new SyncEvent();
        runner.postTask(()-> System.out.println("hello then"));
        runner.postTask(event::signal);
        event.await();

        boolean ok = worker.quit();
        Assertions.assertTrue(ok);
    }
}
