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
using System;
using System.Collections.Generic;
using System.Text;

namespace ScriptGUI
{
    internal class Layout //: IDisposable
    {
        internal Layout(FString name, String file_name, bool visible)
        {
            mFileName = file_name;
            mLayoutName = name;
            mVisible = visible;
        }
        internal Layout(Widget parent, FString name, String file_name, bool visible)
        {
            mFileName = file_name;
            mLayoutName = name;
            mVisible = visible;
            mParent = parent;

            if (null != mParent)
            {
                Layout parentLayout = mParent.ParentLayout;
                if (null != parentLayout)
                {
                    parentLayout.addChildLayout(this);
                }
            }
        }
        ~Layout()
        {
            //Unload();
        }
        internal bool IsLoaded
        {
            get
            {
                return (null != mWidget);
            }
        }
        internal FString Name
        {
            get
            {
                return mLayoutName;
            }
        }
        internal Widget Root
        {
            get
            {            
                return mWidget;
            }

        }
        internal ExecuteResult Load()
        {
            
            if (null != mParent && (!mParent.IsValid))
            {
                return ExecuteResult.ParentNotExist;
            }
            Instance inst = GUI.LoadLayout(mParent, mFileName);
            if (inst.IsValid)
            {
                mWidget = Widget.CreateWidget(inst, null, this);
                mWidget.Visible = mVisible;
                return ExecuteResult.Success; 
            }
            return ExecuteResult.False;
        }
        internal void Unload()
        {
            if (IsLoaded)
            {
                if (mParent == null || mParent.IsValid)
                {
                    mChilds.UnloadAll();
                    mWidget.Dispose(); 
                }
                mWidget = null;
            }
        }
        internal void Reload()
        {
            Unload();
            Load();
        }
        //internal virtual void Dispose()
        //{
        //    mWidget.Dispose();
        //    mWidget = null;
        //}
        internal bool Visible
        {
            set
            {
                mVisible = value;
                mWidget.Visible = value;
            }
         
        }

        internal bool FindWidget(FString widget_name, out Widget widget)
        {
            if (!IsLoaded)
            {
                widget = null;
                return false;
            }
            if (mWidget.Name == widget_name)
            {
                widget = mWidget;
                return true;
            }

            //先从子layout找，因为，如果widget_name，在子layout里，
            //widget_name的脚本对象必须由子layout管理，要不然会悲剧。
            //
            foreach (Layout ly in mChildLayouts)
            {
                if (ly.FindWidget(widget_name, out widget))
                {
                    return true;
                }
            }

            if (mChilds.GetWidget(widget_name, out widget))
            {
                return true;
            }
            else
            {
                Instance inst = GUI.FindWidget(mWidget.Instance.Ptr, widget_name.Name);
                if (inst.IsValid)
                {
                    widget = Widget.CreateWidget(inst, widget_name.Name, this);
                    mChilds.Add(widget);
                    return true;
                }
                else
                {
                    widget = null;
                    return false;
                }
            }
        }

        internal bool Contains(FString widget_name)
        {
            return mChilds.Contains(widget_name);
        }
        internal bool TryGetWidget(Instance widget_inst, out Widget widget)
        {
            return mChilds.TryGetWidget(widget_inst, out widget);
        }


        internal void AddWidget(Widget wiget)
        {
            mChilds.Add(wiget);
        }
        internal void RemoveWidget(FString widget_name)
        {
            mChilds.Remove(widget_name);
        }

        protected void addChildLayout(Layout ly)
        {
            if (!mChildLayouts.Contains(ly))
            {
                mChildLayouts.Add(ly);
            }
        }

        protected String mFileName;
        protected FString mLayoutName;
        protected Widget mWidget;
        protected bool mVisible;
        protected Widget mParent;
        protected List<Layout> mChildLayouts = new List<Layout>();
        protected WidgetCollection mChilds = new WidgetCollection();
    }
}
