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
public class EdgeDDLParser {

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

	public static void parseEdge() {
		try {
			File inputFile = new File("EdgeDDL.xml");
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc = dBuilder.parse(inputFile);
			doc.getDocumentElement().normalize();
			// System.out.println("Root element :" +
			// doc.getDocumentElement();.getNodeName());
                        TestApp.EDGE_ID = doc.getElementsByTagName(ID).item(0).getTextContent();
       			NodeList nList = doc.getElementsByTagName("bbb");
			// System.out.println("----------------------------");
			for (int i = 0; i < nList.getLength(); i++) {
				Node n = nList.item(i);
				
				if (n.getNodeType() == Node.ELEMENT_NODE) {
					// System.out.println("hello");
					Element devElem = (Element) n;
					NodeList pns = devElem.getElementsByTagName("dev");
					for (int j = 0; j < pns.getLength(); j++) {
                                                String devId= ((Element) (pns.item(j))).getElementsByTagName("id").item(0).getTextContent();
                                                String devIndex= ((Element) (pns.item(j))).getElementsByTagName("index").item(0).getTextContent();
                                                System.out.println("devid:"+devId+"devindex:"+devIndex);
						TestApp.deviceMappings.put(devIndex, devId);
                                                TestApp.deviceMappingsReverse.put(devId, devIndex);
                                                TestApp.bbDevMap.put(devId, TestApp.EDGE_ID);
					}
				}
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println("error occurred" + e);
		}

        }	
}

