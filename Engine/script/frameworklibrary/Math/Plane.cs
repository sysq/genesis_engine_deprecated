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
    /// 定义一个平面
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct Plane : IEquatable<Plane>
    {
        /// <summary>
        /// 平面的法向量
        /// </summary>
        public Vector3 Normal;
        /// <summary>
        /// 到原点的距离
        /// </summary>
        public float D;
        /// <summary>
        /// 创建一个平面实例
        /// </summary>
        /// <param name="a">法向量X轴分量</param>
        /// <param name="b">法向量Y轴分量</param>
        /// <param name="c">法向量Z轴分量</param>
        /// <param name="d">平面到原点的距离</param>
        public Plane(float a, float b, float c, float d)
        {
            this.Normal.X = a;
            this.Normal.Y = b;
            this.Normal.Z = c;
            this.D = d;
        }
        /// <summary>
        /// 创建一个平面实例 
        /// </summary>
        /// <param name="normal">平面的法向量</param>
        /// <param name="d">平面到原点的距离</param>
        public Plane(Vector3 normal, float d)
        {
            this.Normal = normal;
            this.D = d;
        }

        /// <summary>
        /// 创建一个平面实例
        /// </summary>
        /// <param name="point1">三个顶点确定一个平面之一</param>
        /// <param name="point2">三个顶点确定一个平面之二</param>
        /// <param name="point3">三个顶点确定一个平面之三</param>
        public Plane(Vector3 point1, Vector3 point2, Vector3 point3)
        {
            float num10 = point2.X - point1.X;
            float num9 = point2.Y - point1.Y;
            float num8 = point2.Z - point1.Z;
            float num7 = point3.X - point1.X;
            float num6 = point3.Y - point1.Y;
            float num5 = point3.Z - point1.Z;
            float num4 = (num9 * num5) - (num8 * num6);
            float num3 = (num8 * num7) - (num10 * num5);
            float num2 = (num10 * num6) - (num9 * num7);
            float num11 = ((num4 * num4) + (num3 * num3)) + (num2 * num2);
            float num = 1f / ((float) Math.Sqrt((double) num11));
            this.Normal.X = num4 * num;
            this.Normal.Y = num3 * num;
            this.Normal.Z = num2 * num;
            this.D = -(((this.Normal.X * point1.X) + (this.Normal.Y * point1.Y)) + (this.Normal.Z * point1.Z));
        }
        /// <summary>
        /// 创建一个平面实例
        /// </summary>
        /// <param name="normal">平面的法向量</param>
        /// <param name="point">平面上的一个点</param>
        public Plane(Vector3 normal, Vector3 point)
        {
            this.Normal = normal;
            this.D = -(((this.Normal.X * point.X) + (this.Normal.Y * point.Y)) + (this.Normal.Z * point.Z));
        }
        /// <summary>
        /// 判断两个平面是否相等 
        /// </summary>
        /// <param name="other">指定的平面</param>
        /// <returns>相等true，不相等false</returns>
        public bool Equals(Plane other)
        {
            return ((((this.Normal.X == other.Normal.X) && (this.Normal.Y == other.Normal.Y)) && (this.Normal.Z == other.Normal.Z)) && (this.D == other.D));
        }
        /// <summary>
        /// 判断两个平面是否相等
        /// </summary>
        /// <param name="obj">指定的对象</param>
        /// <returns>相等true，不相等false</returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is Plane)
            {
                flag = this.Equals((Plane) obj);
            }
            return flag;
        }
        /// <summary>
        /// 获取平面对象的哈希码 
        /// </summary>
        /// <returns>哈希码</returns>
        public override int GetHashCode()
        {
            return (this.Normal.GetHashCode() + this.D.GetHashCode());
        }
        /// <summary>
        /// 用字符串表示平面
        /// </summary>
        /// <returns>表示当前平面的字符串</returns>
        public override string ToString()
        {
            CultureInfo currentCulture = CultureInfo.CurrentCulture;
            return string.Format(currentCulture, "Normal:{0} D:{1}", new object[] { this.Normal.ToString(), this.D.ToString(currentCulture) });
        }
        /// <summary>
        /// 用一个矩阵转换标准化平面
        /// </summary> 
        /// <param name="matrix">转换矩阵</param>
        /// <param name="plane">要转换的平面，在这个方法调用前，要保证平面的法向量是单位向量</param>
        /// <returns></returns>
        public static Plane Transform(Matrix44 matrix, Plane plane)
        {
            Plane plane2;
            Matrix44 matrix2;
            Matrix44.Invert(ref matrix, out matrix2);
            float x = plane.Normal.X;
            float y = plane.Normal.Y;
            float z = plane.Normal.Z;
            float d = plane.D;
            plane2.Normal.X = (((x * matrix2.M00) + (y * matrix2.M10)) + (z * matrix2.M20)) + (d * matrix2.M30);
            plane2.Normal.Y = (((x * matrix2.M01) + (y * matrix2.M11)) + (z * matrix2.M21)) + (d * matrix2.M31);
            plane2.Normal.Z = (((x * matrix2.M02) + (y * matrix2.M12)) + (z * matrix2.M22)) + (d * matrix2.M32);
            plane2.D = (((x * matrix2.M03) + (y * matrix2.M13)) + (z * matrix2.M23)) + (d * matrix2.M33);
            return plane2;
        }
        /// <summary>
        /// 用一个矩阵转换标准化平面
        /// </summary> 
        /// <param name="matrix">转换矩阵</param>
        /// <param name="plane">要转换的平面，在这个方法调用前，要保证平面的法向量是单位向量</param>
        /// <param name="result">[输出属性] 一个现有的平面保存这个结果</param>
        public static void Transform(ref Matrix44 matrix, ref Plane plane, out Plane result)
        {
            Matrix44 matrix2;
            Matrix44.Invert(ref matrix, out matrix2);
            float x = plane.Normal.X;
            float y = plane.Normal.Y;
            float z = plane.Normal.Z;
            float d = plane.D;
            result.Normal.X = (((x * matrix2.M00) + (y * matrix2.M10)) + (z * matrix2.M20)) + (d * matrix2.M30);
            result.Normal.Y = (((x * matrix2.M01) + (y * matrix2.M11)) + (z * matrix2.M21)) + (d * matrix2.M31);
            result.Normal.Z = (((x * matrix2.M02) + (y * matrix2.M12)) + (z * matrix2.M22)) + (d * matrix2.M32);
            result.D = (((x * matrix2.M03) + (y * matrix2.M13)) + (z * matrix2.M23)) + (d * matrix2.M33);
        }
        /// <summary>
        /// 检查这个平面是否与指定包围盒相交
        /// </summary>
        /// <param name="box">指定的包围盒</param>
        /// <returns>表明关系的枚举类型</returns>
        public PlaneIntersectionStatus Intersects(BoundingBox box)
        {
            Vector3 vector;
            Vector3 vector2;
            vector2.X = (this.Normal.X >= 0f) ? box.Min.X : box.Max.X;
            vector2.Y = (this.Normal.Y >= 0f) ? box.Min.Y : box.Max.Y;
            vector2.Z = (this.Normal.Z >= 0f) ? box.Min.Z : box.Max.Z;
            vector.X = (this.Normal.X >= 0f) ? box.Max.X : box.Min.X;
            vector.Y = (this.Normal.Y >= 0f) ? box.Max.Y : box.Min.Y;
            vector.Z = (this.Normal.Z >= 0f) ? box.Max.Z : box.Min.Z;
            float num = ((this.Normal.X * vector2.X) + (this.Normal.Y * vector2.Y)) + (this.Normal.Z * vector2.Z);
            if ((num + this.D) > 0f)
            {
                return PlaneIntersectionStatus.Front;
            }
            num = ((this.Normal.X * vector.X) + (this.Normal.Y * vector.Y)) + (this.Normal.Z * vector.Z);
            if ((num + this.D) < 0f)
            {
                return PlaneIntersectionStatus.Back;
            }
            return PlaneIntersectionStatus.Intersecting;
        }
        /// <summary>
        /// 检查这个平面是否与指定包围盒相交
        /// </summary>
        /// <param name="box">指定的包围盒</param>
        /// <param name="result">[输出属性] 表明关系的枚举类型</param>
        public void Intersects(ref BoundingBox box, out PlaneIntersectionStatus result)
        {
            Vector3 vector;
            Vector3 vector2;
            vector2.X = (this.Normal.X >= 0f) ? box.Min.X : box.Max.X;
            vector2.Y = (this.Normal.Y >= 0f) ? box.Min.Y : box.Max.Y;
            vector2.Z = (this.Normal.Z >= 0f) ? box.Min.Z : box.Max.Z;
            vector.X = (this.Normal.X >= 0f) ? box.Max.X : box.Min.X;
            vector.Y = (this.Normal.Y >= 0f) ? box.Max.Y : box.Min.Y;
            vector.Z = (this.Normal.Z >= 0f) ? box.Max.Z : box.Min.Z;
            float num = ((this.Normal.X * vector2.X) + (this.Normal.Y * vector2.Y)) + (this.Normal.Z * vector2.Z);
            if ((num + this.D) > 0f)
            {
                result = PlaneIntersectionStatus.Front;
            }
            else
            {
                num = ((this.Normal.X * vector.X) + (this.Normal.Y * vector.Y)) + (this.Normal.Z * vector.Z);
                if ((num + this.D) < 0f)
                {
                    result = PlaneIntersectionStatus.Back;
                }
                else
                {
                    result = PlaneIntersectionStatus.Intersecting;
                }
            }
        }
        /// <summary>
        /// 检查这个平面是否与指定包围断面锥相交
        /// </summary>
        /// <param name="frustum">指定的包围断面锥</param>
        /// <returns>表明关系的枚举类型</returns>
        public PlaneIntersectionStatus Intersects(BoundingFrustum frustum)
        {
            if (null == frustum)
            {
                throw new ArgumentNullException("frustum", FrameworkResources.NullNotAllowed);
            }
            return frustum.Intersects(this);
        }
        /// <summary>
        /// 检查这个平面是否与指定包围球相交
        /// </summary>
        /// <param name="sphere">指定的包围球</param>
        /// <returns>表明关系的枚举类型</returns>
        public PlaneIntersectionStatus Intersects(BoundingSphere sphere)
        {
            float num2 = ((sphere.Center.X * this.Normal.X) + (sphere.Center.Y * this.Normal.Y)) + (sphere.Center.Z * this.Normal.Z);
            float num = num2 + this.D;
            if (num > sphere.Radius)
            {
                return PlaneIntersectionStatus.Front;
            }
            if (num < -sphere.Radius)
            {
                return PlaneIntersectionStatus.Back;
            }
            return PlaneIntersectionStatus.Intersecting;
        }
        /// <summary>
        /// 检查这个平面是否与指定射线相交
        /// </summary>
        /// <param name="ray">指定的射线</param>
        /// <returns>相交为true，不相交为false</returns>
        public bool Intersects(Ray ray)
        {
            return ray.Intersects(this);
        }

        public bool Intersects(ref Ray ray, out float result)
        {
            float vdot = Vector3.Dot(ray.Direction, this.Normal);
            float ndot = -Vector3.Dot(ray.Position, this.Normal) - this.D;
            result = 0f;
            float dist = Math.Abs(vdot);
            if (dist < 0.000001F)
                return false;

            // the resulting intersection is behind the origin of the ray
            // if the result is negative ( enter < 0 )
            result = ndot / vdot;

            return result > 0.0F;
        }
        /// <summary>
        /// 检查这个平面是否与指定包围球相交
        /// </summary>
        /// <param name="sphere">指定的包围球</param>
        /// <param name="result">表明关系的枚举类型</param>
        public void Intersects(ref BoundingSphere sphere, out PlaneIntersectionStatus result)
        {
            float num2 = ((sphere.Center.X * this.Normal.X) + (sphere.Center.Y * this.Normal.Y)) + (sphere.Center.Z * this.Normal.Z);
            float num = num2 + this.D;
            if (num > sphere.Radius)
            {
                result = PlaneIntersectionStatus.Front;
            }
            else if (num < -sphere.Radius)
            {
                result = PlaneIntersectionStatus.Back;
            }
            else
            {
                result = PlaneIntersectionStatus.Intersecting;
            }
        }
        /// <summary>
        /// 判断两个平面是否相等
        /// </summary>
        /// <param name="lhs">符号左边的平面</param>
        /// <param name="rhs">符号右边的平面</param>
        /// <returns>相等true，不相等false</returns>
        public static bool operator ==(Plane lhs, Plane rhs)
        {
            return lhs.Equals(rhs);
        }
        /// <summary>
        /// 判断两个平面是否不相等
        /// </summary>
        /// <param name="lhs">符号左边的平面</param>
        /// <param name="rhs">符号左边的平面</param>
        /// <returns>不相等true，相等false<</returns>
        public static bool operator !=(Plane lhs, Plane rhs)
        {
            if (((lhs.Normal.X == rhs.Normal.X) && (lhs.Normal.Y == rhs.Normal.Y)) && (lhs.Normal.Z == rhs.Normal.Z))
            {
                return !(lhs.D == rhs.D);
            }
            return true;
        }
    }
}

