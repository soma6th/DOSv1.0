package com.soma.superman.pidroneandroid.network;

import android.util.Log;


import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

/**
 * Created by whee6409 on 15. 11. 6.
 */
public class PacketVO {
    private String P_H;
    private String P_X;
    private String P_Y;
    private String P_Z;
    private String P_T;

    public PacketVO() {
        super();
    }

    public PacketVO(String jsonMessage){
        super();
        JSONObject jsonObject;
        try {
            jsonObject = (JSONObject)new JSONParser().parse(jsonMessage);
            this.P_H = jsonObject.get("P_H").toString();
            this.P_X = jsonObject.get("P_X").toString();
            this.P_Y = jsonObject.get("P_Y").toString();
            this.P_Z = jsonObject.get("P_Z").toString();
            this.P_T = jsonObject.get("P_T").toString();
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }

    public PacketVO(String P_H, String P_X, String P_Y, String P_Z, String P_T) {
        super();
        this.P_H = P_H;
        this.P_X = P_X;
        this.P_Y = P_Y;
        this.P_Z = P_Z;
        this.P_T = P_T;
    }

    @Override
    public String toString() {
        return "Packet : "+P_H+", "+P_X+", "+P_Y+", "+P_Z+", "+P_T;
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
