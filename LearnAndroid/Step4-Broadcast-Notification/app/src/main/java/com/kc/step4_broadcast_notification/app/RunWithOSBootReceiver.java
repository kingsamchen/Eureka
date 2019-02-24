package com.kc.step4_broadcast_notification.app;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

/**
 * @ 0xCCCCCCCC
 */
public class RunWithOSBootReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        Toast.makeText(context, "Run when OS start", Toast.LENGTH_SHORT).show();
    }
}
