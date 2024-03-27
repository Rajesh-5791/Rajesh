// SQLite CRUDS class

import java.sql.*;

class cSQLiteCruds implements iCRUDS
{
    private Connection conn;
    private String query, tableName;
    private PreparedStatement statement;
    private ResultSet queryResult;
    private String[] fields;

    public cSQLiteCruds(String pDatabaseName, String[] pFieldNames, String pTableName) throws SQLException 
    {
        fields = pFieldNames;
        tableName = pTableName;
        conn = DriverManager.getConnection("jdbc:sqlite:D:/Training/sqlite-tools-win-x64-3450100/" + pDatabaseName + ".db");
        System.out.println("Connected to the database.");
    }

    public int insertRecord(String[] pRecordDetails) throws SQLException
    {
        int counter = 0;
        StringBuilder queryBuilder = new StringBuilder("INSERT INTO ").append(tableName);

        queryBuilder.append(" VALUES (");

        for (counter = 0; counter < pRecordDetails.length; counter++) 
        {
            queryBuilder.append("?");
            if (counter < pRecordDetails.length - 1) 
            {
                queryBuilder.append(", ");
            }
        }

        queryBuilder.append(")");
        query = queryBuilder.toString();
        statement = conn.prepareStatement(query);
        
        for (counter = 0; counter < pRecordDetails.length; counter++) 
        {
            statement.setString(counter + 1, pRecordDetails[counter]);
        }

        int rowInserted = statement.executeUpdate();
        return rowInserted;
    }

    public int getRowCount() throws SQLException 
    {
        int rowCount = 0;
        query = "SELECT COUNT(*) FROM " + tableName;
        statement = conn.prepareStatement(query);
        queryResult = statement.executeQuery();
        if (queryResult.next())
        {
            rowCount = queryResult.getInt(1);
        }
        return rowCount;
    }

    public String[][] loadRecords() throws SQLException 
    {
        int rowCount = getRowCount(), counter = 0;
        query = "SELECT * FROM " + tableName;
        statement = conn.prepareStatement(query);
        queryResult = statement.executeQuery();
        String[][] Records = new String[rowCount][fields.length];

        int index = 0;
        while (queryResult.next()) 
        {
            for (counter = 1; counter <= fields.length; counter++)
            {
                Records[index][counter - 1] = queryResult.getString(counter);
            }
            index++;
        }
        return Records;
    }
}