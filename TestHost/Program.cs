using System;
using System.Collections;

namespace WurstLink.TestHost
{
    internal class Program
    {
        public static void Main(string[] args)
        {
            var server = new Server(7777);
            
            server.SendPacket(new Packet("Ping!"));

            Console.Read();
            
        }
    }
}