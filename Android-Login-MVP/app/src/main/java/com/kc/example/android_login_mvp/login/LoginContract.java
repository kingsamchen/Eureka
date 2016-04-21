package com.kc.example.android_login_mvp.login;

/**
 @ 0xCCCCCCCC
 */
public interface LoginContract {

    interface View {

        void showSuccessfullyLogin();

        void showLoginError();

    }

    interface Presenter {

        void login(String username, String password);

    }
}
