
#include <iostream>
#include "Serial.h"

int main()
{
    std::cout << "Serial Port Library!\n";

	serial::Serial s;
	std::cout << s.isOpen() << std::endl;
	s.open("COM3");
	std::cout << s.isOpen() << std::endl;

	auto start = std::chrono::system_clock::now();

	auto future = s.receiveAsync(5, 5000);
	auto const v = future.get();

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	if (v.size() < 1)
		std::cout << "TIMEOUT" << "elapsed time: " << elapsed_seconds.count() <<  std::endl;
	else {
		for (auto val : v) {
			std::cout << std::hex << val << " ";
		}
		std::cout << std::endl;
		s.transmit(v);
	}

	s.close();
}
