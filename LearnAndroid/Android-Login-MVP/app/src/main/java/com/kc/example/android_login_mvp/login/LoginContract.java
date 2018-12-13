package com.kc.example.android_login_mvp.login;

import com.kc.example.android_login_mvp.BasePresenter;

/**
 @ 0xCCCCCCCC
 */
public interface LoginContract {

    interface View {

        void showSuccessfullyLogin();

        void showLoginError();

        void setReady();

        void setNotReady();
    }

    interface Presenter extends BasePresenter {

        void login(String username, String password);

        void checkHasPreparedForLogin(String username, String password);

    }
}
