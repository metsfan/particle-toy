package com.aeskreis.particletoy.activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;

import com.aeskreis.particletoy.R;
import com.aeskreis.particletoy.util.ExternalFileLoader;
import com.aeskreis.particletoy.view.HomeView;

public class HomeActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        ExternalFileLoader.createDefaultDirectory();

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_home);

        HomeView view = (HomeView)this.findViewById(R.id.homeView);
        view.setDatabaseConnection(this.getDatabaseConnection());
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        HomeView view = (HomeView)this.findViewById(R.id.homeView);
        view.loadData();
    }
}
