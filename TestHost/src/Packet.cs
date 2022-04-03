using System;
using System.Reflection;
using System.Text;

namespace WurstLink.TestHost
{
    public class Packet
    {
        public int ReadPosition { get; set; }
        public int Length       => _memory.Count;
        public int UnreadLength => Length - ReadPosition;
        public byte[] Bytes
        {
            get
            {
                WriteLength();
                return _readbuf = _memory.Bytes;
            }
        }

        private Memory _memory;
        private byte[] _readbuf;

        public Packet(int bytecount = 4096)
        {
            _memory = new Memory(bytecount);
            _readbuf = new byte[bytecount];
            ReadPosition = 0;
        }

        public Packet(byte[] bytes)
        {
            _memory = new Memory(bytes.Length);
            ReadPosition = 0;
            _readbuf = null;
            SetBytes(bytes);
        }
        
        public Packet(string msg)
        {
            var bytes = Encoding.UTF8.GetBytes(msg);
            _memory = new Memory(bytes.Length);
            ReadPosition = 0;
            _readbuf = null;
            SetBytes(bytes);
        }

        public void SetBytes(byte[] bytes)
        {
            _memory.Write(bytes);
            _readbuf = _memory.Bytes;
        }

        #region Reading

        public byte ReadByte(bool moveReadPos = true)
        {
            if (_memory.Count > ReadPosition)
            {
                byte val = _readbuf[ReadPosition];
                if (moveReadPos)
                    ReadPosition++;

                return val;
            }
            else throw new Exception("Could not read byte!");
        }

        public byte[] ReadBytes(int count, bool moveReadPos = true)
        {
            if (_memory.Count > ReadPosition)
            {
                var bytes = new byte[count];
                for (int i = ReadPosition, j = 0; i < count; i++)
                {
                    bytes[j] = _memory[i];
                    if (moveReadPos)
                        ReadPosition++;
                }

                return bytes;
            }
            else throw new Exception($"Could not read {count} bytes from buffer!");
        }

        public short ReadShort(bool moveReadPos = true)     => BitConverter.ToInt16(ReadBytes(1 << 1, moveReadPos), 0);
        public int ReadInt(bool moveReadPos = true)         => BitConverter.ToInt32(ReadBytes(1 << 2, moveReadPos), 0);
        public long ReadLong(bool moveReadPos = true)       => BitConverter.ToInt64(ReadBytes(1 << 3, moveReadPos), 0);

        public float ReadFloat(bool moveReadPos = true)     => BitConverter.ToSingle(ReadBytes(1 << 2, moveReadPos), 0);
        public double ReadDouble(bool moveReadPos = true)   => BitConverter.ToDouble(ReadBytes(1 << 3, moveReadPos), 0);

        public bool ReadBool(bool moveReadPos = true)       => BitConverter.ToBoolean(ReadBytes(1 << 0, moveReadPos), 0);

        public string ReadString(bool moveReadPos = true)   => Encoding.UTF8.GetString(_readbuf, ReadPosition, ReadInt());
        
        #endregion
        
        #region Writing
        public void Write(byte b)       => _memory.Write(b);
        public void Write(byte[] bytes) => _memory.Write(bytes);
        public void Write(bool b)       => _memory.Write(BitConverter.GetBytes(b));
        public void Write(short w)      => _memory.Write(BitConverter.GetBytes(w));
        public void Write(int dw)       => _memory.Write(BitConverter.GetBytes(dw));
        public void Write(long qw)      => _memory.Write(BitConverter.GetBytes(qw));
        
        public void Write(float dw)     => _memory.Write(BitConverter.GetBytes(dw));
        public void Write(double qw)    => _memory.Write(BitConverter.GetBytes(qw));
        
        public void Write(string str)
        {
            Write(str.Length);
            _memory.Write(Encoding.UTF8.GetBytes(str));
        }
        #endregion

        private void WriteLength() => _memory.Write(BitConverter.GetBytes(_memory.Count), 0);

        ~Packet()
        {
            _memory.Dispose();
        }
    }
}