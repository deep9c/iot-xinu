/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author crystalonix
 */
public class ClientMessageHandler implements WebsocketClientEndpoint.MessageHandler {

    private static String INVALID = "-1";

    @Override
    public void handleMessage(String message) {
        String[] elems = message.split(TestApp.DELIM);
        if (TestApp.deviceMappingsReverse.containsKey(elems[1])) {
            if (elems[0].equals(INVALID)) {
                System.out.println("Invalid bbb id has come:"+ elems[0]+":"+elems[1]);
                elems[0] = TestApp.bbDevMap.get(elems[1]);
            }
            elems[1] = TestApp.deviceMappingsReverse.get(elems[1]);
            String msg = DeviceHandler.joinElems(elems, TestApp.DELIM);
            try {
                (new DeviceListenerThread()).sendToDevice(Integer.parseInt(elems[0]), msg);
            } catch (Exception e) {
                System.out.println("Incorrect format of the message");
            }
        } else {
            //TODO: handle this properly 
            (new DeviceListenerThread()).sendToDevice(0, message);
        }
    }
}
