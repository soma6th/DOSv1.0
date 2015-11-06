package com.soma.superman.pidroneandroid.level;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.view.SurfaceHolder;

import com.soma.superman.pidroneandroid.R;
import com.soma.superman.pidroneandroid.level.config.DisplayType;
import com.soma.superman.pidroneandroid.level.config.Viscosity;
import com.soma.superman.pidroneandroid.level.orientation.Orientation;

import java.text.DecimalFormat;

/**
 * Created by whee6409 on 15. 11. 5.
 */
public class LevelPainter implements Runnable{
    //
    private boolean initialized;
    private boolean wait;

    //
    private SurfaceHolder surfaceHolder;

    // Dimensions
    private int height;
    private int width;
    private int canvasHeight;
    private int canvasWidth;
    private int minLevelX;
    private int maxLevelX;
    private int minLevelY;
    private int maxLevelY;
    private int levelMinusBubbleHeight;
    private int levelMinusBubbleWidth;
    private int middleX;
    private int middleY;
    private int bubbleHeight;
    private int bubbleWidth;
    private int halfBubbleHeight;
    private int halfBubbleWidth;
    private int halfMarkerGap;
    private int minBubble;
    private int maxBubble;
    private int markerThickness;
    private int levelBorderHeight;
    private int levelBorderWidth;
    private int levelMaxDimension;
    private int lcdWidth;
    private int lcdHeight;
    private int displayGap;
    private int displayPadding;
    private int sensorGap;
    private int sensorY;

    private int levelWidth;
    private int levelHeight;

    private int infoHeight;
    private int infoY;


    // rect
    private Rect displayRect;

    // angles
    private float angle1;
    private float angle2;
    private double n, teta, l;

    private static final double LEVEL_ASPECT_RATIO 	= 0.150;
    private static final double BUBBLE_WIDTH 		= 0.150;
    private static final double BUBBLE_ASPECT_RATIO = 1.000;
    private static final double BUBBLE_CROPPING		= 0.500;
    private static final double MARKER_GAP	 		= BUBBLE_WIDTH + 0.020;

    // angle max
    private static final double MAX_SINUS = Math.sin(Math.PI / 4);

    private Orientation orientation;

    // bubble physics
    private long currentTime;
    private long lastTime;
    private double timeDiff;
    private double posX;
    private double posY;
    private double angleX;
    private double angleY;
    private double speedX;
    private double speedY;
    private double x, y;

    // drawables
    private Drawable level2D;
    private Drawable bubble2D;
    private Drawable marker2D;
    private Drawable display;

    private String infoText;

    // Speed adjustment
    private Viscosity viscosity;
    private double viscosityValue;

    // format of angles
    private DecimalFormat displayFormat;
    private String displayBackgroundText;

    // fonts and colors
    private static final String FONT_LCD = "fonts/lcd.ttf";
    private Paint lcdForegroundPaint;
    private Paint lcdBackgroundPaint;
    private Paint infoPaint;
    private int backgroundColor;

    // config angles
    private boolean showAngle;
    private DisplayType angleType;

    // animation
    private final Handler handler;
    private long frameRate;

    public LevelPainter(SurfaceHolder surfaceHolder, Context context, Handler handler,
                        int width, int height, boolean showAngle, DisplayType angleType,
                        Viscosity viscosity) {
        this.surfaceHolder = surfaceHolder;
        this.handler = handler;
        this.frameRate = 1000 / context.getResources().getInteger(R.integer.frame_rate);

        this.bubble2D = context.getResources().getDrawable(R.drawable.bubble_2d);
        this.level2D = context.getResources().getDrawable(R.drawable.level_2d);
        this.marker2D = context.getResources().getDrawable(R.drawable.marker_2d);
        this.display = context.getResources().getDrawable(R.drawable.display);

        this.viscosity = viscosity;

        this.showAngle = showAngle;
        this.displayFormat = new DecimalFormat(angleType.getDisplayFormat());
        this.displayBackgroundText = angleType.getDisplayBackgroundText();
        this.angleType = angleType;

        this.backgroundColor = context.getResources().getColor(R.color.silver);

        this.infoText = context.getString(R.string.calibrate_info);

        Typeface lcd = Typeface.createFromAsset(context.getAssets(), FONT_LCD);

        this.infoPaint = new Paint();
        this.infoPaint.setColor(context.getResources().getColor(R.color.black));
        this.infoPaint.setAntiAlias(true);
        this.infoPaint.setTextSize(context.getResources().getDimensionPixelSize(R.dimen.info_text));
        this.infoPaint.setTypeface(Typeface.create(Typeface.SANS_SERIF, Typeface.NORMAL));
        this.infoPaint.setTextAlign(Paint.Align.CENTER);

        this.lcdForegroundPaint = new Paint();
        this.lcdForegroundPaint.setColor(context.getResources().getColor(R.color.lcd_front));
        this.lcdForegroundPaint.setAntiAlias(true);
        this.lcdForegroundPaint.setTextSize(context.getResources().getDimensionPixelSize(R.dimen.lcd_text));
        this.lcdForegroundPaint.setTypeface(lcd);
        this.lcdForegroundPaint.setTextAlign(Paint.Align.CENTER);

        this.lcdBackgroundPaint = new Paint();
        this.lcdBackgroundPaint.setColor(context.getResources().getColor(R.color.lcd_back));
        this.lcdBackgroundPaint.setAntiAlias(true);
        this.lcdBackgroundPaint.setTextSize(context.getResources().getDimensionPixelSize(R.dimen.lcd_text));
        this.lcdBackgroundPaint.setTypeface(lcd);
        this.lcdBackgroundPaint.setTextAlign(Paint.Align.CENTER);

        Rect rect =  new Rect();
        this.infoPaint.getTextBounds(infoText, 0, infoText.length(), rect);
        this.infoHeight = rect.height();
        this.lcdBackgroundPaint.getTextBounds(displayBackgroundText, 0, displayBackgroundText.length(), rect);
        this.lcdHeight = rect.height();
        this.lcdWidth = rect.width();
        this.levelBorderWidth = context.getResources().getDimensionPixelSize(R.dimen.level_border_width);
        this.levelBorderHeight = context.getResources().getDimensionPixelSize(R.dimen.level_border_height);
        this.markerThickness = context.getResources().getDimensionPixelSize(R.dimen.marker_thickness);
        this.displayGap = context.getResources().getDimensionPixelSize(R.dimen.display_gap);
        this.sensorGap = context.getResources().getDimensionPixelSize(R.dimen.sensor_gap);
        this.displayPadding = context.getResources().getDimensionPixelSize(R.dimen.display_padding);
        this.displayRect = new Rect();

        this.orientation = Orientation.LANDING;
        this.initialized = false;

    }

    public void clean() {
        synchronized (this.surfaceHolder) {
            level2D = null;
            bubble2D = null;
            marker2D = null;
            display = null;
        }
    }


    @Override
    public void run() {
        Canvas c = null;

        // update physics
        currentTime = System.currentTimeMillis();
        if(lastTime > 0) {
            timeDiff = (currentTime - lastTime) / 1000.0;
            posX = orientation.getReverse() * (2 * x - minLevelX - maxLevelX) / levelMinusBubbleWidth;
            switch (orientation) {
                case TOP :
                case BOTTOM :
                    speedX = orientation.getReverse() * (angleX - posX) * viscosityValue;
                    break;
                case LEFT :
                case RIGHT :
                    speedX = orientation.getReverse() * (angleY - posX) * viscosityValue;
                    break;
                case LANDING :
                    posY = (2 * y - minLevelY - maxLevelY) / levelMinusBubbleHeight;
                    speedX = (angleX - posX) * viscosityValue;
                    speedY = (angleY - posY) * viscosityValue;
                    y += speedY * timeDiff;
                    break;
            }
        }

        x += speedX * timeDiff;

        switch (orientation) {
            case LANDING :
                if (Math.sqrt((middleX - x) * (middleX - x)
                        + (middleY - y) * (middleY - y)) > levelMaxDimension / 2 - halfBubbleWidth) {
                    x = (angleX * levelMinusBubbleWidth + minLevelX + maxLevelX) / 2;
                    y = (angleY * levelMinusBubbleHeight + minLevelY + maxLevelY) / 2;
                }
                break;
            default :
                if (x < minLevelX + halfBubbleWidth || x > maxLevelX - halfBubbleWidth) {
                    x = (angleX * levelMinusBubbleWidth + minLevelX + maxLevelX) / 2;
                }
        }

        lastTime = currentTime;

        try {
            c = this.surfaceHolder.lockCanvas(null);
            if(c != null) {
                synchronized (this.surfaceHolder) {
                    doDraw(c);
                }
            }
        } finally {
            if(c != null) {
                this.surfaceHolder.unlockCanvasAndPost(c);
            }
        }

        handler.removeCallbacks(this);
        if(!wait) {
            handler.postDelayed(this, frameRate - System.currentTimeMillis() + lastTime);
        }

    }

    public void pause(boolean paused) {
        wait = !initialized || paused;
        if (!wait) {
            handler.postDelayed(this, frameRate);
        }
    }

    public void setSurfaceSize(int width, int height) {
        canvasHeight = height;;
        canvasWidth = width;

        levelMaxDimension = Math.min(
                Math.min(height, width) - 2 * displayGap,
                Math.max(height, width) - 2 * (sensorGap + 2 * infoHeight + 3 * displayGap + lcdHeight));
        setOrientation(orientation);

    }

    private void setOrientation(Orientation newOrientation) {
        if (!initialized) {
            synchronized (this.surfaceHolder) {
                orientation = newOrientation;

                switch (newOrientation) {
                    case LEFT :		// left
                    case RIGHT :	// right
                        height = canvasWidth;
                        width = canvasHeight;
                        infoY = (canvasHeight - canvasWidth) / 2 + canvasWidth - infoHeight;
                        break;
                    case TOP :		// top
                    case BOTTOM :	// bottom
                    default :		// landing
                        height = canvasHeight;
                        width = canvasWidth;
                        infoY = canvasHeight - infoHeight;
                        break;
                }

                sensorY = infoY - infoHeight - sensorGap;

                middleX = canvasWidth / 2;
                middleY = canvasHeight / 2;

                // level
                switch (newOrientation) {
                    case LANDING :	// landing
                        levelWidth = levelMaxDimension;
                        levelHeight = levelMaxDimension;
                        break;
                    case TOP :		// top
                    case BOTTOM :	// bottom
                    case LEFT :		// left
                    case RIGHT :	// right
                        levelWidth = (int) (width - 2 * displayGap);
                        levelHeight = (int) (levelWidth * LEVEL_ASPECT_RATIO);
                        break;
                }

                viscosityValue = levelWidth * viscosity.getCoeff();

                minLevelX = middleX - levelWidth / 2;
                maxLevelX = middleX + levelWidth / 2;
                minLevelY = middleY - levelHeight / 2;
                maxLevelY = middleY + levelHeight / 2;

                // bubble
                halfBubbleWidth = (int) (levelWidth * BUBBLE_WIDTH / 2);
                halfBubbleHeight = (int) (halfBubbleWidth * BUBBLE_ASPECT_RATIO);
                bubbleWidth = 2 * halfBubbleWidth;
                bubbleHeight = 2 * halfBubbleHeight;
                maxBubble = (int) (maxLevelY - bubbleHeight * BUBBLE_CROPPING);
                minBubble = maxBubble - bubbleHeight;

                // display
                displayRect.set(
                        middleX - lcdWidth / 2 - displayPadding,
                        sensorY  - displayGap - 2 * displayPadding - lcdHeight - infoHeight / 2,
                        middleX + lcdWidth / 2 + displayPadding,
                        sensorY - displayGap - infoHeight / 2);


                // marker
                halfMarkerGap = (int) (levelWidth * MARKER_GAP / 2);

                // autres
                levelMinusBubbleWidth = levelWidth - bubbleWidth - 2 * levelBorderWidth;
                levelMinusBubbleHeight = levelHeight - bubbleHeight - 2 * levelBorderWidth;

                // positionnement
                level2D.setBounds(minLevelX, minLevelY, maxLevelX, maxLevelY);
                marker2D.setBounds(
                        middleX - halfMarkerGap - markerThickness,
                        middleY - halfMarkerGap - markerThickness,
                        middleX + halfMarkerGap + markerThickness,
                        middleY + halfMarkerGap + markerThickness);

                x = ((double) (maxLevelX + minLevelX)) / 2;
                y = ((double) (maxLevelY + minLevelY)) / 2;
                if (!initialized) {
                    initialized = true;
                    pause(false);
                }
            }
        }
    }


    private void doDraw(Canvas canvas) {
        canvas.save();

        canvas.drawColor(backgroundColor);
        canvas.drawText("...", middleX, infoY, infoPaint);

        if (showAngle) {
            display.setBounds(
                    displayRect.left - (displayRect.width() + displayGap) / 2,
                    displayRect.top,
                    displayRect.right - (displayRect.width() + displayGap) / 2,
                    displayRect.bottom);
            display.draw(canvas);
            display.setBounds(
                    displayRect.left + (displayRect.width() + displayGap) / 2,
                    displayRect.top,
                    displayRect.right + (displayRect.width() + displayGap) / 2,
                    displayRect.bottom);
            display.draw(canvas);
            canvas.drawText(
                    displayBackgroundText,
                    middleX - (displayRect.width() + displayGap) / 2,
                    displayRect.centerY() + lcdHeight / 2,
                    lcdBackgroundPaint);
            canvas.drawText(
                    displayFormat.format(angle2),
                    middleX - (displayRect.width() + displayGap) / 2,
                    displayRect.centerY() + lcdHeight / 2,
                    lcdForegroundPaint);
            canvas.drawText(
                    displayBackgroundText,
                    middleX + (displayRect.width() + displayGap) / 2,
                    displayRect.centerY() + lcdHeight / 2,
                    lcdBackgroundPaint);
            canvas.drawText(
                    displayFormat.format(angle1),
                    middleX + (displayRect.width() + displayGap) / 2,
                    displayRect.centerY() + lcdHeight / 2,
                    lcdForegroundPaint);
        }
        bubble2D.setBounds(
                (int) (x - halfBubbleWidth),
                (int) (y - halfBubbleHeight),
                (int) (x + halfBubbleWidth),
                (int) (y + halfBubbleHeight));
        level2D.draw(canvas);
        bubble2D.draw(canvas);
        marker2D.draw(canvas);
        /*
        canvas.drawLine(minLevelX, middleY, middleX - halfMarkerGap, middleY, infoPaint);
        canvas.drawLine(middleX + halfMarkerGap, middleY,
                maxLevelX, middleY, infoPaint);
        canvas.drawLine(middleX, minLevelY,
                middleX, middleY - halfMarkerGap, infoPaint);
        canvas.drawLine(middleX, middleY + halfMarkerGap,
                middleX, maxLevelY, infoPaint);
                */
        canvas.restore();
    }

    public void onOrientationChanged(Orientation newOrientation, float newPitch, float newRoll, float newBalance) {
        if (!orientation.equals(newOrientation)) {
            setOrientation(newOrientation);
        }

        if(!wait) {
            switch (orientation) {
                case TOP :
                case BOTTOM :
                    angle1 = Math.abs(newBalance);
                    angleX = Math.sin(Math.toRadians(newBalance)) / MAX_SINUS;
                    break;
                case LANDING :
                    angle2 = Math.abs(newRoll);
                    angleX = Math.sin(Math.toRadians(newRoll)) / MAX_SINUS;
                case RIGHT :
                case LEFT :
                    angle1 = Math.abs(newPitch);
                    angleY = Math.sin(Math.toRadians(newPitch)) / MAX_SINUS;
                    if (angle1 > 90) {
                        angle1 = 180 - angle1;
                    }
                    break;
            }

            switch (angleType) {
                case INCLINATION:
                    angle1 = 100 * angle1 / 45;
                    angle2 = 100 * angle2 / 45;
                    break;
                case ROOF_PITCH:
                    angle1 = 12 * (float) Math.tan(Math.toRadians(angle1));
                    angle2 = 12 * (float) Math.tan(Math.toRadians(angle2));
                    break;
            }

            if (angle1 > angleType.getMax()) {
                angle1 = angleType.getMax();
            }
            if (angle2 > angleType.getMax()) {
                angle2 = angleType.getMax();
            }

            if (angleX > 1) {
                angleX = 1;
            } else if (angleX < -1) {
                angleX = -1;
            }
            if (angleY > 1) {
                angleY = 1;
            } else if (angleY < -1) {
                angleY = -1;
            }

            if (orientation.equals(Orientation.LANDING) && angleX != 0 && angleY != 0) {
                n = Math.sqrt(angleX * angleX + angleY * angleY);
                teta = Math.acos(Math.abs(angleX) / n);
                l = 1 / Math.max(Math.abs(Math.cos(teta)), Math.abs(Math.sin(teta)));
                angleX = angleX / l;
                angleY = angleY / l;
            }
        }
    }
}
