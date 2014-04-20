package com.aeskreis.particletoy.util;

import android.content.Context;
import android.os.Environment;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by adam on 4/19/14.
 */
public class ExternalFileLoader
{
    public static String DEFAULT_EXTERNAL_BASE_DIRECTORY = "ParticleToy";

    /* Checks if external storage is available for read and write */
    public static boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            return true;
        }
        return false;
    }

    /* Checks if external storage is available to at least read */
    public static boolean isExternalStorageReadable() {
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state) ||
                Environment.MEDIA_MOUNTED_READ_ONLY.equals(state)) {
            return true;
        }
        return false;
    }

    public static String getAbsolutePath(String path)
    {
        return Environment.getExternalStorageDirectory().getAbsolutePath() +
                "/" + DEFAULT_EXTERNAL_BASE_DIRECTORY + "/" + path;
    }

    public static boolean createDirectory(String directory)
    {
        if (!isExternalStorageWritable()) {
            return false;
        }

        File dirFile = new File(getAbsolutePath(directory));
        if (!dirFile.exists()) {
            dirFile.mkdirs();
        }

        return true;
    }

    public static boolean createDefaultDirectory()
    {
        return createDirectory("");
    }

    public static boolean createFile(String filepath)
    {
        if (!isExternalStorageWritable()) {
            return false;
        }

        File file = new File(getAbsolutePath(filepath));
        if (!file.exists()) {
            file.getParentFile().mkdirs();

            try {
                FileOutputStream outputStream = new FileOutputStream(file);
                outputStream.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return false;
    }

    public static String[] getFilesInDirectory(String directory)
    {
        if (!isExternalStorageReadable()) {
            return null;
        }

        File dirFile = new File(getAbsolutePath(directory));
        if (dirFile.exists()) {
            return dirFile.list();
        }

        return null;
    }
}
