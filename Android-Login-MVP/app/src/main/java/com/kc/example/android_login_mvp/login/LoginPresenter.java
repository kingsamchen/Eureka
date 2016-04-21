package com.kc.example.android_login_mvp.login;

/**
 @ 0xCCCCCCCC
 */
public class LoginPresenter implements LoginContract.Presenter {

    LoginContract.View mView;

    public LoginPresenter(LoginContract.View view) {
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

}
