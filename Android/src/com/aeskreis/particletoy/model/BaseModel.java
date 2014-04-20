package com.aeskreis.particletoy.model;

import android.content.ContentValues;

import com.aeskreis.particletoy.database.DatabaseConnection;
import com.aeskreis.particletoy.database.DatabaseRow;

/**
 * Created by adam on 4/19/14.
 */
public abstract class BaseModel
{
    private long mId;

    public BaseModel()
    {
    }

    protected BaseModel(DatabaseRow row) {
        mId = (Long)row.getValue("id");
    }

    public long getId() {
        return mId;
    }

    public void setId(long id) {
        mId = id;
    }

    public void save(DatabaseConnection conn)
    {
        if (conn != null) {
            this.createTableIfNeeded(conn);

            ContentValues values = this.getFieldValues();

            if (mId > 0) {
                conn.executeUpdate(this.getTableName(), values, "id = ?", String.valueOf(mId));
            } else {
                conn.executeInsert(this.getTableName(), values);
            }
        }
    }
    public void delete(DatabaseConnection conn)
    {
        if (conn != null) {
            this.createTableIfNeeded(conn);

            conn.executeDelete(this.getTableName(), "id = ?", String.valueOf(mId));
        }
    }

    protected abstract ContentValues getFieldValues();
    protected abstract String getTableName();
    protected abstract void createTableIfNeeded(DatabaseConnection conn);
}
