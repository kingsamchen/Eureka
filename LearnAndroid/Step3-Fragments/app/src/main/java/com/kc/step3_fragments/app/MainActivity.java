package com.kc.step3_fragments.app;

import android.app.Activity;
import android.app.FragmentManager;
import android.app.FragmentTransaction;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button btnChangeView = (Button)findViewById(R.id.button);
        btnChangeView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AnotherRightFragment rightFragment = new AnotherRightFragment();
                FragmentManager fragmentManager = getFragmentManager();
                FragmentTransaction transaction = fragmentManager.beginTransaction();
                transaction.replace(R.id.right_layout, rightFragment);
                transaction.addToBackStack(null);
                transaction.commit();
            }
        });
    }
}
