/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ObjClass;


public class DBUsers {
    
    public String Name;
    public int Register;
    
    public DBUsers(String Name, int Register){
        this.Name = Name;
        this.Register = Register;
    }
    public DBUsers(){
        this("", 0);
    }
    
    public String GetName ()
    {
        return this.Name;
    }
    public int GetRegister ()
    {
        return this.Register;
    }
    public void SetName (String Name)
    {
        this.Name = Name;
    }
    public void SetRegister (int Register)
    {
        this.Register = Register;
    }
}
