package com.example.leo.myaosapp;

import android.app.Application;

/**
 * Created by Leo on 2015/12/24.
 */
public class Global_variable extends Application {
    public String global_account = "",check_all_msg="",check_you_msg="";
    public int check_time = 0 ,check_all=0,check_you=0;

    public void initial_variable(){
        global_account = "";
        check_all_msg="";
        check_you_msg="";
        check_time = 0;
        check_all=0;
        check_you=0;

    }
}
