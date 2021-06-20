import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;

import javax.swing.JTextArea;

public class Stream {

	public static class PC {

		LinkedList<String> list = new LinkedList<>();
		private Sclient client;		
		public int capacity = 40;
		public String channel_id;
		public PC(String address, int port, String channel_id) {
			super();
			this.client = new Sclient();
			this.channel_id = channel_id;
			client.startConnection(address, port);
			client.sendMessage(this.channel_id);
			System.out.printf("Connected to %s channel-%s", address, channel_id);
		}	
		
		public void produce() throws InterruptedException
		{
			int value = 0;
			while (true) {
				synchronized (this)
				{
			        ArrayList<String> msgs = new ArrayList<String>();
			        for (int i = 0; i < 14; i++) {
						msgs.add(client.recieveMessage());
					}
					
					while (list.size() == capacity)
						wait();

					list.add(String.join("\n", msgs));

					notify();

				}
			}
		}
		public void consume(JTextArea jt) throws InterruptedException
		{
			while (true) {
				synchronized (this)
				{
					while (list.size() == 0)
						wait();

					String msg = list.removeFirst();

					jt.setText(msg);
					notify();
					int wait_seconds = Integer.parseInt(String.valueOf(msg.charAt(0)))*50;
					Thread.sleep(wait_seconds);
				}
			}
		}
	}
}
