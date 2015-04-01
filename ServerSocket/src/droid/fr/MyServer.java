/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package droid.fr;

import android.os.Handler;
import android.os.Message;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;


/**
 *
 * @author clanco
 */
public class MyServer {
    
    Thread m_objThread;
    ServerSocket m_server;
    String m_strMessage;
    DataDisplay m_dataDisplay;
    Object m_connected;
    
    public MyServer()
    {
        
        
    }
    public void setEventListener(DataDisplay dataDisplay)
    {
        m_dataDisplay=dataDisplay;
    }
    public void startListening()
    {
        m_objThread = new Thread (new Runnable() 
        {
            public void run()
            {
                try {
        
                    m_server= new ServerSocket(2001);
                    Socket connectedSocket= m_server.accept();
                    Message clientmessage= Message.obtain();
                    
                    ObjectInputStream ois= new ObjectInputStream(connectedSocket.getInputStream());
                    String strMessage=(String)ois.readObject();
                    clientmessage.obj=strMessage;
                    mHandler.sendMessage(clientmessage);
                    
                    ObjectOutputStream oos= new ObjectOutputStream(connectedSocket.getOutputStream());
                    oos.writeObject("hello dear client");
                    ois.close();
                    oos.close();
                    m_server.close();
                    
                    } 
                catch (Exception e) 
                {
                    Message msg3= Message.obtain();
                    msg3.obj=e.getMessage();
                    mHandler.sendMessage(msg3);
                }
            }   
        });
        m_objThread.start();
    }  
    
    Handler mHandler = new Handler()
    {
        public void handleMessage(Message status){
            m_dataDisplay.Display(status.obj.toString());
        }
                
    };

}
