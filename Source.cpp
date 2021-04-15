#include <iostream>
#include <thread>

#include <boost/asio.hpp>


void write_data(boost::asio::ip::tcp::socket& socket)
{
	std::string data;

	while (std::getline(std::cin, data))
	{
		if (data == "END")
		{
			boost::asio::write(socket, boost::asio::buffer("User left the channel!"));
			break;
		}
		boost::asio::write(socket, boost::asio::buffer("Server: " + data + '!'));
	}
}

void read_data_until(boost::asio::ip::tcp::socket& socket)
{

	boost::asio::streambuf buffer;

	while (boost::asio::read_until(socket, buffer, '!'))
	{
		std::string message;

		std::istream input_stream(&buffer);

		std::getline(input_stream, message, '!');

		std::cout << message << std::endl;

		if (message == "User left the channel")
		{
			boost::asio::write(socket, boost::asio::buffer("User left the channel!"));
			break;
		}
	}
}

int main(int argc, char** argv)
{

	system("chcp 1251");

	system("pause");

	const std::size_t size = 30;

	auto port = 8001;

	bool flag = true;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

	boost::asio::io_service io_service;

	try
	{
		boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

		acceptor.bind(endpoint);

		acceptor.listen(size);

		boost::asio::ip::tcp::socket socket(io_service);

		acceptor.accept(socket);

		std::thread Thread(read_data_until, std::ref(socket));

		write_data(socket);

		Thread.join();
	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		system("pause");

		return e.code().value();
	}

	system("pause");

	return 0;
}