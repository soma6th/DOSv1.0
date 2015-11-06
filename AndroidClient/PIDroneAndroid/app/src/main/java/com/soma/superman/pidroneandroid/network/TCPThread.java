package com.soma.superman.pidroneandroid.network;

import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Created by whee6409 on 15. 11. 6.
 */
public class TCPThread extends Thread{
    public static String ip = "10.10.0.1";
    public static int port = 8003;
    public static boolean flag = false;
    public DataInputStream inputStream;
    public DataOutputStream outputStream;
    public Socket mSocket;

    public TCPThread() {}

    public boolean tcpInit() {
        boolean isInit = false;
        setSocket(ip, port);
        if(readChar() == (char)1) {
            Log.i("read byte 1", "in");
            sendChar((char) 1);
            if(readChar() == (char)1) {
                Log.i("read byte 2", "in");
                isInit = true;
            }
        }
        return isInit;
    }

    @Override
    public void run() {

    }

    public void sendChar(char header) {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(outputStream));
        try {
            if(flag == false) {
                bw.write(header);
                bw.flush();
                Log.i("byte header", "" + header);
            } else if(flag == true) {
                bw.flush();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public char readChar() {
        char header = (char)-1;
        BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));
        try {
            char [] read = null;
            while(read == null) {
                Log.i("read byte", "in");
                read = new char[1];
                br.read(read, 0, 1);
                Log.i("read ", "" + read);
                header = read[0];
                Log.i("read byte", ""+header);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return header;
    }

    public PacketVO readJsonMessage() {
        PacketVO readPacket = new PacketVO();
        BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));
        try {
            String jsonMessage = br.readLine();
            Log.i("read message", "br read line "+jsonMessage);
            readPacket = PacketVO.jsonToPacket(jsonMessage);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return readPacket;
    }

    public int sendPID(String jsonMessage) {
        PrintWriter pw = new PrintWriter(outputStream);
        pw.println(jsonMessage);
        pw.flush();
        return readChar();
    }

    public void sendJsonMessage(String jsonMessage) {
        PrintWriter pw = new PrintWriter(outputStream);
        pw.println(jsonMessage);
        pw.flush();
    }

    public void setSocket(String ip, int port) {
        try {
            Log.d("setSocket", "start");
            mSocket = new Socket(ip, port);
            inputStream = new DataInputStream(mSocket.getInputStream());
            outputStream = new DataOutputStream(mSocket.getOutputStream());
            Log.d("setSocket", "end");
        } catch (IOException e) {
            e.printStackTrace();
            Log.e("setSocket", "error");
        }
    }
}
