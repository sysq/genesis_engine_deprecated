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
    /// 定义射线
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct Ray : IEquatable<Ray>
    {
        /// <summary>
        /// 指定射线的起始位置
        /// </summary>
        public Vector3 Position;
        /// <summary>
        /// 指定表示射线方向的单位向量
        /// </summary>
        public Vector3 Direction;
        /// <summary>
        /// 创建一个射线对象实例 
        /// </summary>
        /// <param name="position">射线的起始位置</param>
        /// <param name="direction">表示射线方向的单位向量</param>
        public Ray(Vector3 position, Vector3 direction)
        {
            this.Position = position;
            this.Direction = direction;
        }
        /// <summary>
        /// 判定两个射线对象实例是否相等
        /// </summary>
        /// <param name="other">与当前射线对象比较的射线对象</param>
        /// <returns>若指定的射线对象与当前的射线对象相等，则返回true；否则，返回false</returns>
        public bool Equals(Ray other)
        {
            return (((((this.Position.X == other.Position.X) && (this.Position.Y == other.Position.Y)) && ((this.Position.Z == other.Position.Z) && (this.Direction.X == other.Direction.X))) && (this.Direction.Y == other.Direction.Y)) && (this.Direction.Z == other.Direction.Z));
        }
        /// <summary>
        /// 判定两个射线对象实例是否相等
        /// </summary>
        /// <param name="obj">与当前射线对象比较的对象</param>
        /// <returns>若指定的对象与当前的射线对象相等，则返回true；否则，返回false</returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if ((obj != null) && (obj is Ray))
            {
                flag = this.Equals((Ray) obj);
            }
            return flag;
        }
        /// <summary>
        /// 获取实例哈希值
        /// </summary>
        /// <returns>当前射线对象的哈希值</returns>
        public override int GetHashCode()
        {
            return (this.Position.GetHashCode() + this.Direction.GetHashCode());
        }
        /// <summary>
        /// 返回当前射线对象的字符串表示
        /// </summary>
        /// <returns>当前射线对象的字符串表示</returns>
        public override string ToString()
        {
            return string.Format(CultureInfo.CurrentCulture, "P:{0}, D:{1}", new object[] { this.Position.ToString(), this.Direction.ToString() });
        }
        /// <summary>
        /// 检测当前包围盒是否与射线相交
        /// </summary>
        /// <param name="box">用于检测相交的包围盒</param>
        /// <returns>若包围盒与射线相交，则返回true；否则，返回false</returns>
        public bool Intersects(BoundingBox box)
        {
            return box.Intersects(this);
        }
        /// <summary>
        /// 检测当前包围盒是否与射线相交
        /// </summary>
        /// <param name="box">用于检测相交的包围盒</param>
        /// <param name="distance">[输出参数]表示射线与包围盒相交的距离。若不相交，则距离的概念没有意义</param>
        /// <returns>若包围盒与射线相交，则返回true；否则，返回false</returns>
        public bool Intersects(ref BoundingBox box, out float distance)
        {
            return box.Intersects(ref this, out distance);
        }
        /// <summary>
        /// 检测当前包围台是否与射线相交
        /// </summary>
        /// <param name="frustum">用于检测相交的包围台</param>
        /// <returns>若包围台与射线相交，则返回true；否则，返回false</returns>
        public bool Intersects(BoundingFrustum frustum)
        {
            if (frustum == null)
            {
                throw new ArgumentNullException("frustum");
            }
            return frustum.Intersects(this);
        }
        /// <summary>
        /// 检测当前包围球是否与射线相交
        /// </summary>
        /// <param name="sphere">用于检测相交的包围球</param>
        /// <returns>若包围球与射线相交，则返回true；否则，返回false</returns>
        public bool Intersects(BoundingSphere sphere)
        {
            float num5 = sphere.Center.X - this.Position.X;
            float num4 = sphere.Center.Y - this.Position.Y;
            float num3 = sphere.Center.Z - this.Position.Z;
            float num7 = ((num5 * num5) + (num4 * num4)) + (num3 * num3);
            float num2 = sphere.Radius * sphere.Radius;
            if (num7 <= num2)
            {
                return true;
            }
            float num = ((num5 * this.Direction.X) + (num4 * this.Direction.Y)) + (num3 * this.Direction.Z);
            if (num < 0f)
            {
                return false;
            }
            float num6 = num7 - (num * num);
            if (num6 > num2)
            {
                return false;
            }
            return true;
        }
        /// <summary>
        /// 检测当前射线是否与指定的包围球相交
        /// </summary>
        /// <param name="sphere">用于检测与射线相交的包围球</param>
        /// <param name="distance">[输出参数]表示射线与包围球相交的距离。若不相交，则距离的概念没有意义</param>
        /// <returns>若包围球与射线相交，则返回true；否则，返回false</returns>
        public bool Intersects(ref BoundingSphere sphere, out float distance)
        {
            distance = 0f;
            float num5 = sphere.Center.X - this.Position.X;
            float num4 = sphere.Center.Y - this.Position.Y;
            float num3 = sphere.Center.Z - this.Position.Z;
            float num7 = ((num5 * num5) + (num4 * num4)) + (num3 * num3);
            float num2 = sphere.Radius * sphere.Radius;
            if (num7 <= num2)
            {
                return true;
            }
            else
            {
                distance = 0;
                float num = ((num5 * this.Direction.X) + (num4 * this.Direction.Y)) + (num3 * this.Direction.Z);
                if (num >= 0f)
                {
                    float num6 = num7 - (num * num);
                    if (num6 <= num2)
                    {
                        float num8 = (float) Math.Sqrt((double) (num2 - num6));
                        distance = num - num8;
                        return true;
                    }
                }
            }
            return false;
        }
        /// <summary>
        /// 检测当前射线是否与指定的平面相交
        /// </summary>
        /// <param name="plane">用于检测与射线相交的平面</param>
        /// <returns>若平面与射线相交，则返回true；否则，返回false</returns>
        public bool Intersects(Plane plane)
        {
            float num2 = ((plane.Normal.X * this.Direction.X) + (plane.Normal.Y * this.Direction.Y)) + (plane.Normal.Z * this.Direction.Z);
            if (Math.Abs(num2) < 1E-05f)
            {
                return false;
            }
            float num3 = ((plane.Normal.X * this.Position.X) + (plane.Normal.Y * this.Position.Y)) + (plane.Normal.Z * this.Position.Z);
            float num = (-plane.D - num3) / num2;
            if (num < 0f)
            {
                if (num < -1E-05f)
                {
                    return false;
                }
            }
            return true;
        }
        /// <summary>
        /// 检测当前射线是否与指定的平面相交
        /// </summary>
        /// <param name="plane">用于检测与射线相交的平面</param>
        /// <param name="distance">[输出参数]表示射线与平面相交的距离。若不相交，则距离的概念没有意义</param>
        /// <returns>若平面与射线相交，则返回true；否则，返回false</returns>
        public bool Intersects(ref Plane plane, out float distance)
        {
            return plane.Intersects(ref this, out distance);
        }
        /// <summary>
        /// 判定两个射线实例是否相等
        /// </summary>
        /// <param name="a">相等比较操作左边的对象</param>
        /// <param name="b">相等比较操作右边的对象</param>
        /// <returns></returns>
        public static bool operator ==(Ray a, Ray b)
        {
            return (((((a.Position.X == b.Position.X) && (a.Position.Y == b.Position.Y)) && ((a.Position.Z == b.Position.Z) && (a.Direction.X == b.Direction.X))) && (a.Direction.Y == b.Direction.Y)) && (a.Direction.Z == b.Direction.Z));
        }
        /// <summary>
        /// 判定两个射线实例是否不相等
        /// </summary>
        /// <param name="a">不相等比较操作左边的对象</param>
        /// <param name="b">不相等比较操作左边的对象</param>
        /// <returns></returns>
        public static bool operator !=(Ray a, Ray b)
        {
            if ((((a.Position.X == b.Position.X) && (a.Position.Y == b.Position.Y)) && ((a.Position.Z == b.Position.Z) && (a.Direction.X == b.Direction.X))) && (a.Direction.Y == b.Direction.Y))
            {
                return !(a.Direction.Z == b.Direction.Z);
            }
            return true;
        }
    }
}

