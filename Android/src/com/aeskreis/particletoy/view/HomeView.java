package com.aeskreis.particletoy.view;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.DataSetObserver;
import android.graphics.Color;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.TextView;

import com.aeskreis.particletoy.R;
import com.aeskreis.particletoy.activity.EditEffectActivity;
import com.aeskreis.particletoy.activity.ViewEffectActivity;
import com.aeskreis.particletoy.database.DatabaseConnection;
import com.aeskreis.particletoy.model.Effect;

import java.util.List;

/**
 * Created by adam on 4/19/14.
 */
public class HomeView extends LinearLayout implements ListAdapter
{
    private Button mCreateButton;
    private ListView mEffectsList;
    private List<Effect> mEffects;

    private DatabaseConnection mConn;

    public HomeView(Context context) {
        super(context);
    }

    public HomeView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public HomeView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public void setDatabaseConnection(DatabaseConnection conn)
    {
        mConn = conn;
    }

    @Override
    protected void onFinishInflate()
    {
        super.onFinishInflate();

        mCreateButton = (Button)this.findViewById(R.id.createButton);
        mCreateButton.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                Activity activity = (Activity)getContext();
                activity.startActivity(new Intent(activity, EditEffectActivity.class));
            }
        });


        final HomeView self = this;

        mEffectsList = (ListView)this.findViewById(R.id.effectsList);
        mEffectsList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                Activity activity = (Activity)getContext();
                Intent intent = new Intent(activity, ViewEffectActivity.class);
                intent.putExtra("effect", mEffects.get(i).getId());
                activity.startActivity(intent);
            }
        });

        mEffectsList.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            @Override
            public boolean onItemLongClick(AdapterView<?> adapterView, View view, int i, long l) {
                AlertDialog.Builder builder = new AlertDialog.Builder(self.getContext());
                String[] items = {
                    "Edit",
                    "Delete"
                };

                final int effectId = i;

                builder.setTitle("Modify " + mEffects.get(i).getName());
                builder.setItems(items, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int button) {
                        switch (button) {
                            case 0:
                                self.editEffect(effectId);
                                break;

                            case 1:
                                self.deleteEffect(effectId);
                                break;
                        }
                    }
                });

                builder.show();

                return true;
            }
        });
    }

    public void loadData()
    {
        mEffects = Effect.getAll(mConn);
        mEffectsList.setAdapter(this);
    }

    private void editEffect(int i)
    {
        Activity activity = (Activity)getContext();
        Intent intent = new Intent(activity, EditEffectActivity.class);
        intent.putExtra("effect", mEffects.get(i).getId());
        activity.startActivity(intent);
    }

    private void deleteEffect(int i)
    {
        final Effect effect = mEffects.get(i);

        AlertDialog.Builder alert = new AlertDialog.Builder(this.getContext());
        alert.setTitle("Are you sure you want to delete \"" + effect.getName() + "\"?");
        alert.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                effect.delete(mConn);

                loadData();
            }
        });
        alert.setNegativeButton("No", null);

        alert.show();
    }

    /** List adapter methods */

    @Override
    public boolean areAllItemsEnabled() {
        return true;
    }

    @Override
    public boolean isEnabled(int i) {
        return true;
    }

    @Override
    public void registerDataSetObserver(DataSetObserver dataSetObserver) {

    }

    @Override
    public void unregisterDataSetObserver(DataSetObserver dataSetObserver) {

    }

    @Override
    public int getCount() {
        return mEffects.size();
    }

    @Override
    public Object getItem(int i) {
        return mEffects.get(i);
    }

    @Override
    public long getItemId(int i) {
        return mEffects.get(i).getId();
    }

    @Override
    public boolean hasStableIds() {
        return true;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        LinearLayout row = (LinearLayout)view;
        if (view == null) {
            row = new LinearLayout(this.getContext());
            row.setOrientation(LinearLayout.VERTICAL);

            TextView label = new TextView(this.getContext());
            label.setId(1);
            label.setTextSize(20);
            label.setPadding(10, 15, 0, 15);
            label.setTextColor(Color.BLACK);
            row.addView(label);
        }

        Effect effect = mEffects.get(i);

        TextView label = (TextView)row.findViewById(1);
        label.setText(effect.getName());

        return row;
    }

    @Override
    public int getItemViewType(int i) {
        return 1;
    }

    @Override
    public int getViewTypeCount() {
        return 1;
    }

    @Override
    public boolean isEmpty() {
        return false;
    }
}
