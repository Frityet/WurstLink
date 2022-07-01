namespace WurstLink.TestServer
{
	public interface IBuilder<out T>
	{
		public T Build();
	}
}