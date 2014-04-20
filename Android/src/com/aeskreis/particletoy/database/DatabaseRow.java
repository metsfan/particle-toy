package com.aeskreis.particletoy.database;

import android.database.Cursor;

import java.util.HashMap;

/**
 * Created by adam on 4/19/14.
 */
public class DatabaseRow
{
    HashMap<String, Object> mFields;

    public DatabaseRow(Cursor cursor)
    {
        mFields = new HashMap<String, Object>();

        for (int i = 0; i < cursor.getColumnCount(); i++) {
            String columnName = cursor.getColumnName(i);
            int type = cursor.getType(i);

            Object value = null;

            switch (type)
            {
                case Cursor.FIELD_TYPE_INTEGER:
                    value = cursor.getLong(i);
                    break;

                case Cursor.FIELD_TYPE_FLOAT:
                    value = cursor.getFloat(i);
                    break;

                case Cursor.FIELD_TYPE_STRING:
                    value = cursor.getString(i);
                    break;

                case Cursor.FIELD_TYPE_BLOB:
                    value = cursor.getBlob(i);
                    break;
            }

            mFields.put(columnName, value);
        }
    }

    public Object getValue(String field)
    {
        return mFields.get(field);
    }
}
