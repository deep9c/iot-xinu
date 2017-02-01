/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author crystalonix
 */
public class Device {

    private int uiDivId;            //TODO ANIK
    private String devPinId;        //TODO ANIK
    private String displayName;     //TODO ANIK 2
    private String status;
    private String description;

    public Device() {
    }
     public Device(int id, String devPinId) {
        this.uiDivId = id;
        this.devPinId = devPinId;
        this.status = "null";
        this.displayName = "null";
        this.description = null;
    }

public Device(int id, String devPinId, String displayName) {
        this.uiDivId = id;
        this.devPinId = devPinId;
        this.displayName = displayName;
        this.status = "null";
        this.description = null;
    }
 
    public int getUiDivId() {           //TODO ANIK
        return uiDivId;
    }

    public String getDevPinId() {       //TODO ANIK
        return devPinId;
    }
    
    public String getDisplayName() {       //TODO ANIK 2
        return displayName;
    }

    public String getStatus() {
        return status;
    }

    public String getDescription() {
        return description;
    }

    public void setUiDivId(int uiDivId) {           //TODO ANIK
        this.uiDivId = uiDivId;
    }

    public void setDevPinId(String devPinId) {      //TODO ANIK
        this.devPinId = devPinId;
    }
    
    public void setDisplayName(String displayName) {      //TODO ANIK 2
        this.displayName = displayName;
    }

    public void setStatus(String status) {
        this.status = status;
    }


    public void setDescription(String description) {
        this.description = description;
    }
}