package droid.fr;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends Activity implements DataDisplay
{
    /** Called when the activity is first created. */
    TextView serverMessage ;
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        serverMessage= (TextView) findViewById(R.id.textView1);
    }
    public void connect(View view)
    {
        MyServer server= new MyServer();
        server.setEventListener(this);
        server.startListening();
    }
    public void Display(String Message)
    {
        serverMessage.setText(""+Message);
    }
}
