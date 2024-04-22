<%-- Delete record --%>

<%@ include file="/Rajesh/Jsp/ConnectDb.jsp" %>
<%@ page import="java.sql.*" %>
<%
    String tableName = request.getParameter("TableName");
    String fieldName = request.getParameter("FieldName");
    String fieldValue = request.getParameter("FieldValue");
    PreparedStatement statement = null;
    int result = 0;
    
    try {
        String query = "DELETE FROM " + tableName + " WHERE " + fieldName + " = ?";
        statement = conn.prepareStatement(query);
        statement.setString(1, fieldValue);
        result = statement.executeUpdate();
    } catch (SQLException e) {
        e.printStackTrace();
    } finally {
        if (statement != null) {
            try {
                statement.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }
    out.println(result > 0 ? "1" : "0");
%>