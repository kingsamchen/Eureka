package com.example.kc.usingmessenger;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.*;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private Messenger mServerMessenger;
    private Messenger mClientMessenger = new Messenger(new MessageHandler());

    private ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            mServerMessenger = new Messenger(iBinder);
            Log.i("-kc-", "MessengerService is connected");
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.i("-kc-", "MessengerService is detached");
        }
    };

    private static class MessageHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Constants.MSG_FROM_SERVER:
                    Log.i("-kc-", "receive msg from server: " +
                                  msg.getData().getString(Constants.MSG_REPLY_TAG));
                    break;

                default:
                    super.handleMessage(msg);
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Intent intent = new Intent(this, MessengerService.class);
        bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
        findViewById(R.id.btn).setOnClickListener(this);
    }

    @Override
    protected void onDestroy() {
        unbindService(mConnection);
        super.onDestroy();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btn:
                Message msg = Message.obtain(null, Constants.MSG_FROM_CLIENT);
                Bundle data = new Bundle();
                data.putString(Constants.MSG_DATA_TAG, "hello, this is client");
                msg.setData(data);
                msg.replyTo = mClientMessenger;
                try {
                    mServerMessenger.send(msg);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
                break;
        }
    }
}
