<%-- Insert record --%>

<%@ include file="/dRajesh/Jsp/ConnectDb.jsp" %>
<%@ page import="java.util.*" %>
<%@ page import="java.sql.*" %>
<%
    String tableName = request.getParameter("TableName");
    String fieldNames = request.getParameter("FieldNames");
    String fieldValues = request.getParameter("FieldValues");
    String[] fieldNamesList = fieldNames.split(",");
    String[] fieldValuesList = fieldValues.split(",");

    PreparedStatement statement = null;
    int result = 0;
    int counter = 0;

    try {
        StringBuilder queryBuilder = new StringBuilder();
        queryBuilder.append("INSERT INTO ").append(tableName).append(" VALUES (");
        for (counter = 0; counter < fieldValuesList.length; counter++) {
            queryBuilder.append("?,");
        }
        queryBuilder.deleteCharAt(queryBuilder.length() - 1);
        queryBuilder.append(") ON DUPLICATE KEY UPDATE ");
        for (counter = 0; counter < fieldNamesList.length; counter++) {
            queryBuilder.append(fieldNamesList[counter]).append("=VALUES(").append(fieldNamesList[counter]).append("),");
        }
        queryBuilder.deleteCharAt(queryBuilder.length() - 1);
        
        statement = conn.prepareStatement(queryBuilder.toString());
        for (counter = 0; counter < fieldValuesList.length; counter++) {
            statement.setString(counter + 1, fieldValuesList[counter]);
        }
        result = statement.executeUpdate();
    } catch (SQLException ex) {
        ex.printStackTrace();
    }
    out.println(result > 0 ? "1" : "0");
%>