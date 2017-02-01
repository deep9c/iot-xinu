
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import javax.inject.Inject;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 * This class listens to all the incoming requests from the device sockets
 *
 * @author crystalonix
 */
public class DeviceListenerThread extends Thread{

    /**
     * This is the single instance which is responsible for reading from/ writing to sockets
     */
    public DeviceHandler deviceHandler = new DeviceHandler();
    public static int BUF_LEN = 65536;
    @Override
    public void run() {
        //1. creating a server socket, parameter is local port number
        try  {
            DatagramSocket sock = TestApp.EDGE_SOCKET;
            //buffer to receive incoming data
            byte[] buffer = new byte[BUF_LEN];
            DatagramPacket incoming = new DatagramPacket(buffer, buffer.length);
            deviceHandler.dummyHnadler();
            System.out.println("Exception here 2");
            //communication loop
            while (true) {
                System.out.println("Exception here 23");
                sock.receive(incoming);		//waiting for xinu
                System.out.println("Exception here 3");
                byte[] data = incoming.getData();
                String s = new String(data, 0, incoming.getLength());

                DatagramPacket dp = new DatagramPacket(s.getBytes(), s.getBytes().length, incoming.getAddress(), incoming.getPort());
                // do something with the incoming data packet 
                deviceHandler.handleData(incoming, sock);  
                System.out.println("Exception here 4");
            }

        } catch (IOException e) {
            System.err.println("IOException " + e);
        }
    }

    void sendToDevice(int i, String message) {
        System.out.println("bb_id:"+i+"The message to be sent is:"+message);
        deviceHandler.sendToDevice(i, message);
    }
}
