// XML CRUDS class

import java.io.File;
import org.w3c.dom.*;
import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

class cXMLCruds implements iCRUDS
{
	File inputFile;
	String subRootName;
	DocumentBuilderFactory dbFactory;
	DocumentBuilder dBuilder;
	Document doc;
	private String[] fields;

	public cXMLCruds(String pFileName, String[] pFieldNames, String pSubRootName) throws Exception
	{
		fields = pFieldNames;
		subRootName = pSubRootName;
        inputFile = new File(pFileName + ".xml");
        dbFactory = DocumentBuilderFactory.newInstance();
        dBuilder = dbFactory.newDocumentBuilder();
        doc = dBuilder.parse(inputFile);
        doc.getDocumentElement().normalize();
	}

    public int insertRecord(String[] pRecordDetails) throws Exception
	{
		int columnCounter = 0;
	    Element root = doc.getDocumentElement();
	    Element newRecord = doc.createElement(subRootName);

	    for (columnCounter = 0; columnCounter < fields.length; columnCounter++) 
	    {
	        Element field = doc.createElement(fields[columnCounter]);
	        field.appendChild(doc.createTextNode(pRecordDetails[columnCounter]));
	        newRecord.appendChild(field);
	    }

	    root.appendChild(newRecord);
	    writeChangesToFile();
	    return 1;
	}

	public String[][] loadRecords() throws Exception
	{
		NodeList allRecords = doc.getElementsByTagName(subRootName);
		int rowCount = allRecords.getLength(), rowCounter = 0, columnCounter = 0;
		String[][] records = new String[rowCount][fields.length];

		for (rowCounter = 0; rowCounter < allRecords.getLength(); rowCounter++) 
		{
		    Element record = (Element) allRecords.item(rowCounter);
		    NodeList fieldList = record.getChildNodes();
		    
		    for (columnCounter = 0; columnCounter < fields.length; columnCounter++) 
		    {
		        String fieldName = fields[columnCounter];
		        records[rowCounter][columnCounter] = record.getElementsByTagName(fieldName).item(0).getTextContent();
		    }
		}
		return records;
	}

	private void writeChangesToFile() throws Exception
	{
	    TransformerFactory transformerFactory = TransformerFactory.newInstance();
	    Transformer transformer = transformerFactory.newTransformer();
	    DOMSource source = new DOMSource(doc);
	    StreamResult result = new StreamResult(inputFile);
	    transformer.transform(source, result);
	}
}