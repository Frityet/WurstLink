using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace WurstLink.TestServer
{
	public static class Packet
	{
		public const ulong MAGIC = 0xB00B135;

		public class Member<T> : IByteConvertable where T : IByteConvertable
		{
			public Member(bool primitive, bool unsigned, T data)
			{
				Primitive = primitive;
				Unsigned = unsigned;
				Data = data;
				Size = Marshal.SizeOf(typeof(T));
			}

			public bool Primitive { get; }
			public bool Unsigned { get; }
			public int Size { get; }
			public T Data { get; }

			public byte[] AsBytes
			{
				get
				{
					var bytes = new List<byte>(sizeof(bool) + sizeof(bool) + sizeof(ulong) + Size);

					bytes.AddRange(BitConverter.GetBytes(Unsigned));
					bytes.AddRange(BitConverter.GetBytes(Primitive));
					bytes.AddRange(BitConverter.GetBytes(Size));
					bytes.AddRange(Data.AsBytes);

					return bytes.ToArray();
				}
			}
		}

		public class Builder : IBuilder<byte[]>
		{
			private readonly List<IByteConvertable> _packet;

			public Builder()
			{
				_packet = new List<IByteConvertable>();
			}

			public byte[] Build()
			{
				var bytes = new List<byte>();

				foreach (IByteConvertable member in _packet)
					bytes.AddRange(member.AsBytes);

				return bytes.ToArray();
			}

			public Builder Add<T>(T data) where T : IByteConvertable
			{
				_packet.Add(new Member<T>(false, false, data));
				return this;
			}
		}
	}
}