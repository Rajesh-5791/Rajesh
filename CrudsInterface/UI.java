// UI for iCRUDS interface

import java.io.IOException;
import java.util.Scanner;

class cUI 
{
    private Scanner scanner = new Scanner(System.in);
    private String[] fields;
    private String tableName;

    public cUI(String pTableName, String[] pFieldNames)
    {
        tableName = pTableName;
        fields = pFieldNames;
    }

    public void printStatusMessage(int pRowInserted, String pOperation) 
    {
        System.out.print(tableName + " details " + pOperation + " is ");
        if (pRowInserted == 1) 
        {
            System.out.print("successful.\n");
        } 
        else 
        {
            System.out.print("failed.\n");
        }
    }

    public String[] getRecordDetails() 
    {
        int fieldsCounter = 0;
        String[] recordDetails = new String[fields.length];
        for (fieldsCounter = 0; fieldsCounter < fields.length; fieldsCounter++) 
        {
            System.out.print("Enter " + fields[fieldsCounter] + ": ");
            recordDetails[fieldsCounter] = scanner.nextLine();
        }
        return recordDetails;
    }

    public void printRecords(String[][] pRecords) 
    {
        if (pRecords != null)
        {
            int counter = 0, fieldsCounter = 0;
            for (String[] record : pRecords) 
            {
                System.out.println("Details of " + tableName + " - " + (++counter));
                System.out.println("-".repeat(40));
                for (fieldsCounter = 0; fieldsCounter < fields.length; fieldsCounter++) 
                {
                    System.out.printf("%-17s: %s%n", fields[fieldsCounter], record[fieldsCounter]);
                }
                System.out.println("-".repeat(40));
            }
        }
        else 
        {
            System.out.println("No data found.");
        }
    }
}