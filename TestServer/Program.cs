using System;

namespace WurstLink.TestServer
{
	public static class Program
	{
		public static void Main()
		{
			using var server = new Server(7777);
			server.Start(player =>
			{
				Console.WriteLine("Client connected!");
				player.Read(
					data =>
					{
						foreach (byte b in data) Console.Write($"{b} ");
					}
				);
			});
			Console.WriteLine("Started server");

			byte[] packet = new Packet.Builder()
				.Add((ByteConvertable.Integer) 6)
				.Add((ByteConvertable.Short) 4)
				.Add((ByteConvertable.String) "Hello!")
				.Add(new ByteConvertable.UnsignedInteger(3341413133))
				.Build();

			Console.ReadKey();
		}
	}
}