package com.soma.superman.pidroneandroid.controller;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

/**
 * Created by whee6409 on 15. 11. 6.
 */
public class Joystick {
    private int LAYOUT_ALPHA = 200;
    private int OFFSET = 0;

    private Context mContext;
    private ViewGroup mLayout;
    private ViewGroup.LayoutParams params;
    private int stick_width, stick_height;

    private double position_x = 0.0, position_y = 0.0, min_distance = 0.0;
    private double distance = 0;

    private DrawCanvas draw;
    private Paint paint;
    private Bitmap stick;

    private boolean touch_state = false;

    public Joystick (Context context, ViewGroup layout, int stick_res_id) {
        mContext = context;

        stick = BitmapFactory.decodeResource(mContext.getResources(), stick_res_id);

        initJoystick();

        stick_width = stick.getWidth();
        stick_height = stick.getHeight();

        draw = new DrawCanvas(mContext);
        paint = new Paint();
        mLayout = layout;
        params = mLayout.getLayoutParams();
    }

    private void initJoystick() {
        setStickSize(150, 150);
        setLayoutSize(500, 500);
        setLayoutAlpha(100);
        setOffset(90);
        setMinimumDistance(10);
    }

    public void drawStick(double x, double y) {
        draw.position(x, y);
        draw();
    }

    public void drawStick(MotionEvent arg1) {
        position_x = arg1.getX() - (params.width / 2.0);
        position_y = arg1.getY() - (params.height / 2.0);
        distance = Math.sqrt(Math.pow(position_x, 2.0) + Math.pow(position_y, 2.0));

        if(arg1.getAction() == MotionEvent.ACTION_DOWN) {
            if(distance <= (params.width / 2.0) - OFFSET) {
                draw.position(arg1.getX(), arg1.getY());
                draw();
                touch_state = true;
            }
        } else if(arg1.getAction() == MotionEvent.ACTION_MOVE && touch_state) {
            if(distance <= (params.width / 2.0) - OFFSET) {
                draw.position(arg1.getX(), arg1.getY());
                draw();
            } else if(distance > (params.width / 2.0) - OFFSET){
                double x = (double) (Math.cos(Math.toRadians(cal_angle(position_x, position_y)))
                        * ((params.width / 2.0) - OFFSET));
                double y = (double) (Math.sin(Math.toRadians(cal_angle(position_x, position_y)))
                        * ((params.height / 2.0) - OFFSET));
                x += (params.width / 2.0);
                y += (params.height / 2.0);
                draw.position(x, y);
                draw();
            } else {
                mLayout.removeView(draw);
            }
        } else if(arg1.getAction() == MotionEvent.ACTION_UP) {
            mLayout.removeView(draw);
            touch_state = false;
        }
    }

    public double getX() {
        double returnX;
        if(distance > min_distance && touch_state) {
            returnX = position_x / 18.0;
            if((returnX <= 10.0) && (returnX >= -10.0)) {
                return returnX;
            } else if(returnX > 10.0){
                return 10.0;
            } else if(returnX < -10.0) {
                return -10.0;
            }
        }
        return 0;
    }

    public double getY() {
        double returnY;
        if(distance > min_distance && touch_state) {
            returnY = position_y / 18.0;
            if((returnY <= 10.0) && (returnY >= -10.0)) {
                return -returnY;
            } else if(returnY > 10.0){
                return -10.0;
            } else if(returnY < -10.0) {
                return 10.0;
            }
        }
        return 0;
    }

    public double getDistance() {
        if(distance > min_distance && touch_state) {
            return distance;
        }
        return 0;
    }

    public void setMinimumDistance(int minDistance) {
        min_distance = minDistance;
    }

    public double getMinimumDistance() {
        return min_distance;
    }

    public void setOffset(int offset) {
        OFFSET = offset;
    }

    public int getOffset() {
        return OFFSET;
    }

    public void setLayoutAlpha(int alpha) {
        LAYOUT_ALPHA = alpha;
        mLayout.getBackground().setAlpha(alpha);
    }

    public int getLayoutAlpha() {
        return LAYOUT_ALPHA;
    }

    public void setStickSize(int width, int height) {
        stick = Bitmap.createScaledBitmap(stick, width, height, false);
        stick_width = stick.getWidth();
        stick_height = stick.getHeight();
    }

    public void setLayoutSize(int width, int height) {
        params.width = width;
        params.height = height;
    }

    public int getLayoutWidth() {
        return params.width;
    }

    public int getLayoutHeight() {
        return params.height;
    }

    private double cal_angle(double x, double y) {
        if(x >= 0 && y >= 0)
            return Math.toDegrees(Math.atan(y / x));
        else if(x < 0 && y >= 0)
            return Math.toDegrees(Math.atan(y / x)) + 180;
        else if(x < 0 && y < 0)
            return Math.toDegrees(Math.atan(y / x)) + 180;
        else if(x >= 0 && y < 0)
            return Math.toDegrees(Math.atan(y / x)) + 360;
        return 0;
    }

    private void draw() {
        try {
            mLayout.removeView(draw);
        } catch (Exception e) { }
        mLayout.addView(draw);
    }

    private class DrawCanvas extends View {
        double x, y;

        private DrawCanvas(Context mContext) {
            super(mContext);
        }

        public void onDraw(Canvas canvas) {
            canvas.drawBitmap(stick, (float)x, (float)y, paint);
        }

        private void position(double pos_x, double pos_y) {
            x = pos_x - (stick_width / 2);
            y = pos_y - (stick_height / 2);
        }
    }
}
