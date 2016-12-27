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
    /// 定义一个平截头体，并提供相应的检测相交关系的方法
    /// </summary>
    [Serializable, TypeConverter(typeof(ExpandableObjectConverter))]
    public class BoundingFrustum : IEquatable<BoundingFrustum>
    { 
        /// <summary>
        /// 指定平截头体有8个角
        /// </summary>
        public const int CornerCount = 8;
        private const int BottomPlaneIndex = 5;
        internal Vector3[] cornerArray;
        private const int FarPlaneIndex = 1;
        private Gjk gjk;
        private const int LeftPlaneIndex = 2;
        private Matrix44 matrix;
        private const int NearPlaneIndex = 0;
        private const int NumPlanes = 6;
        private Plane[] planes;
        private const int RightPlaneIndex = 3;
        private const int TopPlaneIndex = 4;

        private BoundingFrustum()
        {
            this.planes = new Plane[6];
            this.cornerArray = new Vector3[8];
        }
        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="value">观察与投影矩阵组成的复合矩阵</param>
        public BoundingFrustum(Matrix44 value)
        {
            this.planes = new Plane[6];
            this.cornerArray = new Vector3[8];
            this.SetMatrix(ref value);
        }
        /// <summary>
        /// 检测平截头体与指定的包围盒的相交关系
        /// </summary>
        /// <param name="box">要检测的包围盒</param>
        /// <returns>相交关系</returns>
        public ClipStatus Contains(BoundingBox box)
        {
            bool flag = false;
            foreach (Plane plane in this.planes)
            {
                switch (box.Intersects(plane))
                {
                    case PlaneIntersectionStatus.Front:
                        return ClipStatus.Outside;

                    case PlaneIntersectionStatus.Intersecting:
                        flag = true;
                        break;
                }
            }
            if (!flag)
            {
                return ClipStatus.Inside;
            }
            return ClipStatus.Intersecting;
        }
        /// <summary>
        /// 检测当前平截头体与指定的平截头体的相交关系
        /// </summary>
        /// <param name="frustum">要检测的平截头体</param>
        /// <returns>相交关系</returns>
        public ClipStatus Contains(BoundingFrustum frustum)
        {
            if (frustum == null)
            {
                throw new ArgumentNullException("frustum");
            }
            ClipStatus disjoint = ClipStatus.Outside;
            if (this.Intersects(frustum))
            {
                disjoint = ClipStatus.Inside;
                for (int i = 0; i < this.cornerArray.Length; i++)
                {
                    if (this.Contains(frustum.cornerArray[i]) == ClipStatus.Outside)
                    {
                        return ClipStatus.Intersecting;
                    }
                }
            }
            return disjoint;
        }
        /// <summary>
        /// 检测当前平截头体与指定的包围球的相交关系
        /// </summary>
        /// <param name="sphere">要检测的包围球</param>
        /// <returns>相交关系</returns>
        public ClipStatus Contains(BoundingSphere sphere)
        {
            Vector3 center = sphere.Center;
            float radius = sphere.Radius;
            int num2 = 0;
            foreach (Plane plane in this.planes)
            {
                float num5 = ((plane.Normal.X * center.X) + (plane.Normal.Y * center.Y)) + (plane.Normal.Z * center.Z);
                float num3 = num5 + plane.D;
                if (num3 > radius)
                {
                    return ClipStatus.Outside;
                }
                if (num3 < -radius)
                {
                    num2++;
                }
            }
            if (num2 != 6)
            {
                return ClipStatus.Intersecting;
            }
            return ClipStatus.Inside;
        }
        /// <summary>
        /// 检测平截头体与点的包含关系
        /// </summary>
        /// <param name="point">要检测的点</param>
        /// <returns>平截头体与点的包含关系</returns>
        public ClipStatus Contains(Vector3 point)
        {
            foreach (Plane plane in this.planes)
            {
                float num2 = (((plane.Normal.X * point.X) + (plane.Normal.Y * point.Y)) + (plane.Normal.Z * point.Z)) + plane.D;
                if (num2 > 1E-05f)
                {
                    return ClipStatus.Outside;
                }
            }
            return ClipStatus.Inside;
        }
        /// <summary>
        /// 检测当前平截头体与指定的包围盒的相交关系 
        /// </summary>
        /// <param name="box">要检测的包围盒</param>
        /// <param name="result">[输出参数] 当前平截头体与包围盒的相交关系</param>
        public void Contains(ref BoundingBox box, out ClipStatus result)
        {
            bool flag = false;
            foreach (Plane plane in this.planes)
            {
                switch (box.Intersects(plane))
                {
                    case PlaneIntersectionStatus.Front:
                        result = ClipStatus.Outside;
                        return;

                    case PlaneIntersectionStatus.Intersecting:
                        flag = true;
                        break;
                }
            }
            result = flag ? ClipStatus.Intersecting : ClipStatus.Inside;
        }
        /// <summary>
        /// 检测当前平截头体与指定的包围球的相交关系 
        /// </summary>
        /// <param name="box">要检测的包围球</param>
        /// <param name="result">[输出参数] 当前平截头体与包围球的相交关系</param>
        public void Contains(ref BoundingSphere sphere, out ClipStatus result)
        {
            Vector3 center = sphere.Center;
            float radius = sphere.Radius;
            int num2 = 0;
            foreach (Plane plane in this.planes)
            {
                float num5 = ((plane.Normal.X * center.X) + (plane.Normal.Y * center.Y)) + (plane.Normal.Z * center.Z);
                float num3 = num5 + plane.D;
                if (num3 > radius)
                {
                    result = ClipStatus.Outside;
                    return;
                }
                if (num3 < -radius)
                {
                    num2++;
                }
            }
            result = (num2 == 6) ? ClipStatus.Inside : ClipStatus.Intersecting;
        }        
        
        /// <summary>
        /// 检测当前平截头体与点的包含关系
        /// </summary>
        /// <param name="point">要检测的点</param>
        /// <param name="result">[输出参数] 平截头体与点的包含关系</param>
        public void Contains(ref Vector3 point, out ClipStatus result)
        {
            foreach (Plane plane in this.planes)
            {
                float num2 = (((plane.Normal.X * point.X) + (plane.Normal.Y * point.Y)) + (plane.Normal.Z * point.Z)) + plane.D;
                if (num2 > 1E-05f)
                {
                    result = ClipStatus.Outside;
                    return;
                }
            }
            result = ClipStatus.Inside;
        }
        /// <summary>
        /// 判断两个平截头体是否相同
        /// </summary>
        /// <param name="other">要比较的平截头体</param>
        /// <returns>如果相同，返回true，否则，返回false</returns>
        public bool Equals(BoundingFrustum other)
        {
            if (other == null)
            {
                return false;
            }
            return (this.matrix == other.matrix);
        }
        /// <summary>
        /// 判断传入的对象是否是与当前平截头体相同的对象 
        /// </summary>
        /// <param name="obj">要比较的对象</param>
        /// <returns>如果相同，返回true，否则，返回false </returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            BoundingFrustum frustum = obj as BoundingFrustum;
            if (frustum != null)
            {
                flag = this.matrix == frustum.matrix;
            }
            return flag;
        }
        /// <summary>
        /// 返回当前平截头体的角(位置)的数组。返回的点的顺序(位于Z轴正方向，看向Z轴原点)：近左上角，近右上角，近右下角，近左下角，远左上角，远右上角，远右下角，远左下角
        /// </summary>
        /// <returns>点的数组</returns>
        public Vector3[] GetCorners()
        {
            return (Vector3[]) this.cornerArray.Clone();
        }
        /// <summary>
        /// 返回当前平截头体的角(位置)的数组。返回的点的顺序(位于Z轴正方向，看向Z轴原点)：近左上角，近右上角，近右下角，近左下角，远左上角，远右上角，远右下角，远左下角
        /// </summary>
        /// <param name="corners">用于存储返回值的数组(数组大小最小为8) </param>
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
            this.cornerArray.CopyTo(corners, 0);
        }
        /// <summary>
        /// 返回对象的哈希值
        /// </summary>
        /// <returns>当前平截头体对象的哈希值</returns>
        public override int GetHashCode()
        {
            return this.matrix.GetHashCode();
        }
        /// <summary>
        /// 检测当前平截头体与指定的包围盒是否相交
        /// </summary>
        /// <param name="box">要检测的包围盒</param>
        /// <returns>如果相交，返回true，否则，返回false </returns>
        public bool Intersects(BoundingBox box)
        {
            bool flag;
            this.Intersects(ref box, out flag);
            return flag;
        }
        /// <summary>
        /// 检测当前平截头体与指定的平截头体是否相交
        /// </summary>
        /// <param name="frustum">要检测的平截头体</param>
        /// <returns>如果相交，返回true，否则，返回false</returns>
        public bool Intersects(BoundingFrustum frustum)
        {
            Vector3 closestPoint;
            if (frustum == null)
            {
                throw new ArgumentNullException("frustum");
            }
            if (this.gjk == null)
            {
                this.gjk = new Gjk();
            }
            this.gjk.Reset();
            Vector3.Sub(ref this.cornerArray[0], ref frustum.cornerArray[0], out closestPoint);
            if (closestPoint.LengthSquared() < 1E-05f)
            {
                Vector3.Sub(ref this.cornerArray[0], ref frustum.cornerArray[1], out closestPoint);
            }
            float maxValue = float.MaxValue;
            float num3 = 0f;
            do
            {
                Vector3 vector2;
                Vector3 vector3;
                Vector3 vector4;
                Vector3 vector5;
                vector5.X = -closestPoint.X;
                vector5.Y = -closestPoint.Y;
                vector5.Z = -closestPoint.Z;
                this.SupportMapping(ref vector5, out vector4);
                frustum.SupportMapping(ref closestPoint, out vector3);
                Vector3.Sub(ref vector4, ref vector3, out vector2);
                float num4 = ((closestPoint.X * vector2.X) + (closestPoint.Y * vector2.Y)) + (closestPoint.Z * vector2.Z);
                if (num4 > 0f)
                {
                    return false;
                }
                this.gjk.AddSupportPoint(ref vector2);
                closestPoint = this.gjk.ClosestPoint;
                float num2 = maxValue;
                maxValue = closestPoint.LengthSquared();
                num3 = 4E-05f * this.gjk.MaxLengthSquared;
                if ((num2 - maxValue) <= (1E-05f * num2))
                {
                    return false;
                }
            }
            while (!this.gjk.FullSimplex && (maxValue >= num3));
            return true;
        }
        /// <summary>
        /// 检测当前平截头体与指定的包围球是否相交
        /// </summary>
        /// <param name="frustum">要检测的包围球</param>
        /// <returns>如果相交，返回true，否则，返回false</returns>
        public bool Intersects(BoundingSphere sphere)
        {
            bool flag;
            this.Intersects(ref sphere, out flag);
            return flag;
        }
        /// <summary>
        /// 检测当前平截头体与指定的平面是否相交
        /// </summary>
        /// <param name="plane">要检测的平面</param>
        /// <returns>指示是否相交的枚举</returns>
        public PlaneIntersectionStatus Intersects(Plane plane)
        {
            int num = 0;
            for (int i = 0; i < 8; i++)
            {
                float num3;
                Vector3.Dot(ref this.cornerArray[i], ref plane.Normal, out num3);
                if ((num3 + plane.D) > 0f)
                {
                    num |= 1;
                }
                else
                {
                    num |= 2;
                }
                if (num == 3)
                {
                    return PlaneIntersectionStatus.Intersecting;
                }
            }
            if (num != 1)
            {
                return PlaneIntersectionStatus.Back;
            }
            return PlaneIntersectionStatus.Front;
        }
        /// <summary>
        /// 检测当前平截头体与指定的射线是否相交
        /// </summary>
        /// <param name="ray">要检测的射线</param>
        /// <returns>如果相交，返回true，否则，返回false</returns>
        public bool Intersects(Ray ray)
        {
            ClipStatus type;
            this.Contains(ref ray.Position, out type);
            if (type == ClipStatus.Inside)
            {
                return true;
            }
            else
            {
                float minValue = float.MinValue;
                float maxValue = float.MaxValue;
                foreach (Plane plane in this.planes)
                {
                    float num3;
                    float num6;
                    Vector3 normal = plane.Normal;
                    Vector3.Dot(ref ray.Direction, ref normal, out num6);
                    Vector3.Dot(ref ray.Position, ref normal, out num3);
                    num3 += plane.D;
                    if (Math.Abs(num6) < 1E-05f)
                    {
                        if (num3 > 0f)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        float num = -num3 / num6;
                        if (num6 < 0f)
                        {
                            if (num > maxValue)
                            {
                                return true;
                            }
                            if (num > minValue)
                            {
                                minValue = num;
                            }
                        }
                        else
                        {
                            if (num < minValue)
                            {
                                return true;
                            }
                            if (num < maxValue)
                            {
                                maxValue = num;
                            }
                        }
                    }
                }
                float num7 = (minValue >= 0f) ? minValue : maxValue;
                if (num7 >= 0f)
                {
                    return true;
                }
            }
            return false;
        }
        /// <summary>
        /// 检测当前平截头体是否与指定的包围盒相交
        /// </summary>
        /// <param name="box">要检测的包围盒</param>
        /// <param name="result">[输出参数] 如果相交，返回true，否则，返回false</param>
        public void Intersects(ref BoundingBox box, out bool result)
        {
            Vector3 closestPoint;
            Vector3 vector2;
            Vector3 vector3;
            Vector3 vector4;
            Vector3 vector5;
            if (this.gjk == null)
            {
                this.gjk = new Gjk();
            }
            this.gjk.Reset();
            Vector3.Sub(ref this.cornerArray[0], ref box.Min, out closestPoint);
            if (closestPoint.LengthSquared() < 1E-05f)
            {
                Vector3.Sub(ref this.cornerArray[0], ref box.Max, out closestPoint);
            }
            float maxValue = float.MaxValue;
            float num3 = 0f;
            result = false;
        Label_006D:
            vector5.X = -closestPoint.X;
            vector5.Y = -closestPoint.Y;
            vector5.Z = -closestPoint.Z;
            this.SupportMapping(ref vector5, out vector4);
            box.SupportMapping(ref closestPoint, out vector3);
            Vector3.Sub(ref vector4, ref vector3, out vector2);
            float num4 = ((closestPoint.X * vector2.X) + (closestPoint.Y * vector2.Y)) + (closestPoint.Z * vector2.Z);
            if (num4 <= 0f)
            {
                this.gjk.AddSupportPoint(ref vector2);
                closestPoint = this.gjk.ClosestPoint;
                float num2 = maxValue;
                maxValue = closestPoint.LengthSquared();
                if ((num2 - maxValue) > (1E-05f * num2))
                {
                    num3 = 4E-05f * this.gjk.MaxLengthSquared;
                    if (!this.gjk.FullSimplex && (maxValue >= num3))
                    {
                        goto Label_006D;
                    }
                    result = true;
                }
            }
        }
        /// <summary>
        /// 检测当前平截头体是否与指定的包围球相交
        /// </summary>
        /// <param name="box">要检测的包围球</param>
        /// <param name="result">[输出参数] 如果相交，返回true，否则，返回false</param>
        public void Intersects(ref BoundingSphere sphere, out bool result)
        {
            Vector3 unitX;
            Vector3 vector2;
            Vector3 vector3;
            Vector3 vector4;
            Vector3 vector5;
            if (this.gjk == null)
            {
                this.gjk = new Gjk();
            }
            this.gjk.Reset();
            Vector3.Sub(ref this.cornerArray[0], ref sphere.Center, out unitX);
            if (unitX.LengthSquared() < 1E-05f)
            {
                unitX = Vector3.UnitX;
            }
            float maxValue = float.MaxValue;
            float num3 = 0f;
            result = false;
        Label_005A:
            vector5.X = -unitX.X;
            vector5.Y = -unitX.Y;
            vector5.Z = -unitX.Z;
            this.SupportMapping(ref vector5, out vector4);
            sphere.SupportMapping(ref unitX, out vector3);
            Vector3.Sub(ref vector4, ref vector3, out vector2);
            float num4 = ((unitX.X * vector2.X) + (unitX.Y * vector2.Y)) + (unitX.Z * vector2.Z);
            if (num4 <= 0f)
            {
                this.gjk.AddSupportPoint(ref vector2);
                unitX = this.gjk.ClosestPoint;
                float num2 = maxValue;
                maxValue = unitX.LengthSquared();
                if ((num2 - maxValue) > (1E-05f * num2))
                {
                    num3 = 4E-05f * this.gjk.MaxLengthSquared;
                    if (!this.gjk.FullSimplex && (maxValue >= num3))
                    {
                        goto Label_005A;
                    }
                    result = true;
                }
            }
        }
        /// <summary>
        /// 检测当前平截头体是否与指定的平面相交
        /// </summary>
        /// <param name="box">要检测的平面</param>
        /// <param name="result">[输出参数] 指示是否相交的枚举</param>
        public void Intersects(ref Plane plane, out PlaneIntersectionStatus result)
        {
            int num = 0;
            for (int i = 0; i < 8; i++)
            {
                float num3;
                Vector3.Dot(ref this.cornerArray[i], ref plane.Normal, out num3);
                if ((num3 + plane.D) > 0f)
                {
                    num |= 1;
                }
                else
                {
                    num |= 2;
                }
                if (num == 3)
                {
                    result = PlaneIntersectionStatus.Intersecting;
                    return;
                }
            }
            result = (num == 1) ? PlaneIntersectionStatus.Front : PlaneIntersectionStatus.Back;
        }
        /// <summary>
        /// 检测当前平截头体是否与指定的射线相交
        /// </summary>
        /// <param name="ray">要检测的射线</param>
        /// <param name="result">[输出参数] 射线的起点相交点的距离。如果没有相交，则此参数值无意义</param>
        /// <returns>如果相交，返回true，否则，返回false</returns>
        public bool Intersects(ref Ray ray, out float result)
        {
            ClipStatus type;
            this.Contains(ref ray.Position, out type);
            if (type == ClipStatus.Inside)
            {
                result = 0f;
                return true;
            }
            else
            {
                float minValue = float.MinValue;
                float maxValue = float.MaxValue;
                result = 0;
                foreach (Plane plane in this.planes)
                {
                    float num3;
                    float num6;
                    Vector3 normal = plane.Normal;
                    Vector3.Dot(ref ray.Direction, ref normal, out num6);
                    Vector3.Dot(ref ray.Position, ref normal, out num3);
                    num3 += plane.D;
                    if (Math.Abs(num6) < 1E-05f)
                    {
                        if (num3 > 0f)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        float num = -num3 / num6;
                        if (num6 < 0f)
                        {
                            if (num > maxValue)
                            {
                                return true;
                            }
                            if (num > minValue)
                            {
                                minValue = num;
                            }
                        }
                        else
                        {
                            if (num < minValue)
                            {
                                return true;
                            }
                            if (num < maxValue)
                            {
                                maxValue = num;
                            }
                        }
                    }
                }
                float num7 = (minValue >= 0f) ? minValue : maxValue;
                if (num7 >= 0f)
                {
                    result = num7;
                    return true;
                }
            }
            return false;
        }
        /// <summary>
        /// 比较两个平截头体对象是否相同
        /// </summary>
        /// <param name="a">等号操作符的左参数</param>
        /// <param name="b">等号操作符的右参数</param>
        /// <returns></returns>
        public static bool operator ==(BoundingFrustum a, BoundingFrustum b)
        {
            return object.Equals(a, b);
        }
        /// <summary>
        /// 判断两个平截头体对象是否不相同
        /// </summary>
        /// <param name="a">等号操作符的左参数</param>
        /// <param name="b">等号操作符的右参数</param>
        /// <returns>如果不相同，返回true，否则，返回false </returns>
        public static bool operator !=(BoundingFrustum a, BoundingFrustum b)
        {
            return !object.Equals(a, b);
        }

        private void SetMatrix(ref Matrix44 value)
        {
            this.matrix = value;
            this.planes[2].Normal.X = -value.M30 - value.M00;
            this.planes[2].Normal.Y = -value.M31 - value.M01;
            this.planes[2].Normal.Z = -value.M32 - value.M02;
            this.planes[2].D = -value.M33 - value.M03;
            this.planes[3].Normal.X = -value.M30 + value.M00;
            this.planes[3].Normal.Y = -value.M31 + value.M01;
            this.planes[3].Normal.Z = -value.M32 + value.M02;
            this.planes[3].D = -value.M33 + value.M03;
            this.planes[4].Normal.X = -value.M30 + value.M10;
            this.planes[4].Normal.Y = -value.M31 + value.M11;
            this.planes[4].Normal.Z = -value.M32 + value.M12;
            this.planes[4].D = -value.M33 + value.M13;
            this.planes[5].Normal.X = -value.M30 - value.M10;
            this.planes[5].Normal.Y = -value.M31 - value.M11;
            this.planes[5].Normal.Z = -value.M32 - value.M12;
            this.planes[5].D = -value.M33 - value.M13;
            this.planes[0].Normal.X = -value.M20;
            this.planes[0].Normal.Y = -value.M21;
            this.planes[0].Normal.Z = -value.M22;
            this.planes[0].D = -value.M23;
            this.planes[1].Normal.X = -value.M30 + value.M20;
            this.planes[1].Normal.Y = -value.M31 + value.M21;
            this.planes[1].Normal.Z = -value.M32 + value.M22;
            this.planes[1].D = -value.M33 + value.M23;
            for (int i = 0; i < 6; i++)
            {
                float num2 = this.planes[i].Normal.Length();
                this.planes[i].Normal = (Vector3) (this.planes[i].Normal / num2);
                this.planes[i].D /= num2;
            }
            Ray ray = ComputeIntersectionLine(ref this.planes[0], ref this.planes[2]);
            this.cornerArray[0] = ComputeIntersection(ref this.planes[4], ref ray);
            this.cornerArray[3] = ComputeIntersection(ref this.planes[5], ref ray);
            ray = ComputeIntersectionLine(ref this.planes[3], ref this.planes[0]);
            this.cornerArray[1] = ComputeIntersection(ref this.planes[4], ref ray);
            this.cornerArray[2] = ComputeIntersection(ref this.planes[5], ref ray);
            ray = ComputeIntersectionLine(ref this.planes[2], ref this.planes[1]);
            this.cornerArray[4] = ComputeIntersection(ref this.planes[4], ref ray);
            this.cornerArray[7] = ComputeIntersection(ref this.planes[5], ref ray);
            ray = ComputeIntersectionLine(ref this.planes[1], ref this.planes[3]);
            this.cornerArray[5] = ComputeIntersection(ref this.planes[4], ref ray);
            this.cornerArray[6] = ComputeIntersection(ref this.planes[5], ref ray);
        }

        private static Vector3 ComputeIntersection(ref Plane plane, ref Ray ray)
        {
            float num = (-plane.D - Vector3.Dot(plane.Normal, ray.Position)) / Vector3.Dot(plane.Normal, ray.Direction);
            return (ray.Position + ((Vector3)(ray.Direction * num)));
        }
        private static Ray ComputeIntersectionLine(ref Plane p1, ref Plane p2)
        {
            //Ray ray = new Ray
           	//{
            //    Direction = Vector3.Cross(p1.Normal, p2.Normal)
            //};
			Vector3 direction = Vector3.Cross(p1.Normal, p2.Normal);

			float num = direction.LengthSquared();
			Vector3 position = (Vector3)(Vector3.Cross((Vector3)((-p1.D * p2.Normal) + (p2.D * p1.Normal)), direction) / num);
			return new Ray(direction, position);//ray;
        }

        internal void SupportMapping(ref Vector3 v, out Vector3 result)
        {
            float num3;
            int index = 0;
            Vector3.Dot(ref this.cornerArray[0], ref v, out num3);
            for (int i = 1; i < this.cornerArray.Length; i++)
            {
                float num2;
                Vector3.Dot(ref this.cornerArray[i], ref v, out num2);
                if (num2 > num3)
                {
                    index = i;
                    num3 = num2;
                }
            }
            result = this.cornerArray[index];
        }
        /// <summary>
        /// 返回当前平截头体对象的字符串形式
        /// </summary>
        /// <returns>表示当前平截头体对象的字符串</returns>
        public override string ToString()
        {
            return string.Format(CultureInfo.CurrentCulture, "N:{0} F:{1} L:{2} R:{3} T:{4} B:{5}", new object[] { this.Near.ToString(), this.Far.ToString(), this.Left.ToString(), this.Right.ToString(), this.Top.ToString(), this.Bottom.ToString() });
        }
        /// <summary>
        /// 获取平截头体的下平面
        /// </summary>
        public Plane Bottom
        {
            get
            {
                return this.planes[5];
            }
        }
        /// <summary>
        /// 获取平截头体的远平面
        /// </summary>
        public Plane Far
        {
            get
            {
                return this.planes[1];
            }
        }
        /// <summary>
        /// 获取平截头体的左平面
        /// </summary>
        public Plane Left
        {
            get
            {
                return this.planes[2];
            }
        }
        /// <summary>
        /// 获取/设置描述当前平截头体的矩阵
        /// </summary>
        public Matrix44 Matrix
        {
            get
            {
                return this.matrix;
            }
            set
            {
                this.SetMatrix(ref value);
            }
        }
        /// <summary>
        /// 获取平截头体的近平面
        /// </summary>
        public Plane Near
        {
            get
            {
                return this.planes[0];
            }
        }
        /// <summary>
        /// 获取平截头体的右平面
        /// </summary>
        public Plane Right
        {
            get
            {
                return this.planes[3];
            }
        }
        /// <summary>
        /// 获取平截头体的上平面
        /// </summary>
        public Plane Top
        {
            get
            {
                return this.planes[4];
            }
        }
    }
}

