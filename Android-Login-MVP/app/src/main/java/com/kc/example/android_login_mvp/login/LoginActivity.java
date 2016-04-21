package com.kc.example.android_login_mvp.login;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import com.kc.example.android_login_mvp.MainActivity;
import com.kc.example.android_login_mvp.R;

/**
 @ 0xCCCCCCCC
 */
public class LoginActivity extends AppCompatActivity implements LoginContract.View {

    private LoginContract.Presenter mPresenter;

    private Button mBtnLogin;
    private EditText mEditUserName;
    private EditText mEditPassword;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        mPresenter = new LoginPresenter(this);

        // tedious setup.
        mEditUserName = (EditText)findViewById(R.id.edt_name);
        assert mEditUserName != null;
        mEditPassword = (EditText)findViewById(R.id.edt_password);
        assert mEditPassword != null;
        mBtnLogin = (Button)findViewById(R.id.btn_login);
        assert mBtnLogin != null;

        mBtnLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String username = mEditUserName.getText().toString();
                String password = mEditPassword.getText().toString();
                mPresenter.login(username, password);
            }
        });
    }

    @Override
    public void showSuccessfullyLogin() {
        Toast.makeText(this, "Welcome", Toast.LENGTH_SHORT).show();
        startActivity(new Intent(this, MainActivity.class));
        finish();
    }

    @Override
    public void showLoginError() {
        Toast.makeText(this, "Username or password is incorrect!", Toast.LENGTH_SHORT).show();
        mEditPassword.getText().clear();
    }

}
