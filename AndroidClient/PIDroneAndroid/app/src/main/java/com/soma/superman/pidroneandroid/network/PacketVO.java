package com.soma.superman.pidroneandroid.network;

import android.util.Log;


import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

/**
 * Created by whee6409 on 15. 11. 6.
 */
public class PacketVO {
    public static final int DATA_SIZE = 20;
    String P_H;
    // -1이면 에러
    // 0이면 종료
    // 1이면 init -> 안드로이드 먼저 서버에 보내고, 버전을 받음.
    // 2이면 안드로이드 -> 서버
    // 3이면 서버 -> 안드로이드 (status 데이터)
    String P_X;
    String P_Y;
    String P_Z;
    String P_T;

    public PacketVO() { }

    public PacketVO(String P_H, String P_X, String P_Y, String P_Z, String P_T) {
        this.P_H = P_H;
        this.P_X = P_X;
        this.P_Y = P_Y;
        this.P_Z = P_Z;
        this.P_T = P_T;
    }

    public String toString() {
        return "Packet : "+P_H+", "+P_X+", "+P_Y+", "+P_Z+", "+P_T;
    }

    public static PacketVO jsonToPacket(String jsonMessage) {
        PacketVO readPacket = new PacketVO();
        JSONObject jsonObject = null;
        try {
            jsonObject = (JSONObject)new JSONParser().parse(jsonMessage);
            readPacket = new PacketVO();
            readPacket.P_H = jsonObject.get("P_H").toString();
            readPacket.P_X = jsonObject.get("P_X").toString();
            readPacket.P_Y = jsonObject.get("P_Y").toString();
            readPacket.P_Z = jsonObject.get("P_Z").toString();
            readPacket.P_T = jsonObject.get("P_T").toString();
            Log.i("read Packet", "" + readPacket.toString());
        } catch (ParseException e) {
            e.printStackTrace();
        }

        return readPacket;
    }

    public static String packetToJson(PacketVO message) {
        String jsonPacket = new String();
        jsonPacket+="{\"P_H\":"+message.P_H+"," +
                "\"P_X\":"+message.P_X+","+
                "\"P_Y\":"+message.P_Y+","+
                "\"P_Z\":"+message.P_Z+","+
                "\"P_T\":"+message.P_T+"}";
        return jsonPacket;
    }
}
