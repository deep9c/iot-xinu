import java.net.DatagramSocket;
import java.net.SocketException;
import java.net.URI;
import java.net.URISyntaxException;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author crystalonix
 */
public class TestApp {

    public static final String CLOUD_ENDPOINT = "ws://localhost:8080/IOTCloudApplication/actions";
    public static String EDGE_ID = "0";
    public static final String DELIM = "#";
    public static final String BOOT_ACTION = "register";
    public static WebsocketClientEndpoint clientEndPoint = null;
    public static Map<String, String> deviceMappings = new HashMap<>();
    public static Map<String, String> deviceMappingsReverse = new HashMap<>();
    public static int EDGE_LISTENING_PORT = 7790;
    public static DatagramSocket EDGE_SOCKET = null;
    public static Map<String, String> bbDevMap = new HashMap<>();

    static {
        //TODO: remove this hardcoding
        EdgeDDLParser.parseEdge();
        /*
        deviceMappings.put("0", "LED_rgb.gnd_r");
        deviceMappings.put("7", "button.sense");
        deviceMappingsReverse.put("LED_rgb.gnd_r", "0");
        deviceMappingsReverse.put("button.sense", "7");
        bbDevMap.put("LED_rgb.gnd_r", "0");
        bbDevMap.put("button.sense", "0");*/
        try {
            try {
                EDGE_SOCKET = new DatagramSocket(EDGE_LISTENING_PORT);
            } catch (SocketException ex) {
                System.out.println("Exception here 1");
                Logger.getLogger(TestApp.class.getName()).log(Level.SEVERE, null, ex);
            }
            clientEndPoint = new WebsocketClientEndpoint(new URI(CLOUD_ENDPOINT));
            clientEndPoint.addMessageHandler(new ClientMessageHandler());
        } catch (URISyntaxException ex) {
            Logger.getLogger("Error trying to open the cloud endpoint");
        } 
        // parse the ddl and populate the deviceMappings
    }

    public static void main(String[] args) {

        // send message to websocket
        clientEndPoint.registerToCloud(EDGE_ID);
        DeviceListenerThread dvcListenerThread = new DeviceListenerThread();
        dvcListenerThread.start();
        while (true) {
        }

    }
}
