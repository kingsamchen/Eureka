package com.kc.newsreader.app;

import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

/**
 * @ 0xCCCCCCCC
 */
public class NewsTitleFragment extends Fragment {
    private ListView mNewsTitleList;
    private NewsAdapter mAdapter;
    private List<News> mNewsList;
    private boolean mTwoPageMode;

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);
        mNewsList = fakeNews();
        mAdapter = new NewsAdapter(activity, R.layout.news_item, mNewsList);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        mTwoPageMode = getActivity().findViewById(R.id.news_content) != null;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.news_title_fragment, container, false);
        mNewsTitleList = (ListView)view.findViewById(R.id.news_title_list_view);
        mNewsTitleList.setAdapter(mAdapter);
        mNewsTitleList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                News news = mNewsList.get(position);
                if (mTwoPageMode) {
                    NewsContentFragment contentFragment =
                            (NewsContentFragment)getFragmentManager().findFragmentById(R.id.news_content_fragment);
                    contentFragment.refresh(news.getTitle(), news.getContent());
                } else {
                    Intent intent = new Intent(getActivity(), NewsContentActivity.class);
                    intent.putExtra("news_title", news.getTitle());
                    intent.putExtra("news_content", news.getContent());
                    startActivity(intent);
                }
            }
        });
        return view;
    }

    private static List<News> fakeNews() {
        List<News> newsList = new ArrayList<News>();
        newsList.add(new News("title1", "content1"));
        newsList.add(new News("title2", "content2"));
        return newsList;
    }
}
