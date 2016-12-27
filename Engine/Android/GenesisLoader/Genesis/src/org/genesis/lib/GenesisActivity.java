/****************************************************************************
Copyright (c) 2006, Radon Labs GmbH
Copyright (c) 2011-2013,WebJet Business Division,CYOU

http://www.genesis-3d.com.cn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.genesis.lib;

import org.genesis.lib.GenesisGLSurfaceView;
import org.genesis.lib.GenesisHelperListener;

import com.genesis.R;

import android.app.Activity;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.PaintDrawable;
import android.os.Bundle;
import android.os.Message;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.widget.FrameLayout;

public abstract class GenesisActivity extends Activity implements GenesisHelperListener {
	// ===========================================================
	// Constants
	// ===========================================================
	private static final String TAG = GenesisActivity.class.getSimpleName();
	// ===========================================================
	private GenesisGLSurfaceView mGLSurefaceView;
	private int s_topPos; 
	
	public GenesisGLSurfaceView GetSurfaceView()
	{
		return mGLSurefaceView;
	}
	
	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		ExitAct.addActivity(this);
		this.init();
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	protected void onResume() {
		super.onResume();
		
		
		this.mGLSurefaceView.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();

		
		this.mGLSurefaceView.onPause();
	}
	
	@Override
	protected void onStop()
	{
		super.onStop();
		
		this.mGLSurefaceView.onStop();
	}
	
	
	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		this.mGLSurefaceView.onDestroy();
	}
	

	@Override
	public void showDialog(final String pTitle, final String pMessage) {
		Message msg = new Message();
	}

	@Override
	public void showEditTextDialog(final String pTitle, final String pContent, final int pInputMode, final int pInputFlag, final int pReturnType, final int pMaxLength) { 
	
	}
	
	@Override
	public void runOnGLThread(final Runnable pRunnable) {
		this.mGLSurefaceView.queueEvent(pRunnable);
	}
	  public void init() {
	    			
	    	// FrameLayout
	        ViewGroup.LayoutParams framelayout_params =
	            new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT,
	                                       ViewGroup.LayoutParams.FILL_PARENT);
	        FrameLayout framelayout = new FrameLayout(this);
	        framelayout.setLayoutParams(framelayout_params);
	        
	        
	       

	   	        // Cocos2dxGLSurfaceView
	        this.mGLSurefaceView = new GenesisGLSurfaceView(this);
	        framelayout.addView(mGLSurefaceView);
	        
	        //edittext view
	     // Cocos2dxEditText layout
	        FrameLayout.LayoutParams edittext_layout_params = 
		            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.FILL_PARENT,
		            		FrameLayout.LayoutParams.WRAP_CONTENT);
	        final GenesisEditText edittext = new GenesisEditText(this);
	        edittext.setLayoutParams(edittext_layout_params);
	        framelayout.addView(edittext);

	        mGLSurefaceView.setBackgroundResource(R.drawable.load);
		
	        mGLSurefaceView.setGenesisRenderer(new GenesisRenderer(this));
	        this.mGLSurefaceView.setGenesisEditText(edittext);
	        
	        //!-- keyboardheight
	        final FrameLayout fframelayout = framelayout;
	    
	        fframelayout.getViewTreeObserver().addOnGlobalLayoutListener( new ViewTreeObserver.OnGlobalLayoutListener() {

                @Override
                public void onGlobalLayout() {
                    // TODO Auto-generated method stub
                    Rect r = new Rect();
                    edittext.getWindowVisibleDisplayFrame(r);

                    int screenHeight = edittext.getRootView().getHeight();
                    int screenWidth  = edittext.getRootView().getWidth();
                    int heightDifference = screenHeight - (r.bottom - r.top);
                   
                    boolean visible = heightDifference > screenHeight / 3;
                    
                    //set edittext postion
                    int editBottom = edittext.getBottom();
                    int editHeight = edittext.getHeight();
        	        //((EditText) edittext).setBottom(heightDifference);
                    
                    boolean keyboardVisible = false; 
                    int topPos = screenHeight;
                    if(r.bottom < screenHeight)
                    {
                    	keyboardVisible = true;
                    	topPos = r.bottom - editHeight ;
                    }
                    if(s_topPos == topPos)
                    {
                    	return;                    	
                    }
                    s_topPos = topPos;
                    FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) edittext.getLayoutParams();
                    params.leftMargin = 0;
                    params.topMargin = topPos;
                    params.width = screenWidth;
                    params.height = editHeight;
//                    params.rightMargin = screenWidth;
//                    params.bottomMargin = heightDifference; 
                    edittext.setLayoutParams(params);               
                   
                    if(keyboardVisible)
                    {   
                    	FrameLayout flayout = (FrameLayout) edittext.getParent();  
                    	flayout.removeView(edittext);
                    	flayout.addView(edittext);
                    	
                    	edittext.setFocusable(true);
                    	edittext.setFocusableInTouchMode(true);
                    	edittext.requestFocus();  
                    	
                    }
                    
                    //edittext.getParent().bringChildToFront(edittext); 
                   
                    
        	       

                }
               });
	        // Set framelayout as the content view
			setContentView(framelayout);
	    }
	    
}