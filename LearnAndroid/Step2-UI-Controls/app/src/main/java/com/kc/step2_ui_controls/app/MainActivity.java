package com.kc.step2_ui_controls.app;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Toast;

public class MainActivity extends Activity {
    Handler mHandler = new Handler();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btn = (Button)findViewById(R.id.button);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EditText edit = (EditText)findViewById(R.id.edit_text);
                String message = edit.getText().toString();
                Toast.makeText(MainActivity.this, message, Toast.LENGTH_SHORT).show();
                ImageView img = (ImageView)findViewById(R.id.img_view);
                img.setImageResource(R.mipmap.aodamiao);
            }
        });

        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                ProgressBar bar = (ProgressBar)findViewById(R.id.progress_bar);
                bar.setVisibility(View.GONE);
            }
        }, 2000);

        final Button alert_button = (Button)findViewById(R.id.button_alert);
        alert_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder alert_dialog = new AlertDialog.Builder(MainActivity.this);
                alert_dialog.setTitle("alert");
                alert_dialog.setMessage("You breached something important");
                alert_dialog.setCancelable(false);
                alert_dialog.setPositiveButton("Roger that", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Toast.makeText(MainActivity.this, "Good, soldier", Toast.LENGTH_SHORT).show();
                    }
                });
                alert_dialog.show();
            }
        });

        final Button progress_button = (Button)findViewById(R.id.button_progress);
        progress_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ProgressDialog.Builder progress_dialog = new ProgressDialog.Builder(MainActivity.this);
                progress_dialog.setTitle("Notification");
                progress_dialog.setMessage("Doing something, please wait a moment");
                progress_dialog.setCancelable(true);
                progress_dialog.show();
            }
        });
    }

}
