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
    private PreparedStatement StatementUpdateMaterialTableDescriptionById;
    private PreparedStatement StatementUpdateMaterialATableByDescription;
    private PreparedStatement StatementUpdateMaterialBTableByDescription;
    private PreparedStatement StatementUpdateMaterialCTableByDescription;
    private PreparedStatement StatementSelectMaterialTable;
    private PreparedStatement StatementSelectMaterialTableByLockerId;
    
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
        StatementUpdateMaterialTableDescriptionById = DatabaseConnection.prepareStatement("UPDATE MaterialTable SET LockerDescription=? WHERE LockerId=?");
        StatementUpdateMaterialATableByDescription = DatabaseConnection.prepareStatement("UPDATE MaterialTable SET LockerMaterialCount=?, LockerMaterialAKey=? WHERE LockerId=?");
        StatementUpdateMaterialBTableByDescription = DatabaseConnection.prepareStatement("UPDATE MaterialTable SET LockerMaterialCount=?, LockerMaterialBKey=? WHERE LockerId=?");
        StatementUpdateMaterialCTableByDescription = DatabaseConnection.prepareStatement("UPDATE MaterialTable SET LockerMaterialCount=?, LockerMaterialCKey=? WHERE LockerId=?");
        StatementSelectMaterialTable = DatabaseConnection.prepareStatement("SELECT * FROM MaterialTable WHERE LockerStatus=?");
        StatementSelectMaterialTableByLockerId = DatabaseConnection.prepareStatement("SELECT * FROM MaterialTable WHERE LockerId=?");
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
    public void UpdateMaterialDescription (String LockerDescription, String LockerId)
    {
        try
        {
            StatementUpdateMaterialTableDescriptionById.setString(1, LockerDescription);
            StatementUpdateMaterialTableDescriptionById.setString(2, LockerId);
            StatementUpdateMaterialTableDescriptionById.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());  
        }
    }
    
    /* UPDATE MaterialTable SET LockerMaterialCount=?, LockerMaterialAKey=? WHERE LockerId=? */
    public void UpdateMaterialATableByDescription (int LockerMaterialCount, int LockerMaterialAKey, String LockerId)
    {
        try
        {
            StatementUpdateMaterialATableByDescription.setInt(1,LockerMaterialCount);
            StatementUpdateMaterialATableByDescription.setInt(2,LockerMaterialAKey);
            StatementUpdateMaterialATableByDescription.setString(3, LockerId);
            StatementUpdateMaterialATableByDescription.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());
        }         
    }
    
    /* UPDATE MaterialTable SET LockerMaterialCount=?, LockerMaterialBKey=? WHERE LockerId=? */
    public void UpdateMaterialBTableByDescription (int LockerMaterialCount, int LockerMaterialBKey, String LockerId)
    {
        try
        {
            StatementUpdateMaterialBTableByDescription.setInt(1,LockerMaterialCount);
            StatementUpdateMaterialBTableByDescription.setInt(2,LockerMaterialBKey);
            StatementUpdateMaterialBTableByDescription.setString(3, LockerId);
            StatementUpdateMaterialBTableByDescription.executeUpdate();
        }
        catch(SQLException exception)
        {
            System.out.println("Unexpected error\n\r" + exception.getMessage());
        }         
    }
    
    /* UPDATE MaterialTable SET LockerMaterialCount=?, LockerMaterialCKey=? WHERE LockerId=? */
    public void UpdateMaterialCTableByDescription (int LockerMaterialCount, int LockerMaterialCKey, String LockerId)
    {
        try
        {
            StatementUpdateMaterialCTableByDescription.setInt(1,LockerMaterialCount);
            StatementUpdateMaterialCTableByDescription.setInt(2,LockerMaterialCKey);
            StatementUpdateMaterialCTableByDescription.setString(3, LockerId);
            StatementUpdateMaterialCTableByDescription.executeUpdate();
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
    
    /* Execute: SELECT LockerMaterialCount FROM MaterialTable WHERE LockerId=? */
    public DBMaterial SelectMaterialLockerId (String LockerId)
    {
        DBMaterial DBMaterialTag = null;
        
        try
        {
            StatementSelectMaterialTableByLockerId.setString(1, LockerId);
            ResultSet QueryResult = StatementSelectMaterialTableByLockerId.executeQuery();
            
            if(QueryResult.next())
            {
                DBMaterialTag = new DBMaterial(QueryResult.getString("LockerId"), QueryResult.getInt("LockerStatus"), QueryResult.getString("LockerDescription"), QueryResult.getInt("LockerMaterialCount"), QueryResult.getInt("LockerMaterialAKey"), QueryResult.getInt("LockerMaterialBKey"), QueryResult.getInt("LockerMaterialCKey"), QueryResult.getString("LockerRegisteredName"), QueryResult.getInt("LockerRegisteredRegister"), QueryResult.getInt("LockerUserKey"));
            }
            
            QueryResult.close();
            
            return DBMaterialTag;
        }
        catch(SQLException exception)
        {
            return null;
        }
    }
    
}
