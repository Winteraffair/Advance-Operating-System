package com.example.leo.myaosapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class StartPage extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_start_page);

        Button enterbutton = (Button) findViewById(R.id.button);

        enterbutton.setOnClickListener(new Button.OnClickListener(){
            public void onClick(View v){
                startActivity(new Intent(StartPage.this,log_page.class));
                StartPage.this.finish();
            }
        });
    }
}
