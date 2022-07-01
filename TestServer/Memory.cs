using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Runtime.Serialization;
using System.Text;

namespace WurstLink.TestServer
{
	public readonly unsafe struct Pointer<T> : ISerializable where T : unmanaged
	{
		public int Size { get; }
		public int Count { get; }
		public int TotalSize => Size * Count;

		private readonly IntPtr _address;
		private readonly T* _data;

		public Pointer(IntPtr address, int count = 1) : this(data: (T*) address, count: count)
		{
		}

		public Pointer(T* data, int count = 1)
		{
			_address = new IntPtr(data);
			_data = data;
			Size = sizeof(T);
			Count = count;
		}

		public Pointer(int count) : this(address: Marshal.AllocHGlobal(count), count: count)
		{
		}

		public T this[uint index]
		{
			get
			{
				if (index >= Count) throw new IndexOutOfRangeException($"{index}");
				return _data[index];
			}
			set
			{
				if (index >= Count) throw new IndexOutOfRangeException($"{index}");
				_data[index] = value;
			}
		}

		public static implicit operator IntPtr(Pointer<T> ptr)
		{
			return ptr._address;
		}

		public static implicit operator T*(Pointer<T> ptr)
		{
			return ptr._data;
		}

		public void GetObjectData(SerializationInfo info, StreamingContext context)
		{
			info.AddValue(name: nameof(Size), value: Size);
			info.AddValue(name: nameof(Count), value: Count);
			for (var i = 0; i < Count; i++)
			for (var j = 0; j < Size; j++)
				info.AddValue(name: $"Data[{i}][{j}]", value: _data[i]);
		}
	}

	[Serializable]
	public struct Memory<T> where T : unmanaged
	{
		public ulong Size { get; }
		public Pointer<T> Data { get; private set; }

		public Memory(ulong size)
		{
			Size = size;
			Data = new Pointer<T>();
		}
	}

	public interface IByteConvertable
	{
		public byte[] AsBytes { get; }
	}

	public abstract class ByteConvertable<T> : IByteConvertable
	{
		protected readonly T Data;

		protected ByteConvertable(T value)
		{
			Data = value;
		}

		public abstract byte[] AsBytes { get; }
	}

	public static class ByteConvertable
	{
#region Byte converters

		public class Boolean : ByteConvertable<bool>
		{
			public Boolean(bool value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator Boolean(bool val)
			{
				return new Boolean(val);
			}
		}

		public class SignedByte : ByteConvertable<sbyte>
		{
			public SignedByte(sbyte value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator SignedByte(sbyte val)
			{
				return new SignedByte(val);
			}
		}

		public class Character : ByteConvertable<char>
		{
			public Character(char value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator Character(char val)
			{
				return new Character(val);
			}
		}

		public class Short : ByteConvertable<short>
		{
			public Short(short value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator Short(short val)
			{
				return new Short(val);
			}
		}

		public class Integer : ByteConvertable<int>
		{
			public Integer(int value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator Integer(int val)
			{
				return new Integer(val);
			}
		}

		public class Long : ByteConvertable<long>
		{
			public Long(long value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator Long(long val)
			{
				return new Long(val);
			}
		}

		public class Byte : ByteConvertable<byte>
		{
			public Byte(byte value) : base(value)
			{
			}

			public override byte[] AsBytes => new[] {Data};

			public static implicit operator Byte(byte val)
			{
				return new Byte(val);
			}
		}

		public class UnsignedShort : ByteConvertable<ushort>
		{
			public UnsignedShort(ushort value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator UnsignedShort(ushort val)
			{
				return new UnsignedShort(val);
			}
		}

		public class UnsignedInteger : ByteConvertable<uint>
		{
			public UnsignedInteger(uint value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator UnsignedInteger(uint val)
			{
				return new UnsignedInteger(val);
			}
		}

		public class UnsignedLong : ByteConvertable<ulong>
		{
			public UnsignedLong(ulong value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator UnsignedLong(ulong val)
			{
				return new UnsignedLong(val);
			}
		}

		public class Float : ByteConvertable<float>
		{
			public Float(float value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator Float(float val)
			{
				return new Float(val);
			}
		}

		public class Double : ByteConvertable<double>
		{
			public Double(double value) : base(value)
			{
			}

			public override byte[] AsBytes => BitConverter.GetBytes(Data);

			public static implicit operator Double(double val)
			{
				return new Double(val);
			}
		}

		public class String : ByteConvertable<string>
		{
			public String(string value) : base(value)
			{
			}

			public override byte[] AsBytes
			{
				get
				{
					var bytes = new List<byte>(sizeof(int) + sizeof(char) * Data.Length);

					bytes.AddRange(BitConverter.GetBytes(Data.Length));
					bytes.AddRange(Encoding.UTF8.GetBytes(Data));

					return bytes.ToArray();
				}
			}

			public static implicit operator String(string val)
			{
				return new String(val);
			}
		}

#endregion
	}
}