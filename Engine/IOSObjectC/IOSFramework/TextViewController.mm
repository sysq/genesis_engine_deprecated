/****************************************************************************
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
/*
     File: TextViewController.m 
 Abstract: The view controller for hosting the UITextView features of this sample. 
  Version: 2.11 
  
 Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple 
 Inc. ("Apple") in consideration of your agreement to the following 
 terms, and your use, installation, modification or redistribution of 
 this Apple software constitutes acceptance of these terms.  If you do 
 not agree with these terms, please do not use, install, modify or 
 redistribute this Apple software. 
  
 In consideration of your agreement to abide by the following terms, and 
 subject to these terms, Apple grants you a personal, non-exclusive 
 license, under Apple's copyrights in this original Apple software (the 
 "Apple Software"), to use, reproduce, modify and redistribute the Apple 
 Software, with or without modifications, in source and/or binary forms; 
 provided that if you redistribute the Apple Software in its entirety and 
 without modifications, you must retain this notice and the following 
 text and disclaimers in all such redistributions of the Apple Software. 
 Neither the name, trademarks, service marks or logos of Apple Inc. may 
 be used to endorse or promote products derived from the Apple Software 
 without specific prior written permission from Apple.  Except as 
 expressly stated in this notice, no other rights or licenses, express or 
 implied, are granted by Apple herein, including but not limited to any 
 patent rights that may be infringed by your derivative works or by other 
 works in which the Apple Software may be incorporated. 
  
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE 
 MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION 
 THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS 
 FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND 
 OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS. 
  
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL 
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, 
 MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED 
 AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), 
 STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE 
 POSSIBILITY OF SUCH DAMAGE. 
  
 Copyright (C) 2013 Apple Inc. All Rights Reserved. 
  
 */

#import "TextViewController.h"
#import "AppDelegate.h"
#import "ViewController.h"

#include "Shell/Genesis.h"
#ifdef DEBUG
CGRect g_rectPos = CGRectMake(0, -101, 300, 100);
#else
CGRect g_rectPos = CGRectMake(0, -101, 300, 100);
#endif
float  g_textviewHeight = 60;
@implementation MyUITextView
@end

MyUITextView*     g_textEdit;
CGSize            g_sizeDevice;
int               g_maxCharCount = 100;
@interface TextViewController () <UITextViewDelegate>
@property (nonatomic, strong) MyUITextView *textView;
@end



#pragma mark -


@implementation TextViewController
+ (void) showKeyboard: (bool) bShow
{
    
    NSString* empty = @"";
    if (bShow) {
        g_textEdit.frame = g_rectPos;
        //[g_textEdit  setText:empty];
        //[g_textEdit  setText:@"edit"];
        [g_textEdit  setKeyboardType:UIKeyboardTypeDefault];
        g_textEdit.hidden = FALSE;
        [g_textEdit becomeFirstResponder];
    } else {
        [g_textEdit resignFirstResponder];

        g_textEdit.hidden = TRUE;

        [g_textEdit setText:empty];
        
        
    }
}

+ (void) setTextViewText:(const char*) content
{
    NSString* nsContent = [[NSString alloc] initWithUTF8String:content];
    [g_textEdit setText:nsContent];
}



- (void)setupTextView
{
	self.textView = [[UITextView alloc] initWithFrame:self.view.frame];
    g_textEdit = self.textView;
	self.textView.textColor = [UIColor blackColor];
	self.textView.font = [UIFont fontWithName:@"Arial" size:18.0];
	self.textView.delegate = self;
	self.textView.backgroundColor = [UIColor whiteColor];
    self.textView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    
	NSString *textToAdd = @"Now is the time for all good developers to come to serve their country.\n\nNow is the time for all good developers to come to serve their country.\r\rThis text view can also use attributed strings.";
    
    NSMutableAttributedString *attrString = [[NSMutableAttributedString alloc] initWithString:textToAdd];

    // make red text
//    [attrString addAttribute:NSForegroundColorAttributeName
//                   value:[UIColor redColor]
//                   range:NSMakeRange([attrString length] - 19, 19)];
//    
//    // make blue text
//    [attrString addAttribute:NSForegroundColorAttributeName
//                       value:[UIColor blueColor]
//                       range:NSMakeRange([attrString length] - 23, 3)];
//    [attrString addAttribute:NSUnderlineStyleAttributeName
//                       value:[NSNumber numberWithInteger:1]
//                       range:NSMakeRange([attrString length] - 23, 3)];
    
    //[self.textView setAttributedText:attrString];
    
	self.textView.returnKeyType = UIReturnKeyDefault;
	self.textView.keyboardType = UIKeyboardTypeDefault;	// use the default type input method (entire keyboard)
	self.textView.scrollEnabled = YES;

	// note: for UITextView, if you don't like auto correction while typing use:
	// myTextView.autocorrectionType = UITextAutocorrectionTypeNo;
	
	//[self.view addSubview:self.textView];
    self.view = self.textView;
    
    CGSize sizeDevice = [[UIScreen mainScreen] bounds].size;
    g_sizeDevice = sizeDevice;
    g_rectPos = CGRectMake(0, sizeDevice.height, sizeDevice.width, 60);
    
    self.view.frame = g_rectPos;
}

- (void)viewDidLoad
{
	[super viewDidLoad];
	
	self.title = NSLocalizedString(@"TextViewTitle", @"");
	[self setupTextView];
}

- (void)viewWillAppear:(BOOL)animated
{
	[super viewWillAppear:animated];
	
    // listen for keyboard hide/show notifications so we can properly adjust the table's height
	[[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillShow:)
                                                 name:UIKeyboardWillShowNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillHide:)
                                                 name:UIKeyboardWillHideNotification
                                               object:nil];
}


#pragma mark - Notifications

- (void)adjustViewForKeyboardReveal:(BOOL)showKeyboard notificationInfo:(NSDictionary *)notificationInfo
{
    // the keyboard is showing so resize the table's height

}

- (void)keyboardWillShow:(NSNotification *)notification
{
    //NSLog(@" textviewctrl keyboardWillShow");
 
	[self adjustViewForKeyboardReveal:YES notificationInfo:[notification userInfo]];
    CGRect keyboardRect;
    [[notification.userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] getValue:&keyboardRect];
    
    AppDelegate* pApp = (AppDelegate*)[ [UIApplication sharedApplication] delegate ];
    CGRect finalRectRoot = [pApp.viewController.view convertRect:keyboardRect fromView:nil];
    
    g_textviewHeight = keyboardRect.size.height/5;
    
    CGRect kbPos = CGRectMake(finalRectRoot.origin.x, finalRectRoot.origin.y-g_textviewHeight, finalRectRoot.size.width, finalRectRoot.size.height);
    self.view.frame = kbPos;   
    
    
}

- (void)keyboardWillHide:(NSNotification *)aNotification
{
    [self adjustViewForKeyboardReveal:NO notificationInfo:[aNotification userInfo]];
}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
	
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UIKeyboardWillShowNotification
                                                  object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                    name:UIKeyboardWillHideNotification
                                                  object:nil];
}


#pragma mark - UITextViewDelegate

- (void)saveAction:(id)sender
{
	// finish typing text/dismiss the keyboard by removing it as the first responder
	//
	[self.textView resignFirstResponder];
	self.navigationItem.rightBarButtonItem = nil;	// this will remove the "save" button
}

- (void)textViewDidBeginEditing:(UITextView *)textView
{
	// provide my own Save button to dismiss the keyboard
	UIBarButtonItem* saveItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
															target:self
															action:@selector(saveAction:)];
	self.navigationItem.rightBarButtonItem = saveItem;
}
- (void)textViewDidEndEditing:(UITextView *)textView
{    
    
    NSString* strText = textView.text;
    const char* pStr = [strText UTF8String];
    EngineShell::UIInsertText(pStr);
    
    [TextViewController showKeyboard:false ];
}
- (BOOL) textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    if([text isEqualToString:@"\n"])
    {
        [g_textEdit resignFirstResponder];
        g_textEdit.hidden = TRUE;
        
        
        return NO;
    }
    else if( range.location >= g_maxCharCount )
    {
        return NO;
    }
  
    return YES;
    
    
}






@end


