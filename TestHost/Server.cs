using System;
using System.IO;
using System.Net;
using System.Net.Sockets;

namespace WurstLink.TestHost
{
    public class Server
    {
        public const int DATA_BUFFER_SIZE = 4096;
        
        public UInt16 Port { get; }

        private TcpListener _listener;
        private TcpClient _client;
        private byte[] _receiveBuffer;
        private NetworkStream _stream;

        public Server(UInt16 port)
        {
            Port = port;

            _listener = new TcpListener(IPAddress.Any, Port);
            _listener.Start();
            _listener.BeginAcceptTcpClient(OnConnect, null);
            _receiveBuffer = new byte[DATA_BUFFER_SIZE];
            
            Console.WriteLine($"Started server on port {port}!");
        }

        public void SendPacket(Packet packet)
        {
            try
            {
                _stream.BeginWrite(packet.Bytes, 0, packet.Length, null, null);
            }
            catch (Exception e)
            {
                Console.WriteLine($"Could not send packet to {Port}!");
                throw;
            }
        }
        
        private void OnConnect(IAsyncResult res)
        {
            Console.WriteLine("Got client connection!");
            _client = _listener.EndAcceptTcpClient(res);
            _stream = _client.GetStream();
            _stream.BeginRead(_receiveBuffer, 0, DATA_BUFFER_SIZE, OnReceive, null);
        }

        private void OnReceive(IAsyncResult res)
        {
            Console.WriteLine("Received packet!");
            try
            {
                int buflen = _stream.EndRead(res);
                if (buflen < 1)
                {
                    return;
                }

                var buf = new byte[buflen];
                Array.Copy(_receiveBuffer, buf, buflen);

                _stream.BeginRead(_receiveBuffer, 0, DATA_BUFFER_SIZE, OnReceive, null);
            }
            catch (IOException e)
            {
                Console.WriteLine($"Could not read from network stream!\nReason: {e}");
            }
        }
    }
}