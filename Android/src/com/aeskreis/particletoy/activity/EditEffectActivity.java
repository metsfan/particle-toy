package com.aeskreis.particletoy.activity;

import android.content.Intent;
import android.os.Bundle;

import com.aeskreis.particletoy.R;
import com.aeskreis.particletoy.model.Effect;
import com.aeskreis.particletoy.view.EditEffectView;

public class EditEffectActivity extends BaseActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_edit_effect);

        EditEffectView view = (EditEffectView)this.findViewById(R.id.editEffectView);
        view.setDatabaseConnection(this.getDatabaseConnection());

        long id = this.getIntent().getLongExtra("effect", 0);
        if (id > 0) {
            Effect effect = Effect.getById(id, this.getDatabaseConnection());
            view.setEffect(effect);
        }
    }
}
