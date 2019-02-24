package com.example.kc.usingparcelable;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * @ 0xCCCCCCCC
 */
public class ParcelableUser implements Parcelable {
    private String mName;
    private int mAge;
    private int mGender;

    public ParcelableUser(String name, int age, int gender) {
        mName = name;
        mAge = age;
        mGender = gender;
    }

    private ParcelableUser(Parcel parcel) {
        mName = parcel.readString();
        mAge = parcel.readInt();
        mGender = parcel.readInt();
    }

    public static final Parcelable.Creator<ParcelableUser> CREATOR = new Parcelable.Creator<ParcelableUser>() {
        @Override
        public ParcelableUser createFromParcel(Parcel parcel) {
            return new ParcelableUser(parcel);
        }

        @Override
        public ParcelableUser[] newArray(int size) {
            return new ParcelableUser[size];
        }
    };

    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int flags) {
        parcel.writeString(mName);
        parcel.writeInt(mAge);
        parcel.writeInt(mGender);
    }

    public String getName() {
        return mName;
    }
}
