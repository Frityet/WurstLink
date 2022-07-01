using System;
using System.Net;
using System.Net.Sockets;

namespace WurstLink.TestServer
{
	public class Server : IDisposable
	{
		private readonly TcpListener _listener;

		public Server(ushort port)
		{
			Port = port;
			_listener = new TcpListener(localaddr: IPAddress.Any, port: port);
		}

		public ushort Port { get; }
		public Client? ConnectedClient { get; private set; }

		public void Dispose()
		{
			_listener.Stop();
			ConnectedClient?.Disconnect();
		}

		public void Start(Action<Client> afterConnect)
		{
			_listener.Start();
			_listener.BeginAcceptTcpClient(callback: res =>
			                                         {
				                                         ConnectedClient =
					                                         new Client(_listener.EndAcceptTcpClient(res));

				                                         afterConnect(ConnectedClient);
			                                         }, state: null);
		}
	}
}