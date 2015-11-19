package com.soma.superman.pidroneandroid.setting;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.RadioGroup;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.soma.superman.pidroneandroid.R;
import com.soma.superman.pidroneandroid.controller.Joystick;
import com.soma.superman.pidroneandroid.network.PacketVO;
import com.soma.superman.pidroneandroid.network.TCPThread;
import com.soma.superman.pidroneandroid.network.UDPThread;

/**
 * Created by whee6409 on 15. 11. 19.
 */
public class SettingActivity extends Activity {
    // UI components
    RelativeLayout layoutJoystickL, layoutJoystickR;
    Button btnTCPConnect;
    Button btnExit;
    ImageButton btnStop;
    TextView txtPitch, txtRoll, txtThr, txtYaw;
    Joystick joystickL, joystickR;
    View.OnClickListener btnClickListener;

    // UI components for Debug
    Button btnPIDReset;
    Button btnRangeSet;
    Button btnP1;
    Button btnM1;
    Button btnP2;
    Button btnM2;
    Button btnP3;
    Button btnM3;
    Button btnP4;
    Button btnM4;
    Button btnP5;
    Button btnM5;
    Button btnP6;
    Button btnM6;
    EditText etX;
    EditText etY;
    EditText etZ;
    EditText etYp;
    EditText etYi;
    EditText etYd;
    EditText etPitchRange;
    EditText etRollRange;
    EditText etYawRange;
    EditText etThrRange;

    RadioGroup rg1;
    double amount;

    // Thread for Networking
    TCPThread mTcpThread;
    UDPThread mUdpThread;
    PacketVO mMessage;
    String udpMsg;

    // variables
    double pitch = 0.0, roll = 0.0, yaw = 0.0;
    int thr = 0;
    double lastYaw = 0.0;
    double lastThr = 0.0;
    double x, y, z, x1, y1, z1;
    double pitchRange = 20, rollRange = 20, yawRange = 20, thrRange = 2000;


    // Method for initialize UI components (Load and Inflate)
    private void initComponents(){
        btnTCPConnect = (Button)findViewById(R.id.btn_tcp_connect);
        btnExit = (Button)findViewById(R.id.btn_exit);

        etX = (EditText)findViewById(R.id.et_x);
        etY = (EditText)findViewById(R.id.et_y);
        etZ = (EditText)findViewById(R.id.et_z);

        etYp = (EditText)findViewById(R.id.et_yp);
        etYi = (EditText)findViewById(R.id.et_yi);
        etYd = (EditText)findViewById(R.id.et_yd);

        etX.setText("0.00000");
        etY.setText("0.00000");
        etZ.setText("0.00000");

        etYp.setText("0.00000");
        etYi.setText("0.00000");
        etYd.setText("0.00000");

        x = 0.00000;
        y = 0.00000;
        z = 0.00000;

        x1 = 0.00000;
        y1 = 0.00000;
        z1 = 0.00000;

        etPitchRange = (EditText)findViewById(R.id.et_pitch_range);
        etRollRange = (EditText)findViewById(R.id.et_roll_range);
        etYawRange = (EditText)findViewById(R.id.et_yaw_range);
        etThrRange = (EditText)findViewById(R.id.et_thr_range);

        etPitchRange.setText(""+pitchRange);
        etRollRange.setText(""+rollRange);
        etYawRange.setText(""+yawRange);
        etThrRange.setText(""+thrRange);

        btnRangeSet = (Button)findViewById(R.id.btn_range_set);

        btnP1 = (Button)findViewById(R.id.btn_p1);
        btnP2 = (Button)findViewById(R.id.btn_p2);
        btnP3 = (Button)findViewById(R.id.btn_p3);
        btnP4 = (Button)findViewById(R.id.btn_p4);
        btnP5 = (Button)findViewById(R.id.btn_p5);
        btnP6 = (Button)findViewById(R.id.btn_p6);

        btnM1 = (Button)findViewById(R.id.btn_m1);
        btnM2 = (Button)findViewById(R.id.btn_m2);
        btnM3 = (Button)findViewById(R.id.btn_m3);
        btnM4 = (Button)findViewById(R.id.btn_m4);
        btnM5 = (Button)findViewById(R.id.btn_m5);
        btnM6 = (Button)findViewById(R.id.btn_m6);

        rg1 = (RadioGroup)findViewById(R.id.radioGroup);
        amount = 0.1;

        rg1.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                switch (checkedId) {
                    case R.id.rbtn_1:
                        amount = 0.1;
                        break;
                    case R.id.rbtn_2:
                        amount = 0.01;
                        break;
                    case R.id.rbtn_3:
                        amount = 0.001;
                        break;
                    case R.id.rbtn_4:
                        amount = 0.0001;
                        break;
                    case R.id.rbtn_5:
                        amount = 0.00001;
                        break;
                }
            }
        });

        btnPIDReset = (Button)findViewById(R.id.btn_pid_reset);
        btnStop = (ImageButton)findViewById(R.id.btn_stop);

        txtPitch = (TextView)findViewById(R.id.txt_pitch_set);
        txtRoll = (TextView)findViewById(R.id.txt_roll_set);
        txtThr = (TextView)findViewById(R.id.txt_thr_set);
        txtYaw = (TextView)findViewById(R.id.txt_yaw_set);

        txtPitch.setText("Pitch : ");
        txtRoll.setText("Roll : ");
        txtThr.setText("Thr : ");
        txtYaw.setText("Yaw : ");
    }

    private void initActionListeners(){
        this.btnClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                switch (v.getId()){
                    case R.id.btn_tcp_connect:
                        initNetworkThreads();
                        break;
                    case R.id.btn_exit:
                        try {
                            mTcpThread.sendChar((char)5);
                            mTcpThread.mSocket.close();
                            mUdpThread.socket.close();

                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        break;
                    case R.id.btn_range_set:
                        pitchRange = Double.parseDouble(String.valueOf(etPitchRange.getText()));
                        rollRange = Double.parseDouble(String.valueOf(etRollRange.getText()));
                        yawRange = Double.parseDouble(String.valueOf(etYawRange.getText()));
                        thrRange = Double.parseDouble(String.valueOf(etThrRange.getText()));
                        break;
                    case R.id.btn_p1:
                        x = Float.parseFloat(String.valueOf(etX.getText()));
                        x += amount;
                        etX.setText(String.format("%.5f", x).toString());
                        break;
                    case R.id.btn_p2:
                        y = Float.parseFloat(String.valueOf(etY.getText()));
                        y += amount;
                        etY.setText(String.format("%.5f", y).toString());
                        break;
                    case R.id.btn_p3:
                        z = Float.parseFloat(String.valueOf(etZ.getText()));
                        z += amount;
                        etZ.setText(String.format("%.5f", z).toString());
                        break;
                    case R.id.btn_p4:
                        x1 = Float.parseFloat(String.valueOf(etYp.getText()));
                        x1 += amount;
                        etYp.setText(String.format("%.5f", x1).toString());
                        break;
                    case R.id.btn_p5:
                        y1 = Float.parseFloat(String.valueOf(etYi.getText()));
                        y1 += amount;
                        etYi.setText(String.format("%.5f", y1).toString());
                        break;
                    case R.id.btn_p6:
                        z1 = Float.parseFloat(String.valueOf(etYd.getText()));
                        z1 += amount;
                        etYd.setText(String.format("%.5f", z1).toString());
                        break;
                    case R.id.btn_m1:
                        x = Float.parseFloat(String.valueOf(etX.getText()));
                        x -= amount;
                        if(x <= 0) {
                            x = 0;
                        }
                        etX.setText(String.format("%.5f", x).toString());
                        break;
                    case R.id.btn_m2:
                        y = Float.parseFloat(String.valueOf(etY.getText()));
                        y -= amount;
                        if(y <= 0) {
                            y = 0;
                        }
                        etY.setText(String.format("%.5f", y).toString());
                        break;
                    case R.id.btn_m3:
                        z = Float.parseFloat(String.valueOf(etZ.getText()));
                        z -= amount;
                        if(z <= 0) {
                            z = 0;
                        }
                        etZ.setText(String.format("%.5f", z).toString());
                        break;
                    case R.id.btn_m4:
                        x1 = Float.parseFloat(String.valueOf(etYp.getText()));
                        x1 -= amount;
                        if(x1 <= 0) {
                            x1 = 0;
                        }
                        etYp.setText(String.format("%.5f", x1).toString());
                        break;
                    case R.id.btn_m5:
                        y1 = Float.parseFloat(String.valueOf(etYi.getText()));
                        y1 -= amount;
                        if(y1 <= 0) {
                            y1 = 0;
                        }
                        etYi.setText(String.format("%.5f", y1).toString());
                        break;
                    case R.id.btn_m6:
                        z1 = Float.parseFloat(String.valueOf(etYd.getText()));
                        z1 -= amount;
                        if(z1 <= 0) {
                            z1 = 0;
                        }
                        etYd.setText(String.format("%.5f", z1).toString());
                        break;
                    case R.id.btn_pid_reset:
                        String jsonMessage;
                        jsonMessage = "{\"P_P\":" + etX.getText() + "," +
                                "\"P_I\":" + etY.getText() + "," +
                                "\"P_D\":" + etZ.getText() + "," +
                                "\"Y_P\":" + etYp.getText() + "," +
                                "\"Y_I\":" + etYi.getText() + "," +
                                "\"Y_D\":" + etYd.getText() + "}";

                        try {
                            mTcpThread.sendChar((char)3);
                            mTcpThread.flag = true;

                            char check = (char)mTcpThread.sendPID(jsonMessage);
                            if (check == (char)0) {
                                Toast.makeText(getApplicationContext(), "PID 실패", Toast.LENGTH_LONG);
                                mTcpThread.flag = false;
                            } else if (check == (char)1) {
                                Toast.makeText(getApplicationContext(), "PID 성공", Toast.LENGTH_LONG);
                                mTcpThread.flag = false;
                            } else {
                                Toast.makeText(getApplicationContext(), "matching fail", Toast.LENGTH_LONG);
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        break;
                    case R.id.btn_stop:
                        try {
                            mTcpThread.sendChar((char) 2);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                        break;


                }
            }
        };// end of btnClickListener
    }

    private void bindActionListeners(){
        btnTCPConnect.setOnClickListener(this.btnClickListener);
        btnExit.setOnClickListener(this.btnClickListener);
        btnM1.setOnClickListener(this.btnClickListener);
        btnM2.setOnClickListener(this.btnClickListener);
        btnM3.setOnClickListener(this.btnClickListener);
        btnM4.setOnClickListener(this.btnClickListener);
        btnM5.setOnClickListener(this.btnClickListener);
        btnM6.setOnClickListener(this.btnClickListener);
        btnP1.setOnClickListener(this.btnClickListener);
        btnP2.setOnClickListener(this.btnClickListener);
        btnP3.setOnClickListener(this.btnClickListener);
        btnP4.setOnClickListener(this.btnClickListener);
        btnP5.setOnClickListener(this.btnClickListener);
        btnP6.setOnClickListener(this.btnClickListener);
        btnPIDReset.setOnClickListener(this.btnClickListener);
        btnStop.setOnClickListener(this.btnClickListener);
        btnRangeSet.setOnClickListener(this.btnClickListener);
    }

    private void joystickSet() {
        layoutJoystickL = (RelativeLayout)findViewById(R.id.layout_joystick_left_set);
        joystickL = new Joystick(getApplicationContext(), layoutJoystickL, R.drawable.image_button);
        joystickL.drawStick(joystickL.getLayoutWidth() / 2.0f, joystickL.getLayoutHeight() / 2.0f);
        layoutJoystickL.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN ||
                        motionEvent.getAction() == MotionEvent.ACTION_MOVE) {
                    joystickL.setRange(pitchRange, rollRange);
                    joystickL.drawStick(motionEvent);
                    pitch = joystickL.getX();
                    roll = joystickL.getY();
                    txtPitch.setText("Pitch : " + pitch);
                    txtRoll.setText("Roll : " + roll);
                    mMessage = new PacketVO("2", "" + pitch, "" + roll, "" + yaw, "" + thr);
                    String jsonPacket = mMessage.toJson();
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if (mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char) 0);
                        }

                        if (mUdpThread != null) {
                            Log.i("udp ", "message in main activity in");
                            udpMsg = mUdpThread.msg;
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                } else if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    joystickL.setRange(pitchRange, rollRange);
                    joystickL.drawStick(joystickL.getLayoutWidth() / 2.0f, joystickL.getLayoutHeight() / 2.0f);
                    pitch = 0.0f;
                    roll = 0.0f;
                    txtPitch.setText("Pitch : ");
                    txtRoll.setText("Roll : ");
                    mMessage = new PacketVO("2", "" + 0.0, "" + 0.0, "" + 0.0, "" + thr);
                    String jsonPacket = mMessage.toJson();
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if (mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char) 0);
                        }

                        if (mUdpThread != null) {
                            Log.i("udp ", "message in main activity in");
                            udpMsg = mUdpThread.msg;
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });

        layoutJoystickR = (RelativeLayout)findViewById(R.id.layout_joystick_right_set);
        joystickR = new Joystick(getApplicationContext(), layoutJoystickR, R.drawable.image_button);
        joystickR.drawStick(joystickR.getLayoutWidth() / 2.0, joystickR.getLayoutHeight() - joystickR.getOffset());
        layoutJoystickR.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if(motionEvent.getAction() == MotionEvent.ACTION_DOWN ||
                        motionEvent.getAction() == MotionEvent.ACTION_MOVE) {
                    joystickR.setRange(yawRange, thrRange / 100.0);
                    joystickR.drawStick(motionEvent);
                    if(joystickR.getX() != 0.0) {
                        yaw = joystickR.getX();
                        lastYaw = motionEvent.getX();
                    }

                    if(joystickR.getY() != 0.0) {
                        thr = (int) ((joystickR.getY() + (thrRange / 200.0)) * 100.0);
                        lastThr = motionEvent.getY();
                    }
                    txtYaw.setText("Yaw : "+yaw);
                    txtThr.setText("Thr : "+thr);
                    mMessage = new PacketVO("2", ""+pitch, ""+roll, ""+yaw, ""+thr);
                    String jsonPacket = mMessage.toJson();
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if(mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char)0);
                        }

                        if(mUdpThread != null) {
                            Log.i("udp ", "message in main activity in");
                            udpMsg = mUdpThread.msg;
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                } else if(motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    joystickR.setRange(yawRange, thrRange / 100.0);

                    if(yaw == -(yawRange / 2.0)) {  // yaw가 최소값인 경우
                        lastYaw = joystickR.getOffset();
                    } else if (yaw == yawRange / 2.0) { // yaw가 최대값인 경우
                        lastYaw = joystickR.getLayoutWidth() - joystickR.getOffset();
                    }

                    if(thr == 0) {  // thr가 최소값인 경우
                        lastThr = joystickR.getLayoutHeight() - joystickR.getOffset();
                    } else if (thr == thrRange) { // yaw가 최대값인 경우
                        lastThr = joystickR.getOffset();
                    }

                    joystickR.drawStick(lastYaw, lastThr);

                    txtYaw.setText("Yaw : "+yaw);
                    txtThr.setText("Thr : "+thr);
                    mMessage = new PacketVO("2", ""+0.0, ""+0.0, ""+yaw, ""+thr);
                    String jsonPacket = mMessage.toJson();
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if(mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char)0);
                        }

                        if(mUdpThread != null) {
                            Log.i("udp ", "message in main activity in");
                            udpMsg = mUdpThread.msg;
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });
    }

    private void initNetworkThreads(){
        mTcpThread = new TCPThread();
        try {
            if (mTcpThread.tcpInit() == true) {
                Log.i("TCP Connect", "start");
                mTcpThread.run();

            }

            mUdpThread = new UDPThread();
            if (mUdpThread.udpInit() == true) {
                Log.i("UDP Connect", "start");
                mUdpThread.run();
            }
        }catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void closeNetworkThreads() {
        mTcpThread.closeSocket();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(Build.VERSION.SDK_INT > 9) {
            StrictMode.setThreadPolicy((new android.os.StrictMode.ThreadPolicy.Builder()).permitAll().build());
        }
        setContentView(R.layout.activity_setting);

        this.initComponents();
        this.initActionListeners();
        this.bindActionListeners();
        this.joystickSet();

    }//end of onCreate()

    @Override
    protected void onStop() {
        super.onStop();
        if(mTcpThread != null) {
            closeNetworkThreads();
        }
    }
}
