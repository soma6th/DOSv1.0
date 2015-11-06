package com.soma.superman.pidroneandroid;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;

import com.soma.superman.pidroneandroid.controller.ControllerActivity;
import com.soma.superman.pidroneandroid.level.LevelActivity;

public class PIDroneActivity extends Activity {

    ImageButton btnController;
    ImageButton btnSetting;
    ImageButton btnLevel;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pidrone);

        btnController = (ImageButton) findViewById(R.id.imgbtn_control);
        btnSetting = (ImageButton) findViewById(R.id.imgbtn_setting);
        btnLevel = (ImageButton) findViewById(R.id.imgbtn_level);

        btnController.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), ControllerActivity.class);
                startActivity(intent);
            }
        });

        btnLevel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), LevelActivity.class);
                startActivity(intent);
            }
        });
    }
}
