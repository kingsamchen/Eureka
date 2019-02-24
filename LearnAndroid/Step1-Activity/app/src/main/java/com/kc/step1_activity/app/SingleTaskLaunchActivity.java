package com.kc.step1_activity.app;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;


public class SingleTaskLaunchActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_single_task_launch);

        Log.i("kc-launch-mode", this.toString());

        Button btnLaunchItself = (Button)findViewById(R.id.launch_itself);
        btnLaunchItself.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(SingleTaskLaunchActivity.this, SingleTaskLaunchActivity.class));
            }
        });

        Button btnLaunchAnother = (Button)findViewById(R.id.launch_another);
        btnLaunchAnother.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(SingleTaskLaunchActivity.this, MainActivity.class));
            }
        });
    }

}
