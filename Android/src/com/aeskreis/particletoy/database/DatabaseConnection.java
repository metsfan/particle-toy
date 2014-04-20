package com.aeskreis.particletoy.database;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by adam on 4/19/14.
 */
public class DatabaseConnection
{
    private SQLiteDatabase mDatabase;

    public DatabaseConnection(Context context, String databaseName)
    {
        String databasePath = context.getFilesDir().getAbsolutePath() + databaseName;

        mDatabase = SQLiteDatabase.openOrCreateDatabase(databasePath, null);
    }

    public List<DatabaseRow> executeQuery(String query, String... params)
    {
        Cursor cursor = mDatabase.rawQuery(query, params);

        List<DatabaseRow> rows = new ArrayList<DatabaseRow>();

        do {
            cursor.moveToNext();
            if (cursor.isAfterLast()) {
                break;
            }

            rows.add(new DatabaseRow(cursor));
        } while (true);

        return rows;
    }

    public long executeInsert(String table, ContentValues values)
    {
        return mDatabase.insert(table, null, values);
    }

    public int executeUpdate(String table, ContentValues values, String whereClause, String... whereArgs)
    {
        return mDatabase.update(table, values, whereClause, whereArgs);
    }

    public int executeDelete(String table, String whereClause, String... whereArgs)
    {
        return mDatabase.delete(table, whereClause, whereArgs);
    }

    public void execute(String sql)
    {
        mDatabase.execSQL(sql);
    }
}
