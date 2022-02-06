package com.example.flowerwateringbot;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;

import android.text.Editable;
import android.text.TextWatcher;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private  String DEVICE_ADDRESS; //MAC Address of Bluetooth Module
    private final UUID PORT_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");

    private BluetoothDevice device;
    private BluetoothSocket socket;
    private OutputStream outputStream;

    Button forward_btn, left_btn, right_btn, backward_btn, bluetooth_connect_btn,pumpoff_btn, pumpon_btn, auto_btn, stop_btn,settings_btn,send_btn,close_btn;
    EditText mac_text, Rwt_text, Gwt_text, Bwt_text, Rwp_text, Gwp_text, Bwp_text;

    RelativeLayout control_layout,control2_layout,control3_layout;

    String command; //string variable that will store value to be transmitted to the bluetooth module

    @SuppressLint("ClickableViewAccessibility")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //declaration of button variables
        forward_btn = (Button) findViewById(R.id.forward_btn);
        left_btn = (Button) findViewById(R.id.left_btn);
        right_btn = (Button) findViewById(R.id.right_btn);
        backward_btn = (Button) findViewById(R.id.backward_btn);

        control_layout=(RelativeLayout)findViewById(R.id.control_layout);

        pumpoff_btn = (Button) findViewById(R.id.pumpoff_btn);
        pumpon_btn = (Button) findViewById(R.id.pumpon_btn);
        auto_btn = (Button) findViewById(R.id.auto_btn);
        stop_btn = (Button) findViewById(R.id.stop_btn);

        control2_layout=(RelativeLayout)findViewById(R.id.control2_layout);

        settings_btn=(Button)findViewById(R.id.settings_btn);
        send_btn=(Button)findViewById(R.id.send_btn);
        close_btn=(Button)findViewById(R.id.close_btn);

        control3_layout=(RelativeLayout) findViewById(R.id.control3_layout);

        Rwt_text=(EditText)findViewById(R.id.Rwt_txt);
        Gwt_text=(EditText)findViewById(R.id.Gwt_txt);
        Bwt_text=(EditText)findViewById(R.id.Bwt_txt);
        Rwp_text=(EditText)findViewById(R.id.Rwp_txt);
        Gwp_text=(EditText)findViewById(R.id.Gwp_txt);
        Bwp_text=(EditText)findViewById(R.id.Bwp_txt);


        bluetooth_connect_btn = (Button) findViewById(R.id.bluetooth_connect_btn);
        mac_text=(EditText)findViewById(R.id.mac_editText);
        DEVICE_ADDRESS=mac_text.getText().toString();



        settings_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                settings_btn.setEnabled(false);
                settings_btn.setVisibility(View.INVISIBLE);

                control3_layout.setVisibility(View.VISIBLE);
                send_btn.setEnabled(true);
                close_btn.setEnabled(true);
            }
        });

        close_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                send_btn.setEnabled(false);
                close_btn.setEnabled(false);
                control3_layout.setVisibility(View.INVISIBLE);

                settings_btn.setEnabled(true);
                settings_btn.setVisibility(View.VISIBLE);
            }
        });

        send_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                send_btn.setEnabled(false);
                close_btn.setEnabled(false);
                control3_layout.setVisibility(View.INVISIBLE);

                settings_btn.setEnabled(true);
                settings_btn.setVisibility(View.VISIBLE);

                command="";
                command+="DATA ";
                command+=Math.round(Double.parseDouble(Rwt_text.getText().toString())*1000)+" ";
                command+=Math.round(Double.parseDouble(Rwp_text.getText().toString())*3600000)+" ";
                command+=Math.round(Double.parseDouble(Gwt_text.getText().toString())*1000)+" ";
                command+=Math.round(Double.parseDouble(Gwp_text.getText().toString())*3600000)+" ";
                command+=Math.round(Double.parseDouble(Bwt_text.getText().toString())*1000)+" ";
                command+=Math.round(Double.parseDouble(Bwp_text.getText().toString())*3600000)+" ";




                try
                {
                    outputStream.write(command.getBytes());
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
            }
        });

        mac_text.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {
                DEVICE_ADDRESS=mac_text.getText().toString();
            }

            @Override
            public void afterTextChanged(Editable editable) {

            }
        });


        pumpon_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                command = "ON";

                try
                {
                    outputStream.write(command.getBytes()); //transmits the value of command to the bluetooth module
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
            }
        });

        pumpoff_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                command = "OFF";

                try
                {
                    outputStream.write(command.getBytes()); //transmits the value of command to the bluetooth module
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
            }
        });

        auto_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                command = "AUTO";

                try
                {
                    outputStream.write(command.getBytes()); //transmits the value of command to the bluetooth module
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
                try {
                    Thread.sleep(250);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                command = "STOP";
                try
                {
                    outputStream.write(command.getBytes()); //transmits the value of command to the bluetooth module
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
            }
        });
        stop_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                command = "STOP";

                try
                {
                    outputStream.write(command.getBytes()); //transmits the value of command to the bluetooth module
                }
                catch (IOException e)
                {
                    e.printStackTrace();
                }
            }
        });

        //OnTouchListener code for the forward button (button long press)
        forward_btn.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if (event.getAction() == MotionEvent.ACTION_DOWN) //MotionEvent.ACTION_DOWN is when you hold a button down
                {
                    command = "FORWARD";

                    try
                    {
                        outputStream.write(command.getBytes()); //transmits the value of command to the bluetooth module
                    }
                    catch (IOException e)
                    {
                        e.printStackTrace();
                    }
                }
                else if(event.getAction() == MotionEvent.ACTION_UP) //MotionEvent.ACTION_UP is when you release a button
                {
                    command = "STOP";
                    try
                    {
                        outputStream.write(command.getBytes());
                    }
                    catch(IOException e)
                    {
                        e.printStackTrace();
                    }

                }

                return false;
            }

        });

        //OnTouchListener code for the reverse button (button long press)
        backward_btn.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                if(event.getAction() == MotionEvent.ACTION_DOWN)
                {
                    command = "BACKWARD";

                    try
                    {
                        outputStream.write(command.getBytes());
                    }
                    catch (IOException e)
                    {
                        e.printStackTrace();
                    }
                }
                else if(event.getAction() == MotionEvent.ACTION_UP)
                {
                    command = "STOP";
                    try
                    {
                        outputStream.write(command.getBytes());
                    }
                    catch(IOException e)
                    {

                    }

                }
                return false;
            }
        });

        //OnTouchListener code for the forward left button (button long press)
        left_btn.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                if(event.getAction() == MotionEvent.ACTION_DOWN)
                {
                    command = "LEFT";

                    try
                    {
                        outputStream.write(command.getBytes());
                    }
                    catch (IOException e)
                    {
                        e.printStackTrace();
                    }
                }
                else if(event.getAction() == MotionEvent.ACTION_UP)
                {
                    command = "STOP";
                    try
                    {
                        outputStream.write(command.getBytes());
                    }
                    catch(IOException e)
                    {

                    }

                }
                return false;
            }
        });

        //OnTouchListener code for the forward right button (button long press)
        right_btn.setOnTouchListener(new View.OnTouchListener(){
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                if(event.getAction() == MotionEvent.ACTION_DOWN)
                {
                    command = "RIGHT";

                    try
                    {
                        outputStream.write(command.getBytes());
                    }
                    catch (IOException e)
                    {
                        e.printStackTrace();
                    }
                }
                else if(event.getAction() == MotionEvent.ACTION_UP)
                {
                    command = "STOP";
                    try
                    {
                        outputStream.write(command.getBytes());
                    }
                    catch(IOException e)
                    {
                        e.printStackTrace();
                    }

                }
                return false;
            }
        });

        //Button that connects the device to the bluetooth module when pressed
        bluetooth_connect_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if(BTinit())
                {
                    if( BTconnect()){
                        forward_btn.setEnabled(true);
                        backward_btn.setEnabled(true);
                        left_btn.setEnabled(true);
                        right_btn.setEnabled(true);

                        control_layout.setVisibility(View.VISIBLE);

                        pumpoff_btn.setEnabled(true);
                        pumpon_btn.setEnabled(true);
                        stop_btn.setEnabled(true);
                        auto_btn.setEnabled(true);

                        control2_layout.setVisibility(View.VISIBLE);

                        settings_btn.setEnabled(true);
                        settings_btn.setVisibility(View.VISIBLE);


                        mac_text.setEnabled(false);
                        bluetooth_connect_btn.setEnabled(false);
                        mac_text.setVisibility(View.GONE);
                        bluetooth_connect_btn.setVisibility(View.GONE);
                    }
                }else {

                }

            }
        });

    }

    //Initializes bluetooth module
    public boolean BTinit()
    {
        boolean found = false;

        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if(bluetoothAdapter == null) //Checks if the device supports bluetooth
        {
            Toast.makeText(getApplicationContext(), "Device doesn't support bluetooth", Toast.LENGTH_SHORT).show();
        }

        if(!bluetoothAdapter.isEnabled()) //Checks if bluetooth is enabled. If not, the program will ask permission from the user to enable it
        {
            Intent enableAdapter = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableAdapter,0);

            try
            {
                Thread.sleep(1000);
            }
            catch(InterruptedException e)
            {
                e.printStackTrace();
            }
        }

        Set<BluetoothDevice> bondedDevices = bluetoothAdapter.getBondedDevices();

        if(bondedDevices.isEmpty()) //Checks for paired bluetooth devices
        {
            Toast.makeText(getApplicationContext(), "Please pair the device first", Toast.LENGTH_SHORT).show();
        }
        else
        {
            for(BluetoothDevice iterator : bondedDevices)
            {
                if(iterator.getAddress().equals(DEVICE_ADDRESS))
                {
                    device = iterator;
                    found = true;
                    break;
                }
            }
        }

        return found;
    }

    public boolean BTconnect()
    {
        boolean connected = true;

        try
        {
            socket = device.createRfcommSocketToServiceRecord(PORT_UUID); //Creates a socket to handle the outgoing connection
            socket.connect();

            Toast.makeText(getApplicationContext(),
                    "Connection to bluetooth device successful", Toast.LENGTH_LONG).show();
        }
        catch(IOException e)
        {
            e.printStackTrace();
            connected = false;
            Toast.makeText(getApplicationContext(),
                    "Connection to Bluetooth device failed", Toast.LENGTH_LONG).show();
        }

        if(connected)
        {
            try
            {
                outputStream = socket.getOutputStream(); //gets the output stream of the socket
            }
            catch(IOException e)
            {
                e.printStackTrace();
            }
        }

        return connected;
    }

    @Override
    protected void onStart()
    {
        super.onStart();
    }
}