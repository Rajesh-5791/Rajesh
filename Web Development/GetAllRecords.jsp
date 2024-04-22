<%-- Get all records --%>

<%@ page import="java.sql.*" %>
<%@ include file="/dRajesh/Jsp/ConnectDb.jsp" %>
<%
    String tableName = request.getParameter("TableName");
    String fieldName = request.getParameter("FieldName");
    String fieldValue = request.getParameter("FieldValue");
    PreparedStatement preparedStatement = null;
    ResultSet queryResult = null;
    String whereClause, statement;
    String query = "SELECT * FROM v" + tableName + " WHERE ";
    if (fieldName != null && fieldValue != null) {
        whereClause = "JSON_EXTRACT(" + tableName + "View, '$." + fieldName + "') = '" + fieldValue + "'";
    } else {
        whereClause = "1 = 1";
    }
    statement = query + whereClause;
    try {
        preparedStatement = conn.prepareStatement(statement);
        queryResult = preparedStatement.executeQuery();
        out.print("[");
        boolean first = true;
        String tableDetails = tableName + "View";
        while (queryResult.next()) {
            if (!first) {
                out.print(", ");
            } else {
                first = false;
            }
            String jsonItem = queryResult.getString(tableDetails);
            out.print(jsonItem);
        }
        out.print("]");
    } catch (SQLException e) {
        e.printStackTrace();
    } finally {
        if (queryResult != null) {
            try {
                queryResult.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        if (preparedStatement != null) {
            try {
                preparedStatement.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }
%>