package com.aeskreis.particletoy.view;

import android.content.Context;
import com.aeskreis.particletoy.model.Effect;
import com.citymaps.citymapsengine.WindowAndroid;

/**
 * Created by adam on 4/19/14.
 */
public class EffectRendererWindow extends WindowAndroid
{
    static
    {
        System.loadLibrary("MapEngine");
        System.loadLibrary("ParticleToy");
    }

    private static long msNativePtr = 0;

    private native long nativeInitialize();
    private native boolean nativeCompile(long appPtr, long effectPtr, Effect effect);
    private native void nativeRun(long appPtr, long effectPtr);

    private Effect mEffect;
    private long mApplicationPtr;

    public EffectRendererWindow(Context context) {
        super(context);
    }

    @Override
    protected void onInit(long applicationPointer, int width, int height) {
        mApplicationPtr = applicationPointer;

        if (msNativePtr != 0 && mApplicationPtr != 0) {
            if (this.nativeCompile(mApplicationPtr, msNativePtr, mEffect)) {
                //nativeRun(mApplicationPtr, msNativePtr);
            }
        }
    }

    @Override
    protected void onDisable() {

    }

    @Override
    protected void onEnable() {

    }

    @Override
    protected void onIdle() {

    }

    public void setEffect(Effect effect) {
        mEffect = effect;

        if (msNativePtr == 0) {
            msNativePtr = this.nativeInitialize();
        }
    }
}