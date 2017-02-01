window.onload = init;
var socket = new WebSocket("ws://localhost:8080/IOTCloudApplication/actions");
socket.onopen = uiBoot;
socket.onmessage = onMessage;
socket.onclose = onClose;
function onMessage(event) {
    console.log("onMessage called::"+event.data);
    try{
    var device = JSON.parse(event.data);
    
    if ((device.action === "add") && !(device.displayName === "null")) {
        printDeviceElement(device);
    }
    
    if (device.action === "toggle") {
        var node = document.getElementById(device.id);
        var statusText = node.children[2];
        if (device.status === "1" || device.status === 1) {        //1=ON
            statusText.innerHTML = "Status: " + "ON" + " (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",0)>Turn off</a>)";
        } else if (device.status === "0" || device.status === 0) {     //0=OFF
            statusText.innerHTML = "Status: " + "OFF" + " (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",1)>Turn on</a>)";
        }
        
        else if(device.status === "null"){
            statusText.innerHTML = "Status: " + device.status + " (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",1)>Update</a>)";
        }
        else{
            statusText.innerHTML = "Status: " + device.status + "&deg C (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",1)>Update</a>)";
        }
    }
    }
    catch(err){
        console.log("Exception: " + err);
    }
}
function toggleDevice(element,newStatus) {
    console.log("toggleDevice() called");
    var id = element;
    var status = newStatus;     
    var toggleMsg = "cloudcommand#"+id+"#"+status+"#";
    //socket.send(JSON.stringify(DeviceAction));
    socket.send(toggleMsg);
    console.log("toogle command sent: "+toggleMsg);
    
    var node = document.getElementById(id);
        var statusText = node.children[2];
        if (status === "1" || status === 1) {        //1=ON
            statusText.innerHTML = "Status: " + "ON" + " (<a href=\"#\" OnClick=toggleDevice(" + id + ",0)>Turn off</a>)";
        } else if (status === "0" || status === 0) {     //0=OFF
            statusText.innerHTML = "Status: " + "OFF" + " (<a href=\"#\" OnClick=toggleDevice(" + id + ",1)>Turn on</a>)";
        }
        /*
        else if(device.status === "null"){
            statusText.innerHTML = "Status: " + device.status + " (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",1)>Update</a>)";
        }
        else{
            statusText.innerHTML = "Status: " + device.status + "&deg C (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",1)>Update</a>)";
        }*/
    
}
function printDeviceElement(device) {
    var content = document.getElementById("content");
    
    var deviceDiv = document.createElement("div");
    deviceDiv.setAttribute("id", device.id);
    deviceDiv.setAttribute("class", "device " + device.name.replace(".","_"));
    content.appendChild(deviceDiv);
    var deviceName = document.createElement("span");
    deviceName.setAttribute("class", "deviceName");
    var displayname = device.displayName;
    /*if(device.name==="LED_rgb.gnd_r"){
        displayname = "LED Red";        
    }
    else if(device.name==="LED_rgb.gnd_g"){
        displayname = "LED Green";        
    }
    else if(device.name==="LED_rgb.gnd_b"){
        displayname = "LED Blue";        
    }
    else if(device.name==="temp.sense"){
        displayname = "Temperature Sensor";        
    }*/
    deviceName.innerHTML = displayname;
    deviceDiv.appendChild(deviceName);
    var deviceType = document.createElement("span");
    //deviceType.innerHTML = "<b>Type:</b> " + device.type;
    deviceDiv.appendChild(deviceType);
    var deviceStatus = document.createElement("span");
    if (device.status === "1" || device.status === 1) {    //1=ON
        deviceStatus.innerHTML = "<b>Status:</b> " + "ON" + " (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",0)>Turn off</a>)";
    } else if (device.status === "0" || device.status === 0) {     //0=OFF
        deviceStatus.innerHTML = "<b>Status:</b> " + "OFF" + " (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",1)>Turn on</a>)";
        //deviceDiv.setAttribute("class", "device off");
    }
    else if (device.status === "null"){
        deviceStatus.innerHTML = "<b>Status:</b> " + device.status + " (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",1)>Update</a>)";
       
    }
    else{
        deviceStatus.innerHTML = "<b>Status:</b> " + device.status + "&deg C (<a href=\"#\" OnClick=toggleDevice(" + device.id + ",1)>Update</a>)";
       
    }
    deviceDiv.appendChild(deviceStatus);
    
}
function init() {
    //hideForm();
    console.log("init called");
    //socket.send("bootui#");
}
function uiBoot() {
    //hideForm();
    console.log("uiBoot called");
    socket.send("bootui#");
}
function onClose() {
    console.log("onClose called");
}