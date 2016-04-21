package com.kc.example.android_login_mvp.login;

import android.support.annotation.NonNull;

/**
 @ 0xCCCCCCCC
 */
public class LoginPresenter implements LoginContract.Presenter {

    LoginContract.View mView;

    public LoginPresenter(@NonNull LoginContract.View view) {
        mView = view;
    }

    @Override
    public void login(String username, String password) {
        if (username.equals("kc") && password.equals("kckckc")) {
            mView.showSuccessfullyLogin();
        } else {
            mView.showLoginError();
        }
    }

    @Override
    public void checkHasPreparedForLogin(String username, String password) {
        if (username.isEmpty() || password.isEmpty()) {
            mView.setNotReady();
        } else {
            mView.setReady();
        }
    }
}
