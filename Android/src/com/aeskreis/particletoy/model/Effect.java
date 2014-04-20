package com.aeskreis.particletoy.model;

import android.content.ContentValues;
import android.database.sqlite.SQLiteDatabase;

import com.aeskreis.particletoy.database.DatabaseConnection;
import com.aeskreis.particletoy.database.DatabaseRow;
import com.aeskreis.particletoy.util.ExternalFileLoader;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by adam on 4/19/14.
 */
public class Effect extends BaseModel
{
    private String mName;
    private String mVertexShaderFile;
    private String mVertexShaderFilepath;
    private String mFragmentShaderFile;
    private String mFragmentShaderFilepath;
    private String mScriptFile;
    private String mScriptFilepath;

    public Effect() {
        super();
    }

    private Effect(DatabaseRow row) {
        super(row);

        mName = (String)row.getValue("name");
        mVertexShaderFile = (String)row.getValue("vertex_shader_file");
        mFragmentShaderFile = (String)row.getValue("fragment_shader_file");
        mScriptFile = (String)row.getValue("script_file");

        mVertexShaderFilepath = ExternalFileLoader.getAbsolutePath("vertex_shaders/" + mVertexShaderFile);
        mFragmentShaderFilepath = ExternalFileLoader.getAbsolutePath("fragment_shaders/" + mFragmentShaderFile);
        mScriptFilepath = ExternalFileLoader.getAbsolutePath("scripts/" + mScriptFile);
    }

    public String getName() {
        return mName;
    }

    public void setName(String name) {
        this.mName = name;
    }

    public String getVertexShaderFile() {
        return mVertexShaderFile;
    }

    public void setVertexShaderFile(String vertexShaderFile) {
        this.mVertexShaderFile = vertexShaderFile;
        this.mVertexShaderFilepath = ExternalFileLoader.getAbsolutePath("vertex_shaders/" + mVertexShaderFile);
    }

    public String getFragmentShaderFile() {
        return mFragmentShaderFile;
    }

    public void setFragmentShaderFile(String fragmentShaderFile) {
        this.mFragmentShaderFile = fragmentShaderFile;
        this.mFragmentShaderFilepath = ExternalFileLoader.getAbsolutePath("fragment_shaders/" + mFragmentShaderFile);
    }

    public String getScriptFile() {
        return mScriptFile;
    }

    public void setScriptFile(String scriptFile) {
        this.mScriptFile = scriptFile;
        this.mScriptFilepath = ExternalFileLoader.getAbsolutePath("scripts/" + mScriptFile);
    }

    @Override
    protected ContentValues getFieldValues() {
        ContentValues values = new ContentValues();
        values.put("name", mName);
        values.put("vertex_shader_file", mVertexShaderFile);
        values.put("fragment_shader_file", mFragmentShaderFile);
        values.put("script_file", mScriptFile);

        return values;
    }

    @Override
    protected String getTableName() {
        return "effect";
    }

    @Override
    protected void createTableIfNeeded(DatabaseConnection conn) {
        _createTableIfNeeded(conn);
    }

    public static List<Effect> getAll(DatabaseConnection conn)
    {
        List<Effect> effects = new ArrayList<Effect>();

        if (conn != null) {
            _createTableIfNeeded(conn);

            List<DatabaseRow> rows = conn.executeQuery("SELECT * FROM effect ORDER BY id");
            for (DatabaseRow row : rows) {
                effects.add(new Effect(row));
            }
        }

        return effects;
    }

    public static Effect getById(long id, DatabaseConnection conn)
    {
        if (conn != null) {
            _createTableIfNeeded(conn);

            List<DatabaseRow> rows = conn.executeQuery("SELECT * FROM effect WHERE id = ?", String.valueOf(id));
            if (rows.size() > 0) {
                return new Effect(rows.get(0));
            }
        }

        return null;
    }

    private static void _createTableIfNeeded(DatabaseConnection conn)
    {
        if (conn != null) {
            String query = "CREATE TABLE IF NOT EXISTS effect(" +
                    "id INTEGER PRIMARY KEY," +
                    "name TEXT," +
                    "vertex_shader_file TEXT," +
                    "fragment_shader_file TEXT," +
                    "script_file STRING)";

            conn.execute(query);
        }
    }
}
