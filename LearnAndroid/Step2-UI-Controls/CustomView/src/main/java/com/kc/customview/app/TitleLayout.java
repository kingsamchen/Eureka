package com.kc.customview.app;

import android.app.Activity;
import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Toast;

/**
 * @ 0xCCCCCCCC
 */
public class TitleLayout extends LinearLayout {
    public TitleLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
        LayoutInflater.from(context).inflate(R.layout.title, this);
        Button btnTitleBack = (Button)findViewById(R.id.title_back);
        Button btnTitleEdit = (Button)findViewById(R.id.title_edit);
        btnTitleBack.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                ((Activity)getContext()).finish();
            }
        });

        btnTitleEdit.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(getContext(), "This title is not editable", Toast.LENGTH_SHORT).show();
            }
        });
    }
}
