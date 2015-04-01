package test.co;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.TextView;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class MainActivity extends Activity
{
    /** Called when the activity is first created. */
    
    TextView clientMessage;
    TextView serverMessage;
    Thread m_objThreadClient;
    Socket clientSocket;
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        clientMessage=(TextView) findViewById(R.id.textView2);
        serverMessage=(TextView) findViewById(R.id.textView1);
    }
    public void Start(View view)
    {
        m_objThreadClient= new Thread(new Runnable(){
            
           public void run()
           { 
               try
               {
                   clientSocket= new Socket("127.0.0.1",8888);
                   ObjectOutputStream oos = new ObjectOutputStream(clientSocket.getOutputStream());
                   oos.writeObject("Bastien suce des types");
                   Message serverMessage= Message.obtain();
                   ObjectInputStream ois= new ObjectInputStream(clientSocket.getInputStream());
                   //String strMessage = (String)ois.readObject();
                   //serverMessage.obj=strMessage;
                   
                   
                   
                   
                   mHandler.sendMessage(serverMessage);
                   
                   oos.close();
                   ois.close();
                   
                   
               }
               catch (Exception e)
               {
                   e.printStackTrace();
               }
           }
         
           
           
        });
        
        m_objThreadClient.start();
    }
  Handler mHandler = new Handler(){
      
   public void handleMessage(Message msg){
       
       messageDisplay(msg.obj.toString());  
    }

    
    };
  public void messageDisplay(String servermessage)
  {
      serverMessage.setText(""+servermessage);
  }
}
