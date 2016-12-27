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
namespace ScriptRuntime.Graphics.PackedVector
{
    using System;

    internal static class PackUtils
    {
        private static double ClampAndRound(float value, float min, float max)
        {
            if (float.IsNaN(value))
            {
                return 0.0;
            }
            if (float.IsInfinity(value))
            {
                return (float.IsNegativeInfinity(value) ? ((double) min) : ((double) max));
            }
            if (value < min)
            {
                return (double) min;
            }
            if (value > max)
            {
                return (double) max;
            }
            return Math.Round((double) value);
        }

        public static uint PackSigned(uint bitmask, float value)
        {
            float max = bitmask >> 1;
            float min = -max - 1f;
            return (((uint) ((int) ClampAndRound(value, min, max))) & bitmask);
        }

        public static uint PackSNorm(uint bitmask, float value)
        {
            float max = bitmask >> 1;
            value *= max;
            return (((uint) ((int) ClampAndRound(value, -max, max))) & bitmask);
        }

        public static uint PackUNorm(float bitmask, float value)
        {
            value *= bitmask;
            return (uint) ClampAndRound(value, 0f, bitmask);
        }

        public static uint PackUnsigned(float bitmask, float value)
        {
            return (uint) ClampAndRound(value, 0f, bitmask);
        }

        public static float UnpackSNorm(uint bitmask, uint value)
        {
            uint num = (uint) ((bitmask + 1) >> 1);
            if ((value & num) != 0)
            {
                if ((value & bitmask) == num)
                {
                    return -1f;
                }
                value |= ~bitmask;
            }
            else
            {
                value &= bitmask;
            }
            float num2 = bitmask >> 1;
            return (((float) value) / num2);
        }

        public static float UnpackUNorm(uint bitmask, uint value)
        {
            value &= bitmask;
            return (((float) value) / ((float) bitmask));
        }
    }
}

