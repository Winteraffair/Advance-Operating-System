package com.example.leo.myaosapp;

import android.animation.AnimatorSet;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Handler;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;

import java.sql.Struct;
import java.text.SimpleDateFormat;
import java.util.ArrayList;

public class boss_page extends AppCompatActivity {
    Handler mthread_headler = new Handler();
    String result = "", search_string="", userid="", data_msg="", account="",date_string,message;
    int check_time=0,time_now=0,last_time=0;
    long change_time=0;
    member_list[] member = new member_list[5];
    ArrayList<String> myuserlist = new ArrayList<String>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_boss_page);
        ImageButton dashown = (ImageButton) findViewById(R.id.imageButton);
        member[0] = new member_list();
        member[0].initialize("yoyo1234",0);
        ImageButton doraemon = (ImageButton) findViewById(R.id.imageButton2);
        member[1] = new member_list();
        member[1].initialize("dodo1234", 0);
        ImageButton ponhu = (ImageButton) findViewById(R.id.imageButton3);
        member[2] = new member_list();
        member[2].initialize("popo1234", 0);
        ImageButton shaufu = (ImageButton) findViewById(R.id.imageButton4);
        member[3] = new member_list();
        member[3].initialize("soso1234", 0);
        ImageButton ginshawn = (ImageButton) findViewById(R.id.imageButton5);
        member[4] = new member_list();
        member[4].initialize("gogo1234", 0);
        Button broadcast = (Button)findViewById(R.id.button8);
        Button log_out_boss = (Button)findViewById(R.id.button3);

        mthread_headler.post(check_runable);


        dashown.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                userid = member[0].id;
                get_user_msg getUserMsg = new get_user_msg();
                getUserMsg.getmsg();

            }
        });
        doraemon.setOnClickListener(new Button.OnClickListener(){
            public void onClick(View v){
                userid = member[1].id;
                get_user_msg getUserMsg = new get_user_msg();
                getUserMsg.getmsg();
            }
        });
        ponhu.setOnClickListener(new Button.OnClickListener(){
            public void onClick(View v){
                userid = member[2].id;
                get_user_msg getUserMsg = new get_user_msg();
                getUserMsg.getmsg();
            }
        });
        shaufu.setOnClickListener(new Button.OnClickListener(){
            public void onClick(View v){
                userid = member[3].id;
                get_user_msg getUserMsg = new get_user_msg();
                getUserMsg.getmsg();
            }
        });
        ginshawn.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                userid = member[4].id;
                get_user_msg getUserMsg = new get_user_msg();
                getUserMsg.getmsg();
            }
        });
        broadcast.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                get_user_msg getmsglist = new get_user_msg();
                getmsglist.getallmsg();
            }
        });
        log_out_boss.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                startActivity(new Intent(boss_page.this, log_page.class));
                boss_page.this.finish();
            }
        });
    }
    final Runnable check_runable =new Runnable() {

        @Override
        public void run() {
            check_routine cr = new check_routine();
            cr.start();
            SystemClock.sleep(2000);
            runOnUiThread(new Runnable() {
                ImageButton dashown = (ImageButton) findViewById(R.id.imageButton);
                ImageButton doraemon = (ImageButton) findViewById(R.id.imageButton2);
                ImageButton ponhu = (ImageButton) findViewById(R.id.imageButton3);
                ImageButton shaufu = (ImageButton) findViewById(R.id.imageButton4);
                ImageButton ginshawn = (ImageButton) findViewById(R.id.imageButton5);

                @Override
                public void run() {
                    int i;
                    for (i = 0; i < 5; i++) {
                        switch (i) {
                            case 0:
                                if (member[0].online_stat == 0) {
                                    dashown.getBackground().setAlpha(150);
                                } else {
                                    dashown.getBackground().setAlpha(255);
                                }
                                break;
                            case 1:
                                if (member[1].online_stat == 0) {
                                    doraemon.getBackground().setAlpha(150);
                                } else {
                                    doraemon.getBackground().setAlpha(255);
                                }
                                break;
                            case 2:
                                if (member[2].online_stat == 0) {
                                    ponhu.getBackground().setAlpha(150);
                                } else {
                                    ponhu.getBackground().setAlpha(255);
                                }
                                break;
                            case 3:
                                if (member[3].online_stat == 0) {
                                    shaufu.getBackground().setAlpha(150);
                                } else {
                                    shaufu.getBackground().setAlpha(255);
                                }
                                break;
                            case 4:
                                if (member[4].online_stat == 0) {
                                    ginshawn.getBackground().setAlpha(150);
                                } else {
                                    ginshawn.getBackground().setAlpha(255);
                                }
                                break;
                        }
                    }
                }
            });
            mthread_headler.postDelayed(this, 60000);
        }
    };
    class get_user_msg{
        public  void getallmsg(){
            AlertDialog.Builder MyAlert = new AlertDialog.Builder(boss_page.this);
            final EditText Alertedit =new EditText(boss_page.this);
            String[] msg_buffer;
            MyAlert.setNeutralButton("Update Messge", new DialogInterface.OnClickListener() {
                // do something when the button is clicked
                public void onClick(DialogInterface arg0, int arg1) {
                    set_allmsg setmsg = new set_allmsg();
                    message = Alertedit.getText().toString();
                    setmsg.start();
                }
            });
            check_all_msg call = new check_all_msg();
            call.start();
            SystemClock.sleep(1000);
            MyAlert.setTitle("Broadcast to All");
            MyAlert.setView(Alertedit);
            myuserlist.add("Set Your Message Below To Broascast");
            msg_buffer = myuserlist.toArray(new String[myuserlist.size()]);
            MyAlert.setItems(msg_buffer, new DialogInterface.OnClickListener() {
               public void onClick(DialogInterface dialog, int item) {

               }
           });
            MyAlert.show();
        }
        public void getmsg()
        {
            AlertDialog.Builder MyAlert = new AlertDialog.Builder(boss_page.this);
            final EditText Alertedit =new EditText(boss_page.this);

            MyAlert.setNeutralButton("Update Messge", new DialogInterface.OnClickListener() {
                // do something when the button is clicked
                public void onClick(DialogInterface arg0, int arg1) {
                    set_youmsg setmsg = new set_youmsg();
                    message = Alertedit.getText().toString();
                    setmsg.start();
                }
            });
            check_message ck = new check_message();
            ck.start();
            SystemClock.sleep(1000);
            MyAlert.setTitle("Users Information");
            change_time = last_time;
            change_time = change_time * 1000;
            date_string = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(change_time).toString();
            date_string = "Last Message :\n" + data_msg + "\nLast Onine Time :\n" + date_string;
            MyAlert.setMessage(date_string);
            MyAlert.setView(Alertedit);
            MyAlert.show();
        }
    }
    class set_youmsg extends Thread {
        String update_string = "UPDATE users SET checkyoumsg ='";
        String update_int = "UPDATE users SET checkyou = 1 where id ='";
        @Override
        public void run() {
            super.run();
            try {
                search_string =  update_string+message+"'  where id = '"+userid+"'";
                result = httppost.sqlpost(search_string);
                search_string =  update_int+userid+"'";
                result = httppost.sqlpost(search_string);
                Toast.makeText(boss_page.this, "Update Success", Toast.LENGTH_SHORT).show();
            } catch (Exception e) {
            }
        }
    }
    class set_allmsg extends Thread {
        String update_string = "UPDATE users SET checkallmsg ='";
        String update_int = "UPDATE users SET checkall = 1 where mode = 'Normal'";
        @Override
        public void run() {
            super.run();
            try {
                search_string =  update_string+message+"'  where mode ='Normal'";
                result = httppost.sqlpost(search_string);
                result = httppost.sqlpost(update_int);
                Toast.makeText(boss_page.this, "Update Success", Toast.LENGTH_SHORT).show();
            } catch (Exception e) {
            }
        }
    }
    class check_routine extends Thread {
        String query_string = "SELECT * FROM users";

        @Override
        public void run() {
            super.run();
            try {
                search_string =  query_string ;
                result = httppost.sqlpost(search_string);
                JSONArray jsonArray = new JSONArray(result);
                time_now = (int)(System.currentTimeMillis() / 1000);
                for(int i = 0; i < jsonArray.length(); i++) {
                    JSONObject jsonData = jsonArray.getJSONObject(i);
                    account = jsonData.getString("id");
                    check_time = jsonData.getInt("time");
                    if((time_now - check_time) < 60){
                        for(int j=0;j<5;j++){
                            if(account.equals(member[j].id)){
                                member[j].online_stat = 1;
                            }
                        }
                    } else{
                        for(int j=0;j<5;j++){
                            if(account.equals(member[j].id)){
                                member[j].online_stat = 0;
                            }
                        }
                    }
                }
            } catch (Exception e) {
                check_time = 100;
            }
        }
    }
    class check_all_msg extends Thread {
        String query_string = "SELECT * FROM users where mode = 'Normal'";
        @Override
        public void run() {
            super.run();
            try {
                search_string =  query_string;
                result = httppost.sqlpost(search_string);
                JSONArray jsonArray = new JSONArray(result);
                myuserlist.clear();
                for(int i = 0; i < jsonArray.length(); i++) {
                    JSONObject jsonData = jsonArray.getJSONObject(i);
                    change_time = jsonData.getInt("time");
                    change_time = change_time * 1000;
                    date_string = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(change_time).toString();

                    myuserlist.add(jsonData.getString("name")+":\n"+jsonData.getString("message")+"\n"+date_string+"\n");
                }
            } catch (Exception e) {
            }
        }
    }
    class check_message extends Thread {
        String query_string = "SELECT * FROM users WHERE id ='";
        @Override
        public void run() {
            super.run();
            try {
                search_string =  query_string +userid+"'";
                result = httppost.sqlpost(search_string);
                JSONArray jsonArray = new JSONArray(result);
                for(int i = 0; i < jsonArray.length(); i++) {
                    JSONObject jsonData = jsonArray.getJSONObject(i);
                    data_msg = jsonData.getString("message");
                    last_time = jsonData.getInt("time");
                }
            } catch (Exception e) {
            }
        }
    }
    final class member_list{
        public String id;
        public int online_stat;

        public void initialize(String id,int stat){
            this.id = id;
            this.online_stat = stat;
        }
    }
}
