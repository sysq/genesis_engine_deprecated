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
    /// 定义一个轴对齐型的包围盒
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct BoundingBox : IEquatable<BoundingBox>
    {
        /// <summary>
        /// 指定包围盒由八个顶点
        /// </summary>
        public const int CornerCount = 8;
        /// <summary>
        /// 包围盒中最小的点
        /// </summary>
        public Vector3 Min;
        /// <summary>
        /// 包围盒中最大的点
        /// </summary>
        public Vector3 Max;
        /// <summary>
        /// 获取一个数组，由包围盒八个角的点构成. 返回的点符合包围盒表面的角与Z轴垂直. 靠近的面应该是Z值最大的面，然后是Z值最小的面. 0~3 表示Z轴正方向上离原点较近的面 从左上角开始 顺时针依次  4~7  表示Z轴正方向上离原点较远的面 从左上角开始 顺时针依次
        /// </summary>
        /// <returns>一个代表包围盒8个顶点的三维向量数组 </returns>
        public Vector3[] GetCorners()
        {
            return new Vector3[] { new Vector3(this.Min.X, this.Max.Y, this.Max.Z), new Vector3(this.Max.X, this.Max.Y, this.Max.Z), new Vector3(this.Max.X, this.Min.Y, this.Max.Z), new Vector3(this.Min.X, this.Min.Y, this.Max.Z), new Vector3(this.Min.X, this.Max.Y, this.Min.Z), new Vector3(this.Max.X, this.Max.Y, this.Min.Z), new Vector3(this.Max.X, this.Min.Y, this.Min.Z), new Vector3(this.Min.X, this.Min.Y, this.Min.Z) };
        }
        /// <summary>
        /// 获取一个数组，由包围盒八个角的点构成. 返回的点符合包围盒表面的角与Z轴垂直. 靠近的面应该是Z值最大的面，然后是Z值最小的面. 0~3 表示Z轴正方向上离原点较近的面 从左上角开始 顺时针依次  4~7  表示Z轴正方向上离原点较远的面 从左上角开始 顺时针依次
        /// </summary>
        /// <param name="corners">将代表包围盒8个顶点写入三维向量数组</param>
        public void GetCorners(Vector3[] corners)
        {
            if (corners == null)
            {
                throw new ArgumentNullException("corners");
            }
            if (corners.Length < 8)
            {
                throw new ArgumentOutOfRangeException("corners", FrameworkResources.NotEnoughCorners);
            }
            corners[0].X = this.Min.X;
            corners[0].Y = this.Max.Y;
            corners[0].Z = this.Max.Z;
            corners[1].X = this.Max.X;
            corners[1].Y = this.Max.Y;
            corners[1].Z = this.Max.Z;
            corners[2].X = this.Max.X;
            corners[2].Y = this.Min.Y;
            corners[2].Z = this.Max.Z;
            corners[3].X = this.Min.X;
            corners[3].Y = this.Min.Y;
            corners[3].Z = this.Max.Z;
            corners[4].X = this.Min.X;
            corners[4].Y = this.Max.Y;
            corners[4].Z = this.Min.Z;
            corners[5].X = this.Max.X;
            corners[5].Y = this.Max.Y;
            corners[5].Z = this.Min.Z;
            corners[6].X = this.Max.X;
            corners[6].Y = this.Min.Y;
            corners[6].Z = this.Min.Z;
            corners[7].X = this.Min.X;
            corners[7].Y = this.Min.Y;
            corners[7].Z = this.Min.Z;
        }
        /// <summary>
        /// 创建一个包围盒实例 
        /// </summary>
        /// <param name="min">包围盒所包含最小的点</param>
        /// <param name="max">包围盒所包含最大的点</param>
        public BoundingBox(Vector3 min, Vector3 max)
        {
            this.Min = min;
            this.Max = max;
        }
        /// <summary>
        /// 判断两个包围盒是否相等 
        /// </summary>
        /// <param name="other">指定一个与之比较的包围盒 </param>
        /// <returns>相等返回true，不相等返回false </returns>
        public bool Equals(BoundingBox other)
        {
            return ((this.Min == other.Min) && (this.Max == other.Max));
        }
        /// <summary>
        /// 判断两个包围盒是否相等 
        /// </summary>
        /// <param name="obj">指定一个与之比较的对象 </param>
        /// <returns>相等返回true，不相等返回false </returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is BoundingBox)
            {
                flag = this.Equals((BoundingBox) obj);
            }
            return flag;
        }
        /// <summary>
        /// 计算包围盒的哈希码 
        /// </summary>
        /// <returns>包围盒的哈希吗</returns>
        public override int GetHashCode()
        {
            return (this.Min.GetHashCode() + this.Max.GetHashCode());
        }
        /// <summary>
        /// 返回代表当前包围盒的字符串 
        /// </summary>
        /// <returns>代表当前包围盒的字符串 </returns>
        public override string ToString()
        {
            return string.Format(CultureInfo.CurrentCulture, "Min:{0}, Max{1}", new object[] { this.Min.ToString(), this.Max.ToString() });
        }
        /// <summary>
        /// 创建一个能包含指定包围球的最小包围盒 
        /// </summary>
        /// <param name="sphere">指定包围球</param>
        /// <returns>创建的包围盒 </returns>
        public static BoundingBox CreateFromSphere(BoundingSphere sphere)
        {
            BoundingBox box;
            box.Min.X = sphere.Center.X - sphere.Radius;
            box.Min.Y = sphere.Center.Y - sphere.Radius;
            box.Min.Z = sphere.Center.Z - sphere.Radius;
            box.Max.X = sphere.Center.X + sphere.Radius;
            box.Max.Y = sphere.Center.Y + sphere.Radius;
            box.Max.Z = sphere.Center.Z + sphere.Radius;
            return box;
        }
        /// <summary>
        /// 创建一个能包含指定包围球的最小包围盒 
        /// </summary>
        /// <param name="sphere">指定包围球.</param>
        /// <param name="result">[OutAttribute] 创建的包围盒 </param>
        public static void CreateFromSphere(ref BoundingSphere sphere, out BoundingBox result)
        {
            result.Min.X = sphere.Center.X - sphere.Radius;
            result.Min.Y = sphere.Center.Y - sphere.Radius;
            result.Min.Z = sphere.Center.Z - sphere.Radius;
            result.Max.X = sphere.Center.X + sphere.Radius;
            result.Max.Y = sphere.Center.Y + sphere.Radius;
            result.Max.Z = sphere.Center.Z + sphere.Radius;
        }
        /// <summary>
        /// 创建能包含一组顶点的最小包围盒 
        /// </summary>
        /// <param name="points">包围盒需包含的一组顶点 </param>
        /// <returns>创建的包围盒</returns>
        public static BoundingBox CreateFromPoints(IEnumerable<Vector3> points)
        {
            if (points == null)
            {
                throw new ArgumentNullException();
            }
            bool flag = false;
            Vector3 vector3 = new Vector3(float.MaxValue);
            Vector3 vector2 = new Vector3(float.MinValue);
            foreach (Vector3 vector in points)
            {
                Vector3 vector4 = vector;
                Vector3.Min(ref vector3, ref vector4, out vector3);
                Vector3.Max(ref vector2, ref vector4, out vector2);
                flag = true;
            }
            if (!flag)
            {
                throw new ArgumentException(FrameworkResources.BoundingBoxZeroPoints);
            }
            return new BoundingBox(vector3, vector2);
        }
        /// <summary>
        /// 检查当前包围盒是否与指定包围盒相交
        /// </summary>
        /// <param name="box">检查是否与当前包围盒相交的包围盒</param>
        /// <returns>相交返回true，不相交false</returns>
        public bool Intersects(BoundingBox box)
        {
            if ((this.Max.X < box.Min.X) || (this.Min.X > box.Max.X))
            {
                return false;
            }
            if ((this.Max.Y < box.Min.Y) || (this.Min.Y > box.Max.Y))
            {
                return false;
            }
            return ((this.Max.Z >= box.Min.Z) && (this.Min.Z <= box.Max.Z));
        }
        /// <summary>
        /// 检查当前包围盒是否与指定包围盒相交
        /// </summary>
        /// <param name="box">指定的包围盒</param>
        /// <param name="result">[输出属性] 相交true，不相交false</param>
        public void Intersects(ref BoundingBox box, out bool result)
        {
            result = false;
            if ((((this.Max.X >= box.Min.X) && (this.Min.X <= box.Max.X)) && ((this.Max.Y >= box.Min.Y) && (this.Min.Y <= box.Max.Y))) && ((this.Max.Z >= box.Min.Z) && (this.Min.Z <= box.Max.Z)))
            {
                result = true;
            }
        }
        /// <summary>
        /// 检查当前包围盒是否与指定包围断面锥相交 
        /// </summary>
        /// <param name="frustum">指定的包围断面锥</param>
        /// <returns>相交true，不相交false</returns>
        public bool Intersects(BoundingFrustum frustum)
        {
            if (null == frustum)
            {
                throw new ArgumentNullException("frustum", FrameworkResources.NullNotAllowed);
            }
            return frustum.Intersects(this);
        }
        /// <summary>
        /// 检查当前包围盒是否与指定平面相交
        /// </summary>
        /// <param name="plane">指定的平面</param>
        /// <returns>一个枚举类型表明包围盒与平面是否相交</returns>
        public PlaneIntersectionStatus Intersects(Plane plane)
        {
            Vector3 vector;
            Vector3 vector2;
            vector2.X = (plane.Normal.X >= 0f) ? this.Min.X : this.Max.X;
            vector2.Y = (plane.Normal.Y >= 0f) ? this.Min.Y : this.Max.Y;
            vector2.Z = (plane.Normal.Z >= 0f) ? this.Min.Z : this.Max.Z;
            vector.X = (plane.Normal.X >= 0f) ? this.Max.X : this.Min.X;
            vector.Y = (plane.Normal.Y >= 0f) ? this.Max.Y : this.Min.Y;
            vector.Z = (plane.Normal.Z >= 0f) ? this.Max.Z : this.Min.Z;
            float num = ((plane.Normal.X * vector2.X) + (plane.Normal.Y * vector2.Y)) + (plane.Normal.Z * vector2.Z);
            if ((num + plane.D) > 0f)
            {
                return PlaneIntersectionStatus.Front;
            }
            num = ((plane.Normal.X * vector.X) + (plane.Normal.Y * vector.Y)) + (plane.Normal.Z * vector.Z);
            if ((num + plane.D) < 0f)
            {
                return PlaneIntersectionStatus.Back;
            }
            return PlaneIntersectionStatus.Intersecting;
        }
        /// <summary>
        /// 检查当前包围盒是否与指定平面相交 
        /// </summary>
        /// <param name="plane">指定的平面</param>
        /// <param name="result">[输出属性] 枚举类型表明包围盒与平面是否相交</param>
        public void Intersects(ref Plane plane, out PlaneIntersectionStatus result)
        {
            Vector3 vector;
            Vector3 vector2;
            vector2.X = (plane.Normal.X >= 0f) ? this.Min.X : this.Max.X;
            vector2.Y = (plane.Normal.Y >= 0f) ? this.Min.Y : this.Max.Y;
            vector2.Z = (plane.Normal.Z >= 0f) ? this.Min.Z : this.Max.Z;
            vector.X = (plane.Normal.X >= 0f) ? this.Max.X : this.Min.X;
            vector.Y = (plane.Normal.Y >= 0f) ? this.Max.Y : this.Min.Y;
            vector.Z = (plane.Normal.Z >= 0f) ? this.Max.Z : this.Min.Z;
            float num = ((plane.Normal.X * vector2.X) + (plane.Normal.Y * vector2.Y)) + (plane.Normal.Z * vector2.Z);
            if ((num + plane.D) > 0f)
            {
                result = PlaneIntersectionStatus.Front;
            }
            else
            {
                num = ((plane.Normal.X * vector.X) + (plane.Normal.Y * vector.Y)) + (plane.Normal.Z * vector.Z);
                if ((num + plane.D) < 0f)
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
        /// 检查当前包围盒是否与指定包围球相交 
        /// </summary>
        /// <param name="sphere">指定的包围球</param>
        /// <returns>相交true，不相交false</returns>
        public bool Intersects(BoundingSphere sphere)
        {
            float num;
            Vector3 vector;
            Vector3.Clamp(ref sphere.Center, ref this.Min, ref this.Max, out vector);
            Vector3.DistanceSquared(ref sphere.Center, ref vector, out num);
            return (num <= (sphere.Radius * sphere.Radius));
        }
        /// <summary>
        /// 检查当前包围盒是否与指定包围球相交 
        /// </summary>
        /// <param name="sphere">指定的包围球</param>
        /// <param name="result">[OutAttribute] 相交true，不相交false</param>
        public void Intersects(ref BoundingSphere sphere, out bool result)
        {
            float num;
            Vector3 vector;
            Vector3.Clamp(ref sphere.Center, ref this.Min, ref this.Max, out vector);
            Vector3.DistanceSquared(ref sphere.Center, ref vector, out num);
            result = num <= (sphere.Radius * sphere.Radius);
        }
        /// <summary>
        /// 检查当前包围盒是否与指定射线相交
        /// </summary>
        /// <param name="ray">指定的射线</param>
        /// <returns>相交true，不相交false</returns>
        public bool Intersects(Ray ray)
        {
            float num = 0f;
            float maxValue = float.MaxValue;
            if (Math.Abs(ray.Direction.X) < 1E-06f)
            {
                if ((ray.Position.X < this.Min.X) || (ray.Position.X > this.Max.X))
                {
                    return false;
                }
            }
            else
            {
                float num11 = 1f / ray.Direction.X;
                float num8 = (this.Min.X - ray.Position.X) * num11;
                float num7 = (this.Max.X - ray.Position.X) * num11;
                if (num8 > num7)
                {
                    float num14 = num8;
                    num8 = num7;
                    num7 = num14;
                }
                num = MathHelper.Max(num8, num);
                maxValue = MathHelper.Min(num7, maxValue);
                if (num > maxValue)
                {
                    return false;
                }
            }
            if (Math.Abs(ray.Direction.Y) < 1E-06f)
            {
                if ((ray.Position.Y < this.Min.Y) || (ray.Position.Y > this.Max.Y))
                {
                    return false;
                }
            }
            else
            {
                float num10 = 1f / ray.Direction.Y;
                float num6 = (this.Min.Y - ray.Position.Y) * num10;
                float num5 = (this.Max.Y - ray.Position.Y) * num10;
                if (num6 > num5)
                {
                    float num13 = num6;
                    num6 = num5;
                    num5 = num13;
                }
                num = MathHelper.Max(num6, num);
                maxValue = MathHelper.Min(num5, maxValue);
                if (num > maxValue)
                {
                    return false;
                }
            }
            if (Math.Abs(ray.Direction.Z) < 1E-06f)
            {
                if ((ray.Position.Z < this.Min.Z) || (ray.Position.Z > this.Max.Z))
                {
                    return false;
                }
            }
            else
            {
                float num9 = 1f / ray.Direction.Z;
                float num4 = (this.Min.Z - ray.Position.Z) * num9;
                float num3 = (this.Max.Z - ray.Position.Z) * num9;
                if (num4 > num3)
                {
                    float num12 = num4;
                    num4 = num3;
                    num3 = num12;
                }
                num = MathHelper.Max(num4, num);
                maxValue = MathHelper.Min(num3, maxValue);
                if (num > maxValue)
                {
                    return false;
                }
            }
            return true;
        }
        /// <summary>
        /// 检查当前包围盒是否与指定射线相交
        /// </summary>
        /// <param name="ray">指定的射线 </param>
        /// <param name="distance">[OutAttribute] Distance at which the ray intersects the BoundingBox.If no intersect, "distance" is meaningless</param>
        /// <returns>相交true，不相交false</returns>
        public bool Intersects(ref Ray ray, out float distance)
        {
            float num = 0f;
            distance = 0f;
            float maxValue = float.MaxValue;
            if (Math.Abs(ray.Direction.X) < 1E-06f)
            {
                if ((ray.Position.X < this.Min.X) || (ray.Position.X > this.Max.X))
                {
                    return false;
                }
            }
            else
            {
                float num11 = 1f / ray.Direction.X;
                float num8 = (this.Min.X - ray.Position.X) * num11;
                float num7 = (this.Max.X - ray.Position.X) * num11;
                if (num8 > num7)
                {
                    float num14 = num8;
                    num8 = num7;
                    num7 = num14;
                }
                num = MathHelper.Max(num8, num);
                maxValue = MathHelper.Min(num7, maxValue);
                if (num > maxValue)
                {
                    return false;
                }
            }
            if (Math.Abs(ray.Direction.Y) < 1E-06f)
            {
                if ((ray.Position.Y < this.Min.Y) || (ray.Position.Y > this.Max.Y))
                {
                    return false;
                }
            }
            else
            {
                float num10 = 1f / ray.Direction.Y;
                float num6 = (this.Min.Y - ray.Position.Y) * num10;
                float num5 = (this.Max.Y - ray.Position.Y) * num10;
                if (num6 > num5)
                {
                    float num13 = num6;
                    num6 = num5;
                    num5 = num13;
                }
                num = MathHelper.Max(num6, num);
                maxValue = MathHelper.Min(num5, maxValue);
                if (num > maxValue)
                {
                    return false;
                }
            }
            if (Math.Abs(ray.Direction.Z) < 1E-06f)
            {
                if ((ray.Position.Z < this.Min.Z) || (ray.Position.Z > this.Max.Z))
                {
                    return false;
                }
            }
            else
            {
                float num9 = 1f / ray.Direction.Z;
                float num4 = (this.Min.Z - ray.Position.Z) * num9;
                float num3 = (this.Max.Z - ray.Position.Z) * num9;
                if (num4 > num3)
                {
                    float num12 = num4;
                    num4 = num3;
                    num3 = num12;
                }
                num = MathHelper.Max(num4, num);
                maxValue = MathHelper.Min(num3, maxValue);
                if (num > maxValue)
                {
                    return false;
                }
            }
            distance = num;
            return true;
        }
        /// <summary>
        /// 检测当前包围盒是否包含其他包围盒
        /// </summary>
        /// <param name="box">指定的包围盒</param>
        /// <returns>表明两包围盒关系的枚举类型</returns>
        public ClipStatus Contains(BoundingBox box)
        {
            if ((this.Max.X < box.Min.X) || (this.Min.X > box.Max.X))
            {
                return ClipStatus.Outside;
            }
            if ((this.Max.Y < box.Min.Y) || (this.Min.Y > box.Max.Y))
            {
                return ClipStatus.Outside;
            }
            if ((this.Max.Z < box.Min.Z) || (this.Min.Z > box.Max.Z))
            {
                return ClipStatus.Outside;
            }
            if ((((this.Min.X <= box.Min.X) && (box.Max.X <= this.Max.X)) && ((this.Min.Y <= box.Min.Y) && (box.Max.Y <= this.Max.Y))) && ((this.Min.Z <= box.Min.Z) && (box.Max.Z <= this.Max.Z)))
            {
                return ClipStatus.Inside;
            }
            return ClipStatus.Intersecting;
        }
        /// <summary>
        /// 检测当前包围盒是否包含其他包围盒
        /// </summary>
        /// <param name="box">指定的包围盒</param>
        /// <param name="result">[OutAttribute] 表明两包围盒关系的枚举类型 </param>
        public void Contains(ref BoundingBox box, out ClipStatus result)
        {
            result = ClipStatus.Outside;
            if ((((this.Max.X >= box.Min.X) && (this.Min.X <= box.Max.X)) && ((this.Max.Y >= box.Min.Y) && (this.Min.Y <= box.Max.Y))) && ((this.Max.Z >= box.Min.Z) && (this.Min.Z <= box.Max.Z)))
            {
                result = ((((this.Min.X <= box.Min.X) && (box.Max.X <= this.Max.X)) && ((this.Min.Y <= box.Min.Y) && (box.Max.Y <= this.Max.Y))) && ((this.Min.Z <= box.Min.Z) && (box.Max.Z <= this.Max.Z))) ? ClipStatus.Inside : ClipStatus.Intersecting;
            }
        }
        /// <summary>
        /// 检测当前包围盒是否包含指定包围断面锥
        /// </summary>
        /// <param name="frustum">指定的包围断面锥 </param>
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
            foreach (Vector3 vector in frustum.cornerArray)
            {
                if (this.Contains(vector) == ClipStatus.Outside)
                {
                    return ClipStatus.Intersecting;
                }
            }
            return ClipStatus.Inside;
        }
        /// <summary>
        /// 检测当前包围盒是否包含指定点
        /// </summary>
        /// <param name="point">指定点</param>
        /// <returns>表明关系的枚举类型</returns>
        public ClipStatus Contains(Vector3 point)
        {
            if ((((this.Min.X <= point.X) && (point.X <= this.Max.X)) && ((this.Min.Y <= point.Y) && (point.Y <= this.Max.Y))) && ((this.Min.Z <= point.Z) && (point.Z <= this.Max.Z)))
            {
                return ClipStatus.Inside;
            }
            return ClipStatus.Outside;
        }
        /// <summary>
        /// 检测当前包围盒是否包含指定点
        /// </summary>
        /// <param name="point">指定点 </param>
        /// <param name="result">[输出属性] 表明关系的枚举类型</param>
        public void Contains(ref Vector3 point, out ClipStatus result)
        {
            result = ((((this.Min.X <= point.X) && (point.X <= this.Max.X)) && ((this.Min.Y <= point.Y) && (point.Y <= this.Max.Y))) && ((this.Min.Z <= point.Z) && (point.Z <= this.Max.Z))) ? ClipStatus.Inside : ClipStatus.Outside;
        }
        /// <summary>
        /// 检测当前包围盒是否包含指定包围球
        /// </summary>
        /// <param name="sphere">指定包围球</param>
        /// <returns>表明关系的枚举类型</returns>
        public ClipStatus Contains(BoundingSphere sphere)
        {
            float num2;
            Vector3 vector;
            Vector3.Clamp(ref sphere.Center, ref this.Min, ref this.Max, out vector);
            Vector3.DistanceSquared(ref sphere.Center, ref vector, out num2);
            float radius = sphere.Radius;
            if (num2 > (radius * radius))
            {
                return ClipStatus.Outside;
            }
            if (((((this.Min.X + radius) <= sphere.Center.X) && (sphere.Center.X <= (this.Max.X - radius))) && (((this.Max.X - this.Min.X) > radius) && ((this.Min.Y + radius) <= sphere.Center.Y))) && (((sphere.Center.Y <= (this.Max.Y - radius)) && ((this.Max.Y - this.Min.Y) > radius)) && ((((this.Min.Z + radius) <= sphere.Center.Z) && (sphere.Center.Z <= (this.Max.Z - radius))) && ((this.Max.X - this.Min.X) > radius))))
            {
                return ClipStatus.Inside;
            }
            return ClipStatus.Intersecting;
        }
        /// <summary>
        /// 检测当前包围盒是否包含指定包围球
        /// </summary>
        /// <param name="sphere">指定包围球</param>
        /// <param name="result">[输出属性] 表明关系的枚举类型</param>
        public void Contains(ref BoundingSphere sphere, out ClipStatus result)
        {
            float num2;
            Vector3 vector;
            Vector3.Clamp(ref sphere.Center, ref this.Min, ref this.Max, out vector);
            Vector3.DistanceSquared(ref sphere.Center, ref vector, out num2);
            float radius = sphere.Radius;
            if (num2 > (radius * radius))
            {
                result = ClipStatus.Outside;
            }
            else
            {
                result = (((((this.Min.X + radius) <= sphere.Center.X) && (sphere.Center.X <= (this.Max.X - radius))) && (((this.Max.X - this.Min.X) > radius) && ((this.Min.Y + radius) <= sphere.Center.Y))) && (((sphere.Center.Y <= (this.Max.Y - radius)) && ((this.Max.Y - this.Min.Y) > radius)) && ((((this.Min.Z + radius) <= sphere.Center.Z) && (sphere.Center.Z <= (this.Max.Z - radius))) && ((this.Max.X - this.Min.X) > radius)))) ? ClipStatus.Inside : ClipStatus.Intersecting;
            }
        }
        /// <summary>
        /// 用给出的矩阵平移缩放包围盒 
        /// </summary>
        /// <param name="matrix">转化矩阵</param>
        /// <returns>改变后的包围盒</returns>
        public BoundingBox Transform(Matrix44 matrix)
        {
            Vector3[] corners = GetCorners();
            for (int i = 0; i < BoundingBox.CornerCount; ++i )
            {
                Matrix44.TransformPosition(ref matrix, ref corners[i], out corners[i]);
            }
            return CreateFromPoints(corners);
        }
        /// <summary>
        /// 用给出的矩阵平移缩放包围盒 
        /// </summary>
        /// <param name="matrix">转化矩阵</param>
        /// <param name="result">[输出属性] 改变后的包围盒</param>
        public void Transform(ref Matrix44 matrix, out BoundingBox result)
        {
            Vector3[] corners = GetCorners();
            for (int i = 0; i < BoundingBox.CornerCount; ++i)
            {
                Matrix44.TransformPosition(ref matrix, ref corners[i], out corners[i]);
            }
            result = CreateFromPoints(corners);
        }
        /// <summary>
        /// 支持映射
        /// </summary>
        /// <param name="v">源向量</param>
        /// <param name="result">结果</param>
        internal void SupportMapping(ref Vector3 v, out Vector3 result)
        {
            result.X = (v.X >= 0f) ? this.Max.X : this.Min.X;
            result.Y = (v.Y >= 0f) ? this.Max.Y : this.Min.Y;
            result.Z = (v.Z >= 0f) ? this.Max.Z : this.Min.Z;
        }
        /// <summary>
        /// 判断两个包围盒是否相等
        /// </summary>
        /// <param name="a">要比较的包围盒 </param>
        /// <param name="b">要比较的包围盒 </param>
        /// <returns>相等true，不相等false</returns>
        public static bool operator ==(BoundingBox a, BoundingBox b)
        {
            return a.Equals(b);
        }
        /// <summary>
        /// 判断两个包围盒是否不相等 
        /// </summary>
        /// <param name="a">要比较的包围盒 </param>
        /// <param name="b">要比较的包围盒 </param>
        /// <returns>不相等true，相等false</returns>
        public static bool operator !=(BoundingBox a, BoundingBox b)
        {
            if (!(a.Min != b.Min))
            {
                return (a.Max != b.Max);
            }
            return true;
        }
    }
}


