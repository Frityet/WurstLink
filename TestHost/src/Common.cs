using System;

namespace WurstLink.TestHost
{
    public static class Common
    {
        public static UInt32 To2ndPower(UInt32 n)
        {
            n--;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;
            ++n;
            return n;
        }
    }
    
    public static class Extensions
    {
        public static UInt32 To2ndPower(this UInt32 n) => Common.To2ndPower(n);
    }
}