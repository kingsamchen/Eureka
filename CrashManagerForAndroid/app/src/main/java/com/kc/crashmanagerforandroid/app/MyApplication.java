package com.kc.crashmanagerforandroid.app;

import android.app.Application;

/**
 * @ 0xCCCCCCCC
 */
public class MyApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        CrashManager.getInstance().installCrashHandler(this);
    }
}
