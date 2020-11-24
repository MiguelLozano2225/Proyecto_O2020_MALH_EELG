/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ObjClass;


public class DBMaterial {
    
    public String LockerId;
    public int LockerStatus;
    public String LockerDescription;
    public int LockerMaterialCount;
    public int LockerMaterialAKey;
    public int LockerMaterialBKey;
    public int LockerMaterialCKey;
    public String LockerRegisteredName;
    public int LockerRegisteredRegister;
    public int LockerUserKey;
    
    public DBMaterial(String LockerId, int LockerStatus, String LockerDescription, int LockerMaterialCount, int LockerMaterialAKey, int LockerMaterialBKey, int LockerMaterialCKey, String LockerRegisteredName, int LockerRegisteredRegister, int LockerUserKey){
        this.LockerId = LockerId;
        this.LockerStatus = LockerStatus;
        this.LockerDescription = LockerDescription;
        this.LockerMaterialCount = LockerMaterialCount;
        this.LockerMaterialAKey = LockerMaterialAKey;
        this.LockerMaterialBKey = LockerMaterialBKey;
        this.LockerMaterialCKey = LockerMaterialCKey;
        this.LockerRegisteredName = LockerRegisteredName;
        this.LockerRegisteredRegister = LockerRegisteredRegister;
        this.LockerUserKey = LockerUserKey;
    }
    
    public DBMaterial(){
        this("", 0, "", 0, 0, 0, 0, "", 0, 0);
    }
    
    public String GetLockerId ()
    {
        return this.LockerId;
    }
    public int GetLockerStatus ()
    {
        return this.LockerStatus;
    }
    public String GetLockerDescription ()
    {
        return this.LockerDescription;
    }
    public int GetLockerMaterialCount()
    {
        return this.LockerMaterialCount;
    }
    public int GetLockerMaterialAKey()
    {
        return this.LockerMaterialAKey;
    }
    public int GetLockerMaterialBKey()
    {
        return this.LockerMaterialBKey;
    }
    public int GetLockerMaterialCKey()
    {
        return this.LockerMaterialCKey;
    }
    public String GetLockerRegisteredName ()
    {
        return this.LockerRegisteredName;
    }
    public int GetLockerRegisteredRegister ()
    {
        return this.LockerRegisteredRegister;
    }
    public int GetLockerUserKey ()
    {
        return this.LockerUserKey;
    }  
    public void SetLockerId (String LockerId)
    {
        this.LockerId = LockerId;
    }
    public void SetLockerStatus (int LockerStatus)
    {
        this.LockerStatus = LockerStatus;
    }
    public void SetLockerDescription (String LockerDescription)
    {
        this.LockerDescription = LockerDescription;
    }
    public void SetLockerMaterialCount(int LockerMaterialCount)
    {
        this.LockerMaterialCount = LockerMaterialCount;
    }
    public void SetLockerMaterialAKey(int LockerMaterialAKey)
    {
        this.LockerMaterialAKey = LockerMaterialAKey;
    }
    public void SetLockerMaterialBKey(int LockerMaterialBKey)
    {
        this.LockerMaterialBKey = LockerMaterialBKey;
    }
    public void SetLockerMaterialCKey(int LockerMaterialCKey)
    {
        this.LockerMaterialCKey = LockerMaterialCKey;
    }    
    public void SetLockerRegisteredName (String LockerRegisteredName)
    {
        this.LockerRegisteredName = LockerRegisteredName;
    }
    public void SetLockerRegisteredRegister (int LockerRegisteredRegister) 
    {
        this.LockerRegisteredRegister = LockerRegisteredRegister;
    }
    public void SetLockerUserKey (int LockerUserKey)
    {
        this.LockerUserKey = LockerUserKey;
    }
}
