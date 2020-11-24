/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ObjClass;


public class COMPortInfo {
    
    public String COMPortName;
    public boolean BLEisConnected;
    
    public COMPortInfo(String COMPortName, boolean BLEisConnected)
    {
        this.COMPortName = COMPortName;
        this.BLEisConnected = BLEisConnected;
    }
    public COMPortInfo(){
        this("", false);
    }
    
    public String GetCOMPortName ()
    {
        return this.COMPortName;
    }
    public boolean GetBLEisConnected ()
    {
        return this.BLEisConnected;
    }
    public void SetCOMPortName (String COMPortName)
    {
        this.COMPortName = COMPortName;
    }
    public void SetBLEisConnected (boolean BLEisConnected)
    {
        this.BLEisConnected = BLEisConnected;
    }
}
