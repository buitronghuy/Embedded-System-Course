import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.concurrent.Delayed;

import com.fazecast.jSerialComm.*;

public class Main {
	static SerialPort chosenPort;

	public static void main(String[] args) {

		JFrame frame = new JFrame("Piano");
		frame.setSize(500, 100);

		// create a drop-down box, connect button and play piano button, then place them at the top of the window
		JComboBox<String> portList = new JComboBox<String>();
		JButton connectButton = new JButton("Connect");
		JButton pianoButton = new JButton("READY!!!");
		JPanel topPanel = new JPanel();
		topPanel.add(portList);
		topPanel.add(connectButton);
		topPanel.add(pianoButton);
		frame.add(topPanel, BorderLayout.NORTH);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setVisible(true);

		// populate the drop-down box
		SerialPort[] portNames = SerialPort.getCommPorts();
		for (int i = 0; i < portNames.length; i++)
			portList.addItem(portNames[i].getSystemPortName());

		// configure the connect button, play piano button and use another frame to send data from keyboard
		connectButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				if (connectButton.getText().equals("Connect")) {
					// attempt to connect to the serial port
					chosenPort = SerialPort.getCommPort(portList.getSelectedItem().toString());
					chosenPort.setComPortTimeouts(SerialPort.TIMEOUT_SCANNER, 0, 0);
					if (chosenPort.openPort()) {
						connectButton.setText("Disconnect");
						portList.setEnabled(false);

						// send data to COM port
						PrintWriter output = new PrintWriter(chosenPort.getOutputStream());
						
						// use another frame to play piano
						pianoButton.addActionListener(new ActionListener() {
							@Override
							public void actionPerformed(ActionEvent e) {
								if (pianoButton.getText().equals("READY!!!")) {
									pianoButton.setText("Close piano player");
																
									JFrame piano = new JFrame("PIANO PLAYER");
									piano.setSize(1000, 500);
									JPanel panel = new JPanel();
									piano.add(panel);
									//ImageIcon imageIcon = new ImageIcon("background.jpg");
									
									ImageIcon imageIcon = new ImageIcon(new ImageIcon("background.jpg").getImage().getScaledInstance(1000, 400, Image.SCALE_DEFAULT));
									
									JLabel label = new JLabel();
									label.setIcon(imageIcon);
									//label.setPreferredSize(new Dimension(5000, 1000));
									//label.setBounds(0, 0, 10, 5);
									piano.add(label);
									//piano.getContentPane().setBackground(Color.black);
									piano.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
									piano.setVisible(true);
									
									//listen to events from the keyboard
									piano.addKeyListener(new KeyListener() {
										@Override
										public void keyTyped(KeyEvent e) {
										}

										@Override
										public void keyPressed(KeyEvent e) {
											if (e.getKeyCode() > 64 && e.getKeyCode() < 91) {
												output.print(e.getKeyChar());
												output.flush();
											}
										}

										@Override
										public void keyReleased(KeyEvent e) {
											output.print(64);
											output.flush();
										}
									});
									
								} else {
									System.exit(0);
								}
							}
						});
					}
					
				} else {
					// disconnect from the serial port
					chosenPort.closePort();
					portList.setEnabled(true);
					System.exit(0);
				}
			}
		});
	}
}