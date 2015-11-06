package com.soma.superman.pidroneandroid.level;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.soma.superman.pidroneandroid.level.config.DisplayType;
import com.soma.superman.pidroneandroid.level.config.Viscosity;
import com.soma.superman.pidroneandroid.level.orientation.Orientation;

/**
 * Created by whee6409 on 15. 11. 5.
 */
public class LevelView extends SurfaceView implements SurfaceHolder.Callback{
    private LevelPainter painter;

    public LevelView(Context context, AttributeSet attrs) {
        super(context, attrs);
        getHolder().addCallback(this);
        setFocusable(true);
    }

    @Override
    public void onWindowFocusChanged(boolean hasWindowFocus) {
        if(painter != null) {
            painter.pause(!hasWindowFocus);
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int width, int height) {
        if (painter != null) {
            painter.setSurfaceSize(width, height);
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(getContext());
        Log.i("LevelView", "surfaceCreated");
        if (painter == null) {
            Log.i("LevelView", "painter is null");
            painter = new LevelPainter(holder, getContext(), new Handler(), getWidth(), getHeight(),
                    prefs.getBoolean(LevelPreferences.KEY_SHOW_ANGLE, true),
                    DisplayType.valueOf(prefs.getString(LevelPreferences.KEY_DISPLAY_TYPE, "ANGLE")),
                    Viscosity.valueOf(prefs.getString(LevelPreferences.KEY_VISCOSITY, "MEDIUM")));
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        if (painter != null) {
            painter.pause(true);
            painter.clean();
            painter = null;
        }
        // free resources
        System.gc();
    }

    public void onOrientationChanged(Orientation orientation, float pitch, float roll, float balance) {
        if (painter != null) {
            painter.onOrientationChanged(orientation, pitch, roll, balance);
        }
    }
}
