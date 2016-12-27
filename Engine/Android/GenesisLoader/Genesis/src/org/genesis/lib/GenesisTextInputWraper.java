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

import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

public class GenesisTextInputWraper implements TextWatcher, OnEditorActionListener {
	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = GenesisTextInputWraper.class.getSimpleName();

	// ===========================================================
	// Fields
	// ===========================================================

	private final GenesisGLSurfaceView mGenesisGLSurfaceView;
	private String mText;
	private String mOriginText;
	private int    mCurrentInputPos;
	private int    mCountChar;

	// ===========================================================
	// Constructors
	// ===========================================================

	public GenesisTextInputWraper(final GenesisGLSurfaceView pGenesisGLSurfaceView) {
		this.mGenesisGLSurfaceView = pGenesisGLSurfaceView;
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	private boolean isFullScreenEdit() {
		final TextView textField = this.mGenesisGLSurfaceView.getGenesisEditText();
		final InputMethodManager imm = (InputMethodManager) textField.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
		return imm.isFullscreenMode();
	}

	public void setOriginText(final String pOriginText) {
		this.mOriginText = pOriginText;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void afterTextChanged(final Editable s) {
		if (this.isFullScreenEdit()) {
			return;
		}

		if(this.mCountChar > 0) {
			int start = this.mCurrentInputPos;
			int end   = start + this.mCountChar;
			final String insertText = s.subSequence(start,end).toString();
			
			if(insertText.equals("\n"))
			{
				String trueStr = s.subSequence(0,start).toString() +
						 s.subSequence(end,s.length()).toString();
				this.mGenesisGLSurfaceView.insertText( trueStr );
			}
			
		}
		
		return;
		
		
//		int nModified = s.length() - this.mText.length();
//		if (nModified < 0)
//		 {
//				for (; nModified < 0; ++nModified) {
//					this.mGenesisGLSurfaceView.deleteBackward();
//					/*
//					if (BuildConfig.DEBUG) {
//						Log.d(TAG, "deleteBackward");
//					}
//					*/
//				}
//			}
//		else if(this.mCountChar > 0) {
//			int start = this.mCurrentInputPos;
//			int end   = start + this.mCountChar;
//			final String insertText = s.subSequence(start,end).toString();
//			this.mGenesisGLSurfaceView.insertText(insertText);
//			
//			//move cursor
//			/*
//			if (BuildConfig.DEBUG) {
//				Log.d(TAG, "insertText(" + insertText + ")");
//			}
//			*/
//		}
//		this.mText = s.toString();
	}

	@Override
	public void beforeTextChanged(final CharSequence pCharSequence, final int start, final int count, final int after) {
		/*
		if (BuildConfig.DEBUG) {
			Log.d(TAG, "beforeTextChanged(" + pCharSequence + ")start: " + start + ",count: " + count + ",after: " + after);
		}
		*/
		this.mText = pCharSequence.toString();
		this.mCurrentInputPos = start;
	}

	@Override
	public void onTextChanged(final CharSequence pCharSequence, final int start, final int before, final int count) {

		this.mCountChar = count;
	}

	@Override
	public boolean onEditorAction(final TextView pTextView, final int pActionID, final KeyEvent pKeyEvent) {
		if (this.mGenesisGLSurfaceView.getGenesisEditText() == pTextView && this.isFullScreenEdit()) {
			// user press the action button, delete all old text and insert new text
			for (int i = this.mOriginText.length(); i > 0; i--) {
				this.mGenesisGLSurfaceView.deleteBackward();
				/*
				if (BuildConfig.DEBUG) {
					Log.d(TAG, "deleteBackward");
				}
				*/
			}
			String text = pTextView.getText().toString();

			/* If user input nothing, translate "\n" to engine. */
			if (text.compareTo("") == 0) {
				text = "\n";
			}

			if ('\n' != text.charAt(text.length() - 1)) {
				text += '\n';
			}

			final String insertText = text;
			this.mGenesisGLSurfaceView.insertText(insertText);
			/*
			if (BuildConfig.DEBUG) {
				Log.d(TAG, "insertText(" + insertText + ")");
			}
			*/
		}
		
		if (pActionID == EditorInfo.IME_ACTION_DONE) {
			this.mGenesisGLSurfaceView.requestFocus();
		}
		return false;
	}

	// ===========================================================
	// Methods
	// ===========================================================

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}