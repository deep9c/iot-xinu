
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.sql.Connection;
import java.util.HashMap;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.enterprise.context.ApplicationScoped;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author crystalonix
 */
@ApplicationScoped
class DeviceHandler {

    public static String ENCODING = "UTF8";
    public static String BOOT = "boot";
    private static Map<Integer, Conn> connections = new ConcurrentHashMap<>();
    private static int MAX_LENGTH = 1024;
    private String EDGE = "edge#";

    /**
     * This method handles any incoming data packet from a beagle bone device
     *
     * @param incoming
     * @param sock
     */
    void handleData(DatagramPacket incoming, DatagramSocket sock) {
        String data = null;
        try {
            data = new String(incoming.getData(), ENCODING);
            System.out.println("The data client sent is:"+ data);
        } catch (UnsupportedEncodingException ex) {
            Logger.getLogger("Unrecognized character encoding" + ENCODING);
        }
        String[] dataElems = data.split(TestApp.DELIM);
        //handle the boot packet        
        if (dataElems[0].equals(BOOT)) {
            connections.put(Integer.parseInt(dataElems[1]), new Conn(incoming.getAddress(), incoming.getPort()));
        } // handle the data packets
        else {
            dataElems[1] = TestApp.deviceMappings.get(dataElems[1]);
            String message = joinElems(dataElems, TestApp.DELIM);
            System.out.println("The data client sent is:"+ message);
            TestApp.clientEndPoint.sendMessage(message);
        }
    }

    /**
     * Static utility method which joins a set of strings separated by a
     * delimitor
     *
     * @param dataElems
     * @param delim
     * @return
     */
    public static String joinElems(String[] dataElems, String delim) {
        StringBuilder bldr = new StringBuilder();
        for (int i = 0; i < dataElems.length - 1; i++) {
            bldr.append(dataElems[i]).append(TestApp.DELIM);
        }
        bldr.append(dataElems[dataElems.length - 1]);
        return bldr.toString();
    }

    /**
     * This method sends message to a particular beagle-bone device
     *
     * @param bbId
     */
    public void sendToDevice(int bbId, String message) {
        Conn cn = connections.get(bbId);
        if (cn == null || cn.ip == null) {
            System.out.println("Could not find the connection");
            return;
        }
        System.out.println("See the port:" + cn.port + "ip:" + cn.ip.getHostName());
        //TODO:
        //String msg = trimMessage(message);
        message = EDGE + message;
        message = trimMessage(message);
        System.out.println("Final message we are sending is:"+ message);
        DatagramPacket dp = new DatagramPacket(message.getBytes(), message.getBytes().length, cn.ip, cn.port);
        try {
            TestApp.EDGE_SOCKET.send(dp);
        } catch (IOException ex) {
            Logger.getLogger(DeviceHandler.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    /**
     * This method will trim the trailing spaces in the message
     *
     * @param message
     */
    public static String trimMessage(String message) {
        StringBuilder bldr = new StringBuilder();
        String[] strs = message.split(TestApp.DELIM);
        // TODO:
        for (int i = 0; i < (strs.length); i++) {
            if (strs[i].length() < MAX_LENGTH) {
                System.out.println("strs are" + strs[i]);
                bldr.append(strs[i]).append(TestApp.DELIM);
            }
        }
        return bldr.append("#").toString();
    }

    // TODO
    void dummyHnadler() {
        connections.put(0, new Conn());
    }

    /**
     * This class stores the connection details related to a given beagle bone
     * socket
     */
    class Conn {

        // ip address of the connection
        InetAddress ip;
        // port number for the connection
        int port;

        public Conn() {
            ip = null;
            port = 0;
        }

        Conn(InetAddress ip, int port) {
            this.ip = ip;
            this.port = port;
        }
    }

}
