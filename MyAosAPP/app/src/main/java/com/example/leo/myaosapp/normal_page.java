package com.example.leo.myaosapp;

import android.app.AlertDialog;
import android.content.Intent;
import android.os.HandlerThread;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.os.Handler;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;


import java.text.SimpleDateFormat;
//import java.util.logging.Handler;
import java.util.logging.LogRecord;

public class normal_page extends AppCompatActivity {
    String search_string = "";
    String result = "";
    String message = "";
    String database_msg = "";
    String teststring = "";
    String mydate = "";
    Handler mthread_headler = new Handler();

    int check_time=0;
    long time_ms=0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_normal_page);


        final Button log_out_normal = (Button)findViewById(R.id.button5);
        final Button setmessage = (Button)findViewById(R.id.button4);
        final Button allbutton = (Button)findViewById(R.id.button7);
        final Button youbutton = (Button)findViewById(R.id.button6);
        allbutton.setClickable(false);
        youbutton.setClickable(false);
        allbutton.setText("");
        youbutton.setText("");

        allbutton.getBackground().setAlpha(0);
        youbutton.getBackground().setAlpha(0);
        Intent time_service = new Intent(normal_page.this, updata_service.class);
        startService(time_service);
        mthread_headler.post(database_timeset);

        setmessage.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {

                //setmessage.setClickable(false);
                connectDB take_data = new connectDB();
                TextView message_text = (TextView) findViewById(R.id.textView6);
                EditText fillmessage = (EditText) findViewById(R.id.editText3);
                message = fillmessage.getText().toString();
                take_data.start();
                SystemClock.sleep(1000);
                message_text.setText(database_msg);
                fillmessage.getText().clear();
               // setmessage.setClickable(true);
            }
        });
        youbutton.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                msg_checkyou change_msg_checkyou = new msg_checkyou();
                AlertDialog.Builder MyAlert = new AlertDialog.Builder(normal_page.this);
                Global_variable mymsg = (Global_variable) getApplicationContext();
                MyAlert.setTitle("Boss Message");
                MyAlert.setMessage(mymsg.check_you_msg);
                mymsg.check_you=0;
                youbutton.getBackground().setAlpha(0);
                youbutton.setClickable(false);
                youbutton.setText("");
                change_msg_checkyou.start();
                MyAlert.show();
            }
        });
        allbutton.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                msg_checkall change_msg_checkall = new msg_checkall();
                AlertDialog.Builder MyAlert = new AlertDialog.Builder(normal_page.this);
                Global_variable mymsg = (Global_variable) getApplicationContext();
                MyAlert.setTitle("Boss BroadCast Message");
                MyAlert.setMessage(mymsg.check_all_msg);
                mymsg.check_all=0;
                allbutton.getBackground().setAlpha(0);
                allbutton.setClickable(false);
                allbutton.setText("");
                change_msg_checkall.start();
                MyAlert.show();
            }
        });
        log_out_normal.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                Global_variable end_log = (Global_variable) getApplicationContext();
                end_log.initial_variable();

                Intent time_service = new Intent(normal_page.this, updata_service.class);
                stopService(time_service);

                startActivity(new Intent(normal_page.this, log_page.class));
                normal_page.this.finish();
            }
        });

    }
    final Runnable database_timeset =new Runnable() {

        @Override
        public void run() {

            runOnUiThread(new Runnable() {
                TextView time_text = (TextView) findViewById(R.id.textView8);
                Button toyou = (Button)findViewById(R.id.button6);
                Button toall = (Button)findViewById(R.id.button7);
                Global_variable take_global_time = (Global_variable) getApplicationContext();

                @Override
                public void run() {
                    time_ms = take_global_time.check_time;
                    time_ms = time_ms * 1000;
                    mydate = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(time_ms).toString();
                    time_text.setText(mydate);
                    if (take_global_time.check_all == 1) {
                        toall.setClickable(true);
                        toall.setText("Msg to All");
                        toall.getBackground().setAlpha(255);
                    }
                    if(take_global_time.check_you==1){
                        toyou.setClickable(true);
                        toyou.setText("Msg to You");
                        toyou.getBackground().setAlpha(255);
                    }
                }
            });
            mthread_headler.postDelayed(this, 10000);
        }
    };
    class connectDB extends Thread {
        String update_string = "UPDATE users SET message =";
        String query_string = "SELECT * FROM users WHERE id ='";
        Global_variable account = (Global_variable) getApplicationContext();
        @Override
        public void run() {
            super.run();
            try {
                teststring = account.global_account;
                search_string =  update_string +"'"+message+"' WHERE id = '" +account.global_account+"'";
                result = httppost.sqlpost(search_string);
                database_msg = message;
                //Toast.makeText(normal_page.this, "Update Success", Toast.LENGTH_SHORT).show();
            } catch (Exception e) {
                //database_msg = "Update Error";
            }
        }
    }

    class msg_checkall extends Thread {
        String update_string = "UPDATE users SET checkall = 0 where id = '";
        Global_variable update_checkall = (Global_variable) getApplicationContext();
        @Override
        public void run() {
            super.run();
            try {
                search_string = update_string + update_checkall.global_account+"'";
                result = httppost.sqlpost(search_string);

            } catch (Exception e) {

            }
        }
    }
    class msg_checkyou extends Thread {
        String update_string = "UPDATE users SET checkyou = 0 where id = '";
        Global_variable update_checkall = (Global_variable) getApplicationContext();
        @Override
        public void run() {
            super.run();
            try {
                search_string = update_string + update_checkall.global_account+"'";
                result = httppost.sqlpost(search_string);

            } catch (Exception e) {

            }
        }
    }
    protected void onDestroy(){
        super.onDestroy();

        Intent time_service = new Intent(normal_page.this, updata_service.class);
        stopService(time_service);

    }
}
