package com.aeskreis.particletoy.view;

import android.content.Context;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.Window;
import android.widget.LinearLayout;
import com.aeskreis.particletoy.model.Effect;
import com.citymaps.citymapsengine.WindowAndroid;

/**
 * Created by adam on 4/19/14.
 */
public class EffectRendererView extends LinearLayout {

    private EffectRendererWindow msWindow;

    private Effect mEffect;

    public EffectRendererView(Context context) {
        super(context);
    }

    public EffectRendererView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public EffectRendererView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected void onFinishInflate()
    {
        if (msWindow == null) {
            // Only ever build one window, and just keep reusing it
            msWindow = new EffectRendererWindow(this.getContext());
        }

        this.addView(msWindow);
    }

    public void setEffect(Effect effect) {
        mEffect = effect;

        msWindow.setEffect(effect);
    }
}
