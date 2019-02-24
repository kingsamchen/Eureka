// UserManager.aidl
package com.example.kc.usingparcelable;

// Declare any non-default types here with import statements
import com.example.kc.usingparcelable.ParcelableUser;

interface UserManager {
    List<ParcelableUser> getUser();
}
