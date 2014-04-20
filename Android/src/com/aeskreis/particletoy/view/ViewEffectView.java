package com.aeskreis.particletoy.view;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.AttributeSet;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.aeskreis.particletoy.R;
import com.aeskreis.particletoy.activity.EffectRendererActivity;
import com.aeskreis.particletoy.model.Effect;
import com.aeskreis.particletoy.util.ExternalFileLoader;

import org.w3c.dom.Text;

/**
 * Created by adam on 4/19/14.
 */
public class ViewEffectView extends LinearLayout {
    TextView mEffectName;

    TextView mVertexShaderFile;
    TextView mFragmentShaderFile;
    TextView mScriptFile;

    Button mEditVertexShader;
    Button mEditFragmentShader;
    Button mEditScript;

    Button mPlayEffect;

    Effect mEffect;

    public ViewEffectView(Context context) {
        super(context);
    }

    public ViewEffectView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ViewEffectView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public void setEffect(Effect effect)
    {
        mEffect = effect;

        mEffectName.setText(effect.getName());
        mVertexShaderFile.setText(effect.getVertexShaderFile());
        mFragmentShaderFile.setText(effect.getFragmentShaderFile());
        mScriptFile.setText(effect.getScriptFile());
    }

    @Override
    protected void onFinishInflate()
    {
        super.onFinishInflate();

        mEffectName = (TextView)this.findViewById(R.id.effectName);

        mVertexShaderFile = (TextView)this.findViewById(R.id.vertexShaderFile);
        mFragmentShaderFile = (TextView)this.findViewById(R.id.fragmentShaderFile);
        mScriptFile = (TextView)this.findViewById(R.id.scriptFile);

        mEditVertexShader = (Button)this.findViewById(R.id.editVertexShader);
        mEditVertexShader.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                openEditorForFile("vertex_shaders/" + mEffect.getVertexShaderFile());
            }
        });

        mEditFragmentShader = (Button)this.findViewById(R.id.editFragmentShader);
        mEditFragmentShader.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                openEditorForFile("fragment_shaders/" + mEffect.getFragmentShaderFile());
            }
        });

        mEditScript = (Button)this.findViewById(R.id.editScript);
        mEditScript.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                openEditorForFile("scripts/" + mEffect.getScriptFile());
            }
        });

        mPlayEffect = (Button)this.findViewById(R.id.playEffect);
        mPlayEffect.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Activity activity = (Activity)getContext();
                Intent intent = new Intent(activity, EffectRendererActivity.class);
                intent.putExtra("effect", mEffect.getId());
                activity.startActivity(intent);
            }
        });
    }

    private void openEditorForFile(String file)
    {
        String fullFilepath = ExternalFileLoader.getAbsolutePath(file);

        Intent intent = new Intent(Intent.ACTION_EDIT);
        Uri uri = Uri.parse("file://" + fullFilepath);
        intent.setDataAndType(uri, "text/plain");

        this.getContext().startActivity(intent);
    }
}
