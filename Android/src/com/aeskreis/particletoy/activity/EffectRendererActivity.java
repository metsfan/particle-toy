package com.aeskreis.particletoy.activity;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

import com.aeskreis.particletoy.R;
import com.aeskreis.particletoy.model.Effect;
import com.aeskreis.particletoy.view.EffectRendererView;

public class EffectRendererActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_effect_renderer);

        EffectRendererView view = (EffectRendererView)this.findViewById(R.id.effectRendererView);

        long id = this.getIntent().getLongExtra("effect", 0);
        if (id > 0) {
            Effect effect = Effect.getById(id, this.getDatabaseConnection());
            view.setEffect(effect);
        }
    }

}
