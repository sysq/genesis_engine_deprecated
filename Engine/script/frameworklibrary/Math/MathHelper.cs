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
    /// <summary>
    /// 数学运算类
    /// </summary>
    public static class MathHelper
    {
        /// <summary>
        /// 数学常数 e
        /// </summary>
        public const float E = 2.718282f;
        /// <summary>
        /// log10 e.
        /// </summary>
        public const float Log10E = 0.4342945f;
        /// <summary>
        /// log2 e.
        /// </summary>
        public const float Log2E = 1.442695f;
        /// <summary>
        /// 圆周率pi
        /// </summary>
        public const float Pi = 3.141593f;
        /// <summary>
        /// 圆周率pi/2
        /// </summary>
        public const float PiOver2 = 1.570796f;
        /// <summary>
        /// 圆周率pi/4
        /// </summary>
        public const float PiOver4 = 0.7853982f;
        /// <summary>
        /// 圆周率pi*2
        /// </summary>
        public const float TwoPi = 6.283185f;
        /// <summary>
        /// 角度转换为弧度所用常量
        /// </summary>
        public const float Deg2Rad = 0.01745329f;
        /// <summary>
        /// 弧度转换为角度所用常量
        /// </summary>
        public const float Rad2Deg = 57.29578f;
        /// <summary>
        /// 一个很小的浮点值
        /// </summary>
        public const float Epsilon = float.Epsilon;
        /// <summary>
        /// 代表正无穷
        /// </summary>
        public const float Infinity = float.PositiveInfinity;
        /// <summary>
        /// 代表负无穷
        /// </summary>
        public const float NegativeInfinity = float.NegativeInfinity;
        /// <summary>
        /// 在规定的范围内限定一个值
        /// </summary>
        /// <param name="value">需要限定的值</param>
        /// <param name="min">最小值</param>
        /// <param name="max">最大值</param>
        /// <returns>返回值</returns>
        public static float Clamp(float value, float min, float max)
        {
            value = (value > max) ? max : value;
            value = (value < min) ? min : value;
            return value;
        }
        /// <summary>
        /// 在规定的范围内限定一个值
        /// </summary>
        /// <param name="value">需要限定的值</param>
        /// <param name="min">最小值</param>
        /// <param name="max">最大值</param>
        /// <returns>返回值</returns>
        public static int Clamp(int value, int min, int max)
        {
            value = (value > max) ? max : value;
            value = (value < min) ? min : value;
            return value;
        }
        /// <summary>
        /// Hermite样条插值
        /// </summary>
        /// <param name="value1">源值</param>
        /// <param name="tangent1">源切线</param>
        /// <param name="value2">源值</param>
        /// <param name="tangent2">源切线</param>
        /// <param name="amount">权重</param>
        /// <returns>Hermite样条插值的结果</returns>
        public static float Hermite(float value1, float tangent1, float value2, float tangent2, float amount)
        {
            float num3 = amount;
            float num = num3 * num3;
            float num2 = num3 * num;
            float num7 = ((2f * num2) - (3f * num)) + 1f;
            float num6 = (-2f * num2) + (3f * num);
            float num5 = (num2 - (2f * num)) + num3;
            float num4 = num2 - num;
            return ((((value1 * num7) + (value2 * num6)) + (tangent1 * num5)) + (tangent2 * num4));
        }
        /// <summary>
        /// 两个向量之间的线性插值
        /// </summary>
        /// <param name="value1">源值</param>
        /// <param name="value2">源值</param>
        /// <param name="amount">表明value2的权重，数字0到1之间</param>
        /// <returns>结果所得值</returns>
        public static float Lerp(float value1, float value2, float amount)
        {
            return (value1 + ((value2 - value1) * amount));
        }
        /// <summary>
        /// 返回较大的值
        /// </summary>
        /// <param name="a">源值</param>
        /// <param name="b">源值</param>
        /// <returns>较大的值</returns>
        public static float Max(float a, float b)
        {
            return ((a <= b) ? b : a);
        }
        /// <summary>
        /// 返回较小的值
        /// </summary>
        /// <param name="a">源值</param>
        /// <param name="b">源值</param>
        /// <returns>较小的值</returns>
        public static float Min(float a, float b)
        {
            return ((a >= b) ? b : a);
        }
        /// <summary>
        /// 返回较大的值 
        /// </summary>
        /// <param name="a">源值</param>
        /// <param name="b">源值</param>
        /// <returns>较大的值</returns>
        public static int Max(int a, int b)
        {
            return ((a <= b) ? b : a);
        }
        /// <summary>
        /// 返回较小的值
        /// </summary>
        /// <param name="a">源值</param>
        /// <param name="b">源值</param>
        /// <returns>较小</returns>
        public static int Min(int a, int b)
        {
            return ((a >= b) ? b : a);
        }
        /// <summary>
        /// 用立方等式在两个值之间插值
        /// </summary>
        /// <param name="value1">源值</param>
        /// <param name="value2">源值</param>
        /// <param name="amount">权重</param>
        /// <returns>插值结果</returns>
        public static float SmoothStep(float value1, float value2, float amount)
        {
            float num = Clamp(amount, 0f, 1f);
            return Lerp(value1, value2, (num * num) * (3f - (2f * num)));
        }
        /// <summary>
        /// 将弧度转换为角度
        /// </summary>
        /// <param name="radians">角的弧度值</param>
        /// <returns>角的角度值</returns>
        public static float RadiansToDegrees(float radians)
        {
            return (radians * 57.29578f);
        }
        /// <summary>
        /// 将弧度转换为角度
        /// </summary>
        /// <param name="degrees">角的角度值</param>
        /// <returns>角的弧度值</returns>
        public static float DegreesToRadians(float degrees)
        {
            return (degrees * 0.01745329f);
        }
        /// <summary>
        /// 指定角的正弦
        /// </summary>
        /// <param name="f">一个角的弧度表示</param>
        /// <returns>角的正弦值，如果角为NaN或者是正无穷或负无穷，返回NaN</returns>
        public static float Sin(float f)
        {
            return (float)Math.Sin((double)f);
        }
        /// <summary>
        /// 指定角的余弦
        /// </summary>
        /// <param name="f">一个角的弧度表示</param>
        /// <returns>角的余弦值</returns>
        public static float Cos(float f)
        {
            return (float)Math.Cos((double)f);
        }
        /// <summary>
        /// 指定角的正切
        /// </summary>
        /// <param name="f">一个角的弧度表示</param>
        /// <returns>角的正切值，如果角为NaN或者是正无穷或负无穷，返回NaN</returns>
        public static float Tan(float f)
        {
            return (float)Math.Tan((double)f);
        }
        /// <summary>
        /// 指定值的反正弦
        /// </summary>
        /// <param name="f">正弦值</param>
        /// <returns>一个角的弧度表示</returns>
        public static float ASin(float f)
        {
            return (float)Math.Asin((double)f);

        }
        /// <summary>
        /// 指定值的反余弦
        /// </summary>
        /// <param name="f">余弦值</param>
        /// <returns>一个角的弧度表示</returns>
        public static float ACos(float f)
        {
            return (float)Math.Acos((double)f);
        }
        /// <summary>
        /// 指定值的反正切
        /// </summary>
        /// <param name="f">正切值</param>
        /// <returns>一个角的弧度表示</returns>
        public static float ATan(float f)
        {
            return (float)Math.Atan((double)f);
        }
        /// <summary>
        /// Returns the angle whose tangent is the quotient of two specified numbers.
        /// </summary>
        /// <param name="a">点的X坐标</param>
        /// <param name="b">点的Y坐标</param>
        /// <returns>一个角的弧度表示, 例如 -π≤θ≤π, 和 tan(θ) = y / x, 点(x, y)在笛卡尔坐标系中.遵守如下规律: 
        ///(x, y)在第一象限 ,0 &lt; θ &lt; π/2.
        ///(x, y)在第二象限 , π/2 &lt; θ≤π.
        ///(x, y)在第三象限 , -π &lt; θ &lt; -π/2.
        ///(x, y)在第四象限 , -π/2 &lt; θ &lt; 0.
        ///如果点在象限边界，值如下：
        ///如果X为0，Y为非负, θ = 0.
        ///如果X为0，Y为负, θ = π.
        ///如果X为负，Y为0, θ = π/2.
        ///如果X为正，Y为0, θ = -π/2.
        ///</returns>
        public static float ATan2(float a, float b)
        {
            return (float)Math.Atan2((double)a, (double)b);
        }
        /// <summary>
        /// 求指定数的平方根
        /// </summary>
        /// <param name="f">指定数值</param>
        /// <returns>零或者正数返回结果，负数返回NaN</returns>
        public static float Sqrt(float f)
        {
            return (float)Math.Sqrt((double)f);
        }
        /// <summary>
        /// 求指定数的绝对值
        /// </summary>
        /// <param name="f">指定数</param>
        /// <returns>绝对值</returns>
        public static float Abs(float f)
        {
            return Math.Abs(f);
        }
        /// <summary>
        /// 求指定数的绝对值
        /// </summary>
        /// <param name="value">指定数</param>
        /// <returns>绝对值</returns>
        public static int Abs(int value)
        {
            return Math.Abs(value);
        }
        /// <summary>
        /// 求指定数的指定次方
        /// </summary>
        /// <param name="fBase">底数</param>
        /// <param name="fExponent">指数 </param>
        /// <returns>结果值</returns>
        public static float Pow(float fBase, float fExponent)
        {
            return (float)Math.Pow((double)fBase, (double)fExponent);
        }
        /// <summary>
        /// 数学常量e的指定次方
        /// </summary>
        /// <param name="power">指数</param>
        /// <returns>结果值</returns>
        public static float Exp(float power)
        {
            return (float)Math.Exp((double)power);
        }
        /// <summary>
        /// 求指定数的对数
        /// </summary>
        /// <param name="f">指定数</param>
        /// <returns>结果</returns>
        public static float Log(float f)
        {
            return (float)Math.Log((double)f);
        }
        /// <summary>
        /// 求指定数的以10为底对数
        /// </summary>
        /// <param name="f">指定数</param>
        /// <returns>结果</returns>
        public static float Log10(float f)
        {
            return (float)Math.Log10((double)f);
        }
        /// <summary>
        /// 求比指定数大或相等的最小整数
        /// </summary>
        /// <param name="f">一个数 </param>
        /// <returns>比指定数大或相等的最小整数</returns>
        public static float Ceil(float f)
        {
            return (float)Math.Ceiling((double)f);
        }
        /// <summary>
        /// 求比指定数小或相等的最大整数
        /// </summary>
        /// <param name="f">一个数 </param>
        /// <returns>比指定数小或相等的最大整数</returns>
        public static float Floor(float f)
        {
            return (float)Math.Floor((double)f);
        }
        /// <summary>
        /// 求距指定数最近的一个整数
        /// </summary>
        /// <param name="f">一个数.</param>
        /// <returns>结果</returns>
        public static float Round(float f)
        {
            return (float)Math.Round((double)f);
        }
        /// <summary>
        /// 求比指定数大或相等的最小整数
        /// </summary>
        /// <param name="f">一个数 </param>
        /// <returns>比指定数大或相等的最小整数</returns>
        public static int ICeil(float f)
        {
            return (int)Math.Ceiling((double)f);
        }
        /// <summary>
        /// 求比指定数小或相等的最大整数
        /// </summary>
        /// <param name="f">一个数</param>
        /// <returns>比指定数小或相等的最大整数</returns>
        public static int IFloor(float f)
        {
            return (int)Math.Floor((double)f);
        }
        /// <summary>
        /// 求距指定数最近的一个整数
        /// </summary>
        /// <param name="f">一个数</param>
        /// <returns>距指定数最近的一个整数</returns>
        public static int IRound(float f)
        {
            return (int)Math.Round((double)f);
        }
        /// <summary>
        /// 检测一个数是否是2的次幂
        /// </summary>
        /// <param name="value">一个数</param>
        /// <returns>是返回true，不是false</returns>
        public static bool IsPowerOfTwo(int value)
        {
            return ((value & (value - 1)) == 0);
        }
        /// <summary>
        /// 求最接近较大的2的次幂
        /// </summary>
        /// <param name="v">一个数</param>
        /// <returns>最接近较大的2的次幂</returns>
        public static int NextPowerOfTwo(int v)
        {
            v -= 1;
            v |= v >> 16;
            v |= v >> 8;
            v |= v >> 4;
            v |= v >> 2;
            v |= v >> 1;
            return v + 1;
        }
    }
}

