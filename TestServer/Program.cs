using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace WurstLink.TestServer
{
	public static class Program
	{
		public static void Main()
		{
			byte[] obj = new Packet.Builder().Build();
			File.WriteAllBytes(path: "test.bin", bytes: obj);

			byte[] data = File.ReadAllBytes("test.bin");
		}

		public readonly struct TestStruct : Packet.ISerialisableCustomType
		{
			public Packet.CustomTypeDescriptor Members { get; }

			public int Integer { get; }
			public float Float { get; }
			public string Name { get; }

			public TestStruct(int i, string name)
			{
				Integer = i;
				Float = i;
				Name = name;
				Members = new Packet.CustomTypeDescriptor(new[]
				                                          {
					                                          Packet.Member.Type.SIGNED_INTEGER,
					                                          Packet.Member.Type.FLOAT,
					                                          Packet.Member.Type.STRING
				                                          });
			}

			public byte[] GetBytes()
			{
				var arr = new List<byte>();

				arr.AddRange(Members.GetBytes());
				arr.AddRange(BitConverter.GetBytes(Integer));
				arr.AddRange(BitConverter.GetBytes(Float));
				arr.AddRange(Encoding.UTF8.GetBytes(Name));

				return arr.ToArray();
			}
		}
	}
}