package com.kc.step1_activity.app;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;


public class MainActivity extends Activity {
    static final int req_code = 1;

    @Override
    protected void onStart() {
        super.onStart();
        Log.i("0xKC", "onStart() is called");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i("0xKC", "onResume() is called");
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.i("0xKC", "onPause() is called");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.i("0xKC", "onDestroy() is called");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.i("0xKC", "onRestart() is called");
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.i("0xKC", "onStop() is called");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Log.i("0xKC", "onCreate() is called");
        Log.i("kc-taskid", "Current task id for MainActivity is: " + getTaskId());

        Button btn = (Button)findViewById(R.id.button);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String data = "hello, the second activity";
                Intent intent = new Intent(MainActivity.this, SecondActivity.class);
                intent.putExtra("welcome_message", data);
                startActivityForResult(intent, req_code);
            }
        });

        Button btnStartNormal = (Button)findViewById(R.id.start_normal);
        btnStartNormal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, SecondActivity.class);
                startActivity(intent);
            }
        });

        Button btnStartDialog = (Button)findViewById(R.id.start_dialog);
        btnStartDialog.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(MainActivity.this, DialogActivity.class);
                startActivity(intent);
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        switch (id) {
            case R.id.view_web_page:
                Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse("http://cn.bing.com"));
                startActivity(intent);
                break;
            case R.id.launch_standard:
                startActivity(new Intent(MainActivity.this, StandardLaunchActivity.class));
                break;
            case R.id.launch_single_top:
                startActivity(new Intent(MainActivity.this, SingleTopLaunchActivity.class));
                break;
            case R.id.launch_single_task:
                startActivity(new Intent(MainActivity.this, SingleTaskLaunchActivity.class));
                break;
            case R.id.launch_single_instance:
                startActivity(new Intent(MainActivity.this, SingleInstanceLaunchActivity.class));
                break;
            case R.id.action_settings:
                Toast.makeText(MainActivity.this, "You clicked setting menu", Toast.LENGTH_SHORT).show();
                break;
            default:
                break;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case req_code:
                if (resultCode == RESULT_OK) {
                    Toast.makeText(MainActivity.this, data.getStringExtra("reply_message"), Toast.LENGTH_SHORT)
                         .show();
                }
                break;

            default:
                super.onActivityResult(requestCode, resultCode, data);
        }
    }
}
