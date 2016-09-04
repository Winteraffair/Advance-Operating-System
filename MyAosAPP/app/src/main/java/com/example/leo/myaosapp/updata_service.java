package com.example.leo.myaosapp;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Created by Leo on 2015/12/26.
 */
public class updata_service extends Service{
    Handler handler = new Handler();
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        handler.removeCallbacks(showTime);
        super.onDestroy();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        handler.post(showTime);

        return super.onStartCommand(intent, flags, startId);
    }

    public Runnable showTime = new Runnable() {
        /*String update ="UPDATE users SET time =";
        String query = "SELECT * FROM users WHERE id ='";
        String search="",result_updata="";*/

        @Override
        public void run() {
            time_update tu = new time_update();
            tu.start();
            handler.postDelayed(this, 20000);
        }
    };
    private boolean isConnected(){
        ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = cm.getActiveNetworkInfo();
        if (networkInfo != null && networkInfo.isConnected()) {
            return true;
        }
        return false;
    }

    class time_update extends Thread {
        String update_string = "UPDATE users SET time =";
        String query_string = "SELECT * FROM users WHERE id ='";
        String search_string,result;
        Global_variable update_time = (Global_variable) getApplicationContext();
        @Override
        public void run() {
            super.run();
            try {
                if(isConnected()) {
                    update_time.check_time = (int) (System.currentTimeMillis() / 1000);
                    search_string = update_string + update_time.check_time + " WHERE id = '" + update_time.global_account + "'";
                    result = httppost.sqlpost(search_string);
                    search_string = query_string + update_time.global_account + "'";
                    result = httppost.sqlpost(search_string);
                    JSONArray jsonArray = new JSONArray(result);
                    update_time.check_time = 0;
                    for (int i = 0; i < jsonArray.length(); i++) {
                        JSONObject jsonData = jsonArray.getJSONObject(i);
                        update_time.check_time = jsonData.getInt("time");
                        update_time.check_all=jsonData.getInt("checkall");
                        update_time.check_you=jsonData.getInt("checkyou");
                        update_time.check_all_msg = jsonData.getString("checkallmsg");
                        update_time.check_you_msg = jsonData.getString("checkyoumsg");
                    }
                }else{
                    final int notifyID = 1; // 通知的識別號碼
                    final NotificationManager notificationManager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE); // 取得系統的通知服務
                    final Notification notification = new Notification.Builder(getApplicationContext()).setSmallIcon(R.drawable.boss).setContentTitle("Boss").setContentText("Check Your Internet Connection Please").build(); // 建立通知
                    notificationManager.notify(notifyID, notification); // 發送通知
                }
            } catch (Exception e) {
                update_time.check_time = 100;
            }
        }
    }
}
