using System;
using System.IO;
using System.Net.Sockets;
using UnityEngine;
using WurstLink.Common;

namespace WurstLink.Managers
{
    public class NetworkManager : Manager<NetworkManager>
    {
        public const int DATA_BUFFER_SIZE = 4096;
        
        public IPv4Address Address = new IPv4Address(127, 0, 0, 1, 7777);

        private TcpClient _client;
        private NetworkStream _stream;
        private byte[] _receiveBuffer;

        private void Start()
        {
            _client = new TcpClient()
            {
                ReceiveBufferSize   = DATA_BUFFER_SIZE,
                SendBufferSize      = DATA_BUFFER_SIZE
            };

            _receiveBuffer = new byte[DATA_BUFFER_SIZE];
        }

        public void Connect()
        {
            Debug.Log($"Connecting to {Address}");
            _client.BeginConnect(Address.ToIPAddress(), Address.Port, OnConnect, _client);
        }
        
        private void OnConnect(IAsyncResult res)
        { 
            _client.EndConnect(res);

            if (!_client.Connected)
            {
                Debug.LogError($"Could not connect to IP {Address}");
                return;
            }

            _stream = _client.GetStream();
            _stream.BeginRead(_receiveBuffer, 0, DATA_BUFFER_SIZE, OnReceive, null);
        }

        private void OnReceive(IAsyncResult res)
        {
            Debug.Log("Received packet!");
            try
            {
                int buflen = _stream.EndRead(res);
                if (buflen < 1)
                {
                    Debug.LogError("Buffer length is too small!");
                    return;
                }

                var buf = new byte[buflen];
                Array.Copy(_receiveBuffer, buf, buflen);

                _stream.BeginRead(_receiveBuffer, 0, DATA_BUFFER_SIZE, OnReceive, null);
            }
            catch (IOException e)
            {
                Debug.LogError($"Could not read from network stream!\nReason: {e}");
                return;
            }
        }
    }
}