package com.soma.superman.pidroneandroid.level;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import android.widget.Toast;

import com.soma.superman.pidroneandroid.R;
import com.soma.superman.pidroneandroid.level.orientation.Orientation;
import com.soma.superman.pidroneandroid.level.orientation.OrientationListener;
import com.soma.superman.pidroneandroid.level.orientation.OrientationProvider;

/**
 * Created by whee6409 on 15. 11. 5.
 */
public class LevelActivity extends Activity implements OrientationListener{
    private static LevelActivity CONTEXT;
    private LevelView view;
    private OrientationProvider provider;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_level);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        CONTEXT = this;
        view = (LevelView) findViewById(R.id.level);
    }

    protected void onResume() {
        super.onResume();
        Log.d("LevelActivity", "LevelActivity resumed");
        provider = OrientationProvider.getInstance();

        if (provider.isSupported()) {
            provider.startListening(this);
        } else {
            Toast.makeText(this, getText(R.string.not_supported), Toast.LENGTH_LONG).show();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (provider.isListening()) {
            provider.stopListening();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
    }

    public static LevelActivity getContext() {
        return CONTEXT;
    }

    public void onOrientationChanged(Orientation orientation, float pitch, float roll, float balance) {
        view.onOrientationChanged(orientation, pitch, roll, balance);
    }

    @Override
    public void onCalibrationReset(boolean success) {
        Toast.makeText(this, success ?
                R.string.calibrate_restored : R.string.calibrate_failed, Toast.LENGTH_LONG).show();
    }

    @Override
    public void onCalibrationSaved(boolean success) {
        Toast.makeText(this, success ?
                R.string.calibrate_saved : R.string.calibrate_failed, Toast.LENGTH_LONG).show();
    }
}
