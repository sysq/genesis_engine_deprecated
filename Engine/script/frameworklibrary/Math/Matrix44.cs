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
    /// 定义矩阵
    /// </summary>
    [Serializable, StructLayout(LayoutKind.Sequential)]
    public struct Matrix44 : IEquatable<Matrix44>
    {
        #region Public Fields

        /// <summary>
        /// 矩阵第0行第0列的值
        /// </summary>
        public float M00;
        /// <summary>
        /// 矩阵第0行第1列的值
        /// </summary>
        public float M01;
        /// <summary>
        /// 矩阵第0行第2列的值
        /// </summary>
        public float M02;
        /// <summary>
        /// 矩阵第0行第3列的值
        /// </summary>
        public float M03;

        /// <summary>
        /// 矩阵第1行第0列的值
        /// </summary>
        public float M10;
        /// <summary>
        /// 矩阵第1行第1列的值
        /// </summary>
        public float M11;
        /// <summary>
        /// 矩阵第1行第2列的值
        /// </summary>
        public float M12;
        /// <summary>
        /// 矩阵第1行第3列的值
        /// </summary>
        public float M13;

        /// <summary>
        /// 矩阵第2行第0列的值
        /// </summary>
        public float M20;
        /// <summary>
        /// 矩阵第2行第1列的值
        /// </summary>
        public float M21;
        /// <summary>
        /// 矩阵第2行第2列的值
        /// </summary>
        public float M22;
        /// <summary>
        /// 矩阵第2行第3列的值
        /// </summary>
        public float M23;

        /// <summary>
        /// 矩阵第3行第0列的值
        /// </summary>
        public float M30;
        /// <summary>
        /// 矩阵第3行第1列的值
        /// </summary>
        public float M31;
        /// <summary>
        /// 矩阵第3行第2列的值
        /// </summary>
        public float M32;
        /// <summary>
        /// 矩阵第3行第3列的值
        /// </summary>
        public float M33;


        #endregion

        private static Matrix44 _identity;

        /// <summary>
        /// 返回一个单位矩阵实例
        /// </summary>
        public static Matrix44 Identity
        {
            get
            {
                return _identity;
            }
        }

        #region Public Properties
        /// <summary>
        /// 获取或设置矩阵的第1列的列向量
        /// </summary>
        public Vector3 Up
        {
            get
            {
                Vector3 vector;
                vector.X = this.M01;
                vector.Y = this.M11;
                vector.Z = this.M21;
                return vector;
            }
            set
            {
                this.M01 = value.X;
                this.M11 = value.Y;
                this.M21 = value.Z;
            }
        }
        /// <summary>
        /// 获取或设置矩阵的第1列的反向列向量
        /// </summary>
        public Vector3 Down
        {
            get
            {
                Vector3 vector;
                vector.X = -this.M01;
                vector.Y = -this.M11;
                vector.Z = -this.M21;
                return vector;
            }
            set
            {
                this.M01 = -value.X;
                this.M11 = -value.Y;
                this.M21 = -value.Z;
            }
        }
        /// <summary>
        /// 获取或设置矩阵的第0列的列向量
        /// </summary>
        public Vector3 Right
        {
            get
            {
                Vector3 vector;
                vector.X = this.M00;
                vector.Y = this.M10;
                vector.Z = this.M20;
                return vector;
            }
            set
            {
                this.M00 = value.X;
                this.M10 = value.Y;
                this.M20 = value.Z;
            }
        }
        /// <summary>
        /// 获取或设置矩阵的第0列的反向列向量
        /// </summary>
        public Vector3 Left
        {
            get
            {
                Vector3 vector;
                vector.X = -this.M00;
                vector.Y = -this.M10;
                vector.Z = -this.M20;
                return vector;
            }
            set
            {
                this.M00 = -value.X;
                this.M10 = -value.Y;
                this.M20 = -value.Z;
            }
        }
        /// <summary>
        /// 获取或设置矩阵的第2列的反向列向量
        /// </summary>
        public Vector3 Forward
        {
            get
            {
                Vector3 vector;
                vector.X = -this.M02;
                vector.Y = -this.M12;
                vector.Z = -this.M22;
                return vector;
            }
            set
            {
                this.M02 = -value.X;
                this.M12 = -value.Y;
                this.M22 = -value.Z;
            }
        }
        /// <summary>
        /// 获取或设置矩阵的第2列的列向量
        /// </summary>
        public Vector3 Backward
        {
            get
            {
                Vector3 vector;
                vector.X = this.M02;
                vector.Y = this.M12;
                vector.Z = this.M22;
                return vector;
            }
            set
            {
                this.M02 = value.X;
                this.M12 = value.Y;
                this.M22 = value.Z;
            }
        }
        #endregion
        /// <summary>
        /// 获取或设置矩阵的一个元素
        /// </summary>
        /// <param name="row">行索引</param>
        /// <param name="col">列索引</param>
        /// <returns>相应的元素</returns>
        public unsafe float this[int row, int col]
        {
            get
            {
                fixed (float* th = &(this.M00))
                {
                    return th[row * 4 + col];
                }
            }
            set
            {
                fixed (float* th = &(this.M00))
                {
                    th[row * 4 + col] = value;
                }
            }
        }
        /// <summary>
        /// 获取或设置矩阵的一个元素
        /// </summary>
        /// <param name="index">元素索引</param>
        /// <returns>相应的元素</returns>
        public unsafe float this[int index]
        {
            get
            {
                fixed (float* th = &(this.M00))
                {
                    return th[index];
                }
            }
            set
            {
                fixed (float* th = &(this.M00))
                {
                    th[index] = value;
                }
            }
        }
        /// <summary>
        /// 获取矩阵的一行
        /// </summary>
        /// <param name="index">行索引</param>
        /// <returns>矩阵相应的行</returns>
        public Vector4 GetRow(int index)
        {
            Vector4 vector;
            vector.X = this[index, 0];
            vector.Y = this[index, 1];
            vector.Z = this[index, 2];
            vector.W = this[index, 3];
            return vector;
        }
        /// <summary>
        /// 设置矩阵的一行
        /// </summary>
        /// <param name="index">行索引</param>
        /// <param name="value">一行的值</param>
        public void SetRow(int index, Vector4 value)
        {
            this[index, 0] = value.X;
            this[index, 1] = value.Y;
            this[index, 2] = value.Z;
            this[index, 3] = value.W;
        }
        /// <summary>
        /// 获取矩阵的一列
        /// </summary>
        /// <param name="index">列索引</param>
        /// <returns>矩阵相应的列</returns>
        public Vector4 GetColumn(int index)
        {
            Vector4 vector;
            vector.X = this[0, index];
            vector.Y = this[1, index];
            vector.Z = this[2, index];
            vector.W = this[3, index];
            return vector;
        }
        /// <summary>
        /// 设置矩阵的一列
        /// </summary>
        /// <param name="index">列索引</param>
        /// <param name="value">一列的值</param>
        public void SetColumn(int index, Vector4 value)
        {
            this[0, index] = value.X;
            this[1, index] = value.Y;
            this[2, index] = value.Z;
            this[3, index] = value.W;
        }
        /// <summary>
        /// 初始化一个矩阵实例
        /// </summary>
        /// <param name="m00">初始化m00的值</param>
        /// <param name="m01">初始化m01的值</param>
        /// <param name="m02">初始化m02的值</param>
        /// <param name="m03">初始化m03的值</param>
        /// <param name="m10">初始化m10的值</param>
        /// <param name="m11">初始化m11的值</param>
        /// <param name="m12">初始化m12的值</param>
        /// <param name="m13">初始化m13的值</param>
        /// <param name="m20">初始化m20的值</param>
        /// <param name="m21">初始化m21的值</param>
        /// <param name="m22">初始化m22的值</param>
        /// <param name="m23">初始化m23的值</param>
        /// <param name="m30">初始化m30的值</param>
        /// <param name="m31">初始化m31的值</param>
        /// <param name="m32">初始化m32的值</param>
        /// <param name="m33">初始化m33的值</param>
        public Matrix44(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33)
        {
            this.M00 = m00;
            this.M01 = m01;
            this.M02 = m02;
            this.M03 = m03;
            this.M10 = m10;
            this.M11 = m11;
            this.M12 = m12;
            this.M13 = m13;
            this.M20 = m20;
            this.M21 = m21;
            this.M22 = m22;
            this.M23 = m23;
            this.M30 = m30;
            this.M31 = m31;
            this.M32 = m32;
            this.M33 = m33;
        }
        /// <summary>
        /// 创建一个平移矩阵
        /// </summary>
        /// <param name="position">在x、y和z轴上的平移量</param>
        /// <returns>创建的平移矩阵</returns>
        public static Matrix44 CreateTranslation(Vector3 position)
        {
            Matrix44 matrix;
            matrix.M00 = 1f;
            matrix.M01 = 0f;
            matrix.M02 = 0f;
            matrix.M03 = position.X;

            matrix.M10 = 0f;
            matrix.M11 = 1f;
            matrix.M12 = 0f;
            matrix.M13 = position.Y;

            matrix.M20 = 0f;
            matrix.M21 = 0f;
            matrix.M22 = 1f;
            matrix.M23 = position.Z;

            matrix.M30 = 0f;
            matrix.M31 = 0f;
            matrix.M32 = 0f;
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 创建一个平移矩阵
        /// </summary>
        /// <param name="position">在x、y和z轴上的平移量</param>
        /// <param name="matrix">[输出参数]创建的平移矩阵</param>
        public static void CreateTranslation(ref Vector3 position, out Matrix44 matrix)
        {
            
            matrix.M00 = 1f;
            matrix.M01 = 0f;
            matrix.M02 = 0f;
            matrix.M03 = position.X;

            matrix.M10 = 0f;
            matrix.M11 = 1f;
            matrix.M12 = 0f;
            matrix.M13 = position.Y;

            matrix.M20 = 0f;
            matrix.M21 = 0f;
            matrix.M22 = 1f;
            matrix.M23 = position.Z;

            matrix.M30 = 0f;
            matrix.M31 = 0f;
            matrix.M32 = 0f;
            matrix.M33 = 1f;
        }
        /// <summary>
        /// 创建一个缩放矩阵
        /// </summary>
        /// <param name="scales">在x、y和z轴上的缩放量</param>
        /// <returns>创建的缩放矩阵</returns>
        public static Matrix44 CreateScale(Vector3 scales)
        {
            Matrix44 matrix;

            matrix.M00 = scales.X;
            matrix.M01 = 0f;
            matrix.M02 = 0f;
            matrix.M03 = 0f;

            matrix.M10 = 0f;
            matrix.M11 = scales.Y;
            matrix.M12 = 0f;
            matrix.M13 = 0f;

            matrix.M20 = 0f;
            matrix.M21 = 0f;
            matrix.M22 = scales.Z;
            matrix.M23 = 0f;

            matrix.M30 = 0f;
            matrix.M31 = 0f;
            matrix.M32 = 0f;
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 创建一个缩放矩阵
        /// </summary>
        /// <param name="scales">在x、y和z轴上的缩放量</param>
        /// <param name="matrix">[输出参数]创建的缩放矩阵</param>
        public static void CreateScale(ref Vector3 scales, out Matrix44 matrix)
        {
            matrix.M00 = scales.X;
            matrix.M01 = 0f;
            matrix.M02 = 0f;
            matrix.M03 = 0f;

            matrix.M10 = 0f;
            matrix.M11 = scales.Y;
            matrix.M12 = 0f;
            matrix.M13 = 0f;

            matrix.M20 = 0f;
            matrix.M21 = 0f;
            matrix.M22 = scales.Z;
            matrix.M23 = 0f;

            matrix.M30 = 0f;
            matrix.M31 = 0f;
            matrix.M32 = 0f;
            matrix.M33 = 1f;
        }
        /// <summary>
        /// 构建一个基于视域的透视投影矩阵并返回该值
        /// </summary>
        /// <param name="fieldOfView">沿着y轴方向的视域，用弧度表示</param>
        /// <param name="aspectRatio">宽高比，定义为视平面宽度与高度之比</param>
        /// <param name="nearPlaneDistance">到近视平面的距离</param>
        /// <param name="farPlaneDistance">到远视平面的距离</param>
        /// <returns>透视投影矩阵</returns>
        public static Matrix44 CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
        {
            Matrix44 matrix;
            if ((fieldOfView <= 0f) || (fieldOfView >= 3.141593f))
            {
                throw new ArgumentOutOfRangeException("fieldOfView", string.Format(CultureInfo.CurrentCulture, FrameworkResources.OutRangeFieldOfView, new object[] { "fieldOfView" }));
            }
            if (nearPlaneDistance <= 0f)
            {
                throw new ArgumentOutOfRangeException("nearPlaneDistance", string.Format(CultureInfo.CurrentCulture, FrameworkResources.NegativePlaneDistance, new object[] { "nearPlaneDistance" }));
            }
            if (farPlaneDistance <= 0f)
            {
                throw new ArgumentOutOfRangeException("farPlaneDistance", string.Format(CultureInfo.CurrentCulture, FrameworkResources.NegativePlaneDistance, new object[] { "farPlaneDistance" }));
            }
            if (nearPlaneDistance >= farPlaneDistance)
            {
                throw new ArgumentOutOfRangeException("nearPlaneDistance", FrameworkResources.OppositePlanes);
            }
            float num = 1f / ((float)Math.Tan((double)(fieldOfView * 0.5f)));
            float num9 = num / aspectRatio;
            matrix.M00 = num9;
            matrix.M10 = matrix.M20 = matrix.M30 = 0f;
            matrix.M11 = num;
            matrix.M01 = matrix.M21 = matrix.M31 = 0f;
            matrix.M02 = matrix.M12 = 0f;
            matrix.M22 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
            matrix.M32 = -1f;
            matrix.M03 = matrix.M13 = matrix.M33 = 0f;
            matrix.M23 = (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance);
            return matrix;
        }
        /// <summary>
        /// 构建一个基于视域的透视投影矩阵并返回该值
        /// </summary>
        /// <param name="fieldOfView">沿着y轴方向的视域，用弧度表示</param>
        /// <param name="aspectRatio">宽高比，定义为视平面宽度与高度之比</param>
        /// <param name="nearPlaneDistance">到近视平面的距离</param>
        /// <param name="farPlaneDistance">到远视平面的距离</param>
        /// <param name="matrix">[输出参数]透视投影矩阵</param>
        public static void CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance, out Matrix44 matrix)
        {

            if ((fieldOfView <= 0f) || (fieldOfView >= 3.141593f))
            {
                throw new ArgumentOutOfRangeException("fieldOfView", string.Format(CultureInfo.CurrentCulture, FrameworkResources.OutRangeFieldOfView, new object[] { "fieldOfView" }));
            }
            if (nearPlaneDistance <= 0f)
            {
                throw new ArgumentOutOfRangeException("nearPlaneDistance", string.Format(CultureInfo.CurrentCulture, FrameworkResources.NegativePlaneDistance, new object[] { "nearPlaneDistance" }));
            }
            if (farPlaneDistance <= 0f)
            {
                throw new ArgumentOutOfRangeException("farPlaneDistance", string.Format(CultureInfo.CurrentCulture, FrameworkResources.NegativePlaneDistance, new object[] { "farPlaneDistance" }));
            }
            if (nearPlaneDistance >= farPlaneDistance)
            {
                throw new ArgumentOutOfRangeException("nearPlaneDistance", FrameworkResources.OppositePlanes);
            }
            float num = 1f / ((float)Math.Tan((double)(fieldOfView * 0.5f)));
            float num9 = num / aspectRatio;
            matrix.M00 = num9;
            matrix.M10 = matrix.M20 = matrix.M30 = 0f;
            matrix.M11 = num;
            matrix.M01 = matrix.M21 = matrix.M31 = 0f;
            matrix.M02 = matrix.M12 = 0f;
            matrix.M22 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
            matrix.M32 = -1f;
            matrix.M03 = matrix.M13 = matrix.M33 = 0f;
            matrix.M23 = (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance);
        }
        /// <summary>
        /// 构建一个基于视域的透视投影矩阵并返回该值
        /// </summary>
        /// <param name="width">视景体近视平面的宽度</param>
        /// <param name="height">视景体近视平面的高度</param>
        /// <param name="nearPlaneDistance">到近视平面的距离</param>
        /// <param name="farPlaneDistance">到远视平面的距离</param>
        /// <returns>投影矩阵</returns>
        public static Matrix44 CreatePerspective(float width, float height, float nearPlaneDistance, float farPlaneDistance)
        {
            Matrix44 matrix;
            if (nearPlaneDistance <= 0f)
            {
                throw new ArgumentOutOfRangeException("nearPlaneDistance", string.Format(CultureInfo.CurrentCulture, FrameworkResources.NegativePlaneDistance, new object[] { "nearPlaneDistance" }));
            }
            if (farPlaneDistance <= 0f)
            {
                throw new ArgumentOutOfRangeException("farPlaneDistance", string.Format(CultureInfo.CurrentCulture, FrameworkResources.NegativePlaneDistance, new object[] { "farPlaneDistance" }));
            }
            if (nearPlaneDistance >= farPlaneDistance)
            {
                throw new ArgumentOutOfRangeException("nearPlaneDistance", FrameworkResources.OppositePlanes);
            }
            matrix.M00 = (2f * nearPlaneDistance) / width;
            matrix.M10 = matrix.M20 = matrix.M30 = 0f;
            matrix.M11 = (2f * nearPlaneDistance) / height;
            matrix.M01= matrix.M21 = matrix.M31 = 0f;
            matrix.M22 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
            matrix.M02 = matrix.M12 = 0f;
            matrix.M32 = -1f;
            matrix.M03 = matrix.M13 = matrix.M33 = 0f;
            matrix.M23= (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance);
            return matrix;
        }
        /// <summary>
        /// 构建一个基于视域的透视投影矩阵并返回该值
        /// </summary>
        /// <param name="width">视景体近视平面的宽度</param>
        /// <param name="height">视景体近视平面的高度</param>
        /// <param name="nearPlaneDistance">到近视平面的距离</param>
        /// <param name="farPlaneDistance">到远视平面的距离</param>
        /// <param name="matrix">[输出参数]投影矩阵</param>
        public static void CreatePerspective(float width, float height, float nearPlaneDistance, float farPlaneDistance, out Matrix44 matrix)
        {
            if (nearPlaneDistance <= 0f)
            {
                throw new ArgumentOutOfRangeException("nearPlaneDistance", string.Format(CultureInfo.CurrentCulture, FrameworkResources.NegativePlaneDistance, new object[] { "nearPlaneDistance" }));
            }
            if (farPlaneDistance <= 0f)
            {
                throw new ArgumentOutOfRangeException("farPlaneDistance", string.Format(CultureInfo.CurrentCulture, FrameworkResources.NegativePlaneDistance, new object[] { "farPlaneDistance" }));
            }
            if (nearPlaneDistance >= farPlaneDistance)
            {
                throw new ArgumentOutOfRangeException("nearPlaneDistance", FrameworkResources.OppositePlanes);
            }
            matrix.M00 = (2f * nearPlaneDistance) / width;
            matrix.M10 = matrix.M20 = matrix.M30 = 0f;
            matrix.M11 = (2f * nearPlaneDistance) / height;
            matrix.M01 = matrix.M21 = matrix.M31 = 0f;
            matrix.M22 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
            matrix.M02 = matrix.M12 = 0f;
            matrix.M32 = -1f;
            matrix.M03 = matrix.M13 = matrix.M33 = 0f;
            matrix.M23 = (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance);
        }
        /// <summary>
        /// 构建一个正交投影矩阵
        /// </summary>
        /// <param name="width">视景体的宽度</param>
        /// <param name="height">视景体的高度</param>
        /// <param name="zNearPlane">视景体的最小深度值</param>
        /// <param name="zFarPlane">视景体的最大深度值</param>
        /// <returns>投影矩阵</returns>
        public static Matrix44 CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane)
        {
            Matrix44 matrix;
            matrix.M00 = 2f / width;
            matrix.M10 = matrix.M20 = matrix.M30 = 0f;
            matrix.M11 = 2f / height;
            matrix.M01 = matrix.M21 = matrix.M31 = 0f;
            matrix.M22 = 1f / (zNearPlane - zFarPlane);
            matrix.M02 = matrix.M12 = matrix.M32 = 0f;
            matrix.M03 = matrix.M13 = 0f;
            matrix.M23 = zNearPlane / (zNearPlane - zFarPlane);
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 构建一个正交投影矩阵
        /// </summary>
        /// <param name="width">视景体的宽度</param>
        /// <param name="height">视景体的高度</param>
        /// <param name="zNearPlane">视景体的最小深度值</param>
        /// <param name="zFarPlane">视景体的最大深度值</param>
        /// <param name="matrix">[输出参数]投影矩阵</param>
        public static void CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane, out Matrix44 matrix)
        {
            matrix.M00 = 2f / width;
            matrix.M10 = matrix.M20 = matrix.M30 = 0f;
            matrix.M11 = 2f / height;
            matrix.M01 = matrix.M21 = matrix.M31 = 0f;
            matrix.M22 = 1f / (zNearPlane - zFarPlane);
            matrix.M02 = matrix.M12 = matrix.M32 = 0f;
            matrix.M03 = matrix.M13 = 0f;
            matrix.M23 = zNearPlane / (zNearPlane - zFarPlane);
            matrix.M33 = 1f;
        }
        /// <summary>
        /// 创建一个视口矩阵
        /// </summary>
        /// <param name="cameraPosition">相机的位置</param>
        /// <param name="cameraTarget">相机指向的目标位置</param>
        /// <param name="cameraUpVector">视口相机向上的方向</param>
        /// <returns>创建的视口矩阵</returns>
        public static Matrix44 CreateLookAt(Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUpVector)
        {
            Matrix44 matrix;
            Vector3 vector = Vector3.Normalize(cameraPosition - cameraTarget);
            Vector3 vector2 = Vector3.Normalize(Vector3.Cross(cameraUpVector, vector));
            Vector3 vector3 = Vector3.Cross(vector, vector2);
            matrix.M00 = vector2.X;
            matrix.M10 = vector3.X;
            matrix.M20 = vector.X;
            matrix.M30 = 0f;
            matrix.M01 = vector2.Y;
            matrix.M11 = vector3.Y;
            matrix.M21 = vector.Y;
            matrix.M31 = 0f;
            matrix.M02 = vector2.Z;
            matrix.M12 = vector3.Z;
            matrix.M22 = vector.Z;
            matrix.M32 = 0f;
            matrix.M03 = -Vector3.Dot(vector2, cameraPosition);
            matrix.M13 = -Vector3.Dot(vector3, cameraPosition);
            matrix.M23 = -Vector3.Dot(vector, cameraPosition);
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 创建一个视口矩阵
        /// </summary>
        /// <param name="cameraPosition">相机的位置</param>
        /// <param name="cameraTarget">相机指向的目标位置</param>
        /// <param name="cameraUpVector">视口相机向上的方向</param>
        /// <param name="matrix">[输出参数]创建的视口矩阵</param>
        public static void CreateLookAt(ref Vector3 cameraPosition, ref Vector3 cameraTarget, ref Vector3 cameraUpVector, out Matrix44 matrix)
        {
            Vector3 vector = Vector3.Normalize(cameraPosition - cameraTarget);
            Vector3 vector2 = Vector3.Normalize(Vector3.Cross(cameraUpVector, vector));
            Vector3 vector3 = Vector3.Cross(vector, vector2);
            matrix.M00 = vector2.X;
            matrix.M10 = vector3.X;
            matrix.M20 = vector.X;
            matrix.M30 = 0f;
            matrix.M01 = vector2.Y;
            matrix.M11 = vector3.Y;
            matrix.M21 = vector.Y;
            matrix.M31 = 0f;
            matrix.M02 = vector2.Z;
            matrix.M12 = vector3.Z;
            matrix.M22 = vector.Z;
            matrix.M32 = 0f;
            matrix.M03 = -Vector3.Dot(vector2, cameraPosition);
            matrix.M13 = -Vector3.Dot(vector3, cameraPosition);
            matrix.M23 = -Vector3.Dot(vector, cameraPosition);
            matrix.M33 = 1f;
        }
        /// <summary>
        /// 创建一个基于四元数的旋转矩阵
        /// </summary>
        /// <param name="quaternion">用于创建矩阵的四元数</param>
        /// <returns>创建的矩阵</returns>
        public static Matrix44 CreateFromQuaternion(Quaternion quaternion)
        {
            Matrix44 matrix;
            float num9 = quaternion.X * quaternion.X;
            float num8 = quaternion.Y * quaternion.Y;
            float num7 = quaternion.Z * quaternion.Z;
            float num6 = quaternion.X * quaternion.Y;
            float num5 = quaternion.Z * quaternion.W;
            float num4 = quaternion.Z * quaternion.X;
            float num3 = quaternion.Y * quaternion.W;
            float num2 = quaternion.Y * quaternion.Z;
            float num = quaternion.X * quaternion.W;
            matrix.M00 = 1f - (2f * (num8 + num7));
            matrix.M10 = 2f * (num6 + num5);
            matrix.M20 = 2f * (num4 - num3);
            matrix.M30 = 0f;
            matrix.M01 = 2f * (num6 - num5);
            matrix.M11 = 1f - (2f * (num7 + num9));
            matrix.M21 = 2f * (num2 + num);
            matrix.M31 = 0f;
            matrix.M02 = 2f * (num4 + num3);
            matrix.M12 = 2f * (num2 - num);
            matrix.M22 = 1f - (2f * (num8 + num9));
            matrix.M32 = 0f;
            matrix.M03 = 0f;
            matrix.M13 = 0f;
            matrix.M23 = 0f;
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 创建一个基于四元数的旋转矩阵
        /// </summary>
        /// <param name="quaternion">用于创建矩阵的四元数</param>
        /// <param name="matrix">[输出参数]创建的矩阵</param>
        public static void CreateFromQuaternion(ref Quaternion quaternion, out Matrix44 matrix)
        {
            float num9 = quaternion.X * quaternion.X;
            float num8 = quaternion.Y * quaternion.Y;
            float num7 = quaternion.Z * quaternion.Z;
            float num6 = quaternion.X * quaternion.Y;
            float num5 = quaternion.Z * quaternion.W;
            float num4 = quaternion.Z * quaternion.X;
            float num3 = quaternion.Y * quaternion.W;
            float num2 = quaternion.Y * quaternion.Z;
            float num = quaternion.X * quaternion.W;
            matrix.M00 = 1f - (2f * (num8 + num7));
            matrix.M10 = 2f * (num6 + num5);
            matrix.M20 = 2f * (num4 - num3);
            matrix.M30 = 0f;
            matrix.M01 = 2f * (num6 - num5);
            matrix.M11 = 1f - (2f * (num7 + num9));
            matrix.M21 = 2f * (num2 + num);
            matrix.M31 = 0f;
            matrix.M02 = 2f * (num4 + num3);
            matrix.M12 = 2f * (num2 - num);
            matrix.M22 = 1f - (2f * (num8 + num9));
            matrix.M32 = 0f;
            matrix.M03 = 0f;
            matrix.M13 = 0f;
            matrix.M23 = 0f;
            matrix.M33 = 1f;
        }
        /// <summary>
        /// 创建一个基于指定的偏航角、俯仰角和翻转角的旋转矩阵
        /// </summary>
        /// <param name="yaw">绕着y轴的旋转角，用弧度表示</param>
        /// <param name="pitch">绕着x轴的旋转角，用弧度表示</param>
        /// <param name="roll">绕着z轴的旋转角，用弧度表示</param>
        /// <returns>基于指定的偏航角、俯仰角和翻转角的旋转矩阵</returns>
        public static Matrix44 CreateFromYawPitchRoll(float yaw, float pitch, float roll)
        {
            Quaternion quaternion;
            Quaternion.CreateFromYawPitchRoll(yaw, pitch, roll, out quaternion);
            return CreateFromQuaternion(quaternion);
        }
        /// <summary>
        /// 创建一个基于指定的偏航角、俯仰角和翻转角的旋转矩阵
        /// </summary>
        /// <param name="yaw">绕着y轴的旋转角，用弧度表示</param>
        /// <param name="pitch">绕着x轴的旋转角，用弧度表示</param>
        /// <param name="roll">绕着z轴的旋转角，用弧度表示</param>
        /// <param name="result">[输出参数]基于指定的偏航角、俯仰角和翻转角的旋转矩阵</param>
        public static void CreateFromYawPitchRoll(float yaw, float pitch, float roll, out Matrix44 result)
        {
            Quaternion quaternion;
            Quaternion.CreateFromYawPitchRoll(yaw, pitch, roll, out quaternion);
            result = CreateFromQuaternion(quaternion);
        }
        /// <summary>
        /// 返回一个绕x轴旋转的顶点集表示的矩阵
        /// </summary>
        /// <param name="radians">绕x轴旋转的弧度值。可以用ToRadians方法将度数转成弧度</param>
        /// <returns>旋转矩阵</returns>
        public static Matrix44 CreateRotationX(float radians)
        {
            Matrix44 matrix;
            float num2 = (float)Math.Cos((double)radians);
            float num = (float)Math.Sin((double)radians);
            matrix.M00 = 1f;
            matrix.M10 = 0f;
            matrix.M20 = 0f;
            matrix.M30 = 0f;
            matrix.M01 = 0f;
            matrix.M11 = num2;
            matrix.M21 = num;
            matrix.M31 = 0f;
            matrix.M02 = 0f;
            matrix.M12 = -num;
            matrix.M22 = num2;
            matrix.M32 = 0f;
            matrix.M03 = 0f;
            matrix.M13 = 0f;
            matrix.M23 = 0f;
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 返回一个绕x轴旋转的顶点集表示的矩阵
        /// </summary>
        /// <param name="radians">绕x轴旋转的弧度值。可以用ToRadians方法将度数转成弧度</param>
        /// <param name="result">[输出参数]旋转矩阵</param>
        public static void CreateRotationX(float radians, out Matrix44 result)
        {
            float num2 = (float)Math.Cos((double)radians);
            float num = (float)Math.Sin((double)radians);
            result.M00 = 1f;
            result.M10 = 0f;
            result.M20 = 0f;
            result.M30 = 0f;
            result.M01 = 0f;
            result.M11 = num2;
            result.M21 = num;
            result.M31 = 0f;
            result.M02 = 0f;
            result.M12 = -num;
            result.M22 = num2;
            result.M32 = 0f;
            result.M03 = 0f;
            result.M13 = 0f;
            result.M23 = 0f;
            result.M33 = 1f;
        }
        /// <summary>
        /// 返回一个绕y轴旋转的顶点集表示的矩阵
        /// </summary>
        /// <param name="radians">绕y轴旋转的弧度值。可以用ToRadians方法将度数转成弧度</param>
        /// <returns>旋转矩阵</returns>
        public static Matrix44 CreateRotationY(float radians)
        {
            Matrix44 matrix;
            float num2 = (float)Math.Cos((double)radians);
            float num = (float)Math.Sin((double)radians);
            matrix.M00 = num2;
            matrix.M10 = 0f;
            matrix.M20 = -num;
            matrix.M30 = 0f;
            matrix.M01 = 0f;
            matrix.M11 = 1f;
            matrix.M21 = 0f;
            matrix.M31 = 0f;
            matrix.M02 = num;
            matrix.M12 = 0f;
            matrix.M22 = num2;
            matrix.M32 = 0f;
            matrix.M03 = 0f;
            matrix.M13 = 0f;
            matrix.M23 = 0f;
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 返回一个绕y轴旋转的顶点集表示的矩阵
        /// </summary>
        /// <param name="radians">绕y轴旋转的弧度值。可以用ToRadians方法将度数转成弧度</param>
        /// <param name="result">[输出参数]旋转矩阵</param>
        public static void CreateRotationY(float radians, out Matrix44 result)
        {
            float num2 = (float)Math.Cos((double)radians);
            float num = (float)Math.Sin((double)radians);
            result.M00 = num2;
            result.M10 = 0f;
            result.M20 = -num;
            result.M30 = 0f;
            result.M01 = 0f;
            result.M11 = 1f;
            result.M21 = 0f;
            result.M31 = 0f;
            result.M02 = num;
            result.M12 = 0f;
            result.M22 = num2;
            result.M32 = 0f;
            result.M03 = 0f;
            result.M13 = 0f;
            result.M23 = 0f;
            result.M33 = 1f;
        }
        /// <summary>
        /// 返回一个绕z轴旋转的顶点集表示的矩阵
        /// </summary>
        /// <param name="radians">绕z轴旋转的弧度值。可以用ToRadians方法将度数转成弧度</param>
        /// <returns>旋转矩阵</returns>
        public static Matrix44 CreateRotationZ(float radians)
        {
            Matrix44 matrix;
            float num2 = (float)Math.Cos((double)radians);
            float num = (float)Math.Sin((double)radians);
            matrix.M00 = num2;
            matrix.M10 = num;
            matrix.M20 = 0f;
            matrix.M30 = 0f;
            matrix.M01 = -num;
            matrix.M11 = num2;
            matrix.M21 = 0f;
            matrix.M31 = 0f;
            matrix.M02 = 0f;
            matrix.M12 = 0f;
            matrix.M22 = 1f;
            matrix.M32 = 0f;
            matrix.M03 = 0f;
            matrix.M13 = 0f;
            matrix.M23 = 0f;
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 返回一个绕z轴旋转的顶点集表示的矩阵
        /// </summary>
        /// <param name="radians">绕z轴旋转的弧度值。可以用ToRadians方法将度数转成弧度</param>
        /// <param name="result">[输出参数]旋转矩阵</param>
        public static void CreateRotationZ(float radians, out Matrix44 result)
        {
            float num2 = (float)Math.Cos((double)radians);
            float num = (float)Math.Sin((double)radians);
            result.M00 = num2;
            result.M10 = num;
            result.M20 = 0f;
            result.M30 = 0f;
            result.M01 = -num;
            result.M11 = num2;
            result.M21 = 0f;
            result.M31 = 0f;
            result.M02 = 0f;
            result.M12 = 0f;
            result.M22 = 1f;
            result.M32 = 0f;
            result.M03 = 0f;
            result.M13 = 0f;
            result.M23 = 0f;
            result.M33 = 1f;
        }
        /// <summary>
        /// 创建一个基于任意向量旋转的矩阵
        /// </summary>
        /// <param name="axis">旋转轴</param>
        /// <param name="angle">旋转角</param>
        /// <returns>创建的矩阵</returns>
        public static Matrix44 CreateFromAxisAngle(Vector3 axis, float angle)
        {
            Matrix44 matrix;
            float x = axis.X;
            float y = axis.Y;
            float z = axis.Z;
            float num2 = (float)Math.Sin((double)angle);
            float num = (float)Math.Cos((double)angle);
            float num11 = x * x;
            float num10 = y * y;
            float num9 = z * z;
            float num8 = x * y;
            float num7 = x * z;
            float num6 = y * z;
            matrix.M00 = num11 + (num * (1f - num11));
            matrix.M10 = (num8 - (num * num8)) + (num2 * z);
            matrix.M20 = (num7 - (num * num7)) - (num2 * y);
            matrix.M30 = 0f;
            matrix.M01 = (num8 - (num * num8)) - (num2 * z);
            matrix.M11 = num10 + (num * (1f - num10));
            matrix.M21 = (num6 - (num * num6)) + (num2 * x);
            matrix.M31 = 0f;
            matrix.M02 = (num7 - (num * num7)) + (num2 * y);
            matrix.M12 = (num6 - (num * num6)) - (num2 * x);
            matrix.M22 = num9 + (num * (1f - num9));
            matrix.M32 = 0f;
            matrix.M03 = 0f;
            matrix.M13 = 0f;
            matrix.M23 = 0f;
            matrix.M33 = 1f;
            return matrix;
        }
        /// <summary>
        /// 创建一个基于任意向量旋转的矩阵
        /// </summary>
        /// <param name="axis">旋转轴</param>
        /// <param name="angle">旋转角</param>
        /// <param name="result">[输出参数]创建的矩阵</param>
        public static void CreateFromAxisAngle(ref Vector3 axis, float angle, out Matrix44 result)
        {
            float x = axis.X;
            float y = axis.Y;
            float z = axis.Z;
            float num2 = (float)Math.Sin((double)angle);
            float num = (float)Math.Cos((double)angle);
            float num11 = x * x;
            float num10 = y * y;
            float num9 = z * z;
            float num8 = x * y;
            float num7 = x * z;
            float num6 = y * z;
            result.M00 = num11 + (num * (1f - num11));
            result.M10 = (num8 - (num * num8)) + (num2 * z);
            result.M20 = (num7 - (num * num7)) - (num2 * y);
            result.M30 = 0f;
            result.M01 = (num8 - (num * num8)) - (num2 * z);
            result.M11 = num10 + (num * (1f - num10));
            result.M21 = (num6 - (num * num6)) + (num2 * x);
            result.M31 = 0f;
            result.M02 = (num7 - (num * num7)) + (num2 * y);
            result.M12 = (num6 - (num * num6)) - (num2 * x);
            result.M22 = num9 + (num * (1f - num9));
            result.M32 = 0f;
            result.M03 = 0f;
            result.M13 = 0f;
            result.M23 = 0f;
            result.M33 = 1f;
        }

        /// <summary>
        /// 解析出变换矩阵的缩放、平移和旋转分量 
        /// </summary>
        /// <param name="scale">[输出参数]变换矩阵的缩放分量，采用三元组向量表示</param>
        /// <param name="rotation">[输出参数]变换矩阵的旋转分量，采用四元数表示</param>
        /// <param name="translation">[输出参数]变换矩阵的平移分量，采用三元组向量表示</param>
        public void Decompose(out Vector3 scale, out Quaternion rotation, out Vector3 translation)
        {
            Matrix44 matQ = Matrix44.Identity;

            float fInvLength = 1.0f / (float)Math.Sqrt((double)(this[0, 0] * this[0, 0] + this[1, 0] * this[1, 0] + this[2, 0] * this[2, 0]));
            matQ[0, 0] = this[0, 0] * fInvLength;
            matQ[1, 0] = this[1, 0] * fInvLength;
            matQ[2, 0] = this[2, 0] * fInvLength;

            float fDot = matQ[0, 0] * this[0, 1] + matQ[1, 0] * this[1, 1] + matQ[2, 0] * this[2, 1];
            matQ[0, 1] = this[0, 1] - fDot * matQ[0, 0];
            matQ[1, 1] = this[1, 1] - fDot * matQ[1, 0];
            matQ[2, 1] = this[2, 1] - fDot * matQ[2, 0];

            fInvLength = 1.0f / (float)Math.Sqrt((double)(matQ[0, 1] * matQ[0, 1] + matQ[1, 1] * matQ[1, 1] + matQ[2, 1] * matQ[2, 1]));
            matQ[0, 1] *= fInvLength;
            matQ[1, 1] *= fInvLength;
            matQ[2, 1] *= fInvLength;

            fDot = matQ[0, 0] * this[0, 2] + matQ[1, 0] * this[1, 2] +  matQ[2, 0] * this[2, 2];
            matQ[0, 2] = this[0, 2] - fDot * matQ[0, 0];
            matQ[1, 2] = this[1, 2] - fDot * matQ[1, 0];
            matQ[2, 2] = this[2, 2] - fDot * matQ[2, 0];
            fDot = matQ[0, 1] * this[0, 2] + matQ[1, 1] * this[1, 2] + matQ[2, 1] * this[2, 2];
            matQ[0, 2] -= fDot * matQ[0, 1];
            matQ[1, 2] -= fDot * matQ[1, 1];
            matQ[2, 2] -= fDot * matQ[2, 1];
            fInvLength = 1.0f / (float)Math.Sqrt((double)(matQ[0, 2] * matQ[0, 2] + matQ[1, 2] * matQ[1, 2] + matQ[2, 2] * matQ[2, 2]));
            matQ[0, 2] *= fInvLength;
            matQ[1, 2] *= fInvLength;
            matQ[2, 2] *= fInvLength;

            // guarantee that orthogonal matrix has determinant 1 (no reflections)
            float fDet = matQ[0, 0] * matQ[1, 1] * matQ[2, 2] + matQ[0, 1] * matQ[1, 2] * matQ[2, 0] +
                matQ[0, 2] * matQ[1, 0] * matQ[2, 1] - matQ[0, 2] * matQ[1, 1] * matQ[2, 0] -
                matQ[0, 1] * matQ[1, 0] * matQ[2, 2] - matQ[0, 0] * matQ[1, 2] * matQ[2, 1];

            if (fDet < 0.0)
            {
                for (int iRow = 0; iRow < 3; iRow++)
                {
                    for (int iCol = 0; iCol < 3; iCol++)
                    {
                        matQ[iRow, iCol] = -matQ[iRow, iCol];
                    }
                }
            }

            // the scaling component
            scale = new Vector3(
            matQ[0, 0] * this[0, 0] + matQ[1, 0] * this[1, 0] + matQ[2, 0] * this[2, 0],
            matQ[0, 1] * this[0, 1] + matQ[1, 1] * this[1, 1] + matQ[2, 1] * this[2, 1],
            matQ[0, 2] * this[0, 2] + matQ[1, 2] * this[1, 2] + matQ[2, 2] * this[2, 2]);

            rotation = Quaternion.CreateFromRotationMatrix(matQ);
            translation = new Vector3(this[0, 3], this[1, 3], this[2, 3]);
        }
        /// <summary>
        /// 返回当前对象的字符串表示
        /// </summary>
        /// <returns>对象的字符串表示</returns>
        public override string ToString()
        {
            CultureInfo currentCulture = CultureInfo.CurrentCulture;
            return (string.Format(currentCulture, "{0}, {1}, {2}, {3}; ", new object[] { this.M00.ToString(currentCulture), this.M01.ToString(currentCulture), this.M02.ToString(currentCulture), this.M03.ToString(currentCulture) }) + string.Format(currentCulture, "{0}, {1}, {2}, {3}; ", new object[] { this.M10.ToString(currentCulture), this.M11.ToString(currentCulture), this.M12.ToString(currentCulture), this.M13.ToString(currentCulture) }) + string.Format(currentCulture, "{0}, {1}, {2}, {3}; ", new object[] { this.M20.ToString(currentCulture), this.M21.ToString(currentCulture), this.M22.ToString(currentCulture), this.M23.ToString(currentCulture) }) + string.Format(currentCulture, "{0}, {1}, {2}, {3}", new object[] { this.M30.ToString(currentCulture), this.M31.ToString(currentCulture), this.M32.ToString(currentCulture), this.M33.ToString(currentCulture) }));
        }
        /// <summary>
        /// 判定矩阵是否相等
        /// </summary>
        /// <param name="other">与当前矩阵比较的矩阵</param>
        /// <returns>若矩阵与当前的矩阵相等，则返回true；否则，返回false</returns>
        public bool Equals(Matrix44 other)
        {
            return ((((((this.M00 == other.M00) && (this.M11 == other.M11)) && ((this.M22 == other.M22) && (this.M33 == other.M33))) && (((this.M01 == other.M01) && (this.M02 == other.M02)) && ((this.M03 == other.M03) && (this.M10 == other.M10)))) && ((((this.M12 == other.M12) && (this.M13 == other.M13)) && ((this.M20 == other.M20) && (this.M21 == other.M21))) && (((this.M23 == other.M23) && (this.M30 == other.M30)) && (this.M31 == other.M31)))) && (this.M32 == other.M32));
        }
        /// <summary>
        /// 判定矩阵对象是否相等
        /// </summary>
        /// <param name="obj">用作比较的对象</param>
        /// <returns>若对象与当前的对象相等，则返回true；否则，返回false</returns>
        public override bool Equals(object obj)
        {
            bool flag = false;
            if (obj is Matrix44)
            {
                flag = this.Equals((Matrix44)obj);
            }
            return flag;
        }
        /// <summary>
        /// 获取对象的哈希值
        /// </summary>
        /// <returns>对象的哈希值</returns>
        public override int GetHashCode()
        {
            return (((((((((((((((this.M00.GetHashCode() + this.M01.GetHashCode()) + this.M02.GetHashCode()) + this.M03.GetHashCode()) + this.M10.GetHashCode()) + this.M11.GetHashCode()) + this.M12.GetHashCode()) + this.M13.GetHashCode()) + this.M20.GetHashCode()) + this.M21.GetHashCode()) + this.M22.GetHashCode()) + this.M23.GetHashCode()) + this.M30.GetHashCode()) + this.M31.GetHashCode()) + this.M32.GetHashCode()) + this.M33.GetHashCode());
        }
        /// <summary>
        /// 转置矩阵的行和列
        /// </summary>
        /// <param name="matrix">源矩阵</param>
        /// <returns>转置后的矩阵</returns>
        public static Matrix44 Transpose(Matrix44 matrix)
        {
            Matrix44 matrix2;
            matrix2.M00 = matrix.M00;
            matrix2.M01 = matrix.M10;
            matrix2.M02 = matrix.M20;
            matrix2.M03 = matrix.M30;
            matrix2.M10 = matrix.M01;
            matrix2.M11 = matrix.M11;
            matrix2.M12 = matrix.M21;
            matrix2.M13 = matrix.M31;
            matrix2.M20 = matrix.M02;
            matrix2.M21 = matrix.M12;
            matrix2.M22 = matrix.M22;
            matrix2.M23 = matrix.M32;
            matrix2.M30 = matrix.M03;
            matrix2.M31 = matrix.M13;
            matrix2.M32 = matrix.M23;
            matrix2.M33 = matrix.M33;
            return matrix2;
        }
        /// <summary>
        /// 转置矩阵的行和列
        /// </summary>
        /// <param name="matrix">源矩阵</param>
        /// <param name="result">[输出参数]转置后的矩阵</param>
        public static void Transpose(ref Matrix44 matrix, out Matrix44 result)
        {
            result.M00 = matrix.M00;
            result.M01 = matrix.M10;
            result.M02 = matrix.M20;
            result.M03 = matrix.M30;
            result.M10 = matrix.M01;
            result.M11 = matrix.M11;
            result.M12 = matrix.M21;
            result.M13 = matrix.M31;
            result.M20 = matrix.M02;
            result.M21 = matrix.M12;
            result.M22 = matrix.M22;
            result.M23 = matrix.M32;
            result.M30 = matrix.M03;
            result.M31 = matrix.M13;
            result.M32 = matrix.M23;
            result.M33 = matrix.M33;
        }
        /// <summary>
        /// 计算矩阵的行列式
        /// </summary>
        /// <returns>矩阵的行列式</returns>
        public float Determinant()
        {
            float num22 = M00;
            float num21 = M10;
            float num20 = M20;
            float num19 = M30;
            float num12 = M01;
            float num11 = M11;
            float num10 = M21;
            float num9 = M31;
            float num8 = M02;
            float num7 = M12;
            float num6 = M22;
            float num5 = M32;
            float num4 = M03;
            float num3 = M13;
            float num2 = M23;
            float num = M33;
            float num18 = (num6 * num) - (num5 * num2);
            float num17 = (num7 * num) - (num5 * num3);
            float num16 = (num7 * num2) - (num6 * num3);
            float num15 = (num8 * num) - (num5 * num4);
            float num14 = (num8 * num2) - (num6 * num4);
            float num13 = (num8 * num3) - (num7 * num4);
            return ((((num22 * (((num11 * num18) - (num10 * num17)) + (num9 * num16))) - (num21 * (((num12 * num18) - (num10 * num15)) + (num9 * num14)))) + (num20 * (((num12 * num17) - (num11 * num15)) + (num9 * num13)))) - (num19 * (((num12 * num16) - (num11 * num14)) + (num10 * num13))));
        }
        /// <summary>
        /// 计算矩阵的逆矩阵
        /// </summary>
        /// <param name="matrix">源矩阵</param>
        /// <returns>矩阵的逆矩阵</returns>
        public static Matrix44 Invert(Matrix44 matrix)
        {
            Matrix44 matrix2;
            float num5 = matrix.M00;
            float num4 = matrix.M10;
            float num3 = matrix.M20;
            float num2 = matrix.M30;
            float num9 = matrix.M01;
            float num8 = matrix.M11;
            float num7 = matrix.M21;
            float num6 = matrix.M31;
            float num17 = matrix.M02;
            float num16 = matrix.M12;
            float num15 = matrix.M22;
            float num14 = matrix.M32;
            float num13 = matrix.M03;
            float num12 = matrix.M13;
            float num11 = matrix.M23;
            float num10 = matrix.M33;
            float num23 = (num15 * num10) - (num14 * num11);
            float num22 = (num16 * num10) - (num14 * num12);
            float num21 = (num16 * num11) - (num15 * num12);
            float num20 = (num17 * num10) - (num14 * num13);
            float num19 = (num17 * num11) - (num15 * num13);
            float num18 = (num17 * num12) - (num16 * num13);
            float num39 = ((num8 * num23) - (num7 * num22)) + (num6 * num21);
            float num38 = -(((num9 * num23) - (num7 * num20)) + (num6 * num19));
            float num37 = ((num9 * num22) - (num8 * num20)) + (num6 * num18);
            float num36 = -(((num9 * num21) - (num8 * num19)) + (num7 * num18));
            float num = 1f / ((((num5 * num39) + (num4 * num38)) + (num3 * num37)) + (num2 * num36));
            matrix2.M00 = num39 * num;
            matrix2.M01 = num38 * num;
            matrix2.M02 = num37 * num;
            matrix2.M03 = num36 * num;
            matrix2.M10 = -(((num4 * num23) - (num3 * num22)) + (num2 * num21)) * num;
            matrix2.M11 = (((num5 * num23) - (num3 * num20)) + (num2 * num19)) * num;
            matrix2.M12 = -(((num5 * num22) - (num4 * num20)) + (num2 * num18)) * num;
            matrix2.M13 = (((num5 * num21) - (num4 * num19)) + (num3 * num18)) * num;
            float num35 = (num7 * num10) - (num6 * num11);
            float num34 = (num8 * num10) - (num6 * num12);
            float num33 = (num8 * num11) - (num7 * num12);
            float num32 = (num9 * num10) - (num6 * num13);
            float num31 = (num9 * num11) - (num7 * num13);
            float num30 = (num9 * num12) - (num8 * num13);
            matrix2.M20 = (((num4 * num35) - (num3 * num34)) + (num2 * num33)) * num;
            matrix2.M21 = -(((num5 * num35) - (num3 * num32)) + (num2 * num31)) * num;
            matrix2.M22 = (((num5 * num34) - (num4 * num32)) + (num2 * num30)) * num;
            matrix2.M23 = -(((num5 * num33) - (num4 * num31)) + (num3 * num30)) * num;
            float num29 = (num7 * num14) - (num6 * num15);
            float num28 = (num8 * num14) - (num6 * num16);
            float num27 = (num8 * num15) - (num7 * num16);
            float num26 = (num9 * num14) - (num6 * num17);
            float num25 = (num9 * num15) - (num7 * num17);
            float num24 = (num9 * num16) - (num8 * num17);
            matrix2.M30 = -(((num4 * num29) - (num3 * num28)) + (num2 * num27)) * num;
            matrix2.M31 = (((num5 * num29) - (num3 * num26)) + (num2 * num25)) * num;
            matrix2.M32 = -(((num5 * num28) - (num4 * num26)) + (num2 * num24)) * num;
            matrix2.M33 = (((num5 * num27) - (num4 * num25)) + (num3 * num24)) * num;
            return matrix2;
        }
        /// <summary>
        /// 计算矩阵的逆矩阵
        /// </summary>
        /// <param name="matrix">源矩阵</param>
        /// <param name="result">[输出参数]矩阵的逆矩阵</param>
        public static void Invert(ref Matrix44 matrix, out Matrix44 result)
        {
            float num5 = matrix.M00;
            float num4 = matrix.M10;
            float num3 = matrix.M20;
            float num2 = matrix.M30;
            float num9 = matrix.M01;
            float num8 = matrix.M11;
            float num7 = matrix.M21;
            float num6 = matrix.M31;
            float num17 = matrix.M02;
            float num16 = matrix.M12;
            float num15 = matrix.M22;
            float num14 = matrix.M32;
            float num13 = matrix.M03;
            float num12 = matrix.M13;
            float num11 = matrix.M23;
            float num10 = matrix.M33;
            float num23 = (num15 * num10) - (num14 * num11);
            float num22 = (num16 * num10) - (num14 * num12);
            float num21 = (num16 * num11) - (num15 * num12);
            float num20 = (num17 * num10) - (num14 * num13);
            float num19 = (num17 * num11) - (num15 * num13);
            float num18 = (num17 * num12) - (num16 * num13);
            float num39 = ((num8 * num23) - (num7 * num22)) + (num6 * num21);
            float num38 = -(((num9 * num23) - (num7 * num20)) + (num6 * num19));
            float num37 = ((num9 * num22) - (num8 * num20)) + (num6 * num18);
            float num36 = -(((num9 * num21) - (num8 * num19)) + (num7 * num18));
            float num = 1f / ((((num5 * num39) + (num4 * num38)) + (num3 * num37)) + (num2 * num36));
            result.M00 = num39 * num;
            result.M01 = num38 * num;
            result.M02 = num37 * num;
            result.M03 = num36 * num;
            result.M10 = -(((num4 * num23) - (num3 * num22)) + (num2 * num21)) * num;
            result.M11 = (((num5 * num23) - (num3 * num20)) + (num2 * num19)) * num;
            result.M12 = -(((num5 * num22) - (num4 * num20)) + (num2 * num18)) * num;
            result.M13 = (((num5 * num21) - (num4 * num19)) + (num3 * num18)) * num;
            float num35 = (num7 * num10) - (num6 * num11);
            float num34 = (num8 * num10) - (num6 * num12);
            float num33 = (num8 * num11) - (num7 * num12);
            float num32 = (num9 * num10) - (num6 * num13);
            float num31 = (num9 * num11) - (num7 * num13);
            float num30 = (num9 * num12) - (num8 * num13);
            result.M20 = (((num4 * num35) - (num3 * num34)) + (num2 * num33)) * num;
            result.M21 = -(((num5 * num35) - (num3 * num32)) + (num2 * num31)) * num;
            result.M22 = (((num5 * num34) - (num4 * num32)) + (num2 * num30)) * num;
            result.M23 = -(((num5 * num33) - (num4 * num31)) + (num3 * num30)) * num;
            float num29 = (num7 * num14) - (num6 * num15);
            float num28 = (num8 * num14) - (num6 * num16);
            float num27 = (num8 * num15) - (num7 * num16);
            float num26 = (num9 * num14) - (num6 * num17);
            float num25 = (num9 * num15) - (num7 * num17);
            float num24 = (num9 * num16) - (num8 * num17);
            result.M30 = -(((num4 * num29) - (num3 * num28)) + (num2 * num27)) * num;
            result.M31 = (((num5 * num29) - (num3 * num26)) + (num2 * num25)) * num;
            result.M32 = -(((num5 * num28) - (num4 * num26)) + (num2 * num24)) * num;
            result.M33 = (((num5 * num27) - (num4 * num25)) + (num3 * num24)) * num;
        }
        /// <summary>
        /// 矩阵求和
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <returns>矩阵之和</returns>
        public static Matrix44 Add(Matrix44 matrix1, Matrix44 matrix2)
        {
            Matrix44 matrix;
            matrix.M00 = matrix1.M00 + matrix2.M00;
            matrix.M01 = matrix1.M01 + matrix2.M01;
            matrix.M02 = matrix1.M02 + matrix2.M02;
            matrix.M03 = matrix1.M03 + matrix2.M03;
            matrix.M10 = matrix1.M10 + matrix2.M10;
            matrix.M11 = matrix1.M11 + matrix2.M11;
            matrix.M12 = matrix1.M12 + matrix2.M12;
            matrix.M13 = matrix1.M13 + matrix2.M13;
            matrix.M20 = matrix1.M20 + matrix2.M20;
            matrix.M21 = matrix1.M21 + matrix2.M21;
            matrix.M22 = matrix1.M22 + matrix2.M22;
            matrix.M23 = matrix1.M23 + matrix2.M23;
            matrix.M30 = matrix1.M30 + matrix2.M30;
            matrix.M31 = matrix1.M31 + matrix2.M31;
            matrix.M32 = matrix1.M32 + matrix2.M32;
            matrix.M33 = matrix1.M33 + matrix2.M33;
            return matrix;
        }
        /// <summary>
        /// 矩阵求和
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <param name="result">[输出参数]矩阵之和</param>
        public static void Add(ref Matrix44 matrix1, ref Matrix44 matrix2, out Matrix44 result)
        {
            result.M00 = matrix1.M00 + matrix2.M00;
            result.M01 = matrix1.M01 + matrix2.M01;
            result.M02 = matrix1.M02 + matrix2.M02;
            result.M03 = matrix1.M03 + matrix2.M03;
            result.M10 = matrix1.M10 + matrix2.M10;
            result.M11 = matrix1.M11 + matrix2.M11;
            result.M12 = matrix1.M12 + matrix2.M12;
            result.M13 = matrix1.M13 + matrix2.M13;
            result.M20 = matrix1.M20 + matrix2.M20;
            result.M21 = matrix1.M21 + matrix2.M21;
            result.M22 = matrix1.M22 + matrix2.M22;
            result.M23 = matrix1.M23 + matrix2.M23;
            result.M30 = matrix1.M30 + matrix2.M30;
            result.M31 = matrix1.M31 + matrix2.M31;
            result.M32 = matrix1.M32 + matrix2.M32;
            result.M33 = matrix1.M33 + matrix2.M33;
        }
        /// <summary>
        /// 矩阵相减
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <returns>相减的结果</returns>
        public static Matrix44 Sub(Matrix44 matrix1, Matrix44 matrix2)
        {
            Matrix44 matrix;
            matrix.M00 = matrix1.M00 - matrix2.M00;
            matrix.M01 = matrix1.M01 - matrix2.M01;
            matrix.M02 = matrix1.M02 - matrix2.M02;
            matrix.M03 = matrix1.M03 - matrix2.M03;
            matrix.M10 = matrix1.M10 - matrix2.M10;
            matrix.M11 = matrix1.M11 - matrix2.M11;
            matrix.M12 = matrix1.M12 - matrix2.M12;
            matrix.M13 = matrix1.M13 - matrix2.M13;
            matrix.M20 = matrix1.M20 - matrix2.M20;
            matrix.M21 = matrix1.M21 - matrix2.M21;
            matrix.M22 = matrix1.M22 - matrix2.M22;
            matrix.M23 = matrix1.M23 - matrix2.M23;
            matrix.M30 = matrix1.M30 - matrix2.M30;
            matrix.M31 = matrix1.M31 - matrix2.M31;
            matrix.M32 = matrix1.M32 - matrix2.M32;
            matrix.M33 = matrix1.M33 - matrix2.M33;
            return matrix;
        }
        /// <summary>
        /// 矩阵相减
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <param name="result">[输出参数]相减的结果</param>
        public static void Sub(ref Matrix44 matrix1, ref Matrix44 matrix2, out Matrix44 result)
        {
            result.M00 = matrix1.M00 - matrix2.M00;
            result.M01 = matrix1.M01 - matrix2.M01;
            result.M02 = matrix1.M02 - matrix2.M02;
            result.M03 = matrix1.M03 - matrix2.M03;
            result.M10 = matrix1.M10 - matrix2.M10;
            result.M11 = matrix1.M11 - matrix2.M11;
            result.M12 = matrix1.M12 - matrix2.M12;
            result.M13 = matrix1.M13 - matrix2.M13;
            result.M20 = matrix1.M20 - matrix2.M20;
            result.M21 = matrix1.M21 - matrix2.M21;
            result.M22 = matrix1.M22 - matrix2.M22;
            result.M23 = matrix1.M23 - matrix2.M23;
            result.M30 = matrix1.M30 - matrix2.M30;
            result.M31 = matrix1.M31 - matrix2.M31;
            result.M32 = matrix1.M32 - matrix2.M32;
            result.M33 = matrix1.M33 - matrix2.M33;
        }
        /// <summary>
        /// 矩阵相除
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <returns>相除的结果</returns>
        public static Matrix44 Multiply(Matrix44 matrix1, Matrix44 matrix2)
        {
            Matrix44 matrix;
            matrix.M00 = (((matrix1.M00 * matrix2.M00) + (matrix1.M01 * matrix2.M10)) + (matrix1.M02 * matrix2.M20)) + (matrix1.M03 * matrix2.M30);
            matrix.M01 = (((matrix1.M00 * matrix2.M01) + (matrix1.M01 * matrix2.M11)) + (matrix1.M02 * matrix2.M21)) + (matrix1.M03 * matrix2.M31);
            matrix.M02 = (((matrix1.M00 * matrix2.M02) + (matrix1.M01 * matrix2.M12)) + (matrix1.M02 * matrix2.M22)) + (matrix1.M03 * matrix2.M32);
            matrix.M03 = (((matrix1.M00 * matrix2.M03) + (matrix1.M01 * matrix2.M13)) + (matrix1.M02 * matrix2.M23)) + (matrix1.M03 * matrix2.M33);
            matrix.M10 = (((matrix1.M10 * matrix2.M00) + (matrix1.M11 * matrix2.M10)) + (matrix1.M12 * matrix2.M20)) + (matrix1.M13 * matrix2.M30);
            matrix.M11 = (((matrix1.M10 * matrix2.M01) + (matrix1.M11 * matrix2.M11)) + (matrix1.M12 * matrix2.M21)) + (matrix1.M13 * matrix2.M31);
            matrix.M12 = (((matrix1.M10 * matrix2.M02) + (matrix1.M11 * matrix2.M12)) + (matrix1.M12 * matrix2.M22)) + (matrix1.M13 * matrix2.M32);
            matrix.M13 = (((matrix1.M10 * matrix2.M03) + (matrix1.M11 * matrix2.M13)) + (matrix1.M12 * matrix2.M23)) + (matrix1.M13 * matrix2.M33);
            matrix.M20 = (((matrix1.M20 * matrix2.M00) + (matrix1.M21 * matrix2.M10)) + (matrix1.M22 * matrix2.M20)) + (matrix1.M23 * matrix2.M30);
            matrix.M21 = (((matrix1.M20 * matrix2.M01) + (matrix1.M21 * matrix2.M11)) + (matrix1.M22 * matrix2.M21)) + (matrix1.M23 * matrix2.M31);
            matrix.M22 = (((matrix1.M20 * matrix2.M02) + (matrix1.M21 * matrix2.M12)) + (matrix1.M22 * matrix2.M22)) + (matrix1.M23 * matrix2.M32);
            matrix.M23 = (((matrix1.M20 * matrix2.M03) + (matrix1.M21 * matrix2.M13)) + (matrix1.M22 * matrix2.M23)) + (matrix1.M23 * matrix2.M33);
            matrix.M30 = (((matrix1.M30 * matrix2.M00) + (matrix1.M31 * matrix2.M10)) + (matrix1.M32 * matrix2.M20)) + (matrix1.M33 * matrix2.M30);
            matrix.M31 = (((matrix1.M30 * matrix2.M01) + (matrix1.M31 * matrix2.M11)) + (matrix1.M32 * matrix2.M21)) + (matrix1.M33 * matrix2.M31);
            matrix.M32 = (((matrix1.M30 * matrix2.M02) + (matrix1.M31 * matrix2.M12)) + (matrix1.M32 * matrix2.M22)) + (matrix1.M33 * matrix2.M32);
            matrix.M33 = (((matrix1.M30 * matrix2.M03) + (matrix1.M31 * matrix2.M13)) + (matrix1.M32 * matrix2.M23)) + (matrix1.M33 * matrix2.M33);
            return matrix;
        }
        /// <summary>
        /// 矩阵相乘
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <param name="result">[输出参数]相乘的结果</param>
        public static void Multiply(ref Matrix44 matrix1, ref Matrix44 matrix2, out Matrix44 result)
        {
            float num16 = (((matrix1.M00 * matrix2.M00) + (matrix1.M01 * matrix2.M10)) + (matrix1.M02 * matrix2.M20)) + (matrix1.M03 * matrix2.M30);
            float num15 = (((matrix1.M00 * matrix2.M01) + (matrix1.M01 * matrix2.M11)) + (matrix1.M02 * matrix2.M21)) + (matrix1.M03 * matrix2.M31);
            float num14 = (((matrix1.M00 * matrix2.M02) + (matrix1.M01 * matrix2.M12)) + (matrix1.M02 * matrix2.M22)) + (matrix1.M03 * matrix2.M32);
            float num13 = (((matrix1.M00 * matrix2.M03) + (matrix1.M01 * matrix2.M13)) + (matrix1.M02 * matrix2.M23)) + (matrix1.M03 * matrix2.M33);
            float num12 = (((matrix1.M10 * matrix2.M00) + (matrix1.M11 * matrix2.M10)) + (matrix1.M12 * matrix2.M20)) + (matrix1.M13 * matrix2.M30);
            float num11 = (((matrix1.M10 * matrix2.M01) + (matrix1.M11 * matrix2.M11)) + (matrix1.M12 * matrix2.M21)) + (matrix1.M13 * matrix2.M31);
            float num10 = (((matrix1.M10 * matrix2.M02) + (matrix1.M11 * matrix2.M12)) + (matrix1.M12 * matrix2.M22)) + (matrix1.M13 * matrix2.M32);
            float num9 = (((matrix1.M10 * matrix2.M03) + (matrix1.M11 * matrix2.M13)) + (matrix1.M12 * matrix2.M23)) + (matrix1.M13 * matrix2.M33);
            float num8 = (((matrix1.M20 * matrix2.M00) + (matrix1.M21 * matrix2.M10)) + (matrix1.M22 * matrix2.M20)) + (matrix1.M23 * matrix2.M30);
            float num7 = (((matrix1.M20 * matrix2.M01) + (matrix1.M21 * matrix2.M11)) + (matrix1.M22 * matrix2.M21)) + (matrix1.M23 * matrix2.M31);
            float num6 = (((matrix1.M20 * matrix2.M02) + (matrix1.M21 * matrix2.M12)) + (matrix1.M22 * matrix2.M22)) + (matrix1.M23 * matrix2.M32);
            float num5 = (((matrix1.M20 * matrix2.M03) + (matrix1.M21 * matrix2.M13)) + (matrix1.M22 * matrix2.M23)) + (matrix1.M23 * matrix2.M33);
            float num4 = (((matrix1.M30 * matrix2.M00) + (matrix1.M31 * matrix2.M10)) + (matrix1.M32 * matrix2.M20)) + (matrix1.M33 * matrix2.M30);
            float num3 = (((matrix1.M30 * matrix2.M01) + (matrix1.M31 * matrix2.M11)) + (matrix1.M32 * matrix2.M21)) + (matrix1.M33 * matrix2.M31);
            float num2 = (((matrix1.M30 * matrix2.M02) + (matrix1.M31 * matrix2.M12)) + (matrix1.M32 * matrix2.M22)) + (matrix1.M33 * matrix2.M32);
            float num = (((matrix1.M30 * matrix2.M03) + (matrix1.M31 * matrix2.M13)) + (matrix1.M32 * matrix2.M23)) + (matrix1.M33 * matrix2.M33);
            result.M00 = num16;
            result.M01 = num15;
            result.M02 = num14;
            result.M03 = num13;
            result.M10 = num12;
            result.M11 = num11;
            result.M12 = num10;
            result.M13 = num9;
            result.M20 = num8;
            result.M21 = num7;
            result.M22 = num6;
            result.M23 = num5;
            result.M30 = num4;
            result.M31 = num3;
            result.M32 = num2;
            result.M33 = num;
        }
        /// <summary>
        /// 通过指定的矩阵变换向量
        /// </summary>
        /// <param name="matrix">源向量</param>
        /// <param name="vector">变换矩阵</param>
        /// <returns>变换后的向量</returns>
        public static Vector4 TransformVector4(Matrix44 matrix, Vector4 vector)
        {
            Vector4 vector2;
            float num4 = (((vector.X * matrix.M00) + (vector.Y * matrix.M01)) + (vector.Z * matrix.M02)) + (vector.W * matrix.M03);
            float num3 = (((vector.X * matrix.M10) + (vector.Y * matrix.M11)) + (vector.Z * matrix.M12)) + (vector.W * matrix.M13);
            float num2 = (((vector.X * matrix.M20) + (vector.Y * matrix.M21)) + (vector.Z * matrix.M22)) + (vector.W * matrix.M23);
            float num = (((vector.X * matrix.M30) + (vector.Y * matrix.M31)) + (vector.Z * matrix.M32)) + (vector.W * matrix.M33);
            vector2.X = num4;
            vector2.Y = num3;
            vector2.Z = num2;
            vector2.W = num;
            return vector2;
        }
        /// <summary>
        /// 通过指定的矩阵变换向量
        /// </summary>
        /// <param name="matrix">源向量</param>
        /// <param name="vector">变换矩阵</param>
        /// <param name="result">[输出参数]变换后的向量</param>
        public static void TransformVector4(ref Matrix44 matrix, ref Vector4 vector, out Vector4 result)
        {
            float num4 = (((vector.X * matrix.M00) + (vector.Y * matrix.M01)) + (vector.Z * matrix.M02)) + (vector.W * matrix.M03);
            float num3 = (((vector.X * matrix.M10) + (vector.Y * matrix.M11)) + (vector.Z * matrix.M12)) + (vector.W * matrix.M13);
            float num2 = (((vector.X * matrix.M20) + (vector.Y * matrix.M21)) + (vector.Z * matrix.M22)) + (vector.W * matrix.M23);
            float num = (((vector.X * matrix.M30) + (vector.Y * matrix.M31)) + (vector.Z * matrix.M32)) + (vector.W * matrix.M33);
            result.X = num4;
            result.Y = num3;
            result.Z = num2;
            result.W = num;
        }
        /// <summary>
        /// 通过指定的矩阵变换位置
        /// </summary>
        /// <param name="matrix">变换矩阵</param>
        /// <param name="position">源位置</param>
        /// <returns>变换后的位置</returns>
        public static Vector3 TransformPosition(Matrix44 matrix, Vector3 position)
        {
            Vector3 vector2;
            float num4 = (((position.X * matrix.M00) + (position.Y * matrix.M01)) + (position.Z * matrix.M02)) + (matrix.M03);
            float num3 = (((position.X * matrix.M10) + (position.Y * matrix.M11)) + (position.Z * matrix.M12)) + (matrix.M13);
            float num2 = (((position.X * matrix.M20) + (position.Y * matrix.M21)) + (position.Z * matrix.M22)) + (matrix.M23);
            vector2.X = num4;
            vector2.Y = num3;
            vector2.Z = num2;
            return vector2;
        }
        /// <summary>
        /// 通过指定的矩阵变换位置
        /// </summary>
        /// <param name="matrix">变换矩阵</param>
        /// <param name="position">源位置</param>
        /// <param name="result">[输出参数]变换后的位置</param>
        public static void TransformPosition(ref Matrix44 matrix, ref Vector3 position, out Vector3 result)
        {
            float num4 = (((position.X * matrix.M00) + (position.Y * matrix.M01)) + (position.Z * matrix.M02)) + (matrix.M03);
            float num3 = (((position.X * matrix.M10) + (position.Y * matrix.M11)) + (position.Z * matrix.M12)) + (matrix.M13);
            float num2 = (((position.X * matrix.M20) + (position.Y * matrix.M21)) + (position.Z * matrix.M22)) + (matrix.M23);
            result.X = num4;
            result.Y = num3;
            result.Z = num2;
        }
        /// <summary>
        /// 通过指定的矩阵变换方向。若矩阵包含缩放因子，则方向随之缩放
        /// </summary>
        /// <param name="matrix">变换矩阵</param>
        /// <param name="direction">源方向</param>
        /// <returns>变换后的方向</returns>
        public static Vector3 TransformDirection(Matrix44 matrix, Vector3 direction)
        {
            Vector3 vector2;
            float num4 = (((direction.X * matrix.M00) + (direction.Y * matrix.M01)) + (direction.Z * matrix.M02));
            float num3 = (((direction.X * matrix.M10) + (direction.Y * matrix.M11)) + (direction.Z * matrix.M12));
            float num2 = (((direction.X * matrix.M20) + (direction.Y * matrix.M21)) + (direction.Z * matrix.M22));
            vector2.X = num4;
            vector2.Y = num3;
            vector2.Z = num2;
            return vector2;
        }
        /// <summary>
        /// 通过指定的矩阵变换方向。若矩阵包含缩放因子，则方向随之缩放
        /// </summary>
        /// <param name="matrix">变换矩阵</param>
        /// <param name="direction">源方向</param>
        /// <param name="result">[输出参数]变换后的方向</param>
        public static void TransformDirection(ref Matrix44 matrix, ref Vector3 direction, out Vector3 result)
        {
            float num4 = (((direction.X * matrix.M00) + (direction.Y * matrix.M01)) + (direction.Z * matrix.M02));
            float num3 = (((direction.X * matrix.M10) + (direction.Y * matrix.M11)) + (direction.Z * matrix.M12));
            float num2 = (((direction.X * matrix.M20) + (direction.Y * matrix.M21)) + (direction.Z * matrix.M22));
            result.X = num4;
            result.Y = num3;
            result.Z = num2;
        }
        /// <summary>
        /// 矩阵每一个元素取反
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <returns>取反矩阵</returns>
        public static Matrix44 operator -(Matrix44 matrix1)
        {
            Matrix44 matrix;
            matrix.M00 = -matrix1.M00;
            matrix.M01 = -matrix1.M01;
            matrix.M02 = -matrix1.M02;
            matrix.M03 = -matrix1.M03;
            matrix.M10 = -matrix1.M10;
            matrix.M11 = -matrix1.M11;
            matrix.M12 = -matrix1.M12;
            matrix.M13 = -matrix1.M13;
            matrix.M20 = -matrix1.M20;
            matrix.M21 = -matrix1.M21;
            matrix.M22 = -matrix1.M22;
            matrix.M23 = -matrix1.M23;
            matrix.M30 = -matrix1.M30;
            matrix.M31 = -matrix1.M31;
            matrix.M32 = -matrix1.M32;
            matrix.M33 = -matrix1.M33;
            return matrix;
        }
        /// <summary>
        /// 比较两个矩阵相等
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <returns>若两个矩阵相等，则返回true；否则，返回false。</returns>
        public static bool operator ==(Matrix44 matrix1, Matrix44 matrix2)
        {
            return ((((((matrix1.M00 == matrix2.M00) && (matrix1.M11 == matrix2.M11)) && ((matrix1.M22 == matrix2.M22) && (matrix1.M33 == matrix2.M33))) && (((matrix1.M01 == matrix2.M01) && (matrix1.M02 == matrix2.M02)) && ((matrix1.M03 == matrix2.M03) && (matrix1.M10 == matrix2.M10)))) && ((((matrix1.M12 == matrix2.M12) && (matrix1.M13 == matrix2.M13)) && ((matrix1.M20 == matrix2.M20) && (matrix1.M21 == matrix2.M21))) && (((matrix1.M23 == matrix2.M23) && (matrix1.M30 == matrix2.M30)) && (matrix1.M31 == matrix2.M31)))) && (matrix1.M32 == matrix2.M32));
        }
        /// <summary>
        /// 比较两个矩阵不相等
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <returns>若两个矩阵不相等，则返回true；否则，返回false。</returns>
        public static bool operator !=(Matrix44 matrix1, Matrix44 matrix2)
        {
            if (((((matrix1.M00 == matrix2.M00) && (matrix1.M01 == matrix2.M01)) && ((matrix1.M02 == matrix2.M02) && (matrix1.M03 == matrix2.M03))) && (((matrix1.M10 == matrix2.M10) && (matrix1.M11 == matrix2.M11)) && ((matrix1.M12 == matrix2.M12) && (matrix1.M13 == matrix2.M13)))) && ((((matrix1.M20 == matrix2.M20) && (matrix1.M21 == matrix2.M21)) && ((matrix1.M22 == matrix2.M22) && (matrix1.M23 == matrix2.M23))) && (((matrix1.M30 == matrix2.M30) && (matrix1.M31 == matrix2.M31)) && (matrix1.M32 == matrix2.M32))))
            {
                return !(matrix1.M33 == matrix2.M33);
            }
            return true;
        }
        /// <summary>
        /// 矩阵相加
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <returns>相加的结果</returns>
        public static Matrix44 operator +(Matrix44 matrix1, Matrix44 matrix2)
        {
            Matrix44 matrix;
            matrix.M00 = matrix1.M00 + matrix2.M00;
            matrix.M01 = matrix1.M01 + matrix2.M01;
            matrix.M02 = matrix1.M02 + matrix2.M02;
            matrix.M03 = matrix1.M03 + matrix2.M03;
            matrix.M10 = matrix1.M10 + matrix2.M10;
            matrix.M11 = matrix1.M11 + matrix2.M11;
            matrix.M12 = matrix1.M12 + matrix2.M12;
            matrix.M13 = matrix1.M13 + matrix2.M13;
            matrix.M20 = matrix1.M20 + matrix2.M20;
            matrix.M21 = matrix1.M21 + matrix2.M21;
            matrix.M22 = matrix1.M22 + matrix2.M22;
            matrix.M23 = matrix1.M23 + matrix2.M23;
            matrix.M30 = matrix1.M30 + matrix2.M30;
            matrix.M31 = matrix1.M31 + matrix2.M31;
            matrix.M32 = matrix1.M32 + matrix2.M32;
            matrix.M33 = matrix1.M33 + matrix2.M33;
            return matrix;
        }
        /// <summary>
        /// 矩阵相减
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <returns>相减的结果</returns>
        public static Matrix44 operator -(Matrix44 matrix1, Matrix44 matrix2)
        {
            Matrix44 matrix;
            matrix.M00 = matrix1.M00 - matrix2.M00;
            matrix.M01 = matrix1.M01 - matrix2.M01;
            matrix.M02 = matrix1.M02 - matrix2.M02;
            matrix.M03 = matrix1.M03 - matrix2.M03;
            matrix.M10 = matrix1.M10 - matrix2.M10;
            matrix.M11 = matrix1.M11 - matrix2.M11;
            matrix.M12 = matrix1.M12 - matrix2.M12;
            matrix.M13 = matrix1.M13 - matrix2.M13;
            matrix.M20 = matrix1.M20 - matrix2.M20;
            matrix.M21 = matrix1.M21 - matrix2.M21;
            matrix.M22 = matrix1.M22 - matrix2.M22;
            matrix.M23 = matrix1.M23 - matrix2.M23;
            matrix.M30 = matrix1.M30 - matrix2.M30;
            matrix.M31 = matrix1.M31 - matrix2.M31;
            matrix.M32 = matrix1.M32 - matrix2.M32;
            matrix.M33 = matrix1.M33 - matrix2.M33;
            return matrix;
        }
        /// <summary>
        /// 矩阵相乘
        /// </summary>
        /// <param name="matrix1">源矩阵</param>
        /// <param name="matrix2">源矩阵</param>
        /// <returns>相乘的结果</returns>
        public static Matrix44 operator *(Matrix44 matrix1, Matrix44 matrix2)
        {
            Matrix44 matrix;
            matrix.M00 = (((matrix1.M00 * matrix2.M00) + (matrix1.M01 * matrix2.M10)) + (matrix1.M02 * matrix2.M20)) + (matrix1.M03 * matrix2.M30);
            matrix.M01 = (((matrix1.M00 * matrix2.M01) + (matrix1.M01 * matrix2.M11)) + (matrix1.M02 * matrix2.M21)) + (matrix1.M03 * matrix2.M31);
            matrix.M02 = (((matrix1.M00 * matrix2.M02) + (matrix1.M01 * matrix2.M12)) + (matrix1.M02 * matrix2.M22)) + (matrix1.M03 * matrix2.M32);
            matrix.M03 = (((matrix1.M00 * matrix2.M03) + (matrix1.M01 * matrix2.M13)) + (matrix1.M02 * matrix2.M23)) + (matrix1.M03 * matrix2.M33);
            matrix.M10 = (((matrix1.M10 * matrix2.M00) + (matrix1.M11 * matrix2.M10)) + (matrix1.M12 * matrix2.M20)) + (matrix1.M13 * matrix2.M30);
            matrix.M11 = (((matrix1.M10 * matrix2.M01) + (matrix1.M11 * matrix2.M11)) + (matrix1.M12 * matrix2.M21)) + (matrix1.M13 * matrix2.M31);
            matrix.M12 = (((matrix1.M10 * matrix2.M02) + (matrix1.M11 * matrix2.M12)) + (matrix1.M12 * matrix2.M22)) + (matrix1.M13 * matrix2.M32);
            matrix.M13 = (((matrix1.M10 * matrix2.M03) + (matrix1.M11 * matrix2.M13)) + (matrix1.M12 * matrix2.M23)) + (matrix1.M13 * matrix2.M33);
            matrix.M20 = (((matrix1.M20 * matrix2.M00) + (matrix1.M21 * matrix2.M10)) + (matrix1.M22 * matrix2.M20)) + (matrix1.M23 * matrix2.M30);
            matrix.M21 = (((matrix1.M20 * matrix2.M01) + (matrix1.M21 * matrix2.M11)) + (matrix1.M22 * matrix2.M21)) + (matrix1.M23 * matrix2.M31);
            matrix.M22 = (((matrix1.M20 * matrix2.M02) + (matrix1.M21 * matrix2.M12)) + (matrix1.M22 * matrix2.M22)) + (matrix1.M23 * matrix2.M32);
            matrix.M23 = (((matrix1.M20 * matrix2.M03) + (matrix1.M21 * matrix2.M13)) + (matrix1.M22 * matrix2.M23)) + (matrix1.M23 * matrix2.M33);
            matrix.M30 = (((matrix1.M30 * matrix2.M00) + (matrix1.M31 * matrix2.M10)) + (matrix1.M32 * matrix2.M20)) + (matrix1.M33 * matrix2.M30);
            matrix.M31 = (((matrix1.M30 * matrix2.M01) + (matrix1.M31 * matrix2.M11)) + (matrix1.M32 * matrix2.M21)) + (matrix1.M33 * matrix2.M31);
            matrix.M32 = (((matrix1.M30 * matrix2.M02) + (matrix1.M31 * matrix2.M12)) + (matrix1.M32 * matrix2.M22)) + (matrix1.M33 * matrix2.M32);
            matrix.M33 = (((matrix1.M30 * matrix2.M03) + (matrix1.M31 * matrix2.M13)) + (matrix1.M32 * matrix2.M23)) + (matrix1.M33 * matrix2.M33);
            return matrix;
        }

        static Matrix44()
        {
            _identity = new Matrix44(1f, 0f, 0f, 0f, 0f, 1f, 0f, 0f, 0f, 0f, 1f, 0f, 0f, 0f, 0f, 1f);
        }
    }
}

