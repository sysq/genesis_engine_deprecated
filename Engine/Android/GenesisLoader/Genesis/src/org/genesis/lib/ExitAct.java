package org.genesis.lib;

import java.util.LinkedList; 
import java.util.List; 
import android.app.Activity; 
import android.app.AlertDialog; 
import android.app.Application; 
import android.content.DialogInterface; 
import android.content.Intent; 
 
public class ExitAct extends Application { 
    private static List mList = new LinkedList(); 
    private static ExitAct instance; 
 
    // add Activity  
    public static void addActivity(Activity activity) { 
        mList.add(activity); 
    } 
 
    public static void  exit() { 
        try { 
            for (int i = 0;i< mList.size();i++) { 
            	Activity activity = (Activity) mList.get(i);
                if (activity != null) 
                    activity.finish(); 
            } 
        } catch (Exception e) { 
            e.printStackTrace(); 
        } finally { 
        	 android.os.Process.killProcess(android.os.Process.myPid()); 
        } 
    } 
    
    public void onLowMemory() { 
        super.onLowMemory();     
        System.gc(); 
    }  
}
