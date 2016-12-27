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
    using System.Collections.Generic;
    using System.ComponentModel;
    using System.Globalization;
    using System.Runtime.InteropServices;
    /// <summary>
    /// 定义一个包围球
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct BoundingSphere : IEquatable<BoundingSphere>
    {
        /// <summary>
        /// 球心
        /// </summary>
        public Vector3 Center;
        /// <summary>
        /// 球半径
        /// </summary>
        public float Radius;
        /// <summary>
        /// 创建一个包围球实例
        /// </summary>
        /// <param name="center">圆心</param>
        /// <param name="radius">半径</param>
        public BoundingSphere(Vector3 center, float radius)
        {
            if (radius < 0f)
            {
                throw new ArgumentException(FrameworkResources.NegativeRadius);
            }
            this.Center = center;
            this.Radius = radius;
        }
        /// <summary>
        /// 判断当前包围球是否等于指定包围球
        /// </summary>
        /// <param name="other">指定包围球</param>
        /// <returns>相等true，不相等false</returns>
        public bool Equals(BoundingSphere other)
        {
            return ((this.Center == other.Center) && (this.Radius == other.Radius));
        }
        /// <summary>
        /// 判断当前包围球是否等于指定对象
        /// </summary>
        /// <param name="obj">指定对象 </param>
        /// <returns>相等true，不相等false</returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is BoundingSphere)
            {
                flag = this.Equals((BoundingSphere) obj);
            }
            return flag;
        }
        /// <summary>
        /// 计算哈希值
        /// </summary>
        /// <returns>当前包围球的哈希值</returns>
        public override int GetHashCode()
        {
            return (this.Center.GetHashCode() + this.Radius.GetHashCode());
        }
        /// <summary>
        /// 用字符串表示当前包围球
        /// </summary>
        /// <returns>表示当前包围球的字符串</returns>
        public override string ToString()
        {
            CultureInfo currentCulture = CultureInfo.CurrentCulture;
            return string.Format(currentCulture, "C:{0}, R:{1}", new object[] { this.Center.ToString(), this.Radius.ToString(currentCulture) });
        }
        /// <summary>
        /// 创建能包含指定包围盒的最小包围球
        /// </summary>
        /// <param name="box">指定包围盒</param>
        /// <returns>创建的包围球</returns>
        public static BoundingSphere CreateFromBoundingBox(BoundingBox box)
        {
            float num;
            BoundingSphere sphere;
            Vector3.Lerp(ref box.Min, ref box.Max, 0.5f, out sphere.Center);
            Vector3.Distance(ref box.Min, ref box.Max, out num);
            sphere.Radius = num * 0.5f;
            return sphere;
        }
        /// <summary>
        /// 创建能包含指定包围盒的最小包围球 
        /// </summary>
        /// <param name="box">指定包围盒</param>
        /// <param name="result">[输出属性] 创建的包围球</param>
        public static void CreateFromBoundingBox(ref BoundingBox box, out BoundingSphere result)
        {
            float num;
            Vector3.Lerp(ref box.Min, ref box.Max, 0.5f, out result.Center);
            Vector3.Distance(ref box.Min, ref box.Max, out num);
            result.Radius = num * 0.5f;
        }
        /// <summary>
        /// 创建一个能包含指定点集的包围球 
        /// </summary>
        /// <param name="points">点集</param>
        /// <returns>创建的包围球</returns>
        public static BoundingSphere CreateFromPoints(IEnumerable<Vector3> points)
        {
            float num;
            float num2;
            Vector3 vector2;
            float num4;
            float num5;
            BoundingSphere sphere;
            Vector3 vector5;
            Vector3 vector6;
            Vector3 vector7;
            Vector3 vector8;
            Vector3 vector9;
            if (points == null)
            {
                throw new ArgumentNullException("points");
            }
            IEnumerator<Vector3> enumerator = points.GetEnumerator();
            if (!enumerator.MoveNext())
            {
                throw new ArgumentException(FrameworkResources.BoundingSphereZeroPoints);
            }
            Vector3 vector4 = vector5 = vector6 = vector7 = vector8 = vector9 = enumerator.Current;
            foreach (Vector3 vector in points)
            {
                if (vector.X < vector4.X)
                {
                    vector4 = vector;
                }
                if (vector.X > vector5.X)
                {
                    vector5 = vector;
                }
                if (vector.Y < vector6.Y)
                {
                    vector6 = vector;
                }
                if (vector.Y > vector7.Y)
                {
                    vector7 = vector;
                }
                if (vector.Z < vector8.Z)
                {
                    vector8 = vector;
                }
                if (vector.Z > vector9.Z)
                {
                    vector9 = vector;
                }
            }
            Vector3.Distance(ref vector5, ref vector4, out num5);
            Vector3.Distance(ref vector7, ref vector6, out num4);
            Vector3.Distance(ref vector9, ref vector8, out num2);
            if (num5 > num4)
            {
                if (num5 > num2)
                {
                    Vector3.Lerp(ref vector5, ref vector4, 0.5f, out vector2);
                    num = num5 * 0.5f;
                }
                else
                {
                    Vector3.Lerp(ref vector9, ref vector8, 0.5f, out vector2);
                    num = num2 * 0.5f;
                }
            }
            else if (num4 > num2)
            {
                Vector3.Lerp(ref vector7, ref vector6, 0.5f, out vector2);
                num = num4 * 0.5f;
            }
            else
            {
                Vector3.Lerp(ref vector9, ref vector8, 0.5f, out vector2);
                num = num2 * 0.5f;
            }
            foreach (Vector3 vector10 in points)
            {
                Vector3 vector3;
                vector3.X = vector10.X - vector2.X;
                vector3.Y = vector10.Y - vector2.Y;
                vector3.Z = vector10.Z - vector2.Z;
                float num3 = vector3.Length();
                if (num3 > num)
                {
                    num = (num + num3) * 0.5f;
                    vector2 += (Vector3)((1f - (num / num3)) * vector3);
                }
            }
            sphere.Center = vector2;
            sphere.Radius = num;
            return sphere;
        }
        /// <summary>
        /// 创建能包含指定包围断面锥的最小包围球
        /// </summary>
        /// <param name="frustum">指定包围断面锥</param>
        /// <returns>创建的包围球</returns>
        public static BoundingSphere CreateFromFrustum(BoundingFrustum frustum)
        {
            if (frustum == null)
            {
                throw new ArgumentNullException("frustum");
            }
            return CreateFromPoints(frustum.cornerArray);
        }
        /// <summary>
        /// 检查当前包围球是否与指定包围盒相交
        /// </summary>
        /// <param name="box">指定包围盒</param>
        /// <returns>相交true，不相交false </returns>
        public bool Intersects(BoundingBox box)
        {
            float num;
            Vector3 vector;
            Vector3.Clamp(ref this.Center, ref box.Min, ref box.Max, out vector);
            Vector3.DistanceSquared(ref this.Center, ref vector, out num);
            return (num <= (this.Radius * this.Radius));
        }
        /// <summary>
        /// 检查当前包围球是否与指定包围盒相交 
        /// </summary>
        /// <param name="box">指定包围盒</param>
        /// <param name="result">[输出属性] 相交true，不相交false </param>
        public void Intersects(ref BoundingBox box, out bool result)
        {
            float num;
            Vector3 vector;
            Vector3.Clamp(ref this.Center, ref box.Min, ref box.Max, out vector);
            Vector3.DistanceSquared(ref this.Center, ref vector, out num);
            result = num <= (this.Radius * this.Radius);
        }
        /// <summary>
        /// 检查当前包围球是否与指定包围断面锥相交 
        /// </summary>
        /// <param name="frustum">指定包围断面锥 </param>
        /// <returns>相交true，不相交false </returns>
        public bool Intersects(BoundingFrustum frustum)
        {
            bool flag;
            if (null == frustum)
            {
                throw new ArgumentNullException("frustum", FrameworkResources.NullNotAllowed);
            }
            frustum.Intersects(ref this, out flag);
            return flag;
        }
        /// <summary>
        /// 检查当前包围球是否与指定平面相交
        /// </summary>
        /// <param name="plane">指定平面</param>
        /// <returns>表明关系的枚举类型 </returns>
        public PlaneIntersectionStatus Intersects(Plane plane)
        {
            return plane.Intersects(this);
        }
        /// <summary>
        /// 检查当前包围球是否与指定平面相交
        /// </summary>
        /// <param name="plane">指定平面</param>
        /// <param name="result">[输出属性] 表明关系的枚举类型</param>
        public void Intersects(ref Plane plane, out PlaneIntersectionStatus result)
        {
            plane.Intersects(ref this, out result);
        }
        /// <summary>
        /// 检查当前包围球是否与指定射线相交
        /// </summary>
        /// <param name="ray">指定射线</param>
        /// <returns>相交true，不相交false</returns>
        public bool Intersects(Ray ray)
        {
            return ray.Intersects(this);
        }
        /// <summary>
        /// 检查当前包围球是否与指定射线相交 
        /// </summary>
        /// <param name="ray">指定射线</param>
        /// <param name="distance">[OutAttribute] 射线与包围球的距离，如果不相交，无意义</param>
        /// <returns>[输出属性] 相交true，不相交false</returns>
        public bool Intersects(ref Ray ray, out float distance)
        {
            return ray.Intersects(ref this, out distance);
        }
        /// <summary>
        /// 检查当前包围球是否与指定包围球相交 
        /// </summary>
        /// <param name="sphere">指定包围球</param>
        /// <returns>相交true，不相交false</returns>
        public bool Intersects(BoundingSphere sphere)
        {
            float num3;
            Vector3.DistanceSquared(ref this.Center, ref sphere.Center, out num3);
            float radius = this.Radius;
            float num = sphere.Radius;
            if ((((radius * radius) + ((2f * radius) * num)) + (num * num)) <= num3)
            {
                return false;
            }
            return true;
        }
        /// <summary>
        /// 检查当前包围球是否与指定包围球相交 
        /// </summary>
        /// <param name="sphere">指定包围球</param>
        /// <param name="result">[输出属性] 相交true，不相交false</param>
        public void Intersects(ref BoundingSphere sphere, out bool result)
        {
            float num3;
            Vector3.DistanceSquared(ref this.Center, ref sphere.Center, out num3);
            float radius = this.Radius;
            float num = sphere.Radius;
            result = (((radius * radius) + ((2f * radius) * num)) + (num * num)) > num3;
        }
        /// <summary>
        /// 检查当前包围球是否包含指定包围盒
        /// </summary>
        /// <param name="box">指定包围盒 </param>
        /// <returns>表明关系的枚举类型</returns>
        public ClipStatus Contains(BoundingBox box)
        {
            Vector3 vector;
            if (!box.Intersects(this))
            {
                return ClipStatus.Outside;
            }
            float num = this.Radius * this.Radius;
            vector.X = this.Center.X - box.Min.X;
            vector.Y = this.Center.Y - box.Max.Y;
            vector.Z = this.Center.Z - box.Max.Z;
            if (vector.LengthSquared() > num)
            {
                return ClipStatus.Intersecting;
            }
            vector.X = this.Center.X - box.Max.X;
            vector.Y = this.Center.Y - box.Max.Y;
            vector.Z = this.Center.Z - box.Max.Z;
            if (vector.LengthSquared() > num)
            {
                return ClipStatus.Intersecting;
            }
            vector.X = this.Center.X - box.Max.X;
            vector.Y = this.Center.Y - box.Min.Y;
            vector.Z = this.Center.Z - box.Max.Z;
            if (vector.LengthSquared() > num)
            {
                return ClipStatus.Intersecting;
            }
            vector.X = this.Center.X - box.Min.X;
            vector.Y = this.Center.Y - box.Min.Y;
            vector.Z = this.Center.Z - box.Max.Z;
            if (vector.LengthSquared() > num)
            {
                return ClipStatus.Intersecting;
            }
            vector.X = this.Center.X - box.Min.X;
            vector.Y = this.Center.Y - box.Max.Y;
            vector.Z = this.Center.Z - box.Min.Z;
            if (vector.LengthSquared() > num)
            {
                return ClipStatus.Intersecting;
            }
            vector.X = this.Center.X - box.Max.X;
            vector.Y = this.Center.Y - box.Max.Y;
            vector.Z = this.Center.Z - box.Min.Z;
            if (vector.LengthSquared() > num)
            {
                return ClipStatus.Intersecting;
            }
            vector.X = this.Center.X - box.Max.X;
            vector.Y = this.Center.Y - box.Min.Y;
            vector.Z = this.Center.Z - box.Min.Z;
            if (vector.LengthSquared() > num)
            {
                return ClipStatus.Intersecting;
            }
            vector.X = this.Center.X - box.Min.X;
            vector.Y = this.Center.Y - box.Min.Y;
            vector.Z = this.Center.Z - box.Min.Z;
            if (vector.LengthSquared() > num)
            {
                return ClipStatus.Intersecting;
            }
            return ClipStatus.Inside;
        }
        /// <summary>
        /// 检查当前包围球是否包含指定包围盒
        /// </summary>
        /// <param name="box">指定包围盒 </param>
        /// <param name="result">[输出属性] 表明关系的枚举类型</param>
        public void Contains(ref BoundingBox box, out ClipStatus result)
        {
            bool flag;
            box.Intersects(ref this, out flag);
            if (!flag)
            {
                result = ClipStatus.Outside;
            }
            else
            {
                Vector3 vector;
                float num = this.Radius * this.Radius;
                result = ClipStatus.Intersecting;
                vector.X = this.Center.X - box.Min.X;
                vector.Y = this.Center.Y - box.Max.Y;
                vector.Z = this.Center.Z - box.Max.Z;
                if (vector.LengthSquared() <= num)
                {
                    vector.X = this.Center.X - box.Max.X;
                    vector.Y = this.Center.Y - box.Max.Y;
                    vector.Z = this.Center.Z - box.Max.Z;
                    if (vector.LengthSquared() <= num)
                    {
                        vector.X = this.Center.X - box.Max.X;
                        vector.Y = this.Center.Y - box.Min.Y;
                        vector.Z = this.Center.Z - box.Max.Z;
                        if (vector.LengthSquared() <= num)
                        {
                            vector.X = this.Center.X - box.Min.X;
                            vector.Y = this.Center.Y - box.Min.Y;
                            vector.Z = this.Center.Z - box.Max.Z;
                            if (vector.LengthSquared() <= num)
                            {
                                vector.X = this.Center.X - box.Min.X;
                                vector.Y = this.Center.Y - box.Max.Y;
                                vector.Z = this.Center.Z - box.Min.Z;
                                if (vector.LengthSquared() <= num)
                                {
                                    vector.X = this.Center.X - box.Max.X;
                                    vector.Y = this.Center.Y - box.Max.Y;
                                    vector.Z = this.Center.Z - box.Min.Z;
                                    if (vector.LengthSquared() <= num)
                                    {
                                        vector.X = this.Center.X - box.Max.X;
                                        vector.Y = this.Center.Y - box.Min.Y;
                                        vector.Z = this.Center.Z - box.Min.Z;
                                        if (vector.LengthSquared() <= num)
                                        {
                                            vector.X = this.Center.X - box.Min.X;
                                            vector.Y = this.Center.Y - box.Min.Y;
                                            vector.Z = this.Center.Z - box.Min.Z;
                                            if (vector.LengthSquared() <= num)
                                            {
                                                result = ClipStatus.Inside;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        /// <summary>
        /// 检查当前包围球是否包含指定包围断面锥
        /// </summary>
        /// <param name="frustum">指定包围断面锥</param>
        /// <returns>表明关系的枚举类型</returns>
        public ClipStatus Contains(BoundingFrustum frustum)
        {
            if (null == frustum)
            {
                throw new ArgumentNullException("frustum", FrameworkResources.NullNotAllowed);
            }
            if (!frustum.Intersects(this))
            {
                return ClipStatus.Outside;
            }
            float num2 = this.Radius * this.Radius;
            foreach (Vector3 vector2 in frustum.cornerArray)
            {
                Vector3 vector;
                vector.X = vector2.X - this.Center.X;
                vector.Y = vector2.Y - this.Center.Y;
                vector.Z = vector2.Z - this.Center.Z;
                if (vector.LengthSquared() > num2)
                {
                    return ClipStatus.Intersecting;
                }
            }
            return ClipStatus.Inside;
        }
        /// <summary>
        /// 检查当前包围球是否包含指定点
        /// </summary>
        /// <param name="point">指定点</param>
        /// <returns>表明关系的枚举类型</returns>
        public ClipStatus Contains(Vector3 point)
        {
            if (Vector3.DistanceSquared(point, this.Center) >= (this.Radius * this.Radius))
            {
                return ClipStatus.Outside;
            }
            return ClipStatus.Inside;
        }
        /// <summary>
        /// 检查当前包围球是否包含指定点
        /// </summary>
        /// <param name="point">指定点</param>
        /// <param name="result">[输出属性] 表明关系的枚举类型</param>
        public void Contains(ref Vector3 point, out ClipStatus result)
        {
            float num;
            Vector3.DistanceSquared(ref point, ref this.Center, out num);
            result = (num < (this.Radius * this.Radius)) ? ClipStatus.Inside : ClipStatus.Outside;
        }
        /// <summary>
        /// 检查当前包围球是否包含指定包围球
        /// </summary>
        /// <param name="sphere">指定包围球</param>
        /// <returns>表明关系的枚举类型</returns>
        public ClipStatus Contains(BoundingSphere sphere)
        {
            float num3;
            Vector3.Distance(ref this.Center, ref sphere.Center, out num3);
            float radius = this.Radius;
            float num = sphere.Radius;
            if ((radius + num) < num3)
            {
                return ClipStatus.Outside;
            }
            if ((radius - num) < num3)
            {
                return ClipStatus.Intersecting;
            }
            return ClipStatus.Inside;
        }
        /// <summary>
        /// 检查当前包围球是否包含指定包围球 
        /// </summary>
        /// <param name="sphere">指定包围球 </param>
        /// <param name="result">[输出属性] 表明关系的枚举类型 </param>
        public void Contains(ref BoundingSphere sphere, out ClipStatus result)
        {
            float num3;
            Vector3.Distance(ref this.Center, ref sphere.Center, out num3);
            float radius = this.Radius;
            float num = sphere.Radius;
            result = ((radius + num) >= num3) ? (((radius - num) >= num3) ? ClipStatus.Inside : ClipStatus.Intersecting) : ClipStatus.Outside;
        }
       
        internal void SupportMapping(ref Vector3 v, out Vector3 result)
        {
            float num2 = v.Length();
            float num = this.Radius / num2;
            result.X = this.Center.X + (v.X * num);
            result.Y = this.Center.Y + (v.Y * num);
            result.Z = this.Center.Z + (v.Z * num);
        }
        /// <summary>
        /// 通过给定的矩阵平移和旋转包围球
        /// </summary>
        /// <param name="matrix">变换矩阵</param>
        /// <returns>变换后的包围球</returns>
        public BoundingSphere Transform(Matrix44 matrix)
        {
			Vector3 center = Matrix44.TransformPosition(matrix, Center);//Vector3.Transform(this.Center, matrix)
            
            float num4 = ((matrix.M00 * matrix.M00) + (matrix.M01 * matrix.M01)) + (matrix.M02 * matrix.M02);
            float num3 = ((matrix.M10 * matrix.M10) + (matrix.M11 * matrix.M11)) + (matrix.M12 * matrix.M12);
            float num2 = ((matrix.M20 * matrix.M20) + (matrix.M21 * matrix.M21)) + (matrix.M22 * matrix.M22);
            float num = Math.Max(num4, Math.Max(num3, num2));
			float radius = this.Radius * ((float) Math.Sqrt((double) num));
			return new BoundingSphere(center, radius);
        }
        /// <summary>
        /// 通过给定的矩阵平移和旋转包围球
        /// </summary>
        /// <param name="matrix">变换矩阵</param>
        /// <param name="result">[输出参数]变换后的包围球</param>
        public void Transform(ref Matrix44 matrix, out BoundingSphere result)
        {
            result.Center = Matrix44.TransformPosition(matrix, Center);//Vector3.Transform(this.Center, matrix)
            float num4 = ((matrix.M00 * matrix.M00) + (matrix.M01 * matrix.M01)) + (matrix.M02 * matrix.M02);
            float num3 = ((matrix.M10 * matrix.M10) + (matrix.M11 * matrix.M11)) + (matrix.M12 * matrix.M12);
            float num2 = ((matrix.M20 * matrix.M20) + (matrix.M21 * matrix.M21)) + (matrix.M22 * matrix.M22);
            float num = Math.Max(num4, Math.Max(num3, num2));
            result.Radius = this.Radius * ((float) Math.Sqrt((double) num));
        }
        /// <summary>
        /// 判定两个包围球的实例是否相等
        /// </summary>
        /// <param name="a">做相等操作左边的对象</param>
        /// <param name="b">做相等操作右边的对象</param>
        /// <returns>若左边等于右边，则返回true；否则，返回false</returns>
        public static bool operator ==(BoundingSphere a, BoundingSphere b)
        {
            return a.Equals(b);
        }
        /// <summary>
        /// 判定两个包围球的实例是否不相等
        /// </summary>
        /// <param name="a">做相等操作左边的对象</param>
        /// <param name="b">做相等操作右边的对象</param>
        /// <returns>若左边不等于右边，则返回true；否则，返回false</returns>
        public static bool operator !=(BoundingSphere a, BoundingSphere b)
        {
            if (!(a.Center != b.Center))
            {
                return !(a.Radius == b.Radius);
            }
            return true;
        }
    }
}

