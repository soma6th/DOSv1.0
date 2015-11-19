package com.soma.superman.pidroneandroid.network;

import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

/**
 * Created by whee6409 on 15. 11. 6.
 */
public class UDPThread extends Thread {
    public DatagramSocket socket;
    private DatagramPacket packet;
    private static InetAddress serverIP;
    private static int port = 8004;
    public String msg = null;

    public UDPThread(){
        super();
    }

    public UDPThread(String inetAddress, int port){
        //TODO generic 하게 만들어야 함
    }

    public boolean udpInit() {
        boolean isInit = false;
        try {
            serverIP = InetAddress.getByName("10.10.0.1");
            setSocket(serverIP, port);

            PacketVO mMessage = new PacketVO ("1", "0.0", "0.0", "0.0", "0");
            String jsonMessage = mMessage.toJson();
            sendPacket(jsonMessage);
            JSONObject object = receivePacket();
            try {
                if(object.get("P_H").toString().equals("1")) {
                    mMessage = new PacketVO("1", "1", "2", "3", "4");
                    jsonMessage = mMessage.toJson();
                    sendPacket(jsonMessage);
                    isInit = true;
                    return isInit;
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }

        return isInit;
    }

    private void setSocket(InetAddress ip, int port) {
        try {
            socket = new DatagramSocket();
            socket.connect(ip, port);
        } catch (SocketException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        Log.i("UDP", "run");
        receive();
    }

    public void receive() {
        try {
            String buffer = null;
            while(buffer == null) {
                buffer = new String(packet.getData(), 0, packet.getLength());
                msg = buffer;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public JSONObject receivePacket() {
        JSONObject jsonObject = new JSONObject();
        try {
            String receive = null;
            while(receive == null) {
                receive = new String(packet.getData(), 0, packet.getLength());
                jsonObject = new JSONObject(receive);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return jsonObject;
    }

    // header
    // 1이면 init msg
    // 2이면 control data (controller -> drone)
    // 3이면 status data (drone -> controller)

    public void sendPacket(String mMessage) {
        byte [] buf = mMessage.getBytes();
        try {
            packet = new DatagramPacket(buf, buf.length);
            if(packet != null) {
                socket.send(packet);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
