package com.soma.superman.pidroneandroid_dev;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;

/**
 * Created by whee6409 on 15. 10. 10.
 */
public class Joystick {
    private int STICK_ALPHA = 200;
    private int LAYOUT_ALPHA = 200;
    private int OFFSET = 0;

    private Context mContext;
    private ViewGroup mLayout;
    private LayoutParams params;
    private int stick_width, stick_height;

    private double position_x = 0.0f, position_y = 0.0f, min_distance = 0.0f;
    private double distance = 0, angle = 0;

    private DrawCanvas draw;
    private Paint paint;
    private Bitmap stick;

    private boolean touch_state = false;

    public Joystick (Context context, ViewGroup layout, int stick_res_id) {
        mContext = context;

        stick = BitmapFactory.decodeResource(mContext.getResources(), stick_res_id);

        stick_width = stick.getWidth();
        stick_height = stick.getHeight();

        draw = new DrawCanvas(mContext);
        paint = new Paint();
        mLayout = layout;
        params = mLayout.getLayoutParams();
    }

    public void drawStick(double x, double y) {
        draw.position(x, y);
        draw();
    }

    public void drawStick(MotionEvent arg1) {
        position_x = (double) (arg1.getX() - (params.width / 2.0f));
        position_y = (double) (arg1.getY() - (params.height / 2.0f));
        distance = (double) Math.sqrt(Math.pow(position_x, 2.0f) + Math.pow(position_y, 2.0f));
        angle = (double) cal_angle(position_x, position_y);

        if(arg1.getAction() == MotionEvent.ACTION_DOWN) {
            if(distance <= (params.width / 2.0f) - OFFSET) {
                draw.position(arg1.getX(), arg1.getY());
                draw();
                touch_state = true;
            }
        } else if(arg1.getAction() == MotionEvent.ACTION_MOVE && touch_state) {
            if(distance <= (params.width / 2.0f) - OFFSET) {
                draw.position(arg1.getX(), arg1.getY());
                draw();
            } else if(distance > (params.width / 2.0f) - OFFSET){
                double x = (double) (Math.cos(Math.toRadians(cal_angle(position_x, position_y)))
                        * ((params.width / 2.0f) - OFFSET));
                double y = (double) (Math.sin(Math.toRadians(cal_angle(position_x, position_y)))
                        * ((params.height / 2.0f) - OFFSET));
                x += (params.width / 2.0f);
                y += (params.height / 2.0f);
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

    public double[] getPosition() {
        if(distance > min_distance && touch_state) {
            return new double[] { position_x, position_y };
        }
        return new double[] { 0, 0 };
    }

    public double getX() {
        double returnX;
        if(distance > min_distance && touch_state) {
            returnX = position_x / 18.0f;
            if((returnX <= 10.0f) && (returnX >= -10.0f)) {
                return returnX;
            } else if(returnX > 10.0f){
                return 10.0f;
            } else if(returnX < -10.0f) {
                return -10.0f;
            }
        }
        return 0;
    }

    public double getY() {
        double returnY;
        if(distance > min_distance && touch_state) {
            returnY = position_y / 18.0f;
            if((returnY <= 10.0f) && (returnY >= -10.0f)) {
                return -returnY;
            } else if(returnY > 10.0f){
                return -10.0f;
            } else if(returnY < -10.0f) {
                return 10.0f;
            }
        }
        return 0;
    }

    public double getAngle() {
        if(distance > min_distance && touch_state) {
            return angle;
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

    public void setStickAlpha(int alpha) {
        STICK_ALPHA = alpha;
        paint.setAlpha(alpha);
    }

    public int getStickAlpha() {
        return STICK_ALPHA;
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

    public void setStickWidth(int width) {
        stick = Bitmap.createScaledBitmap(stick, width, stick_height, false);
        stick_width = stick.getWidth();
    }

    public void setStickHeight(int height) {
        stick = Bitmap.createScaledBitmap(stick, stick_width, height, false);
        stick_height = stick.getHeight();
    }

    public int getStickWidth() {
        return stick_width;
    }

    public int getStickHeight() {
        return stick_height;
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

    private class DrawCanvas extends View{
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
