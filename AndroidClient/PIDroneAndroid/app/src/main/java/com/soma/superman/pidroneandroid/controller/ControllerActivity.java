package com.soma.superman.pidroneandroid.controller;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.soma.superman.pidroneandroid.R;
import com.soma.superman.pidroneandroid.network.PacketVO;
import com.soma.superman.pidroneandroid.network.TCPThread;
import com.soma.superman.pidroneandroid.network.UDPThread;

/**
 * Created by whee6409 on 15. 11. 4.
 */
public class ControllerActivity extends Activity{

    // UI components
    RelativeLayout layoutJoystickL, layoutJoystickR;
    TextView txtPitch, txtRoll, txtThr, txtYaw;
    Button btnConnect;
    Joystick joystickL, joystickR;
    View.OnClickListener btnClickListener;

    // Thread for Networking
    TCPThread mTcpThread;
    UDPThread mUdpThread;
    PacketVO mMessage;

    // variables
    double pitch = 0.0;
    double roll = 0.0;
    double yaw = 0.0;
    int thr = 0;
    double lastYaw = 0.0;
    double lastThr = 0.0;

    boolean isConnectTCP = false;
    boolean isConnectUDP = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(Build.VERSION.SDK_INT > 9) {
            StrictMode.setThreadPolicy((new android.os.StrictMode.ThreadPolicy.Builder()).permitAll().build());
        }
        setContentView(R.layout.activity_controller);

        this.initComponents();
        this.initActionListeners();
        this.joystickSet();

    }//end of onCreate()

    @Override
    protected void onStop() {
        super.onStop();
        if(mTcpThread != null) {
            closeNetworkThreads();
        }
    }

    // Method for initialize UI components (Load and Inflate)
    private void initComponents(){
        txtPitch = (TextView)findViewById(R.id.txt_pitch);
        txtRoll = (TextView)findViewById(R.id.txt_roll);
        txtThr = (TextView)findViewById(R.id.txt_thr);
        txtYaw = (TextView)findViewById(R.id.txt_yaw);

        txtPitch.setText("Pitch : ");
        txtRoll.setText("Roll : ");
        txtThr.setText("Thr : ");
        txtYaw.setText("Yaw : ");

        btnConnect = (Button)findViewById(R.id.btn_connect);

        layoutJoystickL = (RelativeLayout)findViewById(R.id.layout_joystick_left);
        layoutJoystickR = (RelativeLayout)findViewById(R.id.layout_joystick_right);

    }

    private void initActionListeners(){
        this.btnClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                switch (v.getId()) {
                    case R.id.btn_connect:
                        if (mTcpThread == null && mUdpThread == null) {
                            initNetworkThreads();
                            if (isConnectTCP && isConnectUDP) {
                                btnConnect.setText("Disconnect");
                            }
                        } else if (mTcpThread != null && mUdpThread != null) {
                            closeNetworkThreads();
                            if (isConnectTCP && isConnectUDP) {
                                btnConnect.setText("Connect");
                            }
                        }
                        break;
                }
            }
        };// end of btnClickListener

        btnConnect.setOnClickListener(btnClickListener);
    }

    private void joystickSet() {
        //left joystick
        joystickL = new Joystick(getApplicationContext(), layoutJoystickL, R.drawable.image_button);
        joystickL.drawStick(joystickL.getLayoutWidth() / 2.0, joystickL.getLayoutHeight() / 2.0);
        layoutJoystickL.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN ||
                        motionEvent.getAction() == MotionEvent.ACTION_MOVE) {
                    joystickL.drawStick(motionEvent);
                    pitch = joystickL.getX();
                    roll = joystickL.getY();
                    txtPitch.setText("Pitch : " + pitch);
                    txtRoll.setText("Roll : " + roll);
                    mMessage = new PacketVO("2", "" + pitch, "" + roll, "" + yaw, "" + thr);
                    String jsonPacket = PacketVO.packetToJson(mMessage);
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if (mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char) 0);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                } else if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    joystickL.drawStick(joystickL.getLayoutWidth() / 2.0, joystickL.getLayoutHeight() / 2.0);
                    pitch = 0.0;
                    roll = 0.0;
                    txtPitch.setText("Pitch : ");
                    txtRoll.setText("Roll : ");
                    mMessage = new PacketVO("2", "" + 0.0, "" + 0.0, "" + 0.0, "" + thr);
                    String jsonPacket = PacketVO.packetToJson(mMessage);
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if (mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char) 0);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        });
        //right joystick
        joystickR = new Joystick(getApplicationContext(), layoutJoystickR, R.drawable.image_button);
        joystickR.drawStick(joystickR.getLayoutWidth() / 2.0, joystickR.getLayoutHeight() - joystickR.getOffset());
        layoutJoystickR.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if (motionEvent.getAction() == MotionEvent.ACTION_DOWN ||
                        motionEvent.getAction() == MotionEvent.ACTION_MOVE) {
                    joystickR.drawStick(motionEvent);
                    if (joystickR.getX() != 0.0) {
                        yaw = joystickR.getX();
                        lastYaw = motionEvent.getX();
                    }
                    if (joystickR.getY() != 0.0) {
                        thr = (int) ((joystickR.getY() + 10) * 100);
                        lastThr = motionEvent.getY();
                    }
                    txtYaw.setText("Yaw : " + yaw);
                    txtThr.setText("Thr : " + thr);
                    mMessage = new PacketVO("2", "" + pitch, "" + roll, "" + yaw, "" + thr);
                    String jsonPacket = PacketVO.packetToJson(mMessage);
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if (mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char) 0);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                } else if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    if (thr == 0) {
                        joystickR.drawStick(joystickR.getLayoutWidth() / 2.0, joystickR.getLayoutHeight() - joystickR.getOffset());
                    } else if (thr == 2000) {
                        joystickR.drawStick(joystickR.getLayoutWidth() / 2.0, joystickR.getOffset());
                    } else {
                        joystickR.drawStick(lastYaw, lastThr);
                    }
                    txtYaw.setText("Yaw : " + yaw);
                    txtThr.setText("Thr : " + thr);
                    mMessage = new PacketVO("2", "" + 0.0, "" + 0.0, "" + 0.0, "" + thr);
                    String jsonPacket = PacketVO.packetToJson(mMessage);
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if (mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char) 0);
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
        if (mTcpThread.tcpInit() == true) {
            Log.i("TCP Connect", "start");
            isConnectTCP = true;
            mTcpThread.run();
        }

        mUdpThread = new UDPThread();
        if (mUdpThread.udpInit() == true) {
            Log.i("UDP Connect", "start");
            isConnectUDP = true;
            mUdpThread.run();
        }
    }

    private void closeNetworkThreads() {
        mTcpThread.closeSocket();
        isConnectTCP = false;
        isConnectUDP = false;
    }

}
