package com.example.kc.usingmessenger;

import android.app.Service;
import android.content.Intent;
import android.os.*;
import android.util.Log;

/**
 * @ 0xCCCCCCCC
 */
public class MessengerService extends Service {

    private static class MessengerHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Constants.MSG_FROM_CLIENT:
                    String data = msg.getData().getString(Constants.MSG_DATA_TAG);
                    Log.i("-kc-", "receive message from client: " + data);
                    Messenger clientMessenger = msg.replyTo;
                    Message replyMsg = Message.obtain(null, Constants.MSG_FROM_SERVER);
                    Bundle replyData = new Bundle();
                    replyData.putString(Constants.MSG_REPLY_TAG, "this is server, how you doing?");
                    replyMsg.setData(replyData);
                    try {
                        clientMessenger.send(replyMsg);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                    break;

                default:
                    super.handleMessage(msg);
            }
        }
    }

    private Messenger mMessenger = new Messenger(new MessengerHandler());

    @Override
    public IBinder onBind(Intent intent) {
        return mMessenger.getBinder();
    }
}
