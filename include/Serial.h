#pragma once

#include <thread>
#include <array>
#include <future>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace serial {

	class Serial
	{

	public:
		typedef boost::asio::serial_port_base::flow_control::type flowControlType;
		typedef boost::asio::serial_port_base::parity::type parityType;
		typedef boost::asio::serial_port_base::stop_bits::type stopBitsType;
		typedef boost::system::errc::errc_t errorCode;

	public:
		Serial();
		~Serial();

		void open(std::string,
			unsigned int = 115200,
			flowControlType = flowControlType::none,
			unsigned int = 8,
			parityType = parityType::none,
			stopBitsType = stopBitsType::one);

		bool isOpen() const;
		void close();
		std::future<std::vector<uint8_t>> receiveAsync(size_t const num_bytes);
		std::future<std::vector<uint8_t>> receiveAsync(size_t const num_bytes, unsigned int timeout);

		void transmit(std::vector<uint8_t> const& data);
		std::size_t transmitAsync(const std::vector<uint8_t>& v);




	private:

		boost::asio::io_context io_context;
		boost::asio::serial_port serial_port;
		boost::asio::io_context::work serial_work;

		void asyncRead();
		void asyncReadHandler(boost::system::error_code const& error, size_t bytes_transferred);
		int16_t readByte();
		std::vector<uint8_t> readBuffer(size_t len);
		std::vector<uint8_t> readBufferTimeout(size_t len);


		std::array<uint8_t, 256> buf;


		std::vector<uint8_t> usableReadBuffer;
		std::size_t readBufferSize = 256;
		std::unique_ptr<std::thread> asyncReadThread;
		mutable std::mutex readBufferMtx; 


		mutable std::mutex errMtx;
		int error_value{};
		void setError(const int error_value);
		int getError() const;

		unsigned int timeoutVal = 60000;



		mutable std::mutex writeMtx;    
		std::condition_variable writeCv; 
		bool writeLocked = false;        
		void asyncWriteHandler(const boost::system::error_code& error, std::size_t bytes_transferred);


	};


	Serial::Serial() : io_context(), serial_port(io_context), serial_work(io_context), asyncReadThread(nullptr), buf{} {};


	Serial::~Serial() {
		if (serial_port.is_open())
			close();
	}

	void Serial::open(std::string dev_node, unsigned int baud, flowControlType flowControl, unsigned int characterSize, parityType parity, stopBitsType stopBits)
	{

		serial_port.open(dev_node);
		if (!serial_port.is_open())
			return;

		serial_port.set_option(boost::asio::serial_port_base::baud_rate(baud));
		serial_port.set_option(boost::asio::serial_port_base::flow_control(flowControl));
		serial_port.set_option(boost::asio::serial_port_base::character_size(characterSize));
		serial_port.set_option(boost::asio::serial_port_base::parity(parity));
		serial_port.set_option(boost::asio::serial_port_base::stop_bits(stopBits));

		asyncReadThread.reset(new std::thread([this] { io_context.run(); }));

		asyncRead();

	}


	bool Serial::isOpen() const
	{
		return serial_port.is_open();
	}


	void Serial::close()
	{
		if (!serial_port.is_open())
			return;

		serial_port.cancel();

		io_context.stop();
		asyncReadThread->join();

		serial_port.close();
	}


	void Serial::asyncRead() {

		serial_port.async_read_some(
			boost::asio::buffer(buf, buf.size()),
			[this](const boost::system::error_code& error, std::size_t bytes_transferred) {
				asyncReadHandler(error, bytes_transferred);
			});
	}


	void Serial::asyncReadHandler(const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		if (error)
			setError(error.value());


		std::unique_lock<std::mutex> lk(readBufferMtx);
		for (auto i = 0; i < bytes_transferred; i++)
		{
			usableReadBuffer.push_back(buf[i]);
		}

		if (usableReadBuffer.size() > readBufferSize)
		{

			unsigned int overflow = usableReadBuffer.size() - readBufferSize;
			usableReadBuffer.erase(usableReadBuffer.begin(), usableReadBuffer.begin() + overflow);
		}
		lk.unlock();

		asyncRead();

	}

	void Serial::asyncWriteHandler(const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		if (error)
			setError(error.value());

		std::unique_lock<std::mutex> lk(writeMtx);
		writeLocked = false;
		lk.unlock();
		writeCv.notify_one();
	}


	int16_t Serial::readByte()
	{
		std::unique_lock<std::mutex> lk(readBufferMtx);

		if (!usableReadBuffer.size())
			return -1;

		int res = usableReadBuffer[0];
		usableReadBuffer.erase(usableReadBuffer.begin());
		return res;
	}


	std::vector<uint8_t> Serial::readBuffer(size_t len)
	{
		std::vector<uint8_t> res;

		while (res.size() < len)
		{
			const int16_t b = readByte();
			if (b > -1)
			{
				res.push_back((uint8_t)b);
			}
		}
		return res;
	}


	std::vector<uint8_t> Serial::readBufferTimeout(size_t len)
	{
		auto start = std::chrono::system_clock::now();
		bool timeout = false;
		std::vector<uint8_t> res;

		while (res.size() < len && !timeout)
		{
			auto now = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
			if (elapsed.count() >= timeoutVal)
			{
				timeout = true;
			}
			else
			{
				const int16_t b = readByte();
				if (b > -1)
				{
					res.push_back((uint8_t)b);
				}
			}
		}
		return res;
	}



	std::future<std::vector<uint8_t>> Serial::receiveAsync(size_t const num_bytes)
	{
		return std::async(std::launch::deferred, boost::bind(&Serial::readBuffer, this, num_bytes));
	}

	std::future<std::vector<uint8_t>> Serial::receiveAsync(size_t const num_bytes, unsigned int timeout)
	{
		timeoutVal = timeout;
		return std::async(std::launch::deferred, boost::bind(&Serial::readBufferTimeout, this, num_bytes));
	}


	void Serial::transmit(std::vector<uint8_t> const& data)
	{
		boost::asio::write(serial_port, boost::asio::buffer(data));
	}


	std::size_t Serial::transmitAsync(const std::vector<uint8_t>& v)
	{
		std::unique_lock<std::mutex> lk(writeMtx);
		if (writeLocked)
			writeCv.wait(lk);

		writeLocked = true;
		lk.unlock();

		boost::asio::async_write(
			serial_port,
			boost::asio::buffer(v, v.size()),
			[this](const boost::system::error_code& error, std::size_t bytes_transferred) {
				asyncWriteHandler(error, bytes_transferred);
			});

		return v.size();
	}


	void Serial::setError(const int error)
	{
		std::unique_lock<std::mutex> elk(errMtx);
		error_value = error;

	}

	int Serial::getError()  const
	{
		std::unique_lock<std::mutex> lk(errMtx);
		return error_value;
	}

}


