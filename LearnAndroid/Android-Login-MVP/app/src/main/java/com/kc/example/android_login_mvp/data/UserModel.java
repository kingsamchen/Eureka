package com.kc.example.android_login_mvp.data;

import android.support.v4.util.ArrayMap;

import java.util.Map;

/**
 * @ 0xCCCCCCCC
 */
public final class UserModel {
    private Map<String, String> mUserAccount = new ArrayMap<>();

    {
        mUserAccount.put("kc", "kckckc");
    }

    public boolean validateUserAccount(String username, String password) {
        String pwd = mUserAccount.get(username);
        return pwd != null && pwd.equals(password);
    }
}
