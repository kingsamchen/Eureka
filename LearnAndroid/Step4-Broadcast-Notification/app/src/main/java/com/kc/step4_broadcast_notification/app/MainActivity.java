package com.kc.step4_broadcast_notification.app;

import android.app.Activity;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
    public static final String MY_ACTION = "action.kc.is.handsome";

    private NetworkChangeReceiver mNetworkStateReceiver;
    private MyReceiver mMyReceiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button btn = (Button)findViewById(R.id.btn);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MY_ACTION);
                LocalBroadcastManager.getInstance(MainActivity.this).sendBroadcast(intent);
            }
        });

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(ConnectivityManager.CONNECTIVITY_ACTION);
        mNetworkStateReceiver = new NetworkChangeReceiver();
        registerReceiver(mNetworkStateReceiver, intentFilter);
        Log.i("kc-", "network change receiver setup");

        IntentFilter filter = new IntentFilter();
        filter.addAction(MY_ACTION);
        mMyReceiver = new MyReceiver();
        LocalBroadcastManager.getInstance(this).registerReceiver(mMyReceiver, filter);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(mNetworkStateReceiver);
        Log.i("kc-", "network change receiver removed");

        LocalBroadcastManager.getInstance(this).unregisterReceiver(mMyReceiver);
    }
}
