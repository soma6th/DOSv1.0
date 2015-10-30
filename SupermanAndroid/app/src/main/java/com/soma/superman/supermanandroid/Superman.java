package com.soma.superman.supermanandroid;

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

import java.io.IOException;
import java.util.ArrayList;

public class Superman extends Activity {

    Button btnTCPConnect, btnExit, btnPIDReset;

    ImageButton btnStop;
    TextView txtPitch, txtRoll, txtThr, txtYaw;
    Joystick joystickL, joystickR;
    RelativeLayout layoutJoystickL, layoutJoystickR;

    // debugging
    TextView udpReceiveMsg; ArrayList<String> udpReceiveMsgData = new ArrayList<String>();
    Button btnP1, btnM1, btnP2, btnM2, btnP3, btnM3;
    EditText etX, etY, etZ;
    RadioGroup rg1;

    TCPThread mTcpThread;
    UDPThread mUdpThread;
    PacketVO mMessage;

    static double pitch = 0.0, roll = 0.0, yaw = 0.0;
    static int thr = 0;
    static double lastThr = 0;

    double x, y, z;
    double amount;

    String udpMsg;

    @Override
    protected void onStop() {
        super.onStop();
        if(mTcpThread != null) {
            try {
                mTcpThread.inputStream.close();
                mTcpThread.outputStream.close();
                mTcpThread.mSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(Build.VERSION.SDK_INT > 9) {
            StrictMode.setThreadPolicy((new android.os.StrictMode.ThreadPolicy.Builder()).permitAll().build());
        }
        setContentView(R.layout.activity_superman);




        btnTCPConnect = (Button)findViewById(R.id.btn_tcp_connect);
        btnTCPConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mTcpThread = new TCPThread();
                if (mTcpThread.tcpInit() == true) {
                    Log.i("TCP Connect", "start");
                    mTcpThread.run();
                }

                mUdpThread = new UDPThread();
                if (mUdpThread.udpInit() == true) {
                    Log.i("UDP Connect", "start");
                    mUdpThread.run();
                }
            }
        });

        udpReceiveMsg = (TextView)findViewById(R.id.tv_udp_receive);
        udpReceiveMsg.post(new Runnable() {
            @Override
            public void run() {
                udpReceiveMsg.setText("" +mUdpThread.msg);
            }
        });

        // TODO: upgrade to asyncTask
        // http://stackoverflow.com/questions/4369537/update-ui-from-thread

        btnExit = (Button)findViewById(R.id.btn_exit);
        btnExit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    mTcpThread.sendChar((char)5);
                    mTcpThread.mSocket.close();
                    mUdpThread.socket.close();

                } catch (Exception e) {
                    e.printStackTrace();
                }

            }
        });

        // start of pid debugging

        etX = (EditText)findViewById(R.id.et_x);
        etY = (EditText)findViewById(R.id.et_y);
        etZ = (EditText)findViewById(R.id.et_z);

        etX.setText("0.00000");
        etY.setText("0.00000");
        etZ.setText("0.00000");

        x = 0.00000;
        y = 0.00000;
        z = 0.00000;

        btnP1 = (Button)findViewById(R.id.btn_p1);
        btnP2 = (Button)findViewById(R.id.btn_p2);
        btnP3 = (Button)findViewById(R.id.btn_p3);

        btnM1 = (Button)findViewById(R.id.btn_m1);
        btnM2 = (Button)findViewById(R.id.btn_m2);
        btnM3 = (Button)findViewById(R.id.btn_m3);

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

        btnP1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                x = Float.parseFloat(String.valueOf(etX.getText()));
                x += amount;
                etX.setText(String.format("%.5f", x).toString());
            }
        });

        btnP2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                y = Float.parseFloat(String.valueOf(etY.getText()));
                y += amount;
                etY.setText(String.format("%.5f", y).toString());
            }
        });

        btnP3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                z = Float.parseFloat(String.valueOf(etZ.getText()));
                z += amount;
                etZ.setText(String.format("%.5f", z).toString());
            }
        });

        btnM1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                x = Float.parseFloat(String.valueOf(etX.getText()));
                x -= amount;
                if(x <= 0) {
                    x = 0;
                }
                etX.setText(String.format("%.5f", x).toString());
            }
        });

        btnM2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                y = Float.parseFloat(String.valueOf(etY.getText()));
                y -= amount;
                if(y <= 0) {
                    y = 0;
                }
                etY.setText(String.format("%.5f", y).toString());
            }
        });

        btnM3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                z = Float.parseFloat(String.valueOf(etZ.getText()));
                z -= amount;
                if(z <= 0) {
                    z = 0;
                }
                etZ.setText(String.format("%.5f", z).toString());
            }
        });

        btnPIDReset = (Button)findViewById(R.id.btn_pid_reset);
        btnPIDReset.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String jsonMessage;
                jsonMessage = "{\"P_P\":" + etX.getText() + "," +
                        "\"P_I\":" + etY.getText() + "," +
                        "\"P_D\":" + etZ.getText() + "}";
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
            }
        });


        // end of pid debugging

        btnStop = (ImageButton)findViewById(R.id.btn_stop);
        btnStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    mTcpThread.sendChar((char) 2);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        txtPitch = (TextView)findViewById(R.id.txt_pitch);
        txtRoll = (TextView)findViewById(R.id.txt_roll);
        txtThr = (TextView)findViewById(R.id.txt_thr);
        txtYaw = (TextView)findViewById(R.id.txt_yaw);

        txtPitch.setText("Pitch : ");
        txtRoll.setText("Roll : ");
        txtThr.setText("Thr : ");
        txtYaw.setText("Yaw : ");

        layoutJoystickL = (RelativeLayout)findViewById(R.id.layout_joystick_left);
        joystickL = new Joystick(getApplicationContext(), layoutJoystickL, R.drawable.image_button);
        joystickL.setStickSize(150, 150);
        joystickL.setLayoutSize(500, 500);
        joystickL.setLayoutAlpha(100);
        joystickL.setOffset(90);
        joystickL.setMinimumDistance(10);
        joystickL.drawStick(joystickL.getLayoutWidth() / 2.0f, joystickL.getLayoutHeight() / 2.0f);
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

                        if(mUdpThread != null) {
                            Log.i("udp ", "message in main activity in");
                            udpMsg = mUdpThread.msg;
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                } else if (motionEvent.getAction() == MotionEvent.ACTION_UP) {
                    joystickL.drawStick(joystickL.getLayoutWidth() / 2.0f, joystickL.getLayoutHeight() / 2.0f);
                    pitch = 0.0f;
                    roll = 0.0f;
                    txtPitch.setText("Pitch : ");
                    txtRoll.setText("Roll : ");
                    mMessage = new PacketVO("2", "" + 0.0, "" + 0.0, "" + 0.0, "" + thr);
                    String jsonPacket = PacketVO.packetToJson(mMessage);
                    try {
                        mUdpThread.sendPacket(jsonPacket);
                        if (mUdpThread.receivePacket().get("P_H").toString().equals("2")) {
                            mTcpThread.sendChar((char) 0);
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

        layoutJoystickR = (RelativeLayout)findViewById(R.id.layout_joystick_right);
        joystickR = new Joystick(getApplicationContext(), layoutJoystickR, R.drawable.image_button);
        joystickR.setStickSize(150, 150);
        joystickR.setLayoutSize(500, 500);
        joystickR.setLayoutAlpha(100);
        joystickR.setOffset(90);
        joystickR.setMinimumDistance(10);
        joystickR.drawStick(joystickR.getLayoutWidth() / 2.0f, joystickR.getLayoutHeight() - joystickR.getOffset());
        layoutJoystickR.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                if(motionEvent.getAction() == MotionEvent.ACTION_DOWN ||
                        motionEvent.getAction() == MotionEvent.ACTION_MOVE) {
                    joystickR.drawStick(motionEvent);
                    yaw = joystickR.getX();
                    if(joystickR.getY() != 0.0f) {
                        thr = (int) ((joystickR.getY() + 10) * 100);
                        lastThr = motionEvent.getY();
                    }
                    txtYaw.setText("Yaw : "+yaw);
                    txtThr.setText("Thr : "+thr);
                    mMessage = new PacketVO("2", ""+pitch, ""+roll, ""+yaw, ""+thr);
                    String jsonPacket = PacketVO.packetToJson(mMessage);
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
                    if(thr == 0 ) {
                        joystickR.drawStick(joystickR.getLayoutWidth() / 2.0f, joystickR.getLayoutHeight() - joystickR.getOffset());
                    } else if(thr == 2000) {
                        joystickR.drawStick(joystickR.getLayoutWidth() / 2.0f, joystickR.getOffset());
                    } else {
                        joystickR.drawStick(joystickR.getLayoutHeight() / 2.0f, lastThr);
                    }
                    yaw = 0.0f;
                    txtYaw.setText("Yaw : ");
                    txtThr.setText("Thr : "+thr);
                    mMessage = new PacketVO("2", ""+0.0, ""+0.0, ""+0.0, ""+thr);
                    String jsonPacket = PacketVO.packetToJson(mMessage);
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
}
