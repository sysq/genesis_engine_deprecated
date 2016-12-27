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
    /// 定义一个四维向量，可以使对象关于一个三维向量有效旋转一定角度。
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct Quaternion : IEquatable<Quaternion>
    {       
        /// <summary>
        /// 指定四元组的旋转分量
        /// </summary>
        public float W;
        /// <summary>
        /// 指定四元组的X轴分量
        /// </summary>
        public float X;
        /// <summary>
        /// 指定四元组的Y轴分量
        /// </summary>
        public float Y;
        /// <summary>
        /// 指定四元组的Z轴分量
        /// </summary>
        public float Z;

        private static Quaternion _identity;
        /// <summary>
        /// 返回一个代表不旋转的四元组
        /// </summary>
        public static Quaternion Identity
        {
            get
            {
                return _identity;
            }
        }
        /// <summary>
        /// 初始化一个四元组实例
        /// </summary>
        /// <param name="x">四元组的x值</param>
        /// <param name="y">四元组的y值</param>
        /// <param name="z">四元组的z值</param>
        /// <param name="w">四元组的w值</param>
        public Quaternion(float x, float y, float z, float w)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
            this.W = w;
        }
        /// <summary>
        /// 初始化一个四元组实例
        /// </summary>
        /// <param name="angle">旋转角</param>
        /// <param name="rkAxis">四元组的旋转分量.</param>
        public Quaternion(float angle, Vector3 rkAxis)
        {
            float num2 = angle * 0.5f;
            float num = (float)Math.Sin((double)num2);
            float num3 = (float)Math.Cos((double)num2);
            X = rkAxis.X * num;
            Y = rkAxis.Y * num;
            Z = rkAxis.Z * num;
            W = num3;
        }
        /// <summary>
        /// 初始化一个四元组实例
        /// </summary>
        /// <param name="xaxis">X轴分量</param>
        /// <param name="yaxis">Y轴分量</param>
        /// <param name="zaxis">Z轴分量</param>
        public Quaternion(Vector3 xaxis, Vector3 yaxis, Vector3 zaxis)
        {
            Matrix44 kRot = Matrix44.Identity;

            kRot[0,0] = xaxis.X;
            kRot[1,0] = xaxis.Y;
            kRot[2,0] = xaxis.Z;

            kRot[0,1] = yaxis.X;
            kRot[1,1] = yaxis.Y;
            kRot[2,1] = yaxis.Z;

            kRot[0,2] = zaxis.X;
            kRot[1,2] = zaxis.Y;
            kRot[2,2] = zaxis.Z;

            CreateFromRotationMatrix(ref kRot, out this);
        }
        /// <summary>
        /// 用指定的偏航角，俯仰角，翻转角初始化四元组
        /// </summary>
        /// <param name="yaw">Y轴旋转弧度值</param>
        /// <param name="pitch">X轴旋转弧度值</param>
        /// <param name="roll">Z轴旋转弧度值</param>
        public Quaternion(float yaw, float pitch, float roll)
        {
            float num9 = roll * 0.5f;
            float num6 = (float)Math.Sin((double)num9);
            float num5 = (float)Math.Cos((double)num9);
            float num8 = pitch * 0.5f;
            float num4 = (float)Math.Sin((double)num8);
            float num3 = (float)Math.Cos((double)num8);
            float num7 = yaw * 0.5f;
            float num2 = (float)Math.Sin((double)num7);
            float num = (float)Math.Cos((double)num7);
            X = ((num * num4) * num5) + ((num2 * num3) * num6);
            Y = ((num2 * num3) * num5) - ((num * num4) * num6);
            Z = ((num * num3) * num6) - ((num2 * num4) * num5);
            W = ((num * num3) * num5) + ((num2 * num4) * num6);
        }
        /// <summary>
        /// 用字符表示当前对象
        /// </summary>
        /// <returns>表示当前对象的字符串</returns>
        public override string ToString()
        {
            CultureInfo currentCulture = CultureInfo.CurrentCulture;
            return string.Format(currentCulture, "{0}, {1}, {2}, {3}", new object[] { this.X.ToString(currentCulture), this.Y.ToString(currentCulture), this.Z.ToString(currentCulture), this.W.ToString(currentCulture) });
        }
        /// <summary>
        /// 判断是否相等
        /// </summary>
        /// <param name="other">指定的四元组</param>
        /// <returns>相等true，不相等false </returns>
        public bool Equals(Quaternion other)
        {
            return ((((this.X == other.X) && (this.Y == other.Y)) && (this.Z == other.Z)) && (this.W == other.W));
        }
        /// <summary>
        /// 返回一个值表明现有实例等于指定对象
        /// </summary>
        /// <param name="obj">指定对象</param>
        /// <returns>相等true，不相等false</returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is Quaternion)
            {
                flag = this.Equals((Quaternion) obj);
            }
            return flag;
        }
        /// <summary>
        /// 计算哈希值
        /// </summary>
        /// <returns>对象的哈希值</returns>
        public override int GetHashCode()
        {
            return (((this.X.GetHashCode() + this.Y.GetHashCode()) + this.Z.GetHashCode()) + this.W.GetHashCode());
        }
        /// <summary>
        /// 计算一个四元组长度的平方
        /// </summary>
        /// <returns>四元组长度的平方</returns>
        public float LengthSquared()
        {
            return ((((this.X * this.X) + (this.Y * this.Y)) + (this.Z * this.Z)) + (this.W * this.W));
        }
        /// <summary>
        /// 计算四元组长度
        /// </summary>
        /// <returns>四元组的长度</returns>
        public float Length()
        {
            float num = (((this.X * this.X) + (this.Y * this.Y)) + (this.Z * this.Z)) + (this.W * this.W);
            return (float) Math.Sqrt((double) num);
        }
        /// <summary>
        /// 用四元组长度划分分量
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
        /// 用四元组长度划分分量
        /// </summary>
        /// <param name="quaternion">源四元组</param>
        /// <returns>标准化四元组</returns>
        public static Quaternion Normalize(Quaternion quaternion)
        {
            Quaternion quaternion2;
            float num2 = (((quaternion.X * quaternion.X) + (quaternion.Y * quaternion.Y)) + (quaternion.Z * quaternion.Z)) + (quaternion.W * quaternion.W);
            float num = 1f / ((float) Math.Sqrt((double) num2));
            quaternion2.X = quaternion.X * num;
            quaternion2.Y = quaternion.Y * num;
            quaternion2.Z = quaternion.Z * num;
            quaternion2.W = quaternion.W * num;
            return quaternion2;
        }
        /// <summary>
        /// 用四元组长度划分分量
        /// </summary>
        /// <param name="quaternion">源四元组</param>
        /// <param name="result">[输出属性] 标准化四元组</param>
        public static void Normalize(ref Quaternion quaternion, out Quaternion result)
        {
            float num2 = (((quaternion.X * quaternion.X) + (quaternion.Y * quaternion.Y)) + (quaternion.Z * quaternion.Z)) + (quaternion.W * quaternion.W);
            float num = 1f / ((float)Math.Sqrt((double)num2));
            result.X = quaternion.X * num;
            result.Y = quaternion.Y * num;
            result.Z = quaternion.Z * num;
            result.W = quaternion.W * num;
        }
        /// <summary>
        /// 返回相反的四元组
        /// </summary>
        /// <param name="quaternion">源四元组</param>
        /// <returns>逆四元组</returns>
        public static Quaternion Inverse(Quaternion quaternion)
        {
            Quaternion quaternion2;
            float num2 = (((quaternion.X * quaternion.X) + (quaternion.Y * quaternion.Y)) + (quaternion.Z * quaternion.Z)) + (quaternion.W * quaternion.W);
            float num = 1f / num2;
            quaternion2.X = -quaternion.X * num;
            quaternion2.Y = -quaternion.Y * num;
            quaternion2.Z = -quaternion.Z * num;
            quaternion2.W = quaternion.W * num;
            return quaternion2;
        }
        /// <summary>
        /// 返回相反的四元组
        /// </summary>
        /// <param name="quaternion">源四元组</param>
        /// <param name="result">[输出属性] 逆四元组</param>
        public static void Inverse(ref Quaternion quaternion, out Quaternion result)
        {
            float num2 = (((quaternion.X * quaternion.X) + (quaternion.Y * quaternion.Y)) + (quaternion.Z * quaternion.Z)) + (quaternion.W * quaternion.W);
            float num = 1f / num2;
            result.X = -quaternion.X * num;
            result.Y = -quaternion.Y * num;
            result.Z = -quaternion.Z * num;
            result.W = quaternion.W * num;
        }
        /// <summary>
        /// 用轴向量和旋转角创建一个四元组
        /// </summary>
        /// <param name="axis">轴向量</param>
        /// <param name="angle">旋转角</param>
        /// <returns></returns>
        public static Quaternion CreateFromAxisAngle(Vector3 axis, float angle)
        {
            Quaternion quaternion;
            float num2 = angle * 0.5f;
            float num = (float) Math.Sin((double) num2);
            float num3 = (float) Math.Cos((double) num2);
            quaternion.X = axis.X * num;
            quaternion.Y = axis.Y * num;
            quaternion.Z = axis.Z * num;
            quaternion.W = num3;
            return quaternion;
        }
        /// <summary>
        /// 用轴向量和旋转角创建一个四元组
        /// </summary>
        /// <param name="axis">轴向量</param>
        /// <param name="angle">旋转角</param>
        /// <param name="result">[输出属性] 创建的四元组</param>
        public static void CreateFromAxisAngle(ref Vector3 axis, float angle, out Quaternion result)
        {
            float num2 = angle * 0.5f;
            float num = (float)Math.Sin((double)num2);
            float num3 = (float)Math.Cos((double)num2);
            result.X = axis.X * num;
            result.Y = axis.Y * num;
            result.Z = axis.Z * num;
            result.W = num3;
        }
        /// <summary>
        /// 用指定的偏航角，俯仰角，翻转角创建四元组 
        /// </summary>
        /// <param name="yaw">Y轴旋转弧度值</param>
        /// <param name="pitch">X轴旋转弧度值</param>
        /// <param name="roll">Z轴旋转弧度值</param>
        /// <returns>创建的四元组</returns>
        public static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll)
        {
            Quaternion quaternion;
            float num9 = roll * 0.5f;
            float num6 = (float) Math.Sin((double) num9);
            float num5 = (float) Math.Cos((double) num9);
            float num8 = pitch * 0.5f;
            float num4 = (float) Math.Sin((double) num8);
            float num3 = (float) Math.Cos((double) num8);
            float num7 = yaw * 0.5f;
            float num2 = (float) Math.Sin((double) num7);
            float num = (float) Math.Cos((double) num7);
            quaternion.X = ((num * num4) * num5) + ((num2 * num3) * num6);
            quaternion.Y = ((num2 * num3) * num5) - ((num * num4) * num6);
            quaternion.Z = ((num * num3) * num6) - ((num2 * num4) * num5);
            quaternion.W = ((num * num3) * num5) + ((num2 * num4) * num6);
            return quaternion;
        }
        /// <summary>
        /// 用指定的yaw，pitch，roll创建四元组 
        /// </summary>
        /// <param name="yaw">Y轴旋转弧度值</param>
        /// <param name="pitch">X轴旋转弧度值</param>
        /// <param name="roll">Z轴旋转弧度值</param>
        /// <param name="result">创建的四元组</param>
        public static void CreateFromYawPitchRoll(float yaw, float pitch, float roll, out Quaternion result)
        {
            float num9 = roll * 0.5f;
            float num6 = (float)Math.Sin((double)num9);
            float num5 = (float)Math.Cos((double)num9);
            float num8 = pitch * 0.5f;
            float num4 = (float)Math.Sin((double)num8);
            float num3 = (float)Math.Cos((double)num8);
            float num7 = yaw * 0.5f;
            float num2 = (float)Math.Sin((double)num7);
            float num = (float)Math.Cos((double)num7);
            result.X = ((num * num4) * num5) + ((num2 * num3) * num6);
            result.Y = ((num2 * num3) * num5) - ((num * num4) * num6);
            result.Z = ((num * num3) * num6) - ((num2 * num4) * num5);
            result.W = ((num * num3) * num5) + ((num2 * num4) * num6);
        }
        /// <summary>
        /// 用旋转矩阵创建四元组
        /// </summary>
        /// <param name="matrix">旋转矩阵</param>
        /// <returns>创建的四元组</returns>
        public static Quaternion CreateFromRotationMatrix(Matrix44 matrix)
        {
            float num8 = (matrix.M00 + matrix.M11) + matrix.M22;
            Quaternion quaternion = new Quaternion();
            if (num8 > 0f)
            {
                float num = (float) Math.Sqrt((double) (num8 + 1f));
                quaternion.W = num * 0.5f;
                num = 0.5f / num;
                quaternion.X = (matrix.M21 - matrix.M12) * num;
                quaternion.Y = (matrix.M02 - matrix.M20) * num;
                quaternion.Z = (matrix.M10 - matrix.M01) * num;
                return quaternion;
            }
            if ((matrix.M00 >= matrix.M11) && (matrix.M00 >= matrix.M22))
            {
                float num7 = (float) Math.Sqrt((double) (((1f + matrix.M00) - matrix.M11) - matrix.M22));
                float num4 = 0.5f / num7;
                quaternion.X = 0.5f * num7;
                quaternion.Y = (matrix.M10 + matrix.M01) * num4;
                quaternion.Z = (matrix.M20 + matrix.M02) * num4;
                quaternion.W = (matrix.M21 - matrix.M12) * num4;
                return quaternion;
            }
            if (matrix.M11 > matrix.M22)
            {
                float num6 = (float) Math.Sqrt((double) (((1f + matrix.M11) - matrix.M00) - matrix.M22));
                float num3 = 0.5f / num6;
                quaternion.X = (matrix.M01 + matrix.M10) * num3;
                quaternion.Y = 0.5f * num6;
                quaternion.Z = (matrix.M12 + matrix.M21) * num3;
                quaternion.W = (matrix.M02 - matrix.M20) * num3;
                return quaternion;
            }
            float num5 = (float) Math.Sqrt((double) (((1f + matrix.M22) - matrix.M00) - matrix.M11));
            float num2 = 0.5f / num5;
            quaternion.X = (matrix.M02 + matrix.M20) * num2;
            quaternion.Y = (matrix.M12 + matrix.M21) * num2;
            quaternion.Z = 0.5f * num5;
            quaternion.W = (matrix.M10 - matrix.M01) * num2;
            return quaternion;
        }
        /// <summary>
        /// 用旋转矩阵创建四元组
        /// </summary>
        /// <param name="matrix">旋转矩阵</param>
        /// <param name="result">[输出属性] 创建的四元组</param>
        public static void CreateFromRotationMatrix(ref Matrix44 matrix, out Quaternion result)
        {
            float num8 = (matrix.M00 + matrix.M11) + matrix.M22;
            if (num8 > 0f)
            {
                float num = (float)Math.Sqrt((double)(num8 + 1f));
                result.W = num * 0.5f;
                num = 0.5f / num;
                result.X = (matrix.M21 - matrix.M12) * num;
                result.Y = (matrix.M02 - matrix.M20) * num;
                result.Z = (matrix.M10 - matrix.M01) * num;
                return;
            }
            if ((matrix.M00 >= matrix.M11) && (matrix.M00 >= matrix.M22))
            {
                float num7 = (float)Math.Sqrt((double)(((1f + matrix.M00) - matrix.M11) - matrix.M22));
                float num4 = 0.5f / num7;
                result.X = 0.5f * num7;
                result.Y = (matrix.M10 + matrix.M01) * num4;
                result.Z = (matrix.M20 + matrix.M02) * num4;
                result.W = (matrix.M21 - matrix.M12) * num4;
                return;
            }
            if (matrix.M11 > matrix.M22)
            {
                float num6 = (float)Math.Sqrt((double)(((1f + matrix.M11) - matrix.M00) - matrix.M22));
                float num3 = 0.5f / num6;
                result.X = (matrix.M01 + matrix.M10) * num3;
                result.Y = 0.5f * num6;
                result.Z = (matrix.M12 + matrix.M21) * num3;
                result.W = (matrix.M02 - matrix.M20) * num3;
                return;
            }
            float num5 = (float)Math.Sqrt((double)(((1f + matrix.M22) - matrix.M00) - matrix.M11));
            float num2 = 0.5f / num5;
            result.X = (matrix.M02 + matrix.M20) * num2;
            result.Y = (matrix.M12 + matrix.M21) * num2;
            result.Z = 0.5f * num5;
            result.W = (matrix.M10 - matrix.M01) * num2;
        }
        /// <summary>
        /// 计算两个四元组点乘
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <returns>点乘结果</returns>
        public static float Dot(Quaternion quaternion1, Quaternion quaternion2)
        {
            return ((((quaternion1.X * quaternion2.X) + (quaternion1.Y * quaternion2.Y)) + (quaternion1.Z * quaternion2.Z)) + (quaternion1.W * quaternion2.W));
        }
        /// <summary>
        /// 计算两个四元组点乘
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <param name="result">[输出属性] 点乘结果</param>
        public static void Dot(ref Quaternion quaternion1, ref Quaternion quaternion2, out float result)
        {
            result = (((quaternion1.X * quaternion2.X) + (quaternion1.Y * quaternion2.Y)) + (quaternion1.Z * quaternion2.Z)) + (quaternion1.W * quaternion2.W);
        }
        /// <summary>
        /// 两个四元组之间的线性插值
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <param name="amount">权重</param>
        /// <returns>计算结果.</returns>
        public static Quaternion Slerp(Quaternion quaternion1, Quaternion quaternion2, float amount)
        {
            float num2;
            float num3;
            Quaternion quaternion;
            float num = amount;
            float num4 = (((quaternion1.X * quaternion2.X) + (quaternion1.Y * quaternion2.Y)) + (quaternion1.Z * quaternion2.Z)) + (quaternion1.W * quaternion2.W);
            bool flag = false;
            if (num4 < 0f)
            {
                flag = true;
                num4 = -num4;
            }
            if (num4 > 0.999999f)
            {
                num3 = 1f - num;
                num2 = flag ? -num : num;
            }
            else
            {
                float num5 = (float) Math.Acos((double) num4);
                float num6 = (float) (1.0 / Math.Sin((double) num5));
                num3 = ((float) Math.Sin((double) ((1f - num) * num5))) * num6;
                num2 = flag ? (((float) -Math.Sin((double) (num * num5))) * num6) : (((float) Math.Sin((double) (num * num5))) * num6);
            }
            quaternion.X = (num3 * quaternion1.X) + (num2 * quaternion2.X);
            quaternion.Y = (num3 * quaternion1.Y) + (num2 * quaternion2.Y);
            quaternion.Z = (num3 * quaternion1.Z) + (num2 * quaternion2.Z);
            quaternion.W = (num3 * quaternion1.W) + (num2 * quaternion2.W);
            return quaternion;
        }
        /// <summary>
        /// 两个四元组之间的线性插值
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <param name="amount">权重</param>
        /// <param name="result">[输出属性] 计算结果 </param>
        public static void Slerp(ref Quaternion quaternion1, ref Quaternion quaternion2, float amount, out Quaternion result)
        {
            float num2;
            float num3;
            float num = amount;
            float num4 = (((quaternion1.X * quaternion2.X) + (quaternion1.Y * quaternion2.Y)) + (quaternion1.Z * quaternion2.Z)) + (quaternion1.W * quaternion2.W);
            bool flag = false;
            if (num4 < 0f)
            {
                flag = true;
                num4 = -num4;
            }
            if (num4 > 0.999999f)
            {
                num3 = 1f - num;
                num2 = flag ? -num : num;
            }
            else
            {
                float num5 = (float)Math.Acos((double)num4);
                float num6 = (float)(1.0 / Math.Sin((double)num5));
                num3 = ((float)Math.Sin((double)((1f - num) * num5))) * num6;
                num2 = flag ? (((float)-Math.Sin((double)(num * num5))) * num6) : (((float)Math.Sin((double)(num * num5))) * num6);
            }
            result.X = (num3 * quaternion1.X) + (num2 * quaternion2.X);
            result.Y = (num3 * quaternion1.Y) + (num2 * quaternion2.Y);
            result.Z = (num3 * quaternion1.Z) + (num2 * quaternion2.Z);
            result.W = (num3 * quaternion1.W) + (num2 * quaternion2.W);
        }
        /// <summary>
        /// 两个四元组之间的线性插值
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <param name="amount">权重</param>
        /// <returns>计算结果</returns>
        public static Quaternion Lerp(Quaternion quaternion1, Quaternion quaternion2, float amount)
        {
            float num = amount;
            float num2 = 1f - num;
            Quaternion quaternion = new Quaternion();
            float num5 = (((quaternion1.X * quaternion2.X) + (quaternion1.Y * quaternion2.Y)) + (quaternion1.Z * quaternion2.Z)) + (quaternion1.W * quaternion2.W);
            if (num5 >= 0f)
            {
                quaternion.X = (num2 * quaternion1.X) + (num * quaternion2.X);
                quaternion.Y = (num2 * quaternion1.Y) + (num * quaternion2.Y);
                quaternion.Z = (num2 * quaternion1.Z) + (num * quaternion2.Z);
                quaternion.W = (num2 * quaternion1.W) + (num * quaternion2.W);
            }
            else
            {
                quaternion.X = (num2 * quaternion1.X) - (num * quaternion2.X);
                quaternion.Y = (num2 * quaternion1.Y) - (num * quaternion2.Y);
                quaternion.Z = (num2 * quaternion1.Z) - (num * quaternion2.Z);
                quaternion.W = (num2 * quaternion1.W) - (num * quaternion2.W);
            }
            float num4 = (((quaternion.X * quaternion.X) + (quaternion.Y * quaternion.Y)) + (quaternion.Z * quaternion.Z)) + (quaternion.W * quaternion.W);
            float num3 = 1f / ((float) Math.Sqrt((double) num4));
            quaternion.X *= num3;
            quaternion.Y *= num3;
            quaternion.Z *= num3;
            quaternion.W *= num3;
            return quaternion;
        }
        /// <summary>
        /// 两个四元组之间的线性插值
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <param name="amount">权重</param>
        /// <param name="result">[输出属性] 计算结果</param>
        public static void Lerp(ref Quaternion quaternion1, ref Quaternion quaternion2, float amount, out Quaternion result)
        {
            float num = amount;
            float num2 = 1f - num;
            float num5 = (((quaternion1.X * quaternion2.X) + (quaternion1.Y * quaternion2.Y)) + (quaternion1.Z * quaternion2.Z)) + (quaternion1.W * quaternion2.W);
            if (num5 >= 0f)
            {
                result.X = (num2 * quaternion1.X) + (num * quaternion2.X);
                result.Y = (num2 * quaternion1.Y) + (num * quaternion2.Y);
                result.Z = (num2 * quaternion1.Z) + (num * quaternion2.Z);
                result.W = (num2 * quaternion1.W) + (num * quaternion2.W);
            }
            else
            {
                result.X = (num2 * quaternion1.X) - (num * quaternion2.X);
                result.Y = (num2 * quaternion1.Y) - (num * quaternion2.Y);
                result.Z = (num2 * quaternion1.Z) - (num * quaternion2.Z);
                result.W = (num2 * quaternion1.W) - (num * quaternion2.W);
            }
            float num4 = (((result.X * result.X) + (result.Y * result.Y)) + (result.Z * result.Z)) + (result.W * result.W);
            float num3 = 1f / ((float)Math.Sqrt((double)num4));
            result.X *= num3;
            result.Y *= num3;
            result.Z *= num3;
            result.W *= num3;
        }
        /// <summary>
        /// 改变为与当前四元组共轭的四元组
        /// </summary>
        public void Conjugate()
        {
            this.X = -this.X;
            this.Y = -this.Y;
            this.Z = -this.Z;
        }
        /// <summary>
        /// 求当前四元组共轭的四元组 
        /// </summary>
        /// <param name="value">指定四元组 </param>
        /// <returns>与指定四元组共轭的四元组</returns>
        public static Quaternion Conjugate(Quaternion value)
        {
            Quaternion quaternion;
            quaternion.X = -value.X;
            quaternion.Y = -value.Y;
            quaternion.Z = -value.Z;
            quaternion.W = value.W;
            return quaternion;
        }
        /// <summary>
        /// 求当前四元组共轭的四元组 
        /// </summary>
        /// <param name="value">指定四元组 </param>
        /// <param name="result">[输出属性] 与指定四元组共轭的四元组</param>
        public static void Conjugate(ref Quaternion value, out Quaternion result)
        {
            result.X = -value.X;
            result.Y = -value.Y;
            result.Z = -value.Z;
            result.W = value.W;
        }
        /// <summary>
        /// 返回一个a,b之间的角度值
        /// </summary>
        /// <param name="a">源四元组</param>
        /// <param name="b">源四元组</param>
        /// <returns>计算所得角度值</returns>
        public static float Angle(Quaternion a, Quaternion b)
        {
            return (((float)Math.Acos((float)Math.Min((float)Math.Abs((float)Dot(a, b)), 1f)) * 2f) * 57.29578f);
        }
        /// <summary>
        /// 返回一个a,b之间的角度值
        /// </summary>
        /// <param name="a">源四元组</param>
        /// <param name="b">源四元组</param>
        /// <param name="result">[输出属性] 计算所得角度值</param>
        public static void Angle(ref Quaternion a, ref Quaternion b, out float result)
        {
            result = (((float)Math.Acos((float)Math.Min((float)Math.Abs((float)Dot(a, b)), 1f)) * 2f) * 57.29578f);
        }

        /// <summary>
        /// 返回一个a,b之间的弧度值
        /// </summary>
        /// <param name="a">源四元组</param>
        /// <param name="b">源四元组</param>
        /// <returns>计算所得弧度值</returns>
        public static float Radian(Quaternion a, Quaternion b)
        {
            return (((float)Math.Acos((float)Math.Min((float)Math.Abs((float)Dot(a, b)), 1f)) * 2f));
        }
        /// <summary>
        /// 返回一个a,b之间的弧度值
        /// </summary>
        /// <param name="a">源四元组</param>
        /// <param name="b">源四元组</param>
        /// <param name="result">[输出属性] 计算所得弧度值</param>
        public static void Radian(ref Quaternion a, ref Quaternion b, out float result)
        {
            result = (((float)Math.Acos((float)Math.Min((float)Math.Abs((float)Dot(a, b)), 1f)) * 2f));
        }

        /// <summary>
        /// 各分量相反的四元组
        /// </summary>
        /// <param name="quaternion">源四元组</param>
        /// <returns>相反的四元组</returns>
        public static Quaternion Negate(Quaternion quaternion)
        {
            Quaternion quaternion2;
            quaternion2.X = -quaternion.X;
            quaternion2.Y = -quaternion.Y;
            quaternion2.Z = -quaternion.Z;
            quaternion2.W = -quaternion.W;
            return quaternion2;
        }
        /// <summary>
        /// 各分量相反的四元组 
        /// </summary>
        /// <param name="quaternion">源四元组</param>
        /// <param name="result">[OutAttribute] 相反的四元组</param>
        public static void Negate(ref Quaternion quaternion, out Quaternion result)
        {
            result.X = -quaternion.X;
            result.Y = -quaternion.Y;
            result.Z = -quaternion.Z;
            result.W = -quaternion.W;
        }
        /// <summary>
        /// 四元组减法
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <returns>计算结果</returns>
        public static Quaternion Sub(Quaternion quaternion1, Quaternion quaternion2)
        {
            Quaternion quaternion;
            quaternion.X = quaternion1.X - quaternion2.X;
            quaternion.Y = quaternion1.Y - quaternion2.Y;
            quaternion.Z = quaternion1.Z - quaternion2.Z;
            quaternion.W = quaternion1.W - quaternion2.W;
            return quaternion;
        }
        /// <summary>
        /// 四元组减法 
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <param name="result">[输出属性] 计算结果 </param>
        public static void Sub(ref Quaternion quaternion1, ref Quaternion quaternion2, out Quaternion result)
        {
            result.X = quaternion1.X - quaternion2.X;
            result.Y = quaternion1.Y - quaternion2.Y;
            result.Z = quaternion1.Z - quaternion2.Z;
            result.W = quaternion1.W - quaternion2.W;
        }
        /// <summary>
        /// 用四元组旋转一个向量
        /// </summary>
        /// <param name="rotation">旋转四元组</param>
        /// <param name="vector3">向量</param>
        /// <returns>旋转后向量</returns>
        public static Vector3 Rotate(Quaternion rotation, Vector3 vector3)
        {
            float x = rotation.X * 2F;
            float y = rotation.Y * 2F;
            float z = rotation.Z * 2F;
            float xx = rotation.X * x;
            float yy = rotation.Y * y;
            float zz = rotation.Z * z;
            float xy = rotation.X * y;
            float xz = rotation.X * z;
            float yz = rotation.Y * z;
            float wx = rotation.W * x;
            float wy = rotation.W * y;
            float wz = rotation.W * z;
            Vector3 res;
            res.X = (1F - (yy + zz)) * vector3.X + (xy - wz) * vector3.Y + (xz + wy) * vector3.Z;
            res.Y = (xy + wz) * vector3.X + (1F - (xx + zz)) * vector3.Y + (yz - wx) * vector3.Z;
            res.Z = (xz - wy) * vector3.X + (yz + wx) * vector3.Y + (1F - (xx + yy)) * vector3.Z;
            return res;
        }
        /// <summary>
        /// 用四元组旋转一个向量
        /// </summary>
        /// <param name="rotation">旋转四元组</param>
        /// <param name="vector3">向量</param>
        /// <param name="result">[输出属性] 旋转后的向量</param>
        public static void Rotate(ref Quaternion rotation, ref Vector3 vector3, out Vector3 result)
        {
            float x = rotation.X * 2F;
            float y = rotation.Y * 2F;
            float z = rotation.Z * 2F;
            float xx = rotation.X * x;
            float yy = rotation.Y * y;
            float zz = rotation.Z * z;
            float xy = rotation.X * y;
            float xz = rotation.X * z;
            float yz = rotation.Y * z;
            float wx = rotation.W * x;
            float wy = rotation.W * y;
            float wz = rotation.W * z;

            result.X = (1F - (yy + zz)) * vector3.X + (xy - wz) * vector3.Y + (xz + wy) * vector3.Z;
            result.Y = (xy + wz) * vector3.X + (1F - (xx + zz)) * vector3.Y + (yz - wx) * vector3.Z;
            result.Z = (xz - wy) * vector3.X + (yz + wx) * vector3.Y + (1F - (xx + yy)) * vector3.Z;
        }

        /// <summary>
        /// 四元组乘法
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <returns>乘法结果</returns>
        public static Quaternion Multiply(Quaternion quaternion1, Quaternion quaternion2)
        {
            Quaternion quaternion;
            float x = quaternion1.X;
            float y = quaternion1.Y;
            float z = quaternion1.Z;
            float w = quaternion1.W;
            float num4 = quaternion2.X;
            float num3 = quaternion2.Y;
            float num2 = quaternion2.Z;
            float num = quaternion2.W;
            float num12 = (y * num2) - (z * num3);
            float num11 = (z * num4) - (x * num2);
            float num10 = (x * num3) - (y * num4);
            float num9 = ((x * num4) + (y * num3)) + (z * num2);
            quaternion.X = ((x * num) + (num4 * w)) + num12;
            quaternion.Y = ((y * num) + (num3 * w)) + num11;
            quaternion.Z = ((z * num) + (num2 * w)) + num10;
            quaternion.W = (w * num) - num9;
            return quaternion;
        }
        /// <summary>
        /// 四元组乘法
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <param name="result">[输出属性] 乘法结果</param>
        public static void Multiply(ref Quaternion quaternion1, ref Quaternion quaternion2, out Quaternion result)
        {
            float x = quaternion1.X;
            float y = quaternion1.Y;
            float z = quaternion1.Z;
            float w = quaternion1.W;
            float num4 = quaternion2.X;
            float num3 = quaternion2.Y;
            float num2 = quaternion2.Z;
            float num = quaternion2.W;
            float num12 = (y * num2) - (z * num3);
            float num11 = (z * num4) - (x * num2);
            float num10 = (x * num3) - (y * num4);
            float num9 = ((x * num4) + (y * num3)) + (z * num2);
            result.X = ((x * num) + (num4 * w)) + num12;
            result.Y = ((y * num) + (num3 * w)) + num11;
            result.Z = ((z * num) + (num2 * w)) + num10;
            result.W = (w * num) - num9;
        }


        /// <summary>
        /// 各分量相反的四元组
        /// </summary>
        /// <param name="quaternion">源四元组</param>
        /// <returns>各分量相反的四元组</returns>
        public static Quaternion operator -(Quaternion quaternion)
        {
            Quaternion quaternion2;
            quaternion2.X = -quaternion.X;
            quaternion2.Y = -quaternion.Y;
            quaternion2.Z = -quaternion.Z;
            quaternion2.W = -quaternion.W;
            return quaternion2;
        }

        /// <summary>
        /// 比较四元组是否相等
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <returns>相等true，不相等false</returns>
        public static bool operator ==(Quaternion quaternion1, Quaternion quaternion2)
        {
            return ((((quaternion1.X == quaternion2.X) && (quaternion1.Y == quaternion2.Y)) && (quaternion1.Z == quaternion2.Z)) && (quaternion1.W == quaternion2.W));
        }
        /// <summary>
        /// 比较四元组是否不相等
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <returns>不相等true，相等false</returns>
        public static bool operator !=(Quaternion quaternion1, Quaternion quaternion2)
        {
            if (((quaternion1.X == quaternion2.X) && (quaternion1.Y == quaternion2.Y)) && (quaternion1.Z == quaternion2.Z))
            {
                return !(quaternion1.W == quaternion2.W);
            }
            return true;
        }
        /// <summary>
        /// 四元组减法
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <returns>计算结果</returns>
        public static Quaternion operator -(Quaternion quaternion1, Quaternion quaternion2)
        {
            Quaternion quaternion;
            quaternion.X = quaternion1.X - quaternion2.X;
            quaternion.Y = quaternion1.Y - quaternion2.Y;
            quaternion.Z = quaternion1.Z - quaternion2.Z;
            quaternion.W = quaternion1.W - quaternion2.W;
            return quaternion;
        }
        /// <summary>
        /// 四元组乘法 
        /// </summary>
        /// <param name="quaternion1">源四元组</param>
        /// <param name="quaternion2">源四元组</param>
        /// <returns>计算结果</returns>
        public static Quaternion operator *(Quaternion quaternion1, Quaternion quaternion2)
        {
            Quaternion quaternion;
            float x = quaternion1.X;
            float y = quaternion1.Y;
            float z = quaternion1.Z;
            float w = quaternion1.W;
            float num4 = quaternion2.X;
            float num3 = quaternion2.Y;
            float num2 = quaternion2.Z;
            float num = quaternion2.W;
            float num12 = (y * num2) - (z * num3);
            float num11 = (z * num4) - (x * num2);
            float num10 = (x * num3) - (y * num4);
            float num9 = ((x * num4) + (y * num3)) + (z * num2);
            quaternion.X = ((x * num) + (num4 * w)) + num12;
            quaternion.Y = ((y * num) + (num3 * w)) + num11;
            quaternion.Z = ((z * num) + (num2 * w)) + num10;
            quaternion.W = (w * num) - num9;
            return quaternion;
        }

        static Quaternion()
        {
            _identity = new Quaternion(0f, 0f, 0f, 1f);
        }
    }
}

