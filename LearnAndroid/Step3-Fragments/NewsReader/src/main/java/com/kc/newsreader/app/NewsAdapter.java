package com.kc.newsreader.app;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.List;

/**
 * @ 0xCCCCCCCC
 */
public class NewsAdapter extends ArrayAdapter<News> {
    private int mResId;

    public NewsAdapter(Context context, int resource, List<News> objects) {
        super(context, resource, objects);
        mResId = resource;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        News news = getItem(position);
        View view;
        if (convertView == null) {
            view = LayoutInflater.from(getContext()).inflate(mResId, null);
        } else {
            view = convertView;
        }

        TextView newsTitle = (TextView)view.findViewById(R.id.news_title);
        newsTitle.setText(news.getTitle());

        return view;
    }
}
