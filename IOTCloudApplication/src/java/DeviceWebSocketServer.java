import java.io.IOException;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import javax.enterprise.context.ApplicationScoped;
import javax.inject.Inject;
import javax.websocket.OnClose;
import javax.websocket.OnError;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;
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
@ServerEndpoint("/actions")
public class DeviceWebSocketServer {
    @Inject
    private DeviceSessionHandler sessionHandler;
    Map<String, Session> edges = new HashMap<>();
    private static final String DELIM = "#";
    private static final String REGISTER = "register";
    // map of bbId#pin_name: edge_id
    private static Map<String, String> devices = new HashMap<>();
    public static Map<String, devValues[]> assocs = new HashMap<>();
    private static final String REGISTERUI = "bootui";              // TODO ANIK
    private static final String CLOUDCOMMAND = "cloudcommand";      // TODO ANIK
    private static final String TEMPSENSOR = "temp.sense";      // TODO ANIK
    private static Set<Device> devicesSet = new HashSet<>();        //TODO ANIK
    private static Map<String, String> uiDivMap = new HashMap<>();  //TODO ANIK
    static {
        devices.put("LED_rgb.gnd_r", "0");
        devices.put("LED_rgb.gnd_g", "0");
        devices.put("LED_rgb.gnd_b", "0");
        devices.put("LED_rgb.gnd_r1", "0");        
        devices.put("button.sesnse", "0");
        devices.put("temp.sense", "0");
        System.out.println("CloudDDL parser to be called...");
        CloudDDLParser.parseCloud();
        //String[][] buttonAssoc  = {{0},{}};
        //assocs.put("button.sense", new devValues[]{new devValues("0", 1, new String[]{"LED_rgb.gnd_r#0"}),new devValues("1", 1, new String[]{"LED_rgb.gnd_r#1"})});
        //assocs.put("LED_rgb.gnd_r", new devValues[]{new devValues("0", 1, new String[]{"LED_rgb.gnd_r#0"}),new devValues("1", 1, new String[]{"LED_rgb.gnd_r#1"})});
        //assocs.put("temp.sense", new devValues[]{new devValues("0", 1, new String[]{"temp.sense#1"})});
        devicesSet.add(new Device(0, "LED_rgb.gnd_r", "LED Red1"));      //display name
        devicesSet.add(new Device(1, "LED_rgb.gnd_g", "LED Green"));
        devicesSet.add(new Device(2, "LED_rgb.gnd_b", "LED Blue"));
        devicesSet.add(new Device(3, "LED_rgb.gnd_r1", "LED Red2"));
        devicesSet.add(new Device(4, "button.sense", "null"));
        devicesSet.add(new Device(5, "temp.sense", "Temperature Sensor"));
        uiDivMap.put("0", "LED_rgb.gnd_r");     //TODO ANIK
        uiDivMap.put("1", "LED_rgb.gnd_g");     //TODO ANIK
        uiDivMap.put("2", "LED_rgb.gnd_b");     //TODO ANIK
        uiDivMap.put("3", "LED_rgb.gnd_r1");     //TODO ANIK
        uiDivMap.put("4", "button.sense");     //TODO ANIK            
        uiDivMap.put("5", "temp.sense");     //TODO ANIK            
    }
    private static String INVALID = "-1";
    @OnOpen
    public void open(Session session) {
        sessionHandler.addSession(session);
    }
    @OnClose
    public void close(Session session) {
        sessionHandler.removeSession(session);
    }
    @OnError
    public void onError(Throwable error) {
    }
    @OnMessage
    public void handleMessage(String message, Session session) throws IOException {
        try {
            System.out.println("DeviceWebSocketServer.handleMessage()::" + message);
            // register message
            if (((message.split(DELIM))[0]).equals(REGISTER)) {
                System.out.println("This person has registered:" + (message.split(DELIM))[1]);
                sessionHandler.checkSession(Integer.parseInt((message.split(DELIM))[1]), session);
                //sessionHandler.addSession(Integer.parseInt((message.split(DELIM))[1]), session);
                if (((message.split(DELIM))[1]).equals("1") || ((message.split(DELIM))[1]).equals("0")) {
                    System.out.println("Deep/Anik has registered");
                    sessionHandler.sendMessage(0, message);
                }
            } else if (((message.split(DELIM))[0]).equals(REGISTERUI)) {  // TODO ANIK      boot command from UI (bootui#)
                System.out.println("bootui received");
                sessionHandler.addUISession(session, devicesSet);
            } else if (((message.split(DELIM))[0]).equals(CLOUDCOMMAND)) {    // TODO ANIK       //cloudcommand#uidivid#data (eg:- cloudcommand#1#0#)
                String uiDivId = (message.split(DELIM))[1];     //int ID of LEDs/Temperature sensors
                String DevPinId = uiDivMap.get(uiDivId);        //eg:LED_rgb.gnd_r
                
                //String msg = trimMessage(message);
                String[] dataElems = message.split(DELIM);  
                dataElems[1] = DevPinId;
                System.out.println("CLOUDCOMMAND DevPinId= " + DevPinId);
                //sessionHandler.toggleDeviceByDevPinID(DevPinId,devicesSet,(message.split(DELIM))[2]);
                dataElems[0]= "-1";
                message=joinElems(dataElems, DELIM);
                //TODO: remove this hardcoded edgeid
                sessionHandler.sendMessage(0, message);
                /*if (assocs.containsKey(DevPinId)) {
                    System.out.println("CLOUDCOMMAND about to invoke the associated devices");
                    invokeAssociatedDevices(dataElems);
                }*/
                
                /*if (DevPinId.equals(TEMPSENSOR)) {
                    //TODO : Call Edge with DevPinId data
                } else {
                    System.out.println("to be called ToggleDevice : " + uiDivId + ":" + DevPinId);
                    //sessionHandler.toggleDevice(Integer.parseInt(uiDivId),devicesSet);      //ID of integer uiDivId
                    sessionHandler.toggleDeviceByDevPinID(DevPinId, devicesSet, (message.split(DELIM))[2]);        //eg s = LED_rgb.gnd_r
                }*/
            }// data message
            else {
                //String msg = trimMessage(message);
                System.out.println("data msg received: "+ message +" .... ");
                String[] dataElems = message.split(DELIM);   
                //TODO: uncomment
                sessionHandler.toggleDeviceByDevPinID(dataElems[1],devicesSet,dataElems[2]);
                if (assocs.containsKey(dataElems[1])) {
                    System.out.println("about to invoke the associated devices");
                    invokeAssociatedDevices(dataElems);
                }
            }
        } catch (Exception e) {
            System.out.println("exception came:" + e.getMessage());
        }
//        sessionHandler.sendMessage(session, message);
        //session.getBasicRemote().sendText(message);
    }
    /**
     * This method will invoke the related devices
     *
     * @param dataElems
     */
    public void invokeAssociatedDevices(String[] dataElems) {
        int i = 0;
        boolean flag = false;
        System.out.println("came here 100 " +dataElems[2]);
        Double db = Double.parseDouble(dataElems[2]);
        System.out.println("came here 150 "+dataElems[1]);
        devValues[] arr = assocs.get(dataElems[1]);
        System.out.println("came here 175 "+arr);
        int jj = 0;
        for (; jj < arr.length; jj++) {
            if(arr[jj]==null){
                continue;
            }
            
            System.out.println("came here 200 "+db +" "+arr[i].value);
            if (db.compareTo(Double.parseDouble(arr[i].value)) < 0) {
                System.out.println("came here 300");
                break;
            } else {
                System.out.println("came here 400");
                flag = true;
            }
            i++;
        }
        i--;
        System.out.println("see the value of i"+i);
        if (i >= 0 && flag) {
            dataElems[0] = INVALID;
            System.out.println("came here 450");
            for (int j = 0; j < arr[i].devicesData.length; j++) {                
                System.out.println("this is ok 500");
                dataElems[1] = arr[i].devicesData[j];
                String msg = joinElems(dataElems, DELIM);
                System.out.println("About to send message:" + msg +" ... " +"''" + dataElems[1] + devices.get(dataElems[1].split(DELIM)[0]));
                sessionHandler.sendMessage(Integer.parseInt(devices.get(dataElems[1].split(DELIM)[0])), msg);
                sessionHandler.toggleDeviceByDevPinID(dataElems[1].split(DELIM)[0], devicesSet, dataElems[1].split(DELIM)[1]);
            }
        }
    }
    /**
     * This method will trim the trailing spaces in the message
     *
     * @param message
     */
    /*public static String trimMessage(String message) {
        StringBuilder bldr = new StringBuilder();
        String[] strs = message.split(DELIM);
        for (int i = 0; i < (strs.length - 1); i++) {
            bldr.append(strs[i]).append(DELIM);
        }
        return bldr.append("#").toString();
    }*/
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
            bldr.append(dataElems[i]).append(DELIM);
        }
        bldr.append(dataElems[dataElems.length - 1]).append(DELIM);
        return bldr.toString();
    }
}
 // this class contains the device values to be sent based on a particular device update 
      class devValues {
        String value = null;
        String[] devicesData;
        public devValues(String val, int size, String[] devices) {
            this.value = val;
            devicesData = new String[size];
            for (int i = 0; i < size; i++) {
                devicesData[i] = devices[i];
            }
        }
    }