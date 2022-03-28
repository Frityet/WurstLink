using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace WurstLink.TestHost
{
    public unsafe class Pointer<T> where T : unmanaged
    {
        public T* RawPointer { get; }
        public IntPtr Address { get; }

        public Pointer()
        {
            RawPointer = null;
            Address = IntPtr.Zero;
        }

        public Pointer(IntPtr ptr)
        {
            RawPointer = (T*) ptr;
            Address = ptr;
        }

        public Pointer(T* ptr)
        {
            RawPointer = ptr;
            Address = (IntPtr) ptr;
        }

        public static implicit operator T*(Pointer<T> ptr) => ptr.RawPointer;
        public static implicit operator IntPtr(Pointer<T> ptr) => ptr.Address;

        public T this[int index]
        {
            get => RawPointer[index];
            set => RawPointer[index] = value;
        }
    }
    
    
    public unsafe class Memory : IDisposable
    {
        public int Count { get; private set; }
        public int MaxCount { get; private set; }
        
        public byte[] Bytes
        {
            get
            {
                var arr = new byte[Count];
                
                for (int i = 0; i < Count; i++)
                    arr[i] = _pointer[i];

                return arr;
            }
        }
        
        private byte* _pointer;

        public Memory(int count)
        {
            Count = 0;
            MaxCount = 0;

            if (count < 1)
            {
                _pointer = new Pointer<byte>();
                return;
            }

            MaxCount = (int)((UInt32) count).To2ndPower();

            _pointer = new Pointer<byte>(Marshal.AllocHGlobal(sizeof(byte) * MaxCount));
        }

        private void Grow(int count)
        {
            if (count < 1)
                return;

            Marshal.ReAllocHGlobal((IntPtr)_pointer, (IntPtr)(sizeof(byte) * (MaxCount += (int)((UInt32)count).To2ndPower())));
        }

        private void Grow() => Grow(MaxCount);

        public void Write(byte b)
        {
            if (Count >= MaxCount - 1)
                Grow();

            _pointer[Count++] = b;
        }
        
        public void Write(byte b, int index)
        {
            if (index >= MaxCount - 1)
                Grow(index - MaxCount);

            _pointer[index] = b;
        }
        
        public void Write(byte[] bytes)
        {
            if (Count + bytes.Length >= MaxCount - 1)
                Grow();
        }

        public void Write(byte[] bytes, int index)
        {
            if (index + bytes.Length >= MaxCount - 1)
                Grow(index + bytes.Length - MaxCount);

            for (int i = index, j = 0; j < bytes.Length; j++, i++)
                _pointer[i] = bytes[j];
            
        }

        public void Reset() => Count = 0;

        public void Clear()
        {
            Reset();

            for (int i = 0; i < MaxCount; i++)
                _pointer[i] = 0;
        }

        public Memory Copy()
        {
            var mem = new Memory(Count);

            for (int i = 0; i < Count; i++)
                mem[i] = _pointer[i];

            return mem;
        }

        public byte this[int index]
        {
            get => index > MaxCount ? throw new IndexOutOfRangeException($"Index {index} is out of range for memory (max {MaxCount} bytes)")
                                    : _pointer[index];

            set
            {
                if (index > MaxCount)
                    Grow(index - MaxCount);

                _pointer[index] = value;
            }
        }

        public void Dispose()
        {
            Marshal.FreeHGlobal((IntPtr)_pointer);
            GC.SuppressFinalize(this);
        }
    }
}