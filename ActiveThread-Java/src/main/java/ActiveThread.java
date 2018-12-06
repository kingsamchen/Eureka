/*
 @ 0xCCCCCCCC
*/

import java.util.concurrent.locks.Condition;

public class ActiveThread {
    private Thread _thread;
    private MessageLoop _loop;
    private AutoReentrantLock _loopInitLock = new AutoReentrantLock();
    private Condition _loopInited = _loopInitLock.newCondition();

    public ActiveThread(String name) {
        _thread = new Thread(this::runThread, name);
    }

    public void start() {
        // Doesn't block on here
        _thread.start();
    }

    public boolean quit() {
        if (getMessageLoop() == null) {
            return false;
        }

        // Once we reach here, the message-loop has been inited, and no lock needed.
        _loop.quit();

        return true;
    }

    public long getId() {
        return _thread.getId();
    }

    public String getName() {
        return _thread.getName();
    }

    public boolean isAlive() {
        return _thread.isAlive();
    }

    // Block until the message-loop is prepared.
    public MessageLoop getMessageLoop() {
        if (!_thread.isAlive()) {
            return null;
        }

        try (AutoCloseableLock lock = _loopInitLock.lockAsAuto()) {
            while (_thread.isAlive() && _loop == null) {
                try {
                    _loopInited.await();
                } catch (InterruptedException ignored) {
                }
            }
        }

        return _loop;
    }

    private void runThread() {
        MessageLoop.prepare();

        try (AutoCloseableLock lock = _loopInitLock.lockAsAuto()) {
            _loop = MessageLoop.current();
            _loopInited.signalAll();
        }

        MessageLoop.loop();

        MessageLoop.reset();
    }
}
