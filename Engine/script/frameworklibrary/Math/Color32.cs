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
namespace ScriptRuntime
{
    
    using ScriptRuntime.Graphics.PackedVector;
    using System;
    using System.ComponentModel;
    using System.Globalization;
    using System.Runtime.InteropServices;
    /// <summary>
    /// 用红色分量、绿色分量、蓝色分量和透明度表示的四元颜色
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct Color32 : IEquatable<Color32>
    {
        private uint packedValue;
        internal Color32(uint packedValue)
        {
            this.packedValue = packedValue;
        }
        /// <summary>
        /// 创建类的实例
        /// </summary>
        /// <param name="r">红色分量参数</param>
        /// <param name="g">绿色分量参数</param>
        /// <param name="b">蓝色分量参数</param>
        /// <param name="a">透明度参数</param>
        public Color32(int r, int g, int b, int a)
        {
            if (((((r | g) | b) | a) & -256) != 0)
            {
                r = ClampToByte32(r);
                g = ClampToByte32(g);
                b = ClampToByte32(b);
                a = ClampToByte32(a);
            }
            g = g << 8;
            b = b << 0x10;
            a = a << 0x18;
            this.packedValue = (uint) (((r | g) | b) | a);
        }
        /// <summary>
        /// 获取或设置红色分量值
        /// </summary>
        public byte R
        {
            get
            {
                return (byte) this.packedValue;
            }
            set
            {
                this.packedValue = (this.packedValue & 0xffffff00) | value;
            }
        }
        /// <summary>
        /// 获取或设置绿色分量值
        /// </summary>
        public byte G
        {
            get
            {
                return (byte) (this.packedValue >> 8);
            }
            set
            {
                this.packedValue = (this.packedValue & 0xffff00ff) | ((uint) (value << 8));
            }
        }
        /// <summary>
        /// 获取或设置蓝色分量值
        /// </summary>
        public byte B
        {
            get
            {
                return (byte) (this.packedValue >> 0x10);
            }
            set
            {
                this.packedValue = (this.packedValue & 0xff00ffff) | ((uint) (value << 0x10));
            }
        }
        /// <summary>
        /// 获取或设置颜色透明度
        /// </summary>
        public byte A
        {
            get
            {
                return (byte) (this.packedValue >> 0x18);
            }
            set
            {
                this.packedValue = (this.packedValue & 0xffffff) | ((uint) (value << 0x18));
            }
        }
        //[CLSCompliant(false)]
        /// <summary>
        /// 获取或设置打包后的值
        /// </summary>
        public uint PackedValue
        {
            get
            {
                return this.packedValue;
            }
            set
            {
                this.packedValue = value;
            }
        }
        /// <summary>
        /// 颜色的线性插值
        /// </summary>
        /// <param name="value1">颜色四元组</param>
        /// <param name="value2">颜色四元组</param>
        /// <param name="amount">插值因子</param>
        /// <returns>颜色四元组</returns>
        public static Color32 Lerp(Color32 value1, Color32 value2, float amount)
        {
            Color32 color;
            uint packedValue = value1.packedValue;
            uint num2 = value2.packedValue;
            int num7 = (byte) packedValue;
            int num6 = (byte) (packedValue >> 8);
            int num5 = (byte) (packedValue >> 0x10);
            int num4 = (byte) (packedValue >> 0x18);
            int num15 = (byte) num2;
            int num14 = (byte) (num2 >> 8);
            int num13 = (byte) (num2 >> 0x10);
            int num12 = (byte) (num2 >> 0x18);
            int num = (int) PackUtils.PackUNorm(65536f, amount);
            int num11 = num7 + (((num15 - num7) * num) >> 0x10);
            int num10 = num6 + (((num14 - num6) * num) >> 0x10);
            int num9 = num5 + (((num13 - num5) * num) >> 0x10);
            int num8 = num4 + (((num12 - num4) * num) >> 0x10);
            color.packedValue = (uint) (((num11 | (num10 << 8)) | (num9 << 0x10)) | (num8 << 0x18));
            return color;
        }
        /// <summary>
        /// 乘法操作
        /// </summary>
        /// <param name="value">颜色四元组</param>
        /// <param name="scale">缩放因子</param>
        /// <returns>颜色四元组</returns>
        public static Color32 Multiply(Color32 value, float scale)
        {
            uint num;
            Color32 color;
            uint packedValue = value.packedValue;
            uint num5 = (byte) packedValue;
            uint num4 = (byte) (packedValue >> 8);
            uint num3 = (byte) (packedValue >> 0x10);
            uint num2 = (byte) (packedValue >> 0x18);
            scale *= 65536f;
            if (scale < 0f)
            {
                num = 0;
            }
            else if (scale > 1.677722E+07f)
            {
                num = 0xffffff;
            }
            else
            {
                num = (uint) scale;
            }
            num5 = (num5 * num) >> 0x10;
            num4 = (num4 * num) >> 0x10;
            num3 = (num3 * num) >> 0x10;
            num2 = (num2 * num) >> 0x10;
            if (num5 > 0xff)
            {
                num5 = 0xff;
            }
            if (num4 > 0xff)
            {
                num4 = 0xff;
            }
            if (num3 > 0xff)
            {
                num3 = 0xff;
            }
            if (num2 > 0xff)
            {
                num2 = 0xff;
            }
            color.packedValue = ((num5 | (num4 << 8)) | (num3 << 0x10)) | (num2 << 0x18);
            return color;
        }
        /// <summary>
        /// 乘法操作
        /// </summary>
        /// <param name="value">颜色四元组</param>
        /// <param name="scale">缩放因子</param>
        /// <returns>颜色四元组</returns>
        public static Color32 operator *(Color32 value, float scale)
        {
            uint num;
            Color32 color;
            uint packedValue = value.packedValue;
            uint num5 = (byte) packedValue;
            uint num4 = (byte) (packedValue >> 8);
            uint num3 = (byte) (packedValue >> 0x10);
            uint num2 = (byte) (packedValue >> 0x18);
            scale *= 65536f;
            if (scale < 0f)
            {
                num = 0;
            }
            else if (scale > 1.677722E+07f)
            {
                num = 0xffffff;
            }
            else
            {
                num = (uint) scale;
            }
            num5 = (num5 * num) >> 0x10;
            num4 = (num4 * num) >> 0x10;
            num3 = (num3 * num) >> 0x10;
            num2 = (num2 * num) >> 0x10;
            if (num5 > 0xff)
            {
                num5 = 0xff;
            }
            if (num4 > 0xff)
            {
                num4 = 0xff;
            }
            if (num3 > 0xff)
            {
                num3 = 0xff;
            }
            if (num2 > 0xff)
            {
                num2 = 0xff;
            }
            color.packedValue = ((num5 | (num4 << 8)) | (num3 << 0x10)) | (num2 << 0x18);
            return color;
        }
        /// <summary>
        /// 返回当前对象的字符串表示
        /// </summary>
        /// <returns>对象的字符串表示</returns>
        public override string ToString()
        {
            return string.Format(CultureInfo.CurrentCulture, "{R:{0} G:{1} B:{2} A:{3}}", new object[] { this.R, this.G, this.B, this.A });
        }
        /// <summary>
        /// 为特殊类型提供一种哈希函数
        /// </summary>
        /// <returns>哈希值</returns>
        public override int GetHashCode()
        {
            return this.packedValue.GetHashCode();
        }
        /// <summary>
        /// 判定颜色对象实例是否与指定的对象相等
        /// </summary>
        /// <param name="obj">颜色对象</param>
        /// <returns>若当前实例与指定的对象相等，则返回true；否则，返回false</returns>
        public override bool Equals(object obj)
        {
            return ((obj is Color32) && this.Equals((Color32) obj));
        }
        /// <summary>
        /// 判定颜色是否与同一类型的颜色实例相等
        /// </summary>
        /// <param name="other">颜色四元组</param>
        /// <returns>如果两个颜色相等，则返回true；否则，返回false</returns>
        public bool Equals(Color32 other)
        {
            return this.packedValue.Equals(other.packedValue);
        }
        /// <summary>
        /// 判定颜色相等操作
        /// </summary>
        /// <param name="a">颜色四元组</param>
        /// <param name="b">颜色四元组</param>
        /// <returns>如果两个颜色相同，则返回true；否则，返回false</returns>
        public static bool operator ==(Color32 a, Color32 b)
        {
            return a.Equals(b);
        }
        /// <summary>
        /// 判定颜色不相等操作
        /// </summary>
        /// <param name="a">颜色四元组</param>
        /// <param name="b">颜色四元组</param>
        /// <returns>如果两个颜色不相同，则返回true；否则，返回false</returns>
        public static bool operator !=(Color32 a, Color32 b)
        {
            return !a.Equals(b);
        }
        /// <summary>
        /// 获取系统定义的颜色值 R:0 G:0 B:0 A:255
        /// </summary>
        public static Color32 Black
        {
            get
            {
                return new Color32(0xff000000);
            }
        }
        /// <summary>
        /// 获取系统定义的颜色值 R:0 G:0 B:255 A:255
        /// </summary>
        public static Color32 Blue
        {
            get
            {
                return new Color32(0xffff0000);
            }
        }
        /// <summary>
        /// 获取系统定义的颜色值 R:0 G:255 B:0 A:255
        /// </summary>
        public static Color32 Green
        {
            get
            {
                return new Color32(0xff00ff00);
            }
        }
        /// <summary>
        /// 获取系统定义的颜色值 R:255 G:0 B:0 A:255
        /// </summary>
        public static Color32 Red
        {
            get
            {
                return new Color32(0xff0000ff);
            }
        }
       /// <summary>
       /// 获取系统定义的颜色值 R:255 G:255 B:255 A:255
       /// </summary>
        public static Color32 White
        {
            get
            {
                return new Color32(uint.MaxValue);
            }
        }
        /// <summary>
        /// 获取该对象的ColorF表示
        /// </summary>
        /// <returns>对象的ColorF表示</returns>
        public ColorF ToColorF()
        {
            ColorF colorF;
            colorF.R = PackUtils.UnpackUNorm(0xff, this.packedValue);
            colorF.G = PackUtils.UnpackUNorm(0xff, this.packedValue >> 8);
            colorF.B = PackUtils.UnpackUNorm(0xff, this.packedValue >> 0x10);
            colorF.A = PackUtils.UnpackUNorm(0xff, this.packedValue >> 0x18);
            return colorF;
        }

        private static int ClampToByte32(int value)
        {
            if (value < 0)
            {
                return 0;
            }
            if (value > 0xff)
            {
                return 0xff;
            }
            return value;
        }
    }
}

