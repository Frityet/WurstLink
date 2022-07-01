using System;
using System.Net.Sockets;

namespace WurstLink.TestServer
{
	public class Client
	{
		private readonly TcpClient _connection;

		private readonly int _dataBufferSize;

		private readonly NetworkStream _network;
		private readonly byte[] _receiveBuffer;
		private byte[] _sendBuffer;

		public Client(TcpClient tcp, int dataBufferSize = 8192)
		{
			_connection = tcp;

			_sendBuffer = new byte[dataBufferSize];
			_receiveBuffer = new byte[dataBufferSize];
			_connection.SendBufferSize = dataBufferSize;
			_connection.ReceiveBufferSize = dataBufferSize;

			_dataBufferSize = dataBufferSize;

			_network = _connection.GetStream();
		}

		public void Read(Action<byte[]> afterRead, Action<Exception>? onError = null)
		{
			_network.BeginRead(
				_receiveBuffer,
				0,
				_dataBufferSize,
				res =>
				{
					byte[] data;
					try
					{
						data = ReadFromConnection(res);
					}
					catch (Exception e)
					{
						onError?.Invoke(e);
						return;
					}

					afterRead(data);
				},
				null
			);
		}

		private byte[] ReadFromConnection(IAsyncResult asyncResult)
		{
			int len = _network.EndRead(asyncResult);
			if (len <= 0)
				throw new Exception("Received 0 bytes from client");

			var recv = new byte[len];
			Array.Copy(_receiveBuffer, recv, len);

			return recv;
		}

		public void Disconnect()
		{
			_network.Dispose();
			_connection.Close();
		}
	}
}