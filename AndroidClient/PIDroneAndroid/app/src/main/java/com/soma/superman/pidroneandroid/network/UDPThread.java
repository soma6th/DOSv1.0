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
    boolean isConnect = false;
    public DatagramSocket socket;
    DatagramPacket packet;
    static InetAddress serverIP;
    static int port = 8004;
    public String msg = null;

    public UDPThread() {}

    public boolean udpInit() {
        boolean isInit = false;
        try {
            serverIP = InetAddress.getByName("10.10.0.1");
            setSocket(serverIP, port);

            PacketVO mMessage = new PacketVO ("1", "0.0", "0.0", "0.0", "0");
            String jsonMessage = PacketVO.packetToJson(mMessage);
            sendPacket(jsonMessage);
            JSONObject object = receivePacket();
            Log.i("udp P_H top", "" + object.get("P_H").toString());
            try {
                if(object.get("P_H").toString().equals("1")) {
                    Log.i("udp", "P_H = 1");
                    mMessage = new PacketVO("1", "1", "2", "3", "4");
                    jsonMessage = PacketVO.packetToJson(mMessage);
                    sendPacket(jsonMessage);
                    isInit = true;
                    return isInit;
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        }

        return isInit;
    }

    public void setSocket(InetAddress ip, int port) {
        Log.i("UDP", "run start");
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
                Log.i("udp receive : ", ""+msg);
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
                Log.i("UDP", "receive : "+jsonObject.toString());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return jsonObject;
    }

    public void sendPacket(String mMessage) {
        byte [] buf = mMessage.getBytes();
        Log.i("UDP", "buf = " + buf);

        try {
            packet = new DatagramPacket(buf, buf.length);
            Log.i("UDP", "make packet");
            if(packet != null) {
                socket.send(packet);
                Log.i("UDP send ok", "" + mMessage);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
