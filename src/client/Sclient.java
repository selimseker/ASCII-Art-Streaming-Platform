import java.net.*;
import java.io.*;

public class Sclient {
	
    private Socket clientSocket;
    private PrintWriter out;
    private BufferedReader in;

    public void startConnection(String ip, int port) {
        try {
			clientSocket = new Socket(ip, port);
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
        try {
			out = new PrintWriter(clientSocket.getOutputStream(), true);
		} catch (IOException e) {
			e.printStackTrace();
		}
        try {
			in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		} catch (IOException e) {
			e.printStackTrace();
		}
    }

    public String sendMessage(String msg) {
        out.println(msg);
        String resp = null;
		try {
			resp = in.readLine();
		} catch (IOException e) {
			e.printStackTrace();
		}
        return resp;
    }
    
    public String recieveMessage() {
        String msg = null;
		try {
			msg = in.readLine();
		} catch (IOException e) {
			e.printStackTrace();
		}
        return msg;
    }
    
    

    public void stopConnection() {
        try {
			in.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
        out.close();
        try {
			clientSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
    }
}
