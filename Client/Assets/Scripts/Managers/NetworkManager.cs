using System;
using System.Net;
using System.Net.Sockets;
using JetBrains.Annotations;
using TMPro;
using UnityEngine;

namespace WurstLink.Managers
{
    public class NetworkManager : Manager<NetworkManager>
    {
        public UInt16 Port;
        public uint DataBufferSize = 8192;
        public byte[] ReceiveBuffer, SendBuffer;

        [NotNull] private TcpClient _connection;
        [CanBeNull] private NetworkStream _network;

        private void Start()
        {
            ReceiveBuffer   = new byte[DataBufferSize];
            SendBuffer      = new byte[DataBufferSize];

            _connection = new TcpClient()
            {
                ReceiveBufferSize   = (int)DataBufferSize,
                SendBufferSize      = (int)DataBufferSize
            };
            
        }

        public void Connect(TMP_Text ipAddress)
        {
            Debug.Log(ipAddress.text);
            IPAddress addr = IPAddress.Parse("127.0.0.1");
            Debug.Log($"Connecting to {addr}:{Port}");
            _connection.BeginConnect (
                address: addr,
                port: Port,
                requestCallback: (res) =>
                {
                    _connection.EndConnect(res);
            
                    if (!_connection.Connected)
                        return;

                    _network = _connection.GetStream();
                    _network.BeginRead (
                        buffer: ReceiveBuffer,
                        offset: 0,
                        size: (int)DataBufferSize,
                        callback: OnReceive,
                        state: null
                    );
                },
                state: _connection
            );
        }

        private void OnReceive(IAsyncResult res)
        {
            Debug.Log("Receiving data...");
        }

        private void OnDestroy()
        {
            _network?.Close();
        }
    }
}
