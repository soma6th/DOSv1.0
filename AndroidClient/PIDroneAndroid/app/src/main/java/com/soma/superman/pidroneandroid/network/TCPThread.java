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
    private static String ip = "10.10.0.1";
    private static int port = 8003;
    private DataInputStream inputStream;
    private DataOutputStream outputStream;
    public Socket mSocket;
    public static boolean flag = false;

    public TCPThread() {}

    public boolean tcpInit() {
        boolean isInit = false;
        setSocket(ip, port);
        if(readChar() == (char)1) {
            sendChar((char) 1);
            if(readChar() == (char)1) {
                isInit = true;
            }
        }
        return isInit;
    }

    @Override
    public void run() {
        //TODO reserved : TCP Receive will locate here
    }

    // header
    // -1이면 종료/에러
    // 0이면 hello data
    // 1이면 init msg
    // 2이면 emergency exit
    // 3이면 PID reset

    public void sendChar(char header) {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(outputStream));
        try {
            if(flag == false) {
                bw.write(header);
                bw.flush();
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
                read = new char[1];
                br.read(read, 0, 1);
                header = read[0];
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return header;
    }

    public PacketVO readJsonMessage() {
        PacketVO readPacket;
        String jsonMessage = "";

        //read json message
        BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));
        try {
            jsonMessage = br.readLine();
        } catch (IOException e) {
            e.printStackTrace();
        }

        readPacket = new PacketVO(jsonMessage);

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

    private void setSocket(String ip, int port) {
        try {
            mSocket = new Socket(ip, port);
            inputStream = new DataInputStream(mSocket.getInputStream());
            outputStream = new DataOutputStream(mSocket.getOutputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void closeSocket() {
        try {
            inputStream.close();
            outputStream.close();
            mSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
