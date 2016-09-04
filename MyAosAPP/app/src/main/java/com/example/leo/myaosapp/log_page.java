package com.example.leo.myaosapp;

import android.content.Context;
import android.content.Intent;

import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.SystemClock;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;


import android.widget.EditText;

import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;



public class log_page extends AppCompatActivity {
    String result = "";
    String teststring = "";
    String account = "";
    String password = "";
    String message = "";
    String mode = "";
    String mode_string="Boss";
    //Thread connectDB;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_log_page);
        final EditText id_set = (EditText)findViewById(R.id.editText);
        final Button logbuttom = (Button) findViewById(R.id.button2);
        final TextView text1 = (TextView) findViewById(R.id.textView5);
        final TextView text_account = (TextView) findViewById(R.id.textView3);
        final EditText text_pass = (EditText) findViewById(R.id.editText2);
        logbuttom.setOnClickListener(new Button.OnClickListener() {
            public void onClick(View v) {
                if(isConnected()) {
                    connectDB take_data = new connectDB();
                    take_data.start();
                    SystemClock.sleep(2000);
                    teststring = text_pass.getText().toString();
                    if (teststring.equals(password)) {
                        Global_variable global_account = (Global_variable) getApplicationContext();
                        global_account.global_account = id_set.getText().toString();
                        if(mode.equals(mode_string)) {
                            startActivity(new Intent(log_page.this, boss_page.class));
                            log_page.this.finish();
                        }
                        else{
                            startActivity(new Intent(log_page.this, normal_page.class));
                            log_page.this.finish();
                        }
                    }
                    else{
                        Toast.makeText(log_page.this,"Wrong Account or Password",Toast.LENGTH_SHORT).show();
                    }
                }
                else{

                    Toast.makeText(log_page.this,"Please Check Your Internet Connection",Toast.LENGTH_SHORT).show();
                }

            }
        });
    }
    class connectDB extends Thread {
       /* String update_string = "UPDATE users SET message ='online' WHERE id='";*/
        String query_string = "SELECT * FROM users WHERE id ='";
        EditText id_set = (EditText)findViewById(R.id.editText);
        @Override
        public void run() {
            super.run();
            try {
               /* account =  update_string + id_set.getText().toString()+"'";
                result = httppost.sqlpost(account);*/
                account =  query_string + id_set.getText().toString()+"'";
                result = httppost.sqlpost(account);
                JSONArray jsonArray = new JSONArray(result);
                for(int i = 0; i < jsonArray.length(); i++) {
                    JSONObject jsonData = jsonArray.getJSONObject(i);
                    password = jsonData.getString("password");
                    message = jsonData.getString("message");
                    mode =  jsonData.getString("mode");
                }
               // teststring = jsonData.getString("id");
                //test3.setText("OKOK");
            } catch (Exception e) {
                teststring = e.toString();
            }
        }
    }
    private boolean isConnected(){
        ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = cm.getActiveNetworkInfo();
        if (networkInfo != null && networkInfo.isConnected()) {
            return true;
        }
        return false;
    }
}




