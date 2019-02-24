package com.kc.using_notification.app;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {
    private static final int MY_NOTIFICATION_ID = 0xCC;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btn = (Button)findViewById(R.id.btn_issue);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                issueNotification();
            }
        });
    }

    private void issueNotification() {
        NotificationManager notificationManager = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        Notification.Builder notificationBuilder = new Notification.Builder(this);
        Intent intent = new Intent(this, NotificationActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_CANCEL_CURRENT);
        notificationBuilder.setSmallIcon(R.mipmap.ic_launcher)
                           .setTicker("A quick test notification")
                           .setWhen(System.currentTimeMillis())
                           .setContentTitle("This is notification title")
                           .setContentText("This is gonna be a huge content for this message")
                           .setContentIntent(pendingIntent);
        Notification notification = notificationBuilder.build();
        notification.defaults = Notification.DEFAULT_ALL;
        notificationManager.notify(MY_NOTIFICATION_ID, notification);
    }
}
