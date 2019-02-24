package com.kc.newsreader.app;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

/**
 * @ 0xCCCCCCCC
 */
public class NewsContentFragment extends Fragment {
    private View view;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.news_item_fragment, container, false);
        return view;
    }

    public void refresh(String newsTitle, String newsContent) {
        View layout = view.findViewById(R.id.visibility_layout);
        layout.setVisibility(View.VISIBLE);
        TextView titleText = (TextView)view.findViewById(R.id.news_title);
        TextView contentText = (TextView)view.findViewById(R.id.news_content);
        titleText.setText(newsTitle);
        contentText.setText(newsContent);
    }
}
