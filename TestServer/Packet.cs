using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WurstLink.TestServer
{
	public static class Packet
	{
		public const ulong MAGIC = 0xB00B135;

		// public static IEnumerable<Member> Decode(byte[] binary)
		// {
		// 	
		// }

		public static T[] Slice<T>(this T[] self, int start, int end = -1)
		{
			end = end < 0 ? self.Length : end;

			int total = end - start;
			if (total < 0)
				throw new IndexOutOfRangeException($"{total}");

			var arr = new T[total];
			for (int i = start; i < end; i++)
				arr[start - i] = self[i];
			return arr;
		}

		public interface IBitConvertable
		{
			public byte[] GetBytes();
		}

		public interface ISerialisableCustomType : IBitConvertable
		{
			public CustomTypeDescriptor Members { get; }
		}

		public readonly struct CustomTypeDescriptor : IBitConvertable
		{
			public Member.Type[] Members { get; }
			public int Count { get; }

			public CustomTypeDescriptor(Member.Type[] members)
			{
				Members = members;
				Count = members.Length;
			}

			public byte[] GetBytes()
			{
				var arr = new byte[Members.Length + sizeof(int)];

				for (var i = 0; i < Members.Length; i++)
					arr[i] = (byte) Members[i];

				byte[] bs = BitConverter.GetBytes(Count);
				for (var i = 0; i < bs.Length; i++)
					arr[i + Members.Length] = bs[i];

				return arr;
			}
		}

		public struct Member : IBitConvertable
		{
			public enum Type : byte
			{
				UNSIGNED_BYTE,
				UNSIGNED_SHORT,
				UNSIGNED_INTEGER,
				UNSIGNED_LONG,

				SIGNED_BYTE,
				SIGNED_SHORT,
				SIGNED_INTEGER,
				SIGNED_LONG,

				STRING,

				FLOAT,
				DOUBLE,

				TYPE,
				ARRAY,
				CUSTOM
			}

			public Type MemberType { get; internal set; }
			public int Size { get; internal set; }
			public byte[] Data { get; internal set; }

			public Member(Type type, byte[] data)
			{
				MemberType = type;
				Size = data.Length;
				Data = data;
			}

			public byte[] GetBytes()
			{
				var bytes = new byte[sizeof(bool) + sizeof(Type) + sizeof(int) + Data.Length];
				var i = 0;

				bytes[i++] = (byte) MemberType;

				foreach (byte b in BitConverter.GetBytes(Size))
					bytes[i++] = b;

				foreach (byte b in Data)
					bytes[i++] = b;

				return bytes;
			}

			public readonly struct Array : IBitConvertable
			{
				public Type Type { get; }
				public int Length { get; }
				public byte[,] Data { get; }

				public Array(Type type, byte[,] data, int length = -1)
				{
					Type = type;
					Length = length < 0 ? data.Length : length;
					Data = data;
				}

				public byte[] GetBytes()
				{
					var bytes = new List<byte>();

					bytes.Add((byte) Type);
					bytes.AddRange(BitConverter.GetBytes(Length));
					bytes.AddRange(Data.Cast<byte>());

					return bytes.ToArray();
				}
			}
		}

		public class Builder : IBuilder<byte[]>, IBitConvertable
		{
			private readonly List<Member> _data;
			private int _count;

			public Builder()
			{
				_count = 2;
				_data = new List<Member>
				        {
					        new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(MAGIC)),
					        new Member(type: Member.Type.SIGNED_INTEGER, data: BitConverter.GetBytes(_count++))
				        };
			}

			public byte[] GetBytes()
			{
				return Build();
			}

			public byte[] Build()
			{
				var bytes = new List<byte>();

				foreach (Member member in _data)
					bytes.AddRange(member.GetBytes());

				return bytes.ToArray();
			}

			public Builder Add(byte b)
			{
				_data.Add(new Member(type: Member.Type.UNSIGNED_BYTE, data: new[] {b}));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));
				return this;
			}

			public Builder Add(ushort b)
			{
				_data.Add(new Member(type: Member.Type.UNSIGNED_SHORT, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(uint b)
			{
				_data.Add(new Member(type: Member.Type.UNSIGNED_INTEGER, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(ulong b)
			{
				_data.Add(new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(float b)
			{
				_data.Add(new Member(type: Member.Type.FLOAT, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(double b)
			{
				_data.Add(new Member(type: Member.Type.DOUBLE, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(sbyte b)
			{
				_data.Add(new Member(type: Member.Type.SIGNED_BYTE, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(short b)
			{
				_data.Add(new Member(type: Member.Type.SIGNED_SHORT, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(int b)
			{
				_data.Add(new Member(type: Member.Type.SIGNED_INTEGER, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(long b)
			{
				_data.Add(new Member(type: Member.Type.SIGNED_LONG, data: BitConverter.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add(string b)
			{
				_data.Add(new Member(type: Member.Type.STRING, data: Encoding.UTF8.GetBytes(b)));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add<T>(T obj) where T : IBitConvertable
			{
				_data.Add(new Member(type: Member.Type.CUSTOM, data: obj.GetBytes()));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}

			public Builder Add<T>(T[] objs) where T : IBitConvertable
			{
				var bs = new List<byte[]>(objs.Length);
				bs.AddRange(objs.Select(obj => obj.GetBytes()));

				var arr = new byte[bs.Count, bs[0].Length];
				for (var i = 0; i < bs.Count; i++)
				for (var j = 0; j < bs[i].Length; j++)
					arr[i, j] = bs[i][j];

				_data.Add(new Member(type: Member.Type.ARRAY,
				                     data: new Member.Array(type: Member.Type.CUSTOM,
				                                            data: arr).GetBytes()));
				_data[1] = new Member(type: Member.Type.UNSIGNED_LONG, data: BitConverter.GetBytes(_count++));

				return this;
			}
		}
	}
}