import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.Assertions;

import java.time.Instant;

class SyncEventTest {
    @Test
    void testWaitAndSignal() {
        SyncEvent event = new SyncEvent();

        new Thread(()-> {
            try {
                Thread.sleep(2000);
            } catch (InterruptedException ignored) {
            } finally {
                System.out.println("Wakeup from another thread");
                event.signal();
            }
        }).start();

        System.out.println("Main thread is about to wait!");
        event.await();
    }

    @Test
    void testAutoReset() {
        SyncEvent event = new SyncEvent();
        event.signal();
        event.await();

        new Thread(()-> {
            try {
                Thread.sleep(2000);
            } catch (InterruptedException ignored) {
            } finally {
                System.out.println("Signal again");
                event.signal();
            }
        }).start();

        System.out.println("Wait again");
        event.await();
    }

    @Test
    void testTimedWait() {
        SyncEvent event = new SyncEvent();
        boolean cause = event.awaitUntil(Instant.now().plusSeconds(3));
        Assertions.assertFalse(cause);
    }

    @Test
    void testTimedWaitAlreadyExpired() {
        SyncEvent event = new SyncEvent();

        Instant deadline = Instant.now().minusSeconds(3);

        Instant waitBefore = Instant.now();
        Assertions.assertTrue(waitBefore.isAfter(deadline));

        boolean cause = event.awaitUntil(deadline);
        Instant waitAfter = Instant.now();

        Assertions.assertFalse(cause);
        // Invocation cost should be less than 5ms
        Assertions.assertTrue((waitAfter.toEpochMilli() - waitBefore.toEpochMilli()) < 5);
    }
}
