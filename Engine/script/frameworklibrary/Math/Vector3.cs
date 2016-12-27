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
    /// 三维向量
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct Vector3 : IEquatable<Vector3>
    {
        /// <summary>
        /// 获取或设置向量的X分量
        /// </summary>
        public float X;
        /// <summary>
        /// 获取或设置向量的Y分量
        /// </summary>
        public float Y;
        /// <summary>
        /// 获取或设置向量的Z分量
        /// </summary>
        public float Z;
        private static Vector3 _zero;
        private static Vector3 _one;
        private static Vector3 _unitX;
        private static Vector3 _unitY;
        private static Vector3 _unitZ;
        private static Vector3 _up;
        private static Vector3 _down;
        private static Vector3 _right;
        private static Vector3 _left;
        private static Vector3 _forward;
        private static Vector3 _backward;

        /// <summary>
        /// 返回一个分量为0的三维向量
        /// </summary>
        public static Vector3 Zero
        {
            get
            {
                return _zero;
            }
        }
        /// <summary>
        /// 返回一个分量为1的三维向量
        /// </summary>
        public static Vector3 One
        {
            get
            {
                return _one;
            }
        }
        /// <summary>
        /// 返回X轴方向上的单位向量 (1, 0, 0).
        /// </summary>
        public static Vector3 UnitX
        {
            get
            {
                return _unitX;
            }
        }
        /// <summary>
        /// 返回Y轴方向上的单位向量 (0, 1, 0). 
        /// </summary>
        public static Vector3 UnitY
        {
            get
            {
                return _unitY;
            }
        }
        /// <summary>
        /// 返回Z轴方向上的单位向量 (0, 0, 1). 
        /// </summary>
        public static Vector3 UnitZ
        {
            get
            {
                return _unitZ;
            }
        }
        /// <summary>
        /// 返回一个单位向量指定向上 (0, 1, 0).
        /// </summary>
        public static Vector3 Up
        {
            get
            {
                return _up;
            }
        }
        /// <summary>
        /// 返回一个单位向量指定向下 (0, −1, 0).
        /// </summary>
        public static Vector3 Down
        {
            get
            {
                return _down;
            }
        }
        /// <summary>
        /// 返回一个单位向量指定向右 (1, 0, 0).
        /// </summary>
        public static Vector3 Right
        {
            get
            {
                return _right;
            }
        }
        /// <summary>
        /// 返回一个单位向量指定向左 (−1, 0, 0).
        /// </summary>
        public static Vector3 Left
        {
            get
            {
                return _left;
            }
        }
        /// <summary>
        /// 返回一个单位向量在右坐标系指定前方向(0, 0, −1).
        /// </summary>
        public static Vector3 Forward
        {
            get
            {
                return _forward;
            }
        }
        /// <summary>
        /// 返回一个单位向量在右手坐标系指定后方向 (0, 0, 1).
        /// </summary>
        public static Vector3 Backward
        {
            get
            {
                return _backward;
            }
        }
        /// <summary>
        /// 初始化一个三维向量实例 
        /// </summary>
        /// <param name="x">初始化X轴方向的值</param>
        /// <param name="y">初始化Y轴方向的值</param>
        /// <param name="z">初始化Z轴方向的值</param>
        public Vector3(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }
        /// <summary>
        /// 初始化一个三维向量实例
        /// </summary>
        /// <param name="value">初始化X轴方向Y轴方向Z轴方向同为的值</param>
        public Vector3(float value)
        {
            this.X = this.Y = this.Z = value;
        }
        /// <summary>
        /// 初始化一个三维向量实例
        /// </summary>
        /// <param name="value">一个向量包含初始化X轴方向上和Y轴方向的值</param>
        /// <param name="z">初始化Z轴方向的值</param>
        public Vector3(Vector2 value, float z)
        {
            this.X = value.X;
            this.Y = value.Y;
            this.Z = z;
        }
        /// <summary>
        /// 使用[0],[1],[3]分别对应X轴分量，Y轴分量，Z轴分量.
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
            return string.Format(currentCulture, "{0}, {1}, {2}", new object[] { this.X.ToString(currentCulture), this.Y.ToString(currentCulture), this.Z.ToString(currentCulture) });
        }
        /// <summary>
        /// 判断当前向量与指定向量是否相等
        /// </summary>
        /// <param name="other">指定的二维向量</param>
        /// <returns>返回是否相等</returns>
        public bool Equals(Vector3 other)
        {
            return (((this.X == other.X) && (this.Y == other.Y)) && (this.Z == other.Z));
        }
        /// <summary>
        /// 判断当前实例是否等于指定对象
        /// </summary>
        /// <param name="obj">指定的对象</param>
        /// <returns>返回是否相等</returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is Vector3)
            {
                flag = this.Equals((Vector3) obj);
            }
            return flag;
        }
        /// <summary>
        /// 获取向量对象的哈希码
        /// </summary>
        /// <returns>向量对象的哈希码</returns>
        public override int GetHashCode()
        {
            return ((this.X.GetHashCode() + this.Y.GetHashCode()) + this.Z.GetHashCode());
        }
        /// <summary>
        /// 计算向量模
        /// </summary>
        /// <returns>向量的模</returns>
        public float Length()
        {
            float num = ((this.X * this.X) + (this.Y * this.Y)) + (this.Z * this.Z);
            return (float) Math.Sqrt((double) num);
        }
        /// <summary>
        /// 计算向量长度的平方值
        /// </summary>
        /// <returns>向量长度的平方值</returns>
        public float LengthSquared()
        {
            return (((this.X * this.X) + (this.Y * this.Y)) + (this.Z * this.Z));
        }
        /// <summary>
        /// 计算两个三维向量之间的距离
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个三维向量之间的距离</returns>
        public static float Distance(Vector3 value1, Vector3 value2)
        {
            float num3 = value1.X - value2.X;
            float num2 = value1.Y - value2.Y;
            float num = value1.Z - value2.Z;
            float num4 = ((num3 * num3) + (num2 * num2)) + (num * num);
            return (float) Math.Sqrt((double) num4);
        }
        /// <summary>
        /// 计算两个三维向量之间的距离
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 两个三维向量之间的距离</param>
        public static void Distance(ref Vector3 value1, ref Vector3 value2, out float result)
        {
            float num3 = value1.X - value2.X;
            float num2 = value1.Y - value2.Y;
            float num = value1.Z - value2.Z;
            float num4 = ((num3 * num3) + (num2 * num2)) + (num * num);
            result = (float)Math.Sqrt((double)num4);
        }
        /// <summary>
        /// 计算两个三维向量之间的距离的平方
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个三维向量之间的距离的平方</returns>
        public static float DistanceSquared(Vector3 value1, Vector3 value2)
        {
            float num3 = value1.X - value2.X;
            float num2 = value1.Y - value2.Y;
            float num = value1.Z - value2.Z;
            return (((num3 * num3) + (num2 * num2)) + (num * num));
        }
        /// <summary>
        /// 计算两个三维向量之间的距离的平方
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 两个三维向量之间的距离的平方</param>
        public static void DistanceSquared(ref Vector3 value1, ref Vector3 value2, out float result)
        {
            float num3 = value1.X - value2.X;
            float num2 = value1.Y - value2.Y;
            float num = value1.Z - value2.Z;
            result = ((num3 * num3) + (num2 * num2)) + (num * num);
        }
        /// <summary>
        /// 计算两个向量的点积，如果两个向量是单位向量, 所得乘积是-1到1之间的浮点值 可以用来确定两个向量之间的一些夹角，例如, 它可以表明向量是否正交，平行，或者夹角是锐角还是钝角 
        /// </summary>
        /// <param name="vector1">源向量</param>
        /// <param name="vector2">源向量</param>
        /// <returns>两个向量的点积</returns>
        public static float Dot(Vector3 vector1, Vector3 vector2)
        {
            return (((vector1.X * vector2.X) + (vector1.Y * vector2.Y)) + (vector1.Z * vector2.Z));
        }
        /// <summary>
        /// 计算两个向量的点积，如果两个向量是单位向量, 所得乘积是-1到1之间的浮点值 可以用来确定两个向量之间的一些夹角，例如, 它可以表明向量是否正交，平行，或者夹角是锐角还是钝角 
        /// </summary>
        /// <param name="vector1">源向量</param>
        /// <param name="vector2">源向量</param>
        /// <param name="result">[输出属性] 两个向量的点积</param>
        public static void Dot(ref Vector3 vector1, ref Vector3 vector2, out float result)
        {
            result = ((vector1.X * vector2.X) + (vector1.Y * vector2.Y)) + (vector1.Z * vector2.Z);
        }
        /// <summary>
        /// 返回当前向量的单位向量，方向保持一致
        /// </summary>
        public void Normalize()
        {
            float num2 = ((this.X * this.X) + (this.Y * this.Y)) + (this.Z * this.Z);
            float num = 1f / ((float) Math.Sqrt((double) num2));
            this.X *= num;
            this.Y *= num;
            this.Z *= num;
        }
        /// <summary>
        /// 返回指定向量的单位向量，方向保持一致
        /// </summary>
        /// <param name="value">源向量</param>
        /// <returns>被创建的单位向量</returns>
        public static Vector3 Normalize(Vector3 value)
        {
            Vector3 vector;
            float num2 = ((value.X * value.X) + (value.Y * value.Y)) + (value.Z * value.Z);
            float num = 1f / ((float) Math.Sqrt((double) num2));
            vector.X = value.X * num;
            vector.Y = value.Y * num;
            vector.Z = value.Z * num;
            return vector;
        }
        /// <summary>
        /// 指定向量的单位向量，方向保持一致，用户指定变量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="result">[输出属性] 标准化向量</param>
        public static void Normalize(ref Vector3 value, out Vector3 result)
        {
            float num2 = ((value.X * value.X) + (value.Y * value.Y)) + (value.Z * value.Z);
            float num = 1f / ((float)Math.Sqrt((double)num2));
            result.X = value.X * num;
            result.Y = value.Y * num;
            result.Z = value.Z * num;
        }
        /// <summary>
        /// 计算两个向量的叉积 
        /// </summary>
        /// <param name="vector1">源向量</param>
        /// <param name="vector2">源向量</param>
        /// <returns>两个向量的叉积</returns>
        public static Vector3 Cross(Vector3 vector1, Vector3 vector2)
        {
            Vector3 vector;
            vector.X = (vector1.Y * vector2.Z) - (vector1.Z * vector2.Y);
            vector.Y = (vector1.Z * vector2.X) - (vector1.X * vector2.Z);
            vector.Z = (vector1.X * vector2.Y) - (vector1.Y * vector2.X);
            return vector;
        }
        /// <summary>
        /// 计算两个向量的叉积
        /// </summary>
        /// <param name="vector1">源向量</param>
        /// <param name="vector2">源向量</param>
        /// <param name="result">[输出属性] 两个向量的叉积</param>
        public static void Cross(ref Vector3 vector1, ref Vector3 vector2, out Vector3 result)
        {
            float num3 = (vector1.Y * vector2.Z) - (vector1.Z * vector2.Y);
            float num2 = (vector1.Z * vector2.X) - (vector1.X * vector2.Z);
            float num = (vector1.X * vector2.Y) - (vector1.Y * vector2.X);
            result.X = num3;
            result.Y = num2;
            result.Z = num;
        }
        /// <summary>
        /// 给出向量和法线 确定反射向量 
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="normal">法线向量</param>
        /// <returns>求得的反射向量</returns>
        public static Vector3 Reflect(Vector3 vector, Vector3 normal)
        {
            Vector3 vector2;
            float num = ((vector.X * normal.X) + (vector.Y * normal.Y)) + (vector.Z * normal.Z);
            vector2.X = vector.X - ((2f * num) * normal.X);
            vector2.Y = vector.Y - ((2f * num) * normal.Y);
            vector2.Z = vector.Z - ((2f * num) * normal.Z);
            return vector2;
        }
        /// <summary>
        /// 给出向量和法线 确定反射向量 
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="normal">法线向量</param>
        /// <param name="result">[输出属性] 求得的反射向量</param>
        public static void Reflect(ref Vector3 vector, ref Vector3 normal, out Vector3 result)
        {
            float num = ((vector.X * normal.X) + (vector.Y * normal.Y)) + (vector.Z * normal.Z);
            result.X = vector.X - ((2f * num) * normal.X);
            result.Y = vector.Y - ((2f * num) * normal.Y);
            result.Z = vector.Z - ((2f * num) * normal.Z);
        }
        /// <summary>
        /// 返回一个向量，包含每对向量分量中最小值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>最小向量</returns>
        public static Vector3 Min(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = (value1.X < value2.X) ? value1.X : value2.X;
            vector.Y = (value1.Y < value2.Y) ? value1.Y : value2.Y;
            vector.Z = (value1.Z < value2.Z) ? value1.Z : value2.Z;
            return vector;
        }
        /// <summary>
        /// 返回一个向量，包含每对向量分量中最小值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 最小向量</param>
        public static void Min(ref Vector3 value1, ref Vector3 value2, out Vector3 result)
        {
            result.X = (value1.X < value2.X) ? value1.X : value2.X;
            result.Y = (value1.Y < value2.Y) ? value1.Y : value2.Y;
            result.Z = (value1.Z < value2.Z) ? value1.Z : value2.Z;
        }
        /// <summary>
        /// 返回一个向量，包含每对向量分量中最大值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>最大向量</returns>
        public static Vector3 Max(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = (value1.X > value2.X) ? value1.X : value2.X;
            vector.Y = (value1.Y > value2.Y) ? value1.Y : value2.Y;
            vector.Z = (value1.Z > value2.Z) ? value1.Z : value2.Z;
            return vector;
        }
        /// <summary>
        /// 返回一个向量，包含每对向量分量中最大值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出属性] 最大向量</param>
        public static void Max(ref Vector3 value1, ref Vector3 value2, out Vector3 result)
        {
            result.X = (value1.X > value2.X) ? value1.X : value2.X;
            result.Y = (value1.Y > value2.Y) ? value1.Y : value2.Y;
            result.Z = (value1.Z > value2.Z) ? value1.Z : value2.Z;
        }
        /// <summary>
        /// 在规定的范围内限定一个值
        /// </summary>
        /// <param name="value1">需要限定的向量</param>
        /// <param name="min">最小值</param>
        /// <param name="max">最大值</param>
        /// <returns>限定后的向量</returns>
        public static Vector3 Clamp(Vector3 value1, Vector3 min, Vector3 max)
        {
            Vector3 vector;
            float x = value1.X;
            x = (x > max.X) ? max.X : x;
            x = (x < min.X) ? min.X : x;
            float y = value1.Y;
            y = (y > max.Y) ? max.Y : y;
            y = (y < min.Y) ? min.Y : y;
            float z = value1.Z;
            z = (z > max.Z) ? max.Z : z;
            z = (z < min.Z) ? min.Z : z;
            vector.X = x;
            vector.Y = y;
            vector.Z = z;
            return vector;
        }
        /// <summary>
        /// 在规定的范围内限定一个值
        /// </summary>
        /// <param name="value1">需要限定的向量</param>
        /// <param name="min">最小值</param>
        /// <param name="max">最大值</param>
        /// <param name="result">[输出属性] 限定后的向量</param>
        public static void Clamp(ref Vector3 value1, ref Vector3 min, ref Vector3 max, out Vector3 result)
        {
            float x = value1.X;
            x = (x > max.X) ? max.X : x;
            x = (x < min.X) ? min.X : x;
            float y = value1.Y;
            y = (y > max.Y) ? max.Y : y;
            y = (y < min.Y) ? min.Y : y;
            float z = value1.Z;
            z = (z > max.Z) ? max.Z : z;
            z = (z < min.Z) ? min.Z : z;
            result.X = x;
            result.Y = y;
            result.Z = z;
        }
        /// <summary>
        /// 执行两个向量之间的线性插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">表明value2的权重，数字0到1之间</param>
        /// <returns>两个向量之间的线性插值</returns>
        public static Vector3 Lerp(Vector3 value1, Vector3 value2, float amount)
        {
            Vector3 vector;
            vector.X = value1.X + ((value2.X - value1.X) * amount);
            vector.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            vector.Z = value1.Z + ((value2.Z - value1.Z) * amount);
            return vector;
        }
        /// <summary>
        /// 执行两个向量之间的线性插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">表明value2的权重，数字0到1之间</param>
        /// <param name="result">[输出属性] 两个向量之间的线性插值</param>
        public static void Lerp(ref Vector3 value1, ref Vector3 value2, float amount, out Vector3 result)
        {
            result.X = value1.X + ((value2.X - value1.X) * amount);
            result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            result.Z = value1.Z + ((value2.Z - value1.Z) * amount);
        }
        /// <summary>
        /// 用立方等式在两个值之间插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">权重</param>
        /// <returns>计算结果</returns>
        public static Vector3 SmoothStep(Vector3 value1, Vector3 value2, float amount)
        {
            Vector3 vector;
            amount = (amount > 1f) ? 1f : ((amount < 0f) ? 0f : amount);
            amount = (amount * amount) * (3f - (2f * amount));
            vector.X = value1.X + ((value2.X - value1.X) * amount);
            vector.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            vector.Z = value1.Z + ((value2.Z - value1.Z) * amount);
            return vector;
        }
        /// <summary>
        /// 用立方等式在两个值之间插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">权重</param>
        /// <param name="result">[输出属性] 计算结果</param>
        public static void SmoothStep(ref Vector3 value1, ref Vector3 value2, float amount, out Vector3 result)
        {
            amount = (amount > 1f) ? 1f : ((amount < 0f) ? 0f : amount);
            amount = (amount * amount) * (3f - (2f * amount));
            result.X = value1.X + ((value2.X - value1.X) * amount);
            result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            result.Z = value1.Z + ((value2.Z - value1.Z) * amount);
        }
        /// <summary>
        /// 实现Hermite样条插值
        /// </summary>
        /// <param name="value1">源位置向量</param>
        /// <param name="tangent1">源切向量</param>
        /// <param name="value2">源位置向量</param>
        /// <param name="tangent2">源切向量</param>
        /// <param name="amount">权重因子</param>
        /// <returns>Hermite样条插值的结果</returns>
        public static Vector3 Hermite(Vector3 value1, Vector3 tangent1, Vector3 value2, Vector3 tangent2, float amount)
        {
            Vector3 vector;
            float num = amount * amount;
            float num2 = amount * num;
            float num6 = ((2f * num2) - (3f * num)) + 1f;
            float num5 = (-2f * num2) + (3f * num);
            float num4 = (num2 - (2f * num)) + amount;
            float num3 = num2 - num;
            vector.X = (((value1.X * num6) + (value2.X * num5)) + (tangent1.X * num4)) + (tangent2.X * num3);
            vector.Y = (((value1.Y * num6) + (value2.Y * num5)) + (tangent1.Y * num4)) + (tangent2.Y * num3);
            vector.Z = (((value1.Z * num6) + (value2.Z * num5)) + (tangent1.Z * num4)) + (tangent2.Z * num3);
            return vector;
        }
        /// <summary>
        /// 实现Hermite样条插值
        /// </summary>
        /// <param name="value1">源位置向量</param>
        /// <param name="tangent1">源切向量</param>
        /// <param name="value2">源位置向量</param>
        /// <param name="tangent2">源切向量</param>
        /// <param name="amount">权重因子</param>
        /// <param name="result">[输出参数]Hermite样条插值的结果</param>
        public static void Hermite(ref Vector3 value1, ref Vector3 tangent1, ref Vector3 value2, ref Vector3 tangent2, float amount, out Vector3 result)
        {
            float num = amount * amount;
            float num2 = amount * num;
            float num6 = ((2f * num2) - (3f * num)) + 1f;
            float num5 = (-2f * num2) + (3f * num);
            float num4 = (num2 - (2f * num)) + amount;
            float num3 = num2 - num;
            result.X = (((value1.X * num6) + (value2.X * num5)) + (tangent1.X * num4)) + (tangent2.X * num3);
            result.Y = (((value1.Y * num6) + (value2.Y * num5)) + (tangent1.Y * num4)) + (tangent2.Y * num3);
            result.Z = (((value1.Z * num6) + (value2.Z * num5)) + (tangent1.Z * num4)) + (tangent2.Z * num3);
        }
        /// <summary>
        /// 返回反向向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <returns>反向向量</returns>
        public static Vector3 Negate(Vector3 value)
        {
            Vector3 vector;
            vector.X = -value.X;
            vector.Y = -value.Y;
            vector.Z = -value.Z;
            return vector;
        }
        /// <summary>
        /// 返回反向向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="result">[输出参数]反向向量</param>
        public static void Negate(ref Vector3 value, out Vector3 result)
        {
            result.X = -value.X;
            result.Y = -value.Y;
            result.Z = -value.Z;
        }
        /// <summary>
        /// 向量求和
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个源向量之和</returns>
        public static Vector3 Add(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = value1.X + value2.X;
            vector.Y = value1.Y + value2.Y;
            vector.Z = value1.Z + value2.Z;
            return vector;
        }
        /// <summary>
        /// 向量求和
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]两个源向量之和</param>
        public static void Add(ref Vector3 value1, ref Vector3 value2, out Vector3 result)
        {
            result.X = value1.X + value2.X;
            result.Y = value1.Y + value2.Y;
            result.Z = value1.Z + value2.Z;
        }
        /// <summary>
        /// 向量相减
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>向量相减的结果</returns>
        public static Vector3 Sub(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = value1.X - value2.X;
            vector.Y = value1.Y - value2.Y;
            vector.Z = value1.Z - value2.Z;
            return vector;
        }
        /// <summary>
        /// 向量相减
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]向量相减的结果</param>
        public static void Sub(ref Vector3 value1, ref Vector3 value2, out Vector3 result)
        {
            result.X = value1.X - value2.X;
            result.Y = value1.Y - value2.Y;
            result.Z = value1.Z - value2.Z;
        }
        /// <summary>
        /// 两个向量对应元素相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>返回相乘结果</returns>
        public static Vector3 Multiply(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = value1.X * value2.X;
            vector.Y = value1.Y * value2.Y;
            vector.Z = value1.Z * value2.Z;
            return vector;
        }
        /// <summary>
        /// 两个向量对应元素相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]返回相乘结果</param>
        public static void Multiply(ref Vector3 value1, ref Vector3 value2, out Vector3 result)
        {
            result.X = value1.X * value2.X;
            result.Y = value1.Y * value2.Y;
            result.Z = value1.Z * value2.Z;
        }
        /// <summary>
        /// 向量数乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <returns>数乘结果</returns>
        public static Vector3 Multiply(Vector3 value1, float scaleFactor)
        {
            Vector3 vector;
            vector.X = value1.X * scaleFactor;
            vector.Y = value1.Y * scaleFactor;
            vector.Z = value1.Z * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 向量数乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <param name="result">[输出参数]数乘结果</param>
        public static void Multiply(ref Vector3 value1, float scaleFactor, out Vector3 result)
        {
            result.X = value1.X * scaleFactor;
            result.Y = value1.Y * scaleFactor;
            result.Z = value1.Z * scaleFactor;
        }
        /// <summary>
        /// 两个向量对应元素相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除数向量</param>
        /// <returns>返回相除结果</returns>
        public static Vector3 Divide(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = value1.X / value2.X;
            vector.Y = value1.Y / value2.Y;
            vector.Z = value1.Z / value2.Z;
            return vector;
        }
        /// <summary>
        /// 两个向量对应元素相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除数向量</param>
        /// <param name="result">[输出向量] 返回相除结果</param>
        public static void Divide(ref Vector3 value1, ref Vector3 value2, out Vector3 result)
        {
            result.X = value1.X / value2.X;
            result.Y = value1.Y / value2.Y;
            result.Z = value1.Z / value2.Z;
        }
        /// <summary>
        /// 向量与标量相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="divider">标量值</param>
        /// <returns>相除结果</returns>
        public static Vector3 Divide(Vector3 value1, float divider)
        {
            Vector3 vector;
            float num = 1f / divider;
            vector.X = value1.X * num;
            vector.Y = value1.Y * num;
            vector.Z = value1.Z * num;
            return vector;
        }
        /// <summary>
        /// 向量与标量相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="divider">标量值</param>
        /// <param name="result">[输出参数]相除结果</param>
        public static void Divide(ref Vector3 value1, float divider, out Vector3 result)
        {
            float num = 1f / divider;
            result.X = value1.X * num;
            result.Y = value1.Y * num;
            result.Z = value1.Z * num;
        }

        private static float magnitude(ref Vector3 inV)
        {
            return (float)Math.Sqrt((double)Dot(inV, inV));
        }
        private static float k1OverSqrt2 = 0.7071067811865475244008443621048490f;
        private static readonly float epsilon = 0.00001F;
        private static Vector3 orthoNormalVectorFast(ref Vector3 n)
        {
            Vector3 res;
            if (Math.Abs(n.Z) > k1OverSqrt2)
            {
                // choose p in Y-Z plane
                float a = n.Y * n.Y + n.Z * n.Z;
                float k = 1.0F / (float)Math.Sqrt((double)a);
                res.X = 0;
                res.Y = -n.Z * k;
                res.Z = n.Y * k;
            }
            else
            {
                // choose p in X-Y plane
                float a = n.X * n.X + n.Y * n.Y;
                float k = 1.0F / (float)Math.Sqrt((double)a);
                res.X = -n.Y * k;
                res.Y = n.X * k;
                res.Z = 0;
            }
            return res;
        }
        /// <summary>
        /// 正交化两个向量。以切向量为参考，使得尽可能接近法向量
        /// </summary>
        /// <param name="normal">法向量</param>
        /// <param name="tangent">切向量</param>
        public static void OrthoNormalize(ref Vector3 normal, ref Vector3 tangent)
        {
            // compute u0
            float mag = magnitude(ref normal);
            if (mag > epsilon)
                normal /= mag;
            else
                normal = new Vector3(1.0F, 0.0F, 0.0F);

            // compute u1
            float dot0 = Dot(normal, tangent);
            tangent -= dot0 * normal;
            mag = magnitude(ref tangent);
            if (mag < epsilon)
                tangent = orthoNormalVectorFast(ref normal);
            else
                tangent /= mag;
        }
        /// <summary>
        /// 正交化三个向量
        /// </summary>
        /// <param name="normal">法向量</param>
        /// <param name="tangent">切向量</param>
        /// <param name="binormal">次法线向量</param>
        public static void OrthoNormalize(ref Vector3 normal, ref Vector3 tangent, ref Vector3 binormal)
        {
            // compute u0
            float mag = magnitude(ref normal);
            if (mag > epsilon)
                normal /= mag;
            else
                normal = new Vector3(1.0F, 0.0F, 0.0F);

            // compute u1
            float dot0 = Dot(normal, tangent);
            tangent -= dot0 * normal;
            mag = magnitude(ref tangent);
            if (mag > epsilon)
                tangent /= mag;
            else
                tangent = orthoNormalVectorFast(ref normal);

            // compute u2
            float dot1 = Dot(tangent, binormal);
            dot0 = Dot(normal, binormal);
            binormal -= dot0 * normal + dot1 * tangent;
            mag = magnitude(ref binormal);
            if (mag > epsilon)
                binormal /= mag;
            else
                binormal = Cross(normal, tangent);
        }
        /// <summary>
        /// 返回法向量的投影向量，若法向量模接近零，则返回零向量
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="onNormal">法向量</param>
        /// <returns>源向量相对于法向量的投影结果</returns>
        public static Vector3 Project(Vector3 vector, Vector3 onNormal)
        {
            return onNormal * Dot(vector, onNormal) / Dot(onNormal, onNormal);
        }
        /// <summary>
        /// 返回法向量的投影向量，若法向量模接近零，则返回零向量
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="onNormal">法向量</param>
        /// <param name="result">[输出参数]源向量相对于法向量的投影结果</param>
        public static void Project(ref Vector3 vector, ref Vector3 onNormal, out Vector3 result)
        {
            result = onNormal * Dot(vector, onNormal) / Dot(onNormal, onNormal);
        }
        /// <summary>
        /// 返回第一个向量到第二个向量的角度值
        /// </summary>
        /// <param name="from">源向量</param>
        /// <param name="to">源向量</param>
        /// <returns>第一个向量到第二个向量的角度值</returns>
        public static float Angle(Vector3 from, Vector3 to)
        {
            from.Normalize();
            to.Normalize();
            float dot;
            Vector3.Dot(ref from, ref to, out dot);
            return (MathHelper.ACos(MathHelper.Clamp(dot, -1f, 1f)) * 57.29578f);
        }
        /// <summary>
        /// 返回第一个向量到第二个向量的角度值
        /// </summary>
        /// <param name="from">源向量</param>
        /// <param name="to">源向量</param>
        /// <param name="result">[输出参数]第一个向量到第二个向量的角度值</param>
        public static void Angle(ref Vector3 from, ref Vector3 to, out float result)
        {
            from.Normalize();
            to.Normalize();
            float dot;
            Vector3.Dot(ref from, ref to, out dot);
            result = (MathHelper.ACos(MathHelper.Clamp(dot, -1f, 1f)) * 57.29578f);
        }
        /// <summary>
        /// 返回反向向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <returns>反向向量</returns>
        public static Vector3 operator -(Vector3 value)
        {
            Vector3 vector;
            vector.X = -value.X;
            vector.Y = -value.Y;
            vector.Z = -value.Z;
            return vector;
        }
        /// <summary>
        /// 判定两个向量相等
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>若两向量相等，则返回true；否则，返回false</returns>
        public static bool operator ==(Vector3 value1, Vector3 value2)
        {
            return (((value1.X == value2.X) && (value1.Y == value2.Y)) && (value1.Z == value2.Z));
        }
        /// <summary>
        /// 判定两个向量不相等
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>若两向量不相等，则返回true；否则，返回false</returns>
        public static bool operator !=(Vector3 value1, Vector3 value2)
        {
            if ((value1.X == value2.X) && (value1.Y == value2.Y))
            {
                return !(value1.Z == value2.Z);
            }
            return true;
        }
        /// <summary>
        /// 两个向量求和
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两向量之和</returns>
        public static Vector3 operator +(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = value1.X + value2.X;
            vector.Y = value1.Y + value2.Y;
            vector.Z = value1.Z + value2.Z;
            return vector;
        }
        /// <summary>
        /// 一个向量减去另一个向量
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>相减的结果</returns>
        public static Vector3 operator -(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = value1.X - value2.X;
            vector.Y = value1.Y - value2.Y;
            vector.Z = value1.Z - value2.Z;
            return vector;
        }
        /// <summary>
        /// 两个向量对应元素相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>相乘的结果</returns>
        public static Vector3 operator *(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = value1.X * value2.X;
            vector.Y = value1.Y * value2.Y;
            vector.Z = value1.Z * value2.Z;
            return vector;
        }
        /// <summary>
        /// 一个向量与一个标量相乘
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <returns>相乘的结果</returns>
        public static Vector3 operator *(Vector3 value, float scaleFactor)
        {
            Vector3 vector;
            vector.X = value.X * scaleFactor;
            vector.Y = value.Y * scaleFactor;
            vector.Z = value.Z * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 一个向量与一个标量相乘
        /// </summary>
        /// <param name="scaleFactor">标量值</param>
        /// <param name="value">源向量</param>
        /// <returns>相乘的结果</returns>
        public static Vector3 operator *(float scaleFactor, Vector3 value)
        {
            Vector3 vector;
            vector.X = value.X * scaleFactor;
            vector.Y = value.Y * scaleFactor;
            vector.Z = value.Z * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 一个向量的元素与另一个向量的对应元素相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除数向量</param>
        /// <returns>向量相除的结果</returns>
        public static Vector3 operator /(Vector3 value1, Vector3 value2)
        {
            Vector3 vector;
            vector.X = value1.X / value2.X;
            vector.Y = value1.Y / value2.Y;
            vector.Z = value1.Z / value2.Z;
            return vector;
        }
        /// <summary>
        /// 一个向量除以一个标量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="divider">除数</param>
        /// <returns>相除的结果</returns>
        public static Vector3 operator /(Vector3 value, float divider)
        {
            Vector3 vector;
            float num = 1f / divider;
            vector.X = value.X * num;
            vector.Y = value.Y * num;
            vector.Z = value.Z * num;
            return vector;
        }

        static Vector3()
        {
            _zero = new Vector3();
            _one = new Vector3(1f, 1f, 1f);
            _unitX = new Vector3(1f, 0f, 0f);
            _unitY = new Vector3(0f, 1f, 0f);
            _unitZ = new Vector3(0f, 0f, 1f);
            _up = new Vector3(0f, 1f, 0f);
            _down = new Vector3(0f, -1f, 0f);
            _right = new Vector3(1f, 0f, 0f);
            _left = new Vector3(-1f, 0f, 0f);
            _forward = new Vector3(0f, 0f, -1f);
            _backward = new Vector3(0f, 0f, 1f);
        }
    }
}

