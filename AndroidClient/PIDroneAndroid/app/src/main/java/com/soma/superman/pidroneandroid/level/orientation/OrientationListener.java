package com.soma.superman.pidroneandroid.level.orientation;

/**
 * Created by whee6409 on 15. 11. 5.
 */
public interface OrientationListener {
    public void onOrientationChanged(Orientation orientation, float pitch, float roll, float balance);

    public void onCalibrationSaved(boolean success);

    public void onCalibrationReset(boolean success);
}
