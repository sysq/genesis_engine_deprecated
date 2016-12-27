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
using System;
using System.Runtime.CompilerServices;
using ScriptRuntime;

namespace ScriptRuntime
{
    /// <summary>
    /// 材质实例
    /// </summary>
    public partial class MaterialInstance : Base
    {

        private MaterialInstance(DummyClass__ dummyObj)
        {

        }

        private MaterialInstance()
        {
            //ICall_Material_Bind(this);
        }

        ~MaterialInstance()
        {
            ICall_Material_Release(this);
        }

        /// <summary>
        /// 为材质中的纹理参数设置纹理
        /// </summary>
        /// <param name="paramName">材质中的纹理参数名称</param>
        /// <param name="texture">要设置的纹理</param>
        public void SetTexture(String paramName, Texture texture)
        {
            ICall_Material_SetTexture(this, paramName, texture.GetTextureHandlePtr());
        }

        /// <summary>
        /// 为材质中的纹理参数设置纹理资源路径
        /// </summary>
        /// <param name="paramName">材质中的纹理参数名称</param>
        /// <param name="id">要设置的纹理资源路径</param>
        public void SetTexture(String paramName, String id)
        {
			ICall_Material_SetTextureResource(this, paramName, id, 0);
        }

		/// <summary>
		/// 为材质中的纹理参数设置纹理资源路径
		/// </summary>
		/// <param name="paramName">材质中的纹理参数名称</param>
		/// <param name="id">要设置的纹理资源路径</param>
		/// <param name="priority">默认的0表示同步加载,1表示异步加载</param>
		public void SetTexture(String paramName, String id, int priority)
		{
			ICall_Material_SetTextureResource(this, paramName, id, priority);
		}

        /// <summary>
        /// 为材质中的浮点参数设值 
        /// </summary>
        /// <param name="paramName">材质中的浮点参数名称</param>
        /// <param name="value">要设置的浮点数值</param>
        public void SetValue(String paramName, float value)
        {
            ICall_Material_SetValueFloat(this, paramName, value);
        }

        /// <summary>
        /// 为材质中的向量参数设置向量值
        /// </summary>
        /// <param name="paramName">材质的向量参数名称</param>
        /// <param name="value">要设置的向量参数值</param>
        public void SetValue(String paramName, Vector4 value)
        {
            ICall_Material_SetValueVector4(this, paramName, ref value);
        }

        /// <summary>
        /// 为材质中的矩阵参数设置矩阵值
        /// </summary>
        /// <param name="paramName">材质中的矩阵参数名称</param>
        /// <param name="value">要设置的矩阵参数值</param>
        public void SetValue(String paramName, ref Matrix44 value)
        {
            ICall_Material_SetValueMatrix44(this, paramName, ref value);
        }
    }
}
