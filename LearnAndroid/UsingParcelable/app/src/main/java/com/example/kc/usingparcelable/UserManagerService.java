package com.example.kc.usingparcelable;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;

import java.util.ArrayList;
import java.util.List;

/**
 * @ 0xCCCCCCCC
 */
public class UserManagerService extends Service {
    private List<ParcelableUser> mUsers = new ArrayList<>();

    private IBinder mBinder = new UserManager.Stub() {
        @Override
        public List<ParcelableUser> getUser() throws RemoteException {
            return mUsers;
        }
    };

    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        mUsers.add(new ParcelableUser("kc", 23, 1));
    }
}
