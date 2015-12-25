package com.kc.crashmanagerforandroid.app;

import java.lang.Thread.UncaughtExceptionHandler;

/**
 * @ 0xCCCCCCCC
 */
public class CrashManager implements UncaughtExceptionHandler {
    private static class Holder {
        static final CrashManager INSTANCE = new CrashManager();
    }

    public static CrashManager getInstance() {
        return Holder.INSTANCE;
    }

    @Override
    public void uncaughtException(Thread thread, Throwable ex) {

    }
}
