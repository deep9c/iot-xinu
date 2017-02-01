
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.enterprise.context.ApplicationScoped;
import javax.json.JsonObject;
import javax.json.spi.JsonProvider;
import javax.websocket.Session;

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
public class DeviceSessionHandler {

    private final Map<Integer, Session> sessions = new HashMap<>();
    private final Set<Session> ss = new HashSet<>();
    //private final Set<Device> devices = new HashSet<>();
    private final Set<Session> uiSessions = new HashSet<>();        //TODO ANIK

    public void addSession(Integer edgeId, Session session) {
        System.out.println("edge device getting added" + edgeId);
        sessions.put(edgeId, session);
    }
//TODO: removing session 

    public void removeSession(Session session) {
        ss.remove(session);
    }
    

    void sendMessage(Integer edgeId, String msg) {
        try {
            if (sessions.get(edgeId) == null) {
                System.out.println("edgeId null");
            } else if (sessions.get(edgeId).getBasicRemote() == null) {
                System.out.println("edge not null");
            }
            sessions.get(edgeId).getBasicRemote().sendText(msg);
        } catch (IOException ex) {
            Logger.getLogger(DeviceSessionHandler.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    void sendMessage(Session session, String msg) {
        try {
            session.getBasicRemote().sendText(msg);
        } catch (IOException ex) {
            Logger.getLogger(DeviceSessionHandler.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    void addSession(Session session) {
        ss.add(session);
        //int i = 0;
    }

    void sendAllMessage(String message) {
        for (Session s : ss) {
            sendMessage(s, message);
        }
    }

    void checkSession(int key, Session session) {
        /*for (Session s : ss) {
            if (s == session) {
                System.out.println("Eureka");
                sessions.put(key, session);
            }

        }*/
        sessions.put(key, session);
    }
// ********************************************** TODO ANIK starts *********************************************************
   
    private void sendToSession(Session session, JsonObject message) {           // TODO ANIK
        try {
            System.out.println("sendToSession called");
            session.getBasicRemote().sendText(message.toString());
        } catch (IOException ex) {
            uiSessions.remove(session);
            System.out.println("sendToSession failed");
            //edges.remove(session);
            Logger.getLogger(DeviceSessionHandler.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private JsonObject createAddMessage(Device device) {    //todo anik
        System.out.println(" session handler createAddMessage called");
        JsonProvider provider = JsonProvider.provider();
        JsonObject addMessage = provider.createObjectBuilder()
                .add("action", "add")
                .add("id", device.getUiDivId())
                .add("name", device.getDevPinId())
                .add("status", device.getStatus())
                .add("displayName", device.getDisplayName())          //TODO ANIK 2
                //.add("description", device.getDescription())
                .build();
        System.out.println("DeviceUIDivID= " + device.getUiDivId());
        return addMessage;
    }
    
    
    
    public void addUISession(Session session, Set<Device> devices) {     //TODO ANIK
        System.out.println(" session handler addUISession called");
        uiSessions.add(session);
        //edges.put( session, Integer.toString(sessionId));
        //sessionId++;
        for (Device device : devices) {
            JsonObject addMessage = createAddMessage(device);
            sendToSession(session, addMessage);
        }
    }
    
    
    public void toggleDeviceByDevPinID(String devPinID, Set<Device> devices, String newStatus) {        //TODO ANIK
        System.out.println(" session handler toggleDeviceByDevPinID called");
        JsonProvider provider = JsonProvider.provider();
        Device device = getDeviceByDevPinID(devPinID, devices);
        
        if (device != null) {            
            System.out.println("Got device! devPinId="+devPinID+" Status= " + device.getStatus()+"uiDivId= "+device.getUiDivId());
            device.setStatus(newStatus);
            /*if(!newStatus.equals("")){          //  On/Off/tempvalue
                device.setStatus(newStatus);
            }
            
            else if ("On".equals(device.getStatus()) ) {
                device.setStatus("Off");
            } else if("Off".equals(device.getStatus())) {
                device.setStatus("On");
            }*/
            
            System.out.println("UIToggleJSON to be called! devPinId="+devPinID+" Status= " + device.getStatus()+"uiDivId= "+device.getUiDivId());
            JsonObject updateDevMessage = provider.createObjectBuilder()
                    .add("action", "toggle")
                    .add("id", device.getUiDivId())
                    .add("status", device.getStatus())
                    .build();
            sendToAllConnectedSessions(updateDevMessage);
        }
    }
    
    
    private Device getDeviceByUIDivId(int uiDivId, Set<Device> devices) {       //TODO ANIK
        for (Device device : devices) {
            if (device.getUiDivId()== uiDivId) {
                return device;
            }
        }
        return null;
    }
    
    private Device getDeviceByDevPinID(String devPinID, Set<Device> devices) {       //TODO ANIK
        for (Device device : devices) {
            if (device.getDevPinId().equals(devPinID)) {
                return device;
            }
        }
        return null;
    }
    
    private void sendToAllConnectedSessions(JsonObject message) {           //TODO ANIK
        System.out.println("sendToAllConnectedSessions called");
        for (Session session : uiSessions) {
            sendToSession(session, message);
               System.out.println("sent to session");
        }
    }
    
    // ************************************** TODO ANIK ends *****************
}
