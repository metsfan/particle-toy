package com.aeskreis.particletoy.view;

import android.app.Activity;
import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;

import com.aeskreis.particletoy.R;
import com.aeskreis.particletoy.database.DatabaseConnection;
import com.aeskreis.particletoy.model.Effect;
import com.aeskreis.particletoy.util.ExternalFileLoader;

/**
 * Created by adam on 4/19/14.
 */
public class EditEffectView extends LinearLayout
{
    private DatabaseConnection mConn;

    private Effect mEffect;
    private EditText mNameText;
    private AutoCompleteTextView mVertexShaderText;
    private AutoCompleteTextView mFragmentShaderText;
    private AutoCompleteTextView mScriptText;
    private Button mSaveButton;
    private boolean mLoaded;

    public EditEffectView(Context context) {
        super(context);
    }

    public EditEffectView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public EditEffectView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public void setDatabaseConnection(DatabaseConnection conn)
    {
        mConn = conn;
    }

    public void setEffect(Effect effect)
    {
        mEffect = effect;

        if (mLoaded) {
            this.populateValues();
        }
    }

    @Override
    protected void onFinishInflate()
    {
        super.onFinishInflate();

        mNameText = (EditText)this.findViewById(R.id.name);
        mVertexShaderText = (AutoCompleteTextView)this.findViewById(R.id.vertexShader);
        mFragmentShaderText = (AutoCompleteTextView)this.findViewById(R.id.fragmentShader);
        mScriptText = (AutoCompleteTextView)this.findViewById(R.id.script);

        mSaveButton = (Button)this.findViewById(R.id.saveButton);
        mSaveButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mEffect == null) {
                    mEffect = new Effect();
                }
                mEffect.setName(mNameText.getText().toString());
                mEffect.setVertexShaderFile(mVertexShaderText.getText().toString());
                mEffect.setFragmentShaderFile(mFragmentShaderText.getText().toString());
                mEffect.setScriptFile(mScriptText.getText().toString());
                mEffect.save(mConn);

                ExternalFileLoader.createFile("vertex_shaders/" + mEffect.getVertexShaderFile());
                ExternalFileLoader.createFile("fragment_shaders/" + mEffect.getFragmentShaderFile());
                ExternalFileLoader.createFile("scripts/" + mEffect.getScriptFile());

                Activity activity = (Activity)getContext();
                activity.finish();
            }
        });

        if (mEffect != null) {
            this.populateValues();
        }

        this.setAutocompleteFileOptions(mVertexShaderText, "vertex_shaders");
        this.setAutocompleteFileOptions(mFragmentShaderText, "fragment_shaders");
        this.setAutocompleteFileOptions(mScriptText, "scripts");

        mLoaded = true;
    }

    private void setAutocompleteFileOptions(final AutoCompleteTextView textbox, String directory)
    {
        String[] files = ExternalFileLoader.getFilesInDirectory(directory);
        if (files != null) {
            ArrayAdapter<String> adapter = new ArrayAdapter<String>(this.getContext(),
                    android.R.layout.simple_dropdown_item_1line, files);
            textbox.setAdapter(adapter);

            /*textbox.setOnTouchListener(new OnTouchListener() {
                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {
                    textbox.showDropDown();

                    return false;
                }
            });*/
        }
    }

    private void populateValues()
    {
        mNameText.setText(mEffect.getName());
        mVertexShaderText.setText(mEffect.getVertexShaderFile());
        mFragmentShaderText.setText(mEffect.getFragmentShaderFile());
        mScriptText.setText(mEffect.getScriptFile());
    }
}
