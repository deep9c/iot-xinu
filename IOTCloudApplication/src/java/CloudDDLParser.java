import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
* This class is used to parse the edge DDL file 
* 
* @author crystalonix
*
*/
public class CloudDDLParser {

	private static final String WRITE = "write";
	// device pin
	private static final String DEVICE_TYPE = "gpio_";
	private static final String INPUT_MODE = "in";
	private static final String OUTPUT_MODE = "out";
	private static final String READ = "read";
	// name of the driver file name to be generated
	private static final String FILE_NAME = "pio.c";
	// will hold the id of the edge
	private static final String ID = "edgeid";
	// input directive
	private static final String INCLUDE_INPUT = "#include <ddl_io_in.inc>";
	// output directive
	private static final String INCLUDE_OUTPUT = "#include <ddl_io_out.inc>";
	// this file is used as backbone to generate the driver file
	private static final String BACKBONE_FILE_NAME = "pio_backbone";
	
	

	public static void parseCloud() {
		try {
			File inputFile = new File("CloudDDL.xml");
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc = dBuilder.parse(inputFile);
			doc.getDocumentElement().normalize();
			//// System.out.println("Root element :" +
			/// doc.getDocumentElement();.getNodeName());
			//nList -> node/item -> Element 
			//TestApp.EDGE_ID = doc.getElementsByTagName(ID).item(0).getTextContent();
			NodeList nList = doc.getElementsByTagName("device");	//bbb
			//// System.out.println("----------------------------");
			for (int i = 0; i < nList.getLength(); i++) {
				String devId= ((Element) (nList.item(i))).getElementsByTagName("id").item(0).getTextContent();
				//String displayName= ((Element) (nList.item(i))).getElementsByTagName("displayname").item(0).getTextContent();
				
				//DeviceWebSocketServer.uiDivMap.put(Integer.toString(i),devId);
				//DeviceWebSocketServer.devicesSet.add(new Device(i, devId, displayName));
				//DeviceWebSocketServer.devices.put(devId, "0");
				devValues[] devValuesArray = new devValues[10];
				Node n = nList.item(i);
				
				if (n.getNodeType() == Node.ELEMENT_NODE) {
					// System.out.println("hello");
					Element devElem = (Element) n;
					NodeList assocList = devElem.getElementsByTagName("association");		//dev
					
					
					
					for (int k = 0; k < assocList.getLength(); k++) {
						String assocVal= ((Element) (assocList.item(k))).getElementsByTagName("val").item(0).getTextContent();
						String[] depNames = new String[10];
																		
						Node assocItem = assocList.item(k);
						
						if (assocItem.getNodeType() == Node.ELEMENT_NODE) {
							Element assocElem = (Element) assocItem;
							// TODO: association value
							
							NodeList depList = assocElem.getElementsByTagName("dependency");
							int j = 0;
							for (j = 0; j < depList.getLength(); j++) {
								String depName= ((Element) (depList.item(j))).getElementsByTagName("name").item(0).getTextContent();		
								depNames[j] = depName;
								//String devIndex= ((Element) (depList.item(j))).getElementsByTagName("index").item(0).getTextContent();
								//System.out.println("devid:"+devId+"devindex:"+devIndex);
								
								System.out.println("DDL parser:: devId:"+devId+" assocVal:"+assocVal+" depName:"+depName);
								
								//DeviceWebSocketServer.assocs.put(devId,);
								
								//*** add depName to an array string depNames
							}
							devValuesArray[k] = new devValues(assocVal, j, depNames);
							//DeviceWebSocketServer.assocs.put(devId, new devValues[]{new devValues(assocVal, j, depNames),new devValues("1", 1, new String[]{"LED_rgb.gnd_r#1"})});
						}
					}
					DeviceWebSocketServer.assocs.put(devId,devValuesArray);
				}
			}
			
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("error occurred" + e);
		}

	}	
}

