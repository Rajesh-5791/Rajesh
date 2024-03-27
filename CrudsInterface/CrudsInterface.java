// iCRUDS interface

import java.sql.SQLException;

interface iCRUDS
{
	int insertRecord(String[] pRecordDetails) throws Exception;
	String[][] loadRecords() throws Exception;
}
