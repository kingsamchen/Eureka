package com.kc.using_server_basics.app;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

/**
 * @ 0xCCCCCCCC
 */
public class MyService extends Service {
    @Override
    public void onCreate() {
        super.onCreate();
        Log.i("-kc-", "Service.onCreate");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i("-kc-", "Service.onStartCommand");
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i("-kc-", "Service.onDestroy");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
