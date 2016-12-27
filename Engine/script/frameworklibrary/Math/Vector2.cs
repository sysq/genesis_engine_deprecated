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
    
    using System;
    using System.ComponentModel;
    using System.Globalization;
    using System.Runtime.InteropServices;
    /// <summary>
    /// 二维向量
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct Vector2 : IEquatable<Vector2>
    {
        /// <summary>
        /// 获取或设置X分量
        /// </summary>
        public float X;
        /// <summary>
        /// 获取或设置Y分量
        /// </summary>
        public float Y;
        private static Vector2 _zero;
        private static Vector2 _one;
        private static Vector2 _unitX;
        private static Vector2 _unitY;

        /// <summary>       
        /// 返回一个分量为0的二维向量
        /// </summary>
        public static Vector2 Zero
        {
            get
            {
                return _zero;
            }
        }

        /// <summary>
        /// 返回一个分量为1的二维向量
        /// </summary>
        public static Vector2 One
        {
            get
            {
                return _one;
            }
        }
        /// <summary>
        /// 返回X轴方向上的单位向量
        /// </summary>
        public static Vector2 UnitX
        {
            get
            {
                return _unitX;
            }
        }
        /// <summary>
        /// 返回Y轴方向上的单位向量
        /// </summary>
        public static Vector2 UnitY
        {
            get
            {
                return _unitY;
            }
        }
        /// <summary>
        /// 初始化一个二维向量实例
        /// </summary>
        /// <param name="x">初始化X轴方向的值</param>
        /// <param name="y">初始化Y轴方向的值</param>
        public Vector2(float x, float y)
        {
            this.X = x;
            this.Y = y;
        }
        /// <summary>
        /// 创建一个新的二维向量实例
        /// </summary>
        /// <param name="value">初始化X轴方向Y轴方向同为的值</param>
        public Vector2(float value)
        {
            this.X = this.Y = value;
        }

        /// <summary>
        /// 使用[0],[1]分别对应X轴分量，Y轴分量
        /// </summary>
        /// <param name="index">想要获取分量的索引</param>
        /// <returns>想要获取的分量</returns>
        public unsafe float this[int index]
        {
            
            get
            {
                fixed (float* th = &(this.X))
                {
                    return th[index];
                }
            }
            set
            {
                fixed (float* th = &(this.X))
                {
                    th[index] = value;
                }
            }
        }
        /// <summary>
        /// 用字符串表示
        /// </summary>
        /// <returns>表示当前对象的字符串</returns>
        public override string ToString()
        {
            CultureInfo currentCulture = CultureInfo.CurrentCulture;
            return string.Format(currentCulture, "{0}, {1}", new object[] { this.X.ToString(currentCulture), this.Y.ToString(currentCulture) });
        }
       
        /// <summary>
        /// 判断当前向量与指定向量是否相等
        /// </summary>
        /// <param name="other">指定的二维向量</param>
        /// <returns>返回是否相等</returns>
        public bool Equals(Vector2 other)
        {
            return ((this.X == other.X) && (this.Y == other.Y));
        }
        /// <summary>
        /// 判断是否当前实例等于指定对象 
        /// </summary>
        /// <param name="obj">指定的对象.</param>
        /// <returns>当前实例等于指定对象返回true，不等于返回false</returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is Vector2)
            {
                flag = this.Equals((Vector2) obj);
            }
            return flag;
        }
        /// <summary>
        /// 获取向量对象的哈希码
        /// </summary>
        /// <returns>向量对象的哈希吗</returns>
        public override int GetHashCode()
        {
            return (this.X.GetHashCode() + this.Y.GetHashCode());
        }
        /// <summary>
        /// 计算向量的模
        /// </summary>
        /// <returns>向量的模</returns>
        public float Length()
        {
            float num = (this.X * this.X) + (this.Y * this.Y);
            return (float)Math.Sqrt((double)num);
        }
        /// <summary>
        /// 计算向量长度值的平方
        /// </summary>
        /// <returns>向量长度的平方值</returns>
        public float LengthSquared()
        {
            return ((this.X * this.X) + (this.Y * this.Y));
        }
        /// <summary>
        /// 计算两个向量之间的距离
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个二维向量之间的距离</returns>
        public static float Distance(Vector2 value1, Vector2 value2)
        {
            float num2 = value1.X - value2.X;
            float num = value1.Y - value2.Y;
            float num3 = (num2 * num2) + (num * num);
            return (float) Math.Sqrt((double) num3);
        }
        /// <summary>
        /// 计算两个向量之间的距离
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 两个向量之间的距离</param>
        public static void Distance(ref Vector2 value1, ref Vector2 value2, out float result)
        {
            float num2 = value1.X - value2.X;
            float num = value1.Y - value2.Y;
            float num3 = (num2 * num2) + (num * num);
            result = (float)Math.Sqrt((double)num3);
        }
        /// <summary>
        /// 计算两个向量之间的距离的平方
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个向量之间的距离的平方</returns>
        public static float DistanceSquared(Vector2 value1, Vector2 value2)
        {
            float num2 = value1.X - value2.X;
            float num = value1.Y - value2.Y;
            return ((num2 * num2) + (num * num));
        }
        /// <summary>
        /// 计算两个向量之间的距离的平方
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 两个向量之间的距离的平方</param>
        public static void DistanceSquared(ref Vector2 value1, ref Vector2 value2, out float result)
        {
            float num2 = value1.X - value2.X;
            float num = value1.Y - value2.Y;
            result = (num2 * num2) + (num * num);
        }
        /// <summary>
        /// 返回指定向量的单位向量，方向保持一致
        /// </summary>
        public void Normalize()
        {
            float num2 = (this.X * this.X) + (this.Y * this.Y);
            float num = 1f / ((float) Math.Sqrt((double) num2));
            this.X *= num;
            this.Y *= num;
        }
        /// <summary>
        /// 返回指定向量的单位向量，方向保持一致
        /// </summary>
        /// <param name="value">源向量</param>
        /// <returns>被创建的单位向量</returns>
        public static Vector2 Normalize(Vector2 value)
        {
            Vector2 vector;
            float num2 = (value.X * value.X) + (value.Y * value.Y);
            float num = 1f / ((float) Math.Sqrt((double) num2));
            vector.X = value.X * num;
            vector.Y = value.Y * num;
            return vector;
        }
        /// <summary>
        /// 指定向量的单位向量，方向保持一致，用户指定变量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="result">[输出属性] 标准化向量</param>
        public static void Normalize(ref Vector2 value, out Vector2 result)
        {
            float num2 = (value.X * value.X) + (value.Y * value.Y);
            float num = 1f / ((float)Math.Sqrt((double)num2));
            result.X = value.X * num;
            result.Y = value.Y * num;
        }
        /// <summary>
        /// 给出向量和法线 确定反射向量 
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="normal">法线向量</param>
        /// <returns>求得的反射向量</returns>
        public static Vector2 Reflect(Vector2 vector, Vector2 normal)
        {
            Vector2 vector2;
            float num = (vector.X * normal.X) + (vector.Y * normal.Y);
            vector2.X = vector.X - ((2f * num) * normal.X);
            vector2.Y = vector.Y - ((2f * num) * normal.Y);
            return vector2;
        }
        /// <summary>
        /// 给出向量和法线 确定反射向量 
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="normal">法线向量</param>
        /// <param name="result">[输出属性] 求得的反射向量</param>
        public static void Reflect(ref Vector2 vector, ref Vector2 normal, out Vector2 result)
        {
            float num = (vector.X * normal.X) + (vector.Y * normal.Y);
            result.X = vector.X - ((2f * num) * normal.X);
            result.Y = vector.Y - ((2f * num) * normal.Y);
        }
        /// <summary>
        /// 返回一个向量，包含每对向量分量中最小值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>最小向量</returns>
        public static Vector2 Min(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = (value1.X < value2.X) ? value1.X : value2.X;
            vector.Y = (value1.Y < value2.Y) ? value1.Y : value2.Y;
            return vector;
        }
        /// <summary>
        /// 返回一个向量，包含每对向量分量中最小值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 最小向量</param>
        public static void Min(ref Vector2 value1, ref Vector2 value2, out Vector2 result)
        {
            result.X = (value1.X < value2.X) ? value1.X : value2.X;
            result.Y = (value1.Y < value2.Y) ? value1.Y : value2.Y;
        }
        /// <summary>
        /// 返回一个向量，包含每对向量分量中最大值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>最大向量</returns>
        public static Vector2 Max(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = (value1.X > value2.X) ? value1.X : value2.X;
            vector.Y = (value1.Y > value2.Y) ? value1.Y : value2.Y;
            return vector;
        }
        /// <summary>
        /// 返回一个向量，包含每对向量分量中最大值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 最大向量</param>
        public static void Max(ref Vector2 value1, ref Vector2 value2, out Vector2 result)
        {
            result.X = (value1.X > value2.X) ? value1.X : value2.X;
            result.Y = (value1.Y > value2.Y) ? value1.Y : value2.Y;
        }
        /// <summary>
        /// 在规定的范围内限定一个值
        /// </summary>
        /// <param name="value1">需要限定的向量</param>
        /// <param name="min">最小值</param>
        /// <param name="max">最大值</param>
        /// <returns>限定后的向量</returns>
        public static Vector2 Clamp(Vector2 value1, Vector2 min, Vector2 max)
        {
            Vector2 vector;
            float x = value1.X;
            x = (x > max.X) ? max.X : x;
            x = (x < min.X) ? min.X : x;
            float y = value1.Y;
            y = (y > max.Y) ? max.Y : y;
            y = (y < min.Y) ? min.Y : y;
            vector.X = x;
            vector.Y = y;
            return vector;
        }
        /// <summary>
        /// 在规定的范围内限定一个值
        /// </summary>
        /// <param name="value1">需要限定的向量</param>
        /// <param name="min">最小值.</param>
        /// <param name="max">最大值.</param>
        /// <param name="result">[输出属性] 限定后的向量</param>
        public static void Clamp(ref Vector2 value1, ref Vector2 min, ref Vector2 max, out Vector2 result)
        {
            float x = value1.X;
            x = (x > max.X) ? max.X : x;
            x = (x < min.X) ? min.X : x;
            float y = value1.Y;
            y = (y > max.Y) ? max.Y : y;
            y = (y < min.Y) ? min.Y : y;
            result.X = x;
            result.Y = y;
        }
        /// <summary>
        /// 执行两个向量之间的线性插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">表明value2的权重，数字0到1之间</param>
        /// <returns>两个向量之间的线性插值</returns>
        public static Vector2 Lerp(Vector2 value1, Vector2 value2, float amount)
        {
            Vector2 vector;
            vector.X = value1.X + ((value2.X - value1.X) * amount);
            vector.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            return vector;
        }
        /// <summary>
        /// 执行两个向量之间的线性插值 
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">表明value2的权重，数字0到1之间</param>
        /// <param name="result">[输出属性] 两个向量之间的线性插值</param>
        public static void Lerp(ref Vector2 value1, ref Vector2 value2, float amount, out Vector2 result)
        {
            result.X = value1.X + ((value2.X - value1.X) * amount);
            result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
        }
        /// <summary>
        /// 用立方等式在两个值之间插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">权重</param>
        /// <returns>计算结果</returns>
        public static Vector2 SmoothStep(Vector2 value1, Vector2 value2, float amount)
        {
            Vector2 vector;
            amount = (amount > 1f) ? 1f : ((amount < 0f) ? 0f : amount);
            amount = (amount * amount) * (3f - (2f * amount));
            vector.X = value1.X + ((value2.X - value1.X) * amount);
            vector.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            return vector;
        }
        /// <summary>
        /// 用立方等式在两个值之间插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">权重</param>
        /// <param name="result">[输出属性] 计算结果</param>
        public static void SmoothStep(ref Vector2 value1, ref Vector2 value2, float amount, out Vector2 result)
        {
            amount = (amount > 1f) ? 1f : ((amount < 0f) ? 0f : amount);
            amount = (amount * amount) * (3f - (2f * amount));
            result.X = value1.X + ((value2.X - value1.X) * amount);
            result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
        }
        /// <summary>
        /// 返回一个指向相反方向的向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <returns>指向相反方向的向量</returns>
        public static Vector2 Negate(Vector2 value)
        {
            Vector2 vector;
            vector.X = -value.X;
            vector.Y = -value.Y;
            return vector;
        }
        /// <summary>
        /// 返回一个指向相反方向的向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="result">[输出属性] 指向相反方向的向量</param>
        public static void Negate(ref Vector2 value, out Vector2 result)
        {
            result.X = -value.X;
            result.Y = -value.Y;
        }
        /// <summary>
        /// 计算两个向量的点积，如果两个向量是单位向量, 所得乘积是-1到1之间的浮点值 可以用来确定两个向量之间的一些夹角，例如, 它可以表明向量是否正交，平行，或者夹角是锐角还是钝角 
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个向量的点积</returns>
        public static float Dot(Vector2 value1, Vector2 value2)
        {
            return ((value1.X * value2.X) + (value1.Y * value2.Y));
        }
        /// <summary>
        /// 计算两个向量的点积，如果两个向量是单位向量, 所得乘积是-1到1之间的浮点值 可以用来确定两个向量之间的一些夹角，例如, 它可以表明向量是否正交，平行，或者夹角是锐角还是钝角  
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 两个向量的点积</param>
        public static void Dot(ref Vector2 value1, ref Vector2 value2, out float result)
        {
            result = (value1.X * value2.X) + (value1.Y * value2.Y);
        }
        /// <summary>
        /// 返回向量之间的夹角
        /// </summary>
        /// <param name="from">源向量.</param>
        /// <param name="to">源向量</param>
        /// <returns>[输出属性]向量之间夹角的角度值</returns>
        public static float Angle(Vector2 from, Vector2 to)
        {
            from.Normalize();
            to.Normalize();
            float dot;
            Vector2.Dot(ref from, ref to, out dot);
            return (MathHelper.ACos(MathHelper.Clamp(dot, -1f, 1f)) * 57.29578f);

        }
        /// <summary>
        /// 返回向量之间的夹角
        /// </summary>
        /// <param name="from">源向量</param>
        /// <param name="to">源向量</param>
        /// <param name="result">[输出属性] 向量之间夹角的角度值</param>
        public static void Angle(ref Vector2 from, ref Vector2 to, out float result)
        {
            from.Normalize();
            to.Normalize();
            float dot;
            Vector2.Dot(ref from, ref to, out dot);
            result = (MathHelper.ACos(MathHelper.Clamp(dot, -1f, 1f)) * 57.29578f);
        }
        /// <summary>
        /// 计算两个向量相加
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个向量相加的和</returns>
        public static Vector2 Add(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = value1.X + value2.X;
            vector.Y = value1.Y + value2.Y;
            return vector;
        }
        /// <summary>
        /// 计算两个向量相加
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 两个向量相加的和</param>
        public static void Add(ref Vector2 value1, ref Vector2 value2, out Vector2 result)
        {
            result.X = value1.X + value2.X;
            result.Y = value1.Y + value2.Y;
        }
        /// <summary>
        /// 计算两个向量相减
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个向量相减的差</returns>
        public static Vector2 Sub(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = value1.X - value2.X;
            vector.Y = value1.Y - value2.Y;
            return vector;
        }
        /// <summary>
        /// 计算两个向量相减
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 两个向量相减的差</param>
        public static void Sub(ref Vector2 value1, ref Vector2 value2, out Vector2 result)
        {
            result.X = value1.X - value2.X;
            result.Y = value1.Y - value2.Y;
        }
        /// <summary>
        /// 两个向量的分量对应相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>相乘的结果</returns>
        public static Vector2 Multiply(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = value1.X * value2.X;
            vector.Y = value1.Y * value2.Y;
            return vector;
        }
        /// <summary>
        /// 两个向量的分量对应相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 相乘的结果</param>
        public static void Multiply(ref Vector2 value1, ref Vector2 value2, out Vector2 result)
        {
            result.X = value1.X * value2.X;
            result.Y = value1.Y * value2.Y;
        }
        /// <summary>
        /// 向量乘以一个标量值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <returns>向量乘以一个标量值所得的乘积</returns>
        public static Vector2 Multiply(Vector2 value1, float scaleFactor)
        {
            Vector2 vector;
            vector.X = value1.X * scaleFactor;
            vector.Y = value1.Y * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 向量乘以一个标量值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <param name="result">[输出属性] 向量乘以一个标量值所得的乘积</param>
        public static void Multiply(ref Vector2 value1, float scaleFactor, out Vector2 result)
        {
            result.X = value1.X * scaleFactor;
            result.Y = value1.Y * scaleFactor;
        }
        /// <summary>
        /// 两向量的分量对应相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除向量</param>
        /// <returns>相除的结果</returns>
        public static Vector2 Divide(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = value1.X / value2.X;
            vector.Y = value1.Y / value2.Y;
            return vector;
        }
        /// <summary>
        /// 两向量的分量对应相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除向量</param>
        /// <param name="result">[输出属性] 相除的结果</param>
        public static void Divide(ref Vector2 value1, ref Vector2 value2, out Vector2 result)
        {
            result.X = value1.X / value2.X;
            result.Y = value1.Y / value2.Y;
        }
        /// <summary>
        /// 向量除以一个标量值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="divider">标量值</param>
        /// <returns>向量除以一个标量值所得的向量</returns>
        public static Vector2 Divide(Vector2 value1, float divider)
        {
            Vector2 vector;
            float num = 1f / divider;
            vector.X = value1.X * num;
            vector.Y = value1.Y * num;
            return vector;
        }
        /// <summary>
        /// 向量除以一个标量值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="divider">标量值</param>
        /// <param name="result">[输出属性] 向量除以一个标量值所得的向量.</param>
        public static void Divide(ref Vector2 value1, float divider, out Vector2 result)
        {
            float num = 1f / divider;
            result.X = value1.X * num;
            result.Y = value1.Y * num;
        }
        /// <summary>
        /// 返回一个指向相反方向的向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <returns>返回一个指向相反方向的向量</returns>
        public static Vector2 operator -(Vector2 value)
        {
            Vector2 vector;
            vector.X = -value.X;
            vector.Y = -value.Y;
            return vector;
        }
        /// <summary>
        /// 判断向量是否相等
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>如果相等返回true，不相等返回false</returns>
        public static bool operator ==(Vector2 value1, Vector2 value2)
        {
            return ((value1.X == value2.X) && (value1.Y == value2.Y));
        }
        /// <summary>
        /// 判断向量是否不等
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>如果不相等返回true，相等返回false</returns>
        public static bool operator !=(Vector2 value1, Vector2 value2)
        {
            if (value1.X == value2.X)
            {
                return !(value1.Y == value2.Y);
            }
            return true;
        }
        /// <summary>
        /// 两个向量相加
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个向量相加所得的和</returns>
        public static Vector2 operator +(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = value1.X + value2.X;
            vector.Y = value1.Y + value2.Y;
            return vector;
        }
        /// <summary>
        /// 两个向量相减
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个向量相减所得的差</returns>
        public static Vector2 operator -(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = value1.X - value2.X;
            vector.Y = value1.Y - value2.Y;
            return vector;
        }
        /// <summary>
        /// 两个向量的分量分别相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>相乘的结果</returns>
        public static Vector2 operator *(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = value1.X * value2.X;
            vector.Y = value1.Y * value2.Y;
            return vector;
        }
        /// <summary>
        /// 向量乘以一个标量值 
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <returns>向量乘以一个标量值所得的向量</returns>
        public static Vector2 operator *(Vector2 value, float scaleFactor)
        {
            Vector2 vector;
            vector.X = value.X * scaleFactor;
            vector.Y = value.Y * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 向量乘以一个标量值 
        /// </summary>
        /// <param name="scaleFactor">标量值</param>
        /// <param name="value">源向量</param>
        /// <returns>向量乘以一个标量值所得的向量</returns>
        public static Vector2 operator *(float scaleFactor, Vector2 value)
        {
            Vector2 vector;
            vector.X = value.X * scaleFactor;
            vector.Y = value.Y * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 两个向量的分量对应相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除向量</param>
        /// <returns>相除的结果</returns>
        public static Vector2 operator /(Vector2 value1, Vector2 value2)
        {
            Vector2 vector;
            vector.X = value1.X / value2.X;
            vector.Y = value1.Y / value2.Y;
            return vector;
        }
        /// <summary>
        /// 向量除以一个标量值    
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="divider">标量值</param>
        /// <returns>向量除以一个标量值所得的向量</returns>
        public static Vector2 operator /(Vector2 value1, float divider)
        {
            Vector2 vector;
            float num = 1f / divider;
            vector.X = value1.X * num;
            vector.Y = value1.Y * num;
            return vector;
        }
        static Vector2()
        {
            _zero = new Vector2();
            _one = new Vector2(1f, 1f);
            _unitX = new Vector2(1f, 0f);
            _unitY = new Vector2(0f, 1f);
        }
    }
}

