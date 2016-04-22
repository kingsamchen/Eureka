package com.kc.example.android_login_mvp.login;

import android.support.annotation.NonNull;
import com.kc.example.android_login_mvp.data.UserModel;

/**
 @ 0xCCCCCCCC
 */
public class LoginPresenter implements LoginContract.Presenter {

    LoginContract.View mView;
    UserModel mUserModel;

    public LoginPresenter(@NonNull LoginContract.View view) {
        mView = view;
        mUserModel = new UserModel();
    }

    @Override
    public void start() {
        checkHasPreparedForLogin("", "");
    }

    @Override
    public void stop() {}

    @Override
    public void login(String username, String password) {
        if (mUserModel.validateUserAccount(username, password)) {
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
