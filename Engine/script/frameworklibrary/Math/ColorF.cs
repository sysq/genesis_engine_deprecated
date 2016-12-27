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
    /// 用红色分量、绿色分量、蓝色分量和透明度表示的四元颜色。每个分量值用0到1之间浮点数表示
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct ColorF : IEquatable<ColorF>
    {
        /// <summary>
        /// 获取或设置颜色的红色分量
        /// </summary>
        public float R;
        /// <summary>
        /// 获取或设置颜色的绿色分量
        /// </summary>
        public float G;
        /// <summary>
        /// 获取或设置颜色的蓝色分量
        /// </summary>
        public float B;
        /// <summary>
        /// 获取或设置颜色的透明度
        /// </summary>
        public float A;

        /// <summary>
        /// 创建颜色对象的一个实例
        /// </summary>
        /// <param name="r">红色分量</param>
        /// <param name="g">绿色分量</param>
        /// <param name="b">蓝色分量</param>
        /// <param name="a">透明度</param>
        public ColorF(float r, float g, float b, float a)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }
        /// <summary>
        /// 创建一个透明度为1.0f的颜色实例
        /// </summary>
        /// <param name="r">红色分量</param>
        /// <param name="g">绿色分量</param>
        /// <param name="b">蓝色分量</param>
        public ColorF(float r, float g, float b)
        {
            R = r;
            G = g;
            B = b;
            A = 1.0f;
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
            return (((this.R.GetHashCode() + this.G.GetHashCode()) + this.B.GetHashCode()) + this.A.GetHashCode());
        }
        /// <summary>
        /// 判定颜色对象实例是否与指定的对象相等
        /// </summary>
        /// <param name="obj">颜色对象</param>
        /// <returns>若当前实例与指定的对象相等，则返回true；否则，返回false</returns>
        public override bool Equals(object obj)
        {
            return ((obj is ColorF) && this.Equals((ColorF)obj));
        }
        /// <summary>
        /// 判定颜色是否与同一类型的颜色实例相等
        /// </summary>
        /// <param name="other">颜色四元组</param>
        /// <returns>如果两个颜色相等，则返回true；否则，返回false</returns>
        public bool Equals(ColorF other)
        {
            return ((((this.R == other.R) && (this.G == other.G)) && (this.B == other.B)) && (this.A == other.A));
        }
        /// <summary>
        /// 判定颜色相等操作
        /// </summary>
        /// <param name="a">颜色四元组</param>
        /// <param name="b">颜色四元组</param>
        /// <returns>如果两个颜色相同，则返回true；否则，返回false</returns>
        public static bool operator ==(ColorF value1, ColorF value2)
        {
            return ((((value1.R == value2.R) && (value1.G == value2.G)) && (value1.B == value2.B)) && (value1.A == value2.A));
        }
        /// <summary>
        /// 判定颜色不相等操作
        /// </summary>
        /// <param name="a">颜色四元组</param>
        /// <param name="b">颜色四元组</param>
        /// <returns>如果两个颜色不相同，则返回true；否则，返回false</returns>
        public static bool operator !=(ColorF value1, ColorF value2)
        {
            if (((value1.R == value2.R) && (value1.G == value2.G)) && (value1.B == value2.B))
            {
                return !(value1.A == value2.A);
            }
            return true;
        }
        /// <summary>
        /// 颜色相加
        /// </summary>
        /// <param name="value1">颜色四元组</param>
        /// <param name="value2">颜色四元组</param>
        /// <returns>两个颜色之和</returns>
        public static ColorF operator +(ColorF value1, ColorF value2)
        {
            ColorF vector;
            vector.R = value1.R + value2.R;
            vector.G = value1.G + value2.G;
            vector.B = value1.B + value2.B;
            vector.A = value1.A + value2.A;
            return vector;
        }
        /// <summary>
        /// 颜色相减
        /// </summary>
        /// <param name="value1">源颜色</param>
        /// <param name="value2">源颜色</param>
        /// <returns>相减的结果</returns>
        public static ColorF operator -(ColorF value1, ColorF value2)
        {
            ColorF vector;
            vector.R = value1.R - value2.R;
            vector.G = value1.G - value2.G;
            vector.B = value1.B - value2.B;
            vector.A = value1.A - value2.A;
            return vector;
        }
        /// <summary>
        /// 两个颜色对应分量相乘
        /// </summary>
        /// <param name="value1">源颜色</param>
        /// <param name="value2">源颜色</param>
        /// <returns>相乘的结果</returns>
        public static ColorF operator *(ColorF value1, ColorF value2)
        {
            ColorF vector;
            vector.R = value1.R * value2.R;
            vector.G = value1.G * value2.G;
            vector.B = value1.B * value2.B;
            vector.A = value1.A * value2.A;
            return vector;
        }
        /// <summary>
        /// 颜色与标量值相乘
        /// </summary>
        /// <param name="value1">源颜色</param>
        /// <param name="scaleFactor"></param>
        /// <returns>相乘的结果</returns>
        public static ColorF operator *(ColorF value1, float scaleFactor)
        {
            ColorF vector;
            vector.R = value1.R * scaleFactor;
            vector.G = value1.G * scaleFactor;
            vector.B = value1.B * scaleFactor;
            vector.A = value1.A * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 颜色与标量值相乘
        /// </summary>
        /// <param name="scaleFactor">标量值</param>
        /// <param name="value">源颜色</param>
        /// <returns>相乘的结果</returns>
        public static ColorF operator *(float scaleFactor, ColorF value)
        {
            ColorF vector;
            vector.R = value.R * scaleFactor;
            vector.G = value.G * scaleFactor;
            vector.B = value.B * scaleFactor;
            vector.A = value.A * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 两个颜色对应分量相除
        /// </summary>
        /// <param name="value1">源颜色</param>
        /// <param name="value2">相除的颜色</param>
        /// <returns>颜色相除的结果</returns>
        public static ColorF operator /(ColorF value1, ColorF value2)
        {
            ColorF vector;
            vector.R = value1.R / value2.R;
            vector.G = value1.G / value2.G;
            vector.B = value1.B / value2.B;
            vector.A = value1.A / value2.A;
            return vector;
        }
        /// <summary>
        /// 颜色与一个标量值相除
        /// </summary>
        /// <param name="value1">源颜色</param>
        /// <param name="divider">除数</param>
        /// <returns>相除的结果</returns>
        public static ColorF operator /(ColorF value1, float divider)
        {
            ColorF vector;
            float num = 1f / divider;
            vector.R = value1.R * num;
            vector.G = value1.G * num;
            vector.B = value1.B * num;
            vector.A = value1.A * num;
            return vector;
        }
        /// <summary>
        /// 获取对象的Color32表示
        /// </summary>
        /// <returns>对象的Color32表示</returns>
        public Color32 ToColor32()
        {
           return new Color32(PackHelper(R, G, B, A));
        }
        /// <summary>
        /// 获取对象的四元向量表示
        /// </summary>
        /// <returns>对象的四元向量表示</returns>
        public Vector4 ToVector4()
        {
            return new Vector4(R, G, B, A);
        }

        private static uint PackHelper(float vectorX, float vectorY, float vectorZ, float vectorW)
        {
            uint num4 = PackUtils.PackUNorm(255f, vectorX);
            uint num3 = PackUtils.PackUNorm(255f, vectorY) << 8;
            uint num2 = PackUtils.PackUNorm(255f, vectorZ) << 0x10;
            uint num = PackUtils.PackUNorm(255f, vectorW) << 0x18;
            return (((num4 | num3) | num2) | num);
        }
    }
}


