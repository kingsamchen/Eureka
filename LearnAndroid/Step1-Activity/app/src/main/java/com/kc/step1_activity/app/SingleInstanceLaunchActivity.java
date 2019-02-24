package com.kc.step1_activity.app;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;


public class SingleInstanceLaunchActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_single_instance_launch);

        Log.i("kc-taskid", "Current task id for SingleInstanceLaunch Activity is " + getTaskId());

        Button btnLaunchAnother = (Button)findViewById(R.id.launch_another);
        btnLaunchAnother.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(SingleInstanceLaunchActivity.this, FakeWebViewActivity.class));
            }
        });
    }

}
