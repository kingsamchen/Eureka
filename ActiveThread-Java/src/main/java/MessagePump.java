/*
 @ 0xCCCCCCCC
*/

import java.time.Instant;

interface MessagePump {
    interface Delegate {
        boolean doWork();
        boolean doDelayedWork();
        Instant nextDelayedWorkExpiration();
    }

    void run(Delegate delegate);

    // Quit the pump.
    // This method can only be called on the thread that called run().
    void quit();

    void wakeup();
}
