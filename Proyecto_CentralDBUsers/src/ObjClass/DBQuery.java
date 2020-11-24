/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ObjClass;
import java.sql.*;
import java.util.Vector;

public class DBQuery {
    
    /* Statements for Query's Users Table */
    private Connection DatabaseConnection;
    private PreparedStatement StatementInsertUsersTable;
    private PreparedStatement StatementSelectUsersTableByRegister;
    private PreparedStatement StatementDeleteUsersTable;
    /* Statements for Query's Material Table */
    private PreparedStatement StatementUpdateMaterialTableByRegister;
    private PreparedStatement StatementUpdateMaterialTableByDescription;
    private PreparedStatement StatementUpdateMaterialTableUserKey;    
    private PreparedStatement StatementSelectMaterialTable;
    private PreparedStatement StatementSelectMaterialTableByLockerMaterialCount;
    private PreparedStatement StatementUpdateMaterialLockerStatus;
    
    public DBQuery () throws Exception {
        Class.forName("com.mysql.jdbc.Driver");
    }
    
    public void OpenDB() throws SQLException 
    {
        DatabaseConnection = DriverManager.getConnection("jdbc:mysql://127.0.0.1/CentralDBProject", "root","");
        StatementInsertUsersTable = DatabaseConnection.prepareStatement("INSERT INTO UsersTable VALUES (?,?)");
        StatementSelectUsersTableByRegister = DatabaseConnection.prepareStatement("SELECT * FROM UsersTable WHERE Register=?");
        StatementDeleteUsersTable = DatabaseConnection.prepareStatement("DELETE FROM UsersTable WHERE Register=?");
        
        StatementUpdateMaterialTableByRegister = DatabaseConnection.prepareStatement("UPDATE MaterialTable SET LockerStatus=?, LockerRegisteredName=?, LockerRegisteredRegister=? WHERE LockerId=?");
        StatementUpdateMaterialTableByDescription = DatabaseConnection.prepareStatement("UPDATE MaterialTable SET LockerDescription=? WHERE LockerId=?");
        StatementUpdateMaterialTableUserKey = DatabaseConnection.prepareStatement("UPDATE MaterialTable SET LockerUserKey=? WHERE LockerId=?");
        StatementSelectMaterialTable = DatabaseConnection.prepareStatement("SELECT * FROM MaterialTable WHERE LockerStatus=?");
        StatementSelectMaterialTableByLockerMaterialCount = DatabaseConnection.prepareStatement("SELECT * FROM MaterialTable WHERE LockerMaterialCount>?");
        StatementUpdateMaterialLockerStatus = DatabaseConnection.prepareStatement("UPDATE MaterialTable SET LockerStatus=? WHERE LockerId=?");
    }
    
    public void CloseDB()
    {
        try
        {
            DatabaseConnection.close();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());
        }
    }
    
    /* Execute: INSERT INTO UsersTable VALUES (?,?,?) */
    public void InsertUsersTable (DBUsers DBUsersTag)
    {
        try
        {
            StatementInsertUsersTable.setString(1, DBUsersTag.GetName());
            StatementInsertUsersTable.setInt(2, DBUsersTag.GetRegister());
            StatementInsertUsersTable.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());
        }
    }
    
    /* Execute: SELECT * FROM UsersTable WHERE Register=? */
    public DBUsers SelectUsersTableByRegister (int Register)
    {
        DBUsers DBUsersTag = null;
        
        try
        {
            StatementSelectUsersTableByRegister.setInt(1, Register);
            ResultSet QueryResult = StatementSelectUsersTableByRegister.executeQuery();
            
            if(QueryResult.next())
            {
                DBUsersTag = new DBUsers(QueryResult.getString("Name"), QueryResult.getInt("Register"));
            }
            
            QueryResult.close();
            
            return DBUsersTag;
        }
        catch(SQLException exception)
        {
            return null;
        }
    }
    
    /* Execute: DELETE FROM UsersTable WHERE Register=? */
    public void DeleteUsersTable (int Register)
    {
        try
        {
            StatementDeleteUsersTable.setInt(1, Register);
            StatementDeleteUsersTable.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());
        }
    }
    
    /* Execute: UPDATE MaterialTable SET LockerStatus=?, LockerRegisteredName=?, LockerRegisteredRegister=? WHERE LockerId=? */
    public void UpdateMaterialTableByRegister (int LockerStatus, String LockerRegisteredName, int LockerRegisteredRegister, String LockerId)
    {
        try
        {
            StatementUpdateMaterialTableByRegister.setInt(1, LockerStatus);
            StatementUpdateMaterialTableByRegister.setString(2, LockerRegisteredName);
            StatementUpdateMaterialTableByRegister.setInt(3, LockerRegisteredRegister);
            StatementUpdateMaterialTableByRegister.setString(4, LockerId);
            StatementUpdateMaterialTableByRegister.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());
        }    
    }
    
    /* Execute: UPDATE MaterialTable SET LockerDescription=? WHERE LockerId=? */
    public void UpdateMaterialTableByDescription (String LockerDescription, String LockerId)
    {
        try
        {
            StatementUpdateMaterialTableByDescription.setString(1, LockerDescription);
            StatementUpdateMaterialTableByDescription.setString(2, LockerId);
            StatementUpdateMaterialTableByDescription.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());
        }         
    }
    
    /* Execute: SELECT * FROM MaterialTable WHERE LockerStatus=? */   
    public Vector SelectMaterialTable (int LockerStatus)
    {
        DBMaterial DBMaterialTag = null;
        Vector QueryResultVector = new Vector();
                
        try
        {
            StatementSelectMaterialTable.setInt(1, LockerStatus);
            ResultSet QueryResult = StatementSelectMaterialTable.executeQuery();
            
            while(QueryResult.next())
            {
                DBMaterialTag = new DBMaterial(QueryResult.getString("LockerId"), QueryResult.getInt("LockerStatus"), QueryResult.getString("LockerDescription"), QueryResult.getInt("LockerMaterialCount"), QueryResult.getInt("LockerMaterialAKey"), QueryResult.getInt("LockerMaterialBKey"), QueryResult.getInt("LockerMaterialCKey"), QueryResult.getString("LockerRegisteredName"), QueryResult.getInt("LockerRegisteredRegister"), QueryResult.getInt("LockerUserKey"));
                QueryResultVector.add(DBMaterialTag);
            }
            
            QueryResult.close();
            
            return QueryResultVector;
            
        }
        catch(SQLException exception)
        {
            return null;
        }         
    }
    
    /* Execute: SELECT * FROM MaterialTable WHERE LockerMaterialCount>? */
    public Vector SelectMaterialTableByMaterialCount (int LockerMaterialCount)
    {
        DBMaterial DBMaterialTag = null;
        Vector QueryResultVector = new Vector();
                
        try
        {
            StatementSelectMaterialTableByLockerMaterialCount.setInt(1, LockerMaterialCount);
            ResultSet QueryResult = StatementSelectMaterialTableByLockerMaterialCount.executeQuery();
            
            while(QueryResult.next())
            {
                DBMaterialTag = new DBMaterial(QueryResult.getString("LockerId"), QueryResult.getInt("LockerStatus"), QueryResult.getString("LockerDescription"), QueryResult.getInt("LockerMaterialCount"), QueryResult.getInt("LockerMaterialAKey"), QueryResult.getInt("LockerMaterialBKey"), QueryResult.getInt("LockerMaterialCKey"), QueryResult.getString("LockerRegisteredName"), QueryResult.getInt("LockerRegisteredRegister"), QueryResult.getInt("LockerUserKey"));
                QueryResultVector.add(DBMaterialTag);
            }
            
            QueryResult.close();
            
            return QueryResultVector;
            
        }
        catch(SQLException exception)
        {
            return null;
        }         
    }
    
    /* Execute: UPDATE MaterialTable SET LockerStatus=? WHERE LockerId=? */
    public void UpdateMaterialTableLockerStatus (int LockerStatus, String LockerId)
    {
        try
        {
            StatementUpdateMaterialLockerStatus.setInt(1, LockerStatus);
            StatementUpdateMaterialLockerStatus.setString(2, LockerId);
            StatementUpdateMaterialLockerStatus.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());    
        }
    }
    
    public void UpdateMaterialTableUserKey (int LockerUserKey, String LockerId)
    {
        try
        {
            StatementUpdateMaterialTableUserKey.setInt(1, LockerUserKey);
            StatementUpdateMaterialTableUserKey.setString(2, LockerId);
            StatementUpdateMaterialTableUserKey.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());    
        }
    }
}
