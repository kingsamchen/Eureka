package com.example.kc.viewfundalmentals;

import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.widget.Button;
import android.widget.Scroller;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private Scroller mScroller = null;

    private void smoothScrollTo(final View view, int destX, int destY) {
        final int startX = (int)view.getX();
        final int startY = (int)view.getY();
        final int deltaX = destX - startX;
        final int deltaY = destY - startY;
        ValueAnimator animator = ValueAnimator.ofInt(0, 1).setDuration(1000);
        animator.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animation) {
                float fraction = animation.getAnimatedFraction();
                int targetX = startX + (int) (fraction * deltaX);
                Log.i("-kc-", "targetX: " + String.valueOf(targetX));
                view.scrollTo(-targetX, 0);
            }
        });
        animator.start();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mScroller = new Scroller(this);
        findViewById(R.id.text).setOnTouchListener(new View.OnTouchListener() {
            int lastX, lastY;
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                int x = (int)event.getRawX();
                int y = (int)event.getRawY();
                if (event.getAction() == MotionEvent.ACTION_MOVE) {
                    int deltaX = x - lastX;
                    int deltaY = y - lastY;
                    Log.i("-kc-", String.format("transX:%f\ttransY:%f", v.getTranslationX(), v.getTranslationY()));
                    v.setTranslationX(v.getTranslationX() + deltaX);
                    v.setTranslationY(v.getTranslationY() + deltaY);
                }

                lastX = x;
                lastY = y;
                return true;
            }
        });
        findViewById(R.id.button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                smoothScrollTo(findViewById(R.id.text), 200, 50);
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

}
