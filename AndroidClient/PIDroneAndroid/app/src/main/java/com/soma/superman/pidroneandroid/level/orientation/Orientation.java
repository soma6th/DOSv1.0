package com.soma.superman.pidroneandroid.level.orientation;

/**
 * Created by whee6409 on 15. 11. 5.
 */
public enum Orientation {

    LANDING(1, 0),
    TOP(1, 0),
    RIGHT(1, 90),
    BOTTOM(-1, 180),
    LEFT(-1, -90);

    private int reverse;
    private int rotation;

    public int getReverse() {
        return reverse;
    }

    public int getRotation() {
        return rotation;
    }

    private Orientation(int reverse, int rotation) {
        this.reverse = reverse;
        this.rotation = rotation;
    }

    public boolean isLevel(float pitch, float roll, float balance, float sensibility) {
        switch (this) {
            case BOTTOM :
            case TOP :
                return balance <= sensibility
                        && balance >= - sensibility;
            case LANDING :
                return roll <= sensibility
                        && roll >= - sensibility
                        && (Math.abs(pitch) <= sensibility
                        || Math.abs(pitch) >= 180 - sensibility);
            case LEFT :
            case RIGHT :
                return Math.abs(pitch) <= sensibility
                        || Math.abs(pitch) >= 180 - sensibility;
        }
        return false;
    }
}
