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
using System.Runtime.CompilerServices;
using ScriptRuntime;

namespace ScriptRuntime
{
    /// <summary>
    /// ��ͨ���������
    /// ��Ҫ����������Ч
    /// Ҫʹ��������Ч,�������������
    /// </summary>
    public partial class SoundLowPassFilterComponent : Component
    {
        public static readonly System.Type thisType = typeof(SoundLowPassFilterComponent);

        private SoundLowPassFilterComponent(DummyClass__ dummy)
        {

        }

        /// <summary>
        /// ��ͨ����������Ĺ��캯��
        /// </summary>
        protected SoundLowPassFilterComponent()
        {
            ICall_SoundLowPassFilterComponent_Bind(this);
        }

        /// <summary>
        /// ��ȡ�����õ�ͨ����������������
        /// </summary>
        public float Gain
        {
            get
            {
                return ICall_SoundLowPassFilterComponent_GetGain(this);
            }
            set
            {
                ICall_SoundLowPassFilterComponent_SetGain(this, value);
            }
        }

        /// <summary>
        /// ��ȡ�����õ�ͨ�������ĸ�Ƶ��������
        /// </summary>
        public float GainHF
        {
            get
            {
                return ICall_SoundLowPassFilterComponent_GetGainHF(this);
            }
            set
            {
                ICall_SoundLowPassFilterComponent_SetGainHF(this, value);
            }
        }
    }

}