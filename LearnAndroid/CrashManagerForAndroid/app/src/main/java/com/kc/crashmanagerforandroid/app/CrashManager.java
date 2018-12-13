package com.kc.crashmanagerforandroid.app;

import android.app.ActivityManager;
import android.content.Context;
import android.util.Log;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.io.Writer;
import java.lang.Thread.UncaughtExceptionHandler;

/**
 * @ 0xCCCCCCCC
 */
public class CrashManager implements UncaughtExceptionHandler {
    private Context mAppContext;
    private UncaughtExceptionHandler mOldCrashHandler;

    private static class Holder {
        static final CrashManager INSTANCE = new CrashManager();
    }

    public static CrashManager getInstance() {
        return Holder.INSTANCE;
    }

    // Call in Application.onCreate().
    public void installCrashHandler(Context appContext) {
        mAppContext = appContext;
        mOldCrashHandler = Thread.getDefaultUncaughtExceptionHandler();
        Thread.setDefaultUncaughtExceptionHandler(this);
    }

    @Override
    public void uncaughtException(Thread thread, Throwable ex) {
        StringWriter writer = new StringWriter();

        // Gather device information...
        saveStackTrace(writer, ex);

        if (BuildConfig.DEBUG) {
            Log.i("-kc-", writer.toString());
        }

        // Upload to sever.

        // Display crash notification only in main process.
        if (isMainProcess(mAppContext)) {
            mOldCrashHandler.uncaughtException(thread, ex);
        } else {
            System.exit(1);
        }
    }

    private static void saveStackTrace(Writer writer, Throwable ex) {
        PrintWriter printer = new PrintWriter(writer);
        ex.printStackTrace(printer);
    }

    private static boolean isMainProcess(Context appContext) {
        String packageName = appContext.getPackageName();
        return packageName.equals(getProcessName(appContext));
    }

    private static String getProcessName(Context appContext) {
        ActivityManager manager =
                (ActivityManager)appContext.getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningAppProcessInfo info : manager.getRunningAppProcesses()) {
            if (info.pid == android.os.Process.myPid()) {
                return info.processName;
            }
        }

        return "";
    }
}
