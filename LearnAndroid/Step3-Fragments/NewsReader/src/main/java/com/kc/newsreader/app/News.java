package com.kc.newsreader.app;

/**
 * @ 0xCCCCCCCC
 */
public class News {
    private String mTitle;
    private String mContent;

    public News() {}

    public News(String title, String content) {
        mTitle = title;
        mContent = content;
    }

    public String getTitle() {
        return mTitle;
    }

    public String getContent() {
        return mContent;
    }

    public void setTitle(String title) {
        mTitle = title;
    }

    public void setContent(String content) {
        mContent = content;
    }
}
