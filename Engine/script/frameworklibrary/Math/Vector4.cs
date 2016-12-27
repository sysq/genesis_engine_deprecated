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
    /// 四元向量
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct Vector4 : IEquatable<Vector4>
    {
        /// <summary>
        /// 获取或设置向量的x分量
        /// </summary>
        public float X;
        /// <summary>
        /// 获取或设置向量的y分量
        /// </summary>
        public float Y;
        /// <summary>
        /// 获取或设置向量的z分量
        /// </summary>
        public float Z;
        /// <summary>
        /// 获取或设置向量的w分量
        /// </summary>
        public float W;
        private static Vector4 _zero;
        private static Vector4 _one;
        private static Vector4 _unitX;
        private static Vector4 _unitY;
        private static Vector4 _unitZ;
        private static Vector4 _unitW;
        /// <summary>
        /// 返回所有元素置为零的四元向量
        /// </summary>
        public static Vector4 Zero
        {
            get
            {
                return _zero;
            }
        }
        /// <summary>
        /// 返回所有元素置为1的四元向量
        /// </summary>
        public static Vector4 One
        {
            get
            {
                return _one;
            }
        }
        /// <summary>
        /// 返回四元向量(1, 0, 0, 0). 
        /// </summary>
        public static Vector4 UnitX
        {
            get
            {
                return _unitX;
            }
        }
        /// <summary>
        /// 返回四元向量(0, 1, 0, 0). 
        /// </summary>
        public static Vector4 UnitY
        {
            get
            {
                return _unitY;
            }
        }
        /// <summary>
        /// 返回四元向量 (0, 0, 1, 0). 
        /// </summary>
        public static Vector4 UnitZ
        {
            get
            {
                return _unitZ;
            }
        }
        /// <summary>
        /// 返回四元向量 (0, 0, 0, 1). 
        /// </summary>
        public static Vector4 UnitW
        {
            get
            {
                return _unitW;
            }
        }
        /// <summary>
        /// 初始化一个四元向量实例
        /// </summary>
        /// <param name="x">初始化四元向量元素x的值</param>
        /// <param name="y">初始化四元向量元素y的值</param>
        /// <param name="z">初始化四元向量元素z的值</param>
        /// <param name="w">初始化四元向量元素w的值</param>
        public Vector4(float x, float y, float z, float w)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
            this.W = w;
        }
        /// <summary>
        /// 初始化一个四元向量实例
        /// </summary>
        /// <param name="value">同时初始化四元向量元素x和y的二元向量</param>
        /// <param name="z">初始化四元向量元素z的值</param>
        /// <param name="w">初始化四元向量元素w的值</param>
        public Vector4(Vector2 value, float z, float w)
        {
            this.X = value.X;
            this.Y = value.Y;
            this.Z = z;
            this.W = w;
        }
        /// <summary>
        /// 初始化一个四元向量实例
        /// </summary>
        /// <param name="value">同时初始化四元向量元素x、y和z的向量</param>
        /// <param name="w">初始化四元向量元素w的值</param>
        public Vector4(Vector3 value, float w)
        {
            this.X = value.X;
            this.Y = value.Y;
            this.Z = value.Z;
            this.W = w;
        }
        /// <summary>
        /// 创建一个四元向量实例
        /// </summary>
        /// <param name="value">初始化每一个元素</param>
        public Vector4(float value)
        {
            this.X = this.Y = this.Z = this.W = value;
        }
        /// <summary>
        /// 用[0],[1],[2]方式分别访问元素x、y和z的值
        /// </summary>
        /// <param name="index">对应元素的索引</param>
        /// <returns>对应的元素值</returns>
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
        /// 返回当前对象的字符串表示
        /// </summary>
        /// <returns>对象的字符串表示</returns>
        public override string ToString()
        {
            CultureInfo currentCulture = CultureInfo.CurrentCulture;
            return string.Format(currentCulture, "{0}, {1}, {2}, {3}", new object[] { this.X.ToString(currentCulture), this.Y.ToString(currentCulture), this.Z.ToString(currentCulture), this.W.ToString(currentCulture) });
        }
        /// <summary>
        /// 判定向量是否相等
        /// </summary>
        /// <param name="other">与当前向量比较的向量</param>
        /// <returns>若向量与当前向量相等，则返回true；否则，返回false</returns>
        public bool Equals(Vector4 other)
        {
            return ((((this.X == other.X) && (this.Y == other.Y)) && (this.Z == other.Z)) && (this.W == other.W));
        }
        /// <summary>
        /// 判定当前的实例是否与指定的对象相等
        /// </summary>
        /// <param name="obj">作比较的对象</param>
        /// <returns>若当前实例与指定的对象相等，则返回true；否则，返回false</returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is Vector4)
            {
                flag = this.Equals((Vector4) obj);
            }
            return flag;
        }
        /// <summary>
        /// 获取向量对象的哈希值
        /// </summary>
        /// <returns>向量对象的哈希值</returns>
        public override int GetHashCode()
        {
            return (((this.X.GetHashCode() + this.Y.GetHashCode()) + this.Z.GetHashCode()) + this.W.GetHashCode());
        }
        /// <summary>
        /// 计算向量的模
        /// </summary>
        /// <returns>向量的模</returns>
        public float Length()
        {
            float num = (((this.X * this.X) + (this.Y * this.Y)) + (this.Z * this.Z)) + (this.W * this.W);
            return (float) Math.Sqrt((double) num);
        }
        /// <summary>
        /// 计算向量模的平方
        /// </summary>
        /// <returns>向量模的平方</returns>
        public float LengthSquared()
        {
            return ((((this.X * this.X) + (this.Y * this.Y)) + (this.Z * this.Z)) + (this.W * this.W));
        }
        /// <summary>
        /// 计算两个向量之间的距离
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个向量之间的距离</returns>
        public static float Distance(Vector4 value1, Vector4 value2)
        {
            float num4 = value1.X - value2.X;
            float num3 = value1.Y - value2.Y;
            float num2 = value1.Z - value2.Z;
            float num = value1.W - value2.W;
            float num5 = (((num4 * num4) + (num3 * num3)) + (num2 * num2)) + (num * num);
            return (float) Math.Sqrt((double) num5);
        }
        /// <summary>
        /// 计算两个向量之间的距离
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]两个向量之间的距离</param>
        public static void Distance(ref Vector4 value1, ref Vector4 value2, out float result)
        {
            float num4 = value1.X - value2.X;
            float num3 = value1.Y - value2.Y;
            float num2 = value1.Z - value2.Z;
            float num = value1.W - value2.W;
            float num5 = (((num4 * num4) + (num3 * num3)) + (num2 * num2)) + (num * num);
            result = (float)Math.Sqrt((double)num5);
        }
        /// <summary>
        /// 计算两个向量之间距离的平方
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个向量之间距离的平方</returns>
        public static float DistanceSquared(Vector4 value1, Vector4 value2)
        {
            float num4 = value1.X - value2.X;
            float num3 = value1.Y - value2.Y;
            float num2 = value1.Z - value2.Z;
            float num = value1.W - value2.W;
            return ((((num4 * num4) + (num3 * num3)) + (num2 * num2)) + (num * num));
        }
        /// <summary>
        /// 计算两个向量之间距离的平方
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]两个向量之间距离的平方</param>
        public static void DistanceSquared(ref Vector4 value1, ref Vector4 value2, out float result)
        {
            float num4 = value1.X - value2.X;
            float num3 = value1.Y - value2.Y;
            float num2 = value1.Z - value2.Z;
            float num = value1.W - value2.W;
            result = (((num4 * num4) + (num3 * num3)) + (num2 * num2)) + (num * num);
        }
        /// <summary>
        /// 计算两个向量的点积。若两个向量是单位向量，则点积的结果为一个在-1到1之间的浮点数，该数值用来判定两个向量角度关系。例如，这个值可以表示两个向量是否正交，平行或存在一个锐角钝角的关系
        /// </summary>
        /// <param name="vector1">源向量</param>
        /// <param name="vector2">源向量</param>
        /// <returns>两个向量的点积</returns>
        public static float Dot(Vector4 vector1, Vector4 vector2)
        {
            return ((((vector1.X * vector2.X) + (vector1.Y * vector2.Y)) + (vector1.Z * vector2.Z)) + (vector1.W * vector2.W));
        }
        /// <summary>
        /// 计算两个向量的点积。若两个向量是单位向量，则点积的结果为一个在-1到1之间的浮点数，该数值用来判定两个向量角度关系。例如，这个值可以表示两个向量是否正交，平行或存在一个锐角钝角的关系
        /// </summary>
        /// <param name="vector1">源向量</param>
        /// <param name="vector2">源向量</param>
        /// <param name="result">[输出参数]两个向量的点积</param>
        public static void Dot(ref Vector4 vector1, ref Vector4 vector2, out float result)
        {
            result = (((vector1.X * vector2.X) + (vector1.Y * vector2.Y)) + (vector1.Z * vector2.Z)) + (vector1.W * vector2.W);
        }
        /// <summary>
        /// 转换当前向量为一个单位向量。该结果是一个长度为1且方向与原向量方向一致的向量
        /// </summary>
        public void Normalize()
        {
            float num2 = (((this.X * this.X) + (this.Y * this.Y)) + (this.Z * this.Z)) + (this.W * this.W);
            float num = 1f / ((float) Math.Sqrt((double) num2));
            this.X *= num;
            this.Y *= num;
            this.Z *= num;
            this.W *= num;
        }
        /// <summary>
        /// 根据指定的向量创建一个单位向量。该结果是一个长度为1且方向与原向量方向一致的向量
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <returns>创建的单位向量</returns>
        public static Vector4 Normalize(Vector4 vector)
        {
            Vector4 vector2;
            float num2 = (((vector.X * vector.X) + (vector.Y * vector.Y)) + (vector.Z * vector.Z)) + (vector.W * vector.W);
            float num = 1f / ((float) Math.Sqrt((double) num2));
            vector2.X = vector.X * num;
            vector2.Y = vector.Y * num;
            vector2.Z = vector.Z * num;
            vector2.W = vector.W * num;
            return vector2;
        }
        /// <summary>
        /// 根据指定的向量创建一个单位向量。该结果是一个长度为1且方向与原向量方向一致的向量
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="result">[输出参数]标准向量</param>
        public static void Normalize(ref Vector4 vector, out Vector4 result)
        {
            float num2 = (((vector.X * vector.X) + (vector.Y * vector.Y)) + (vector.Z * vector.Z)) + (vector.W * vector.W);
            float num = 1f / ((float)Math.Sqrt((double)num2));
            result.X = vector.X * num;
            result.Y = vector.Y * num;
            result.Z = vector.Z * num;
            result.W = vector.W * num;
        }
        /// <summary>
        /// 返回两个向量中对应元素相对较小的元素所构成的向量
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>最小化向量</returns>
        public static Vector4 Min(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = (value1.X < value2.X) ? value1.X : value2.X;
            vector.Y = (value1.Y < value2.Y) ? value1.Y : value2.Y;
            vector.Z = (value1.Z < value2.Z) ? value1.Z : value2.Z;
            vector.W = (value1.W < value2.W) ? value1.W : value2.W;
            return vector;
        }
        /// <summary>
        /// 返回两个向量中对应元素相对较小的元素所构成的向量
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]最小化向量</param>
        public static void Min(ref Vector4 value1, ref Vector4 value2, out Vector4 result)
        {
            result.X = (value1.X < value2.X) ? value1.X : value2.X;
            result.Y = (value1.Y < value2.Y) ? value1.Y : value2.Y;
            result.Z = (value1.Z < value2.Z) ? value1.Z : value2.Z;
            result.W = (value1.W < value2.W) ? value1.W : value2.W;
        }
        /// <summary>
        /// 返回两个向量中对应元素相对较大的元素所构成的向量
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>最大化向量</returns>
        public static Vector4 Max(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = (value1.X > value2.X) ? value1.X : value2.X;
            vector.Y = (value1.Y > value2.Y) ? value1.Y : value2.Y;
            vector.Z = (value1.Z > value2.Z) ? value1.Z : value2.Z;
            vector.W = (value1.W > value2.W) ? value1.W : value2.W;
            return vector;
        }
        /// <summary>
        /// 返回两个向量中对应元素相对较大的元素所构成的向量
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]最大化向量</param>
        public static void Max(ref Vector4 value1, ref Vector4 value2, out Vector4 result)
        {
            result.X = (value1.X > value2.X) ? value1.X : value2.X;
            result.Y = (value1.Y > value2.Y) ? value1.Y : value2.Y;
            result.Z = (value1.Z > value2.Z) ? value1.Z : value2.Z;
            result.W = (value1.W > value2.W) ? value1.W : value2.W;
        }
        /// <summary>
        /// 限定一个值在指定范围内
        /// </summary>
        /// <param name="value1">待限定的值</param>
        /// <param name="min">最小值</param>
        /// <param name="max">最大值</param>
        /// <returns>夹在范围内的值</returns>
        public static Vector4 Clamp(Vector4 value1, Vector4 min, Vector4 max)
        {
            Vector4 vector;
            float x = value1.X;
            x = (x > max.X) ? max.X : x;
            x = (x < min.X) ? min.X : x;
            float y = value1.Y;
            y = (y > max.Y) ? max.Y : y;
            y = (y < min.Y) ? min.Y : y;
            float z = value1.Z;
            z = (z > max.Z) ? max.Z : z;
            z = (z < min.Z) ? min.Z : z;
            float w = value1.W;
            w = (w > max.W) ? max.W : w;
            w = (w < min.W) ? min.W : w;
            vector.X = x;
            vector.Y = y;
            vector.Z = z;
            vector.W = w;
            return vector;
        }
        /// <summary>
        /// 限定一个值在指定范围内
        /// </summary>
        /// <param name="value1">待限定的值</param>
        /// <param name="min">最小值</param>
        /// <param name="max">最大值</param>
        /// <param name="result">[输出参数]夹在范围内的值</param>
        public static void Clamp(ref Vector4 value1, ref Vector4 min, ref Vector4 max, out Vector4 result)
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
            float w = value1.W;
            w = (w > max.W) ? max.W : w;
            w = (w < min.W) ? min.W : w;
            result.X = x;
            result.Y = y;
            result.Z = z;
            result.W = w;
        }
        /// <summary>
        /// 计算两个向量的线性插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">指示第二个向量的权重，范围在0到1之间</param>
        /// <returns>两个向量的线性插值</returns>
        public static Vector4 Lerp(Vector4 value1, Vector4 value2, float amount)
        {
            Vector4 vector;
            vector.X = value1.X + ((value2.X - value1.X) * amount);
            vector.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            vector.Z = value1.Z + ((value2.Z - value1.Z) * amount);
            vector.W = value1.W + ((value2.W - value1.W) * amount);
            return vector;
        }
        /// <summary>
        /// 计算两个向量的线性插值
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="amount">指示第二个向量的权重，范围在0到1之间</param>
        /// <param name="result">[输出参数]两个向量的线性插值</param>
        public static void Lerp(ref Vector4 value1, ref Vector4 value2, float amount, out Vector4 result)
        {
            result.X = value1.X + ((value2.X - value1.X) * amount);
            result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            result.Z = value1.Z + ((value2.Z - value1.Z) * amount);
            result.W = value1.W + ((value2.W - value1.W) * amount);
        }
        /// <summary>
        /// 用立方公式在两个值之间做插值
        /// </summary>
        /// <param name="value1">源值</param>
        /// <param name="value2">源值</param>
        /// <param name="amount">权重</param>
        /// <returns>插入的值</returns>
        public static Vector4 SmoothStep(Vector4 value1, Vector4 value2, float amount)
        {
            Vector4 vector;
            amount = (amount > 1f) ? 1f : ((amount < 0f) ? 0f : amount);
            amount = (amount * amount) * (3f - (2f * amount));
            vector.X = value1.X + ((value2.X - value1.X) * amount);
            vector.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            vector.Z = value1.Z + ((value2.Z - value1.Z) * amount);
            vector.W = value1.W + ((value2.W - value1.W) * amount);
            return vector;
        }
        /// <summary>
        /// 用立方公式在两个值之间做插值
        /// </summary>
        /// <param name="value1">源值</param>
        /// <param name="value2">源值</param>
        /// <param name="amount">权重</param>
        /// <param name="result">[输出参数]插入的值</param>
        public static void SmoothStep(ref Vector4 value1, ref Vector4 value2, float amount, out Vector4 result)
        {
            amount = (amount > 1f) ? 1f : ((amount < 0f) ? 0f : amount);
            amount = (amount * amount) * (3f - (2f * amount));
            result.X = value1.X + ((value2.X - value1.X) * amount);
            result.Y = value1.Y + ((value2.Y - value1.Y) * amount);
            result.Z = value1.Z + ((value2.Z - value1.Z) * amount);
            result.W = value1.W + ((value2.W - value1.W) * amount);
        }
        /// <summary>
        /// 计算埃尔米特样条插值
        /// </summary>
        /// <param name="value1">源位置向量</param>
        /// <param name="tangent1">源切向量</param>
        /// <param name="value2">源位置向量</param>
        /// <param name="tangent2">源切向量</param>
        /// <param name="amount">权重因子</param>
        /// <returns>埃尔米特样条插值的结果</returns>
        public static Vector4 Hermite(Vector4 value1, Vector4 tangent1, Vector4 value2, Vector4 tangent2, float amount)
        {
            Vector4 vector;
            float num = amount * amount;
            float num6 = amount * num;
            float num5 = ((2f * num6) - (3f * num)) + 1f;
            float num4 = (-2f * num6) + (3f * num);
            float num3 = (num6 - (2f * num)) + amount;
            float num2 = num6 - num;
            vector.X = (((value1.X * num5) + (value2.X * num4)) + (tangent1.X * num3)) + (tangent2.X * num2);
            vector.Y = (((value1.Y * num5) + (value2.Y * num4)) + (tangent1.Y * num3)) + (tangent2.Y * num2);
            vector.Z = (((value1.Z * num5) + (value2.Z * num4)) + (tangent1.Z * num3)) + (tangent2.Z * num2);
            vector.W = (((value1.W * num5) + (value2.W * num4)) + (tangent1.W * num3)) + (tangent2.W * num2);
            return vector;
        }
        /// <summary>
        /// 计算埃尔米特样条插值
        /// </summary>
        /// <param name="value1">源位置向量</param>
        /// <param name="tangent1">源切向量</param>
        /// <param name="value2">源位置向量</param>
        /// <param name="tangent2">源切向量</param>
        /// <param name="amount">权重因子</param>
        /// <param name="result">[输出参数]埃尔米特样条插值的结果</param>
        public static void Hermite(ref Vector4 value1, ref Vector4 tangent1, ref Vector4 value2, ref Vector4 tangent2, float amount, out Vector4 result)
        {
            float num = amount * amount;
            float num6 = amount * num;
            float num5 = ((2f * num6) - (3f * num)) + 1f;
            float num4 = (-2f * num6) + (3f * num);
            float num3 = (num6 - (2f * num)) + amount;
            float num2 = num6 - num;
            result.X = (((value1.X * num5) + (value2.X * num4)) + (tangent1.X * num3)) + (tangent2.X * num2);
            result.Y = (((value1.Y * num5) + (value2.Y * num4)) + (tangent1.Y * num3)) + (tangent2.Y * num2);
            result.Z = (((value1.Z * num5) + (value2.Z * num4)) + (tangent1.Z * num3)) + (tangent2.Z * num2);
            result.W = (((value1.W * num5) + (value2.W * num4)) + (tangent1.W * num3)) + (tangent2.W * num2);
        }
        /// <summary>
        /// 返回法向量的投影向量，若法向量模接近零，则返回零向量
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="onNormal">法向量</param>
        /// <returns>源向量相对于法向量的投影结果</returns>
        public static Vector4 Project(Vector4 vector, Vector4 onNormal)
        {
            return onNormal * Dot(vector, onNormal) / Dot(onNormal, onNormal);
        }
        /// <summary>
        /// 返回法向量的投影向量，若法向量模接近零，则返回零向量
        /// </summary>
        /// <param name="vector">源向量</param>
        /// <param name="onNormal">法向量</param>
        /// <param name="result">[输出参数]源向量相对于法向量的投影结果</param>
        public static void Project(ref Vector4 vector, ref Vector4 onNormal, out Vector4 result)
        {
            result = onNormal * Dot(vector, onNormal) / Dot(onNormal, onNormal);
        }
        /// <summary>
        /// 返回反向向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <returns>反向向量</returns>
        public static Vector4 Negate(Vector4 value)
        {
            Vector4 vector;
            vector.X = -value.X;
            vector.Y = -value.Y;
            vector.Z = -value.Z;
            vector.W = -value.W;
            return vector;
        }
        /// <summary>
        /// 返回反向向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="result">[输出参数]反向向量</param>
        public static void Negate(ref Vector4 value, out Vector4 result)
        {
            result.X = -value.X;
            result.Y = -value.Y;
            result.Z = -value.Z;
            result.W = -value.W;
        }
        /// <summary>
        /// 向量求和
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两个源向量之和</returns>
        public static Vector4 Add(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = value1.X + value2.X;
            vector.Y = value1.Y + value2.Y;
            vector.Z = value1.Z + value2.Z;
            vector.W = value1.W + value2.W;
            return vector;
        }
        /// <summary>
        /// 向量求和
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]两个源向量之和</param>
        public static void Add(ref Vector4 value1, ref Vector4 value2, out Vector4 result)
        {
            result.X = value1.X + value2.X;
            result.Y = value1.Y + value2.Y;
            result.Z = value1.Z + value2.Z;
            result.W = value1.W + value2.W;
        }
        /// <summary>
        /// 向量相减
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>向量相减的结果</returns>
        public static Vector4 Sub(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = value1.X - value2.X;
            vector.Y = value1.Y - value2.Y;
            vector.Z = value1.Z - value2.Z;
            vector.W = value1.W - value2.W;
            return vector;
        }
        /// <summary>
        /// 向量相减
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]向量相减的结果</param>
        public static void Sub(ref Vector4 value1, ref Vector4 value2, out Vector4 result)
        {
            result.X = value1.X - value2.X;
            result.Y = value1.Y - value2.Y;
            result.Z = value1.Z - value2.Z;
            result.W = value1.W - value2.W;
        }
        /// <summary>
        /// 两个向量对应元素相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>返回相乘结果</returns>
        public static Vector4 Multiply(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = value1.X * value2.X;
            vector.Y = value1.Y * value2.Y;
            vector.Z = value1.Z * value2.Z;
            vector.W = value1.W * value2.W;
            return vector;
        }
        /// <summary>
        /// 两个向量对应元素相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <param name="result">[输出参数]返回相乘结果</param>
        public static void Multiply(ref Vector4 value1, ref Vector4 value2, out Vector4 result)
        {
            result.X = value1.X * value2.X;
            result.Y = value1.Y * value2.Y;
            result.Z = value1.Z * value2.Z;
            result.W = value1.W * value2.W;
        }
        /// <summary>
        /// 向量数乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <returns>数乘结果</returns>
        public static Vector4 Multiply(Vector4 value1, float scaleFactor)
        {
            Vector4 vector;
            vector.X = value1.X * scaleFactor;
            vector.Y = value1.Y * scaleFactor;
            vector.Z = value1.Z * scaleFactor;
            vector.W = value1.W * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 向量数乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <param name="result">[输出参数]数乘结果</param>
        public static void Multiply(ref Vector4 value1, float scaleFactor, out Vector4 result)
        {
            result.X = value1.X * scaleFactor;
            result.Y = value1.Y * scaleFactor;
            result.Z = value1.Z * scaleFactor;
            result.W = value1.W * scaleFactor;
        }
        /// <summary>
        /// 两个向量对应元素相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除数向量</param>
        /// <returns>返回相除结果</returns>
        public static Vector4 Divide(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = value1.X / value2.X;
            vector.Y = value1.Y / value2.Y;
            vector.Z = value1.Z / value2.Z;
            vector.W = value1.W / value2.W;
            return vector;
        }
        /// <summary>
        /// 两个向量对应元素相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除数向量</param>
        /// <param name="result">[输出向量] 返回相除结果</param>
        public static void Divide(ref Vector4 value1, ref Vector4 value2, out Vector4 result)
        {
            result.X = value1.X / value2.X;
            result.Y = value1.Y / value2.Y;
            result.Z = value1.Z / value2.Z;
            result.W = value1.W / value2.W;
        }
        /// <summary>
        /// 向量与标量相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="divider">标量值</param>
        /// <returns>相除结果</returns>
        public static Vector4 Divide(Vector4 value1, float divider)
        {
            Vector4 vector;
            float num = 1f / divider;
            vector.X = value1.X * num;
            vector.Y = value1.Y * num;
            vector.Z = value1.Z * num;
            vector.W = value1.W * num;
            return vector;
        }
        /// <summary>
        /// 向量与标量相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="divider">标量值</param>
        /// <returns>相除结果</returns>
        public static void Divide(ref Vector4 value1, float divider, out Vector4 result)
        {
            float num = 1f / divider;
            result.X = value1.X * num;
            result.Y = value1.Y * num;
            result.Z = value1.Z * num;
            result.W = value1.W * num;
        }
        /// <summary>
        /// 返回反向向量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <returns>反向向量</returns>
        public static Vector4 operator -(Vector4 value)
        {
            Vector4 vector;
            vector.X = -value.X;
            vector.Y = -value.Y;
            vector.Z = -value.Z;
            vector.W = -value.W;
            return vector;
        }
        /// <summary>
        /// 判定两个向量相等
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>若两向量相等，则返回true；否则，返回false</returns>
        public static bool operator ==(Vector4 value1, Vector4 value2)
        {
            return ((((value1.X == value2.X) && (value1.Y == value2.Y)) && (value1.Z == value2.Z)) && (value1.W == value2.W));
        }
        /// <summary>
        /// 判定两个向量不相等
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>若两向量不相等，则返回true；否则，返回false</returns>
        public static bool operator !=(Vector4 value1, Vector4 value2)
        {
            if (((value1.X == value2.X) && (value1.Y == value2.Y)) && (value1.Z == value2.Z))
            {
                return !(value1.W == value2.W);
            }
            return true;
        }
        /// <summary>
        /// 两个向量求和
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>两向量之和</returns>
        public static Vector4 operator +(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = value1.X + value2.X;
            vector.Y = value1.Y + value2.Y;
            vector.Z = value1.Z + value2.Z;
            vector.W = value1.W + value2.W;
            return vector;
        }
        /// <summary>
        /// 一个向量减去另一个向量
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>相减的结果</returns>
        public static Vector4 operator -(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = value1.X - value2.X;
            vector.Y = value1.Y - value2.Y;
            vector.Z = value1.Z - value2.Z;
            vector.W = value1.W - value2.W;
            return vector;
        }
        /// <summary>
        /// 两个向量对应元素相乘
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">源向量</param>
        /// <returns>相乘的结果</returns>
        public static Vector4 operator *(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = value1.X * value2.X;
            vector.Y = value1.Y * value2.Y;
            vector.Z = value1.Z * value2.Z;
            vector.W = value1.W * value2.W;
            return vector;
        }
        /// <summary>
        /// 一个向量与一个标量相乘
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="scaleFactor">标量值</param>
        /// <returns>相乘的结果</returns>
        public static Vector4 operator *(Vector4 value1, float scaleFactor)
        {
            Vector4 vector;
            vector.X = value1.X * scaleFactor;
            vector.Y = value1.Y * scaleFactor;
            vector.Z = value1.Z * scaleFactor;
            vector.W = value1.W * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 一个向量与一个标量相乘
        /// </summary>
        /// <param name="scaleFactor">标量值</param>
        /// <param name="value">源向量</param>
        /// <returns>相乘的结果</returns>
        public static Vector4 operator *(float scaleFactor, Vector4 value1)
        {
            Vector4 vector;
            vector.X = value1.X * scaleFactor;
            vector.Y = value1.Y * scaleFactor;
            vector.Z = value1.Z * scaleFactor;
            vector.W = value1.W * scaleFactor;
            return vector;
        }
        /// <summary>
        /// 一个向量的元素与另一个向量的对应元素相除
        /// </summary>
        /// <param name="value1">源向量</param>
        /// <param name="value2">除数向量</param>
        /// <returns>向量相除的结果</returns>
        public static Vector4 operator /(Vector4 value1, Vector4 value2)
        {
            Vector4 vector;
            vector.X = value1.X / value2.X;
            vector.Y = value1.Y / value2.Y;
            vector.Z = value1.Z / value2.Z;
            vector.W = value1.W / value2.W;
            return vector;
        }
        /// <summary>
        /// 一个向量除以一个标量
        /// </summary>
        /// <param name="value">源向量</param>
        /// <param name="divider">除数</param>
        /// <returns>相除的结果</returns>
        public static Vector4 operator /(Vector4 value1, float divider)
        {
            Vector4 vector;
            float num = 1f / divider;
            vector.X = value1.X * num;
            vector.Y = value1.Y * num;
            vector.Z = value1.Z * num;
            vector.W = value1.W * num;
            return vector;
        }

        static Vector4()
        {
            _zero = new Vector4();
            _one = new Vector4(1f, 1f, 1f, 1f);
            _unitX = new Vector4(1f, 0f, 0f, 0f);
            _unitY = new Vector4(0f, 1f, 0f, 0f);
            _unitZ = new Vector4(0f, 0f, 1f, 0f);
            _unitW = new Vector4(0f, 0f, 0f, 1f);
        }
    }
}

