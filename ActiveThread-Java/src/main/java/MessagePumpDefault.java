/*
 @ 0xCCCCCCCC
*/

import java.time.Instant;

public class MessagePumpDefault implements MessagePump {
    private SyncEvent _event = new SyncEvent();
    private boolean _running = false;

    @Override
    public void run(Delegate delegate) {
        _running = true;

        while (true) {
            boolean doneWork = delegate.doWork();
            if (!_running) {
                break;
            }

            doneWork |= delegate.doDelayedWork();
            if (!_running) {
                break;
            }

            // Since we have completed at least one task, it is highly possible there are tasks
            // still pending.
            if (doneWork) {
                continue;
            }

            Instant expiration = delegate.nextDelayedWorkExpiration();
            if (Instant.EPOCH.equals(expiration)) {
                _event.await();
            } else {
                _event.awaitUntil(expiration);
            }
        }
    }

    @Override
    public void quit() {
        _running = false;
    }

    @Override
    public void wakeup() {
        _event.signal();
    }
}
