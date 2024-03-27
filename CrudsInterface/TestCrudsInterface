// Testing iCRUDS Interface

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Scanner;

class cTestCrudsInterface
{
	public static Scanner oScanner = new Scanner(System.in);

    public static void main(String[] args) 
    {
	    try 
	    {
	        String className = getClassName("className.cfg");
	        String[] dbConfig = getDbConfig("DB.cfg");
	        String[] fieldNames = getFieldNames("Fields.cfg");
	        if (className != null && dbConfig.length == 2 && fieldNames != null) 
	        {
	            String dbName = dbConfig[0];
	            String tableName = dbConfig[1];
	            cUI oUI = new cUI(tableName, fieldNames);
	            iCRUDS oCRUDS = (iCRUDS) Class.forName(className).getConstructor(String.class, String[].class, String.class).newInstance(dbName, fieldNames, tableName);
	            while (true) 
	            {
	                System.out.println("\n======= Menu ========");
	                System.out.println("1. Insert " + tableName);
	                System.out.println("2. Show all " + tableName + "s");
	                System.out.println("3. Exit");
	                System.out.print("\nEnter your choice: ");
	                int choice = oScanner.nextInt();
	                oScanner.nextLine();

	                switch (choice) 
	                {
	                    case 1:
	                        String[] recordDetails = oUI.getRecordDetails();
	                        int rowInserted = oCRUDS.insertRecord(recordDetails);
	                        oUI.printStatusMessage(rowInserted, "insertion");
	                        break;
	                    case 2:
	                        String[][] records = oCRUDS.loadRecords();
	                        oUI.printRecords(records);
	                        break;
	                    case 3:
	                        System.out.println("Exiting program...\nConnection closed.");
	                        System.exit(0);
	                    default:
	                        System.out.println("Invalid choice. Please try again.");
	                }
	            }
	        }
	        else 
	        {
	            System.out.println("Class name not found in configuration file.");
	        }
	    } 
	    catch (Exception e) 
	    {
	        e.printStackTrace();
	    }
	}

	private static String[] getFieldNames(String pFilePath)
	{
    	String[] fieldNames = null;
		try (BufferedReader reader = new BufferedReader(new FileReader(pFilePath))) 
        {
            StringBuilder allFieldsBuilder = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) 
            {
                allFieldsBuilder.append(line).append(" ");
            }
            String allFields = allFieldsBuilder.toString().trim();
            if (!allFields.isEmpty()) 
            {
                fieldNames = allFields.split("\\s+");
            } 
            else
            {
                System.err.println("File is empty.");
            }
        } 
        catch (IOException e)
        {
            e.printStackTrace();
        }
        return fieldNames;
	}

    private static String getClassName(String pFilePath) 
    {
        try 
        {
            return Files.readString(Paths.get(pFilePath)).trim();
        } 
        catch (IOException e)
        {
            e.printStackTrace();
            return null;
        }
    }

    private static String[] getDbConfig(String pFilePath) 
    {
        try 
        {
            String content = Files.readString(Paths.get(pFilePath));
            return content.split("\\r?\\n", 2);
        } 
        catch (IOException e) 
        {
            e.printStackTrace();
            return new String[0];
        }
    }
}
