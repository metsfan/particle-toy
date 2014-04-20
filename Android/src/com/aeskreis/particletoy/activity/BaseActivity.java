package com.aeskreis.particletoy.activity;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.Window;

import com.aeskreis.particletoy.database.DatabaseConnection;

/**
 * Created by adam on 4/19/14.
 */
public class BaseActivity extends Activity {
    private static DatabaseConnection msDatabaseConn = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
    }

    public DatabaseConnection getDatabaseConnection()
    {
        if (msDatabaseConn == null) {
            msDatabaseConn = new DatabaseConnection(this, "particle_toy");
        }

        return msDatabaseConn;
    }
}
