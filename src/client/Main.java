// Java Program to create a simple JTextArea
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;

class Main extends JFrame implements ActionListener {

	// JFrame
	static JFrame f;
	static JTextArea jt;

	public static void main(String[] args)
	{

		String address = args[0];
		int port = Integer.parseInt(args[1]);
		String channel_id = args[2];

		f = new JFrame("textfield");
		Main te = new Main();
		jt = new JTextArea(20, 70);
		jt.setFont(new Font("MONOSPACED", Font.ITALIC, 14));
		JPanel p = new JPanel();
		p.add(jt);
		f.add(p);
		f.setSize(800, 800);
		f.show();

		
		final Stream.PC pc = new Stream.PC(address, port, channel_id);

		Thread t1 = new Thread(new Runnable() {
			@Override
			public void run()
			{
				try {
					pc.produce();
				}
				catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		});

		Thread t2 = new Thread(new Runnable() {
			@Override
			public void run()
			{
				try {
					pc.consume(jt);
				}
				catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		});

		t1.start();
		t2.start();

		try {
			t1.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		try {
			t2.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	// if the button is pressed
	public void actionPerformed(ActionEvent e)
	{
		String s = e.getActionCommand();

	}
}
