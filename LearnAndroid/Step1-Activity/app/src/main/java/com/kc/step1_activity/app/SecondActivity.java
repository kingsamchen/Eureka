package com.kc.step1_activity.app;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;


public class SecondActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_second);

        Intent intent = getIntent();
        String msg = intent.getStringExtra("welcome_message");
        ((TextView)findViewById(R.id.msg)).setText(msg);

        Button reply = (Button)findViewById(R.id.button_2);
        reply.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String reply = "Received your greeting message";
                Intent intent = new Intent();
                intent.putExtra("reply_message", reply);
                setResult(RESULT_OK, intent);
                finish();
            }
        });
    }

}
