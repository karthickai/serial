
#include <iostream>
#include "../include/Serial.h"

int main()
{
    std::cout << "Serial Port Virtual Emulator!\n";
	std::cout << "It will echos msg serially\n\n";
	std::cout << "Enter the COM PORT :  ";
	std::string comport{};
	std::cin >> comport;
	std::cout << "\nEnter the baud rate : ";
	unsigned int baud{};
	std::cin >> baud;
	std::cout << std::endl;


	serial::Serial s;
	s.open(comport, baud);

	if (s.isOpen()) {
		std::cout << "COM PORT OPENED\n";

		while (true) {
			std::future<std::vector<uint8_t>> future = s.receiveAsync(1);

			std::vector<uint8_t> const received_data = future.get();

			s.transmitAsync(received_data);
		}
	}

	std::cout << "COM PORT OPENED\n";


	s.close();
}
