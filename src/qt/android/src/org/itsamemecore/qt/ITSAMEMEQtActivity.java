package org.itsamemecore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class ITSAMEMEQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File itsamemeDir = new File(getFilesDir().getAbsolutePath() + "/.itsameme");
        if (!itsamemeDir.exists()) {
            itsamemeDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
