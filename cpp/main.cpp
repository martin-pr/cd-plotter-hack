#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <SerialStream.h>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main(void) {
	// open the serial port
	LibSerial::SerialStream serial;
	serial.Open("/dev/ttyUSB0", std::ios_base::in | std::ios_base::out);

	serial.SetBaudRate(LibSerial::SerialStreamBuf::BAUD_9600);
	serial.SetCharSize(LibSerial::SerialStreamBuf::CHAR_SIZE_8);
	serial.SetNumOfStopBits(1);
	serial.SetParity(LibSerial::SerialStreamBuf::PARITY_NONE);
	serial.SetFlowControl(LibSerial::SerialStreamBuf::FLOW_CONTROL_NONE);

	sleep(2);

	if(!serial.good()) {
		cerr << "[EE] Error opening port." << endl;
		return 1;
	}

	while(cin.good()) {
		// get rid of any white characters
		while(((cin.peek() == ' ') || (cin.peek() == '\t') || (cin.peek() == '\n') || (cin.peek() == '\r')) && (cin.good()))
			cin.get();

		if(cin.good()) {
			std::stringstream command;

			// first read the command
			char commandArr[3] = {'\0', '\0', '\0'};
			cin.read(commandArr, 2);
			command << commandArr;

			// arguments (ints only)
			std::vector<int> arguments;
			while((cin.peek() != ';') && (cin.good())) {
				int tmp;
				cin >> tmp;
				arguments.push_back(tmp);

				if(cin.peek() == ',')
					cin.get();
			}

			// final character
			if(cin.get() != ';') {
				cerr << "[ERR] Cannot find command's terminating character." << endl;
				return 1;
			}
			cin.get();

			// assemble the command back
			for(unsigned a=0;a<arguments.size();a++)
				if(a == 0)
					command << " " << arguments[a];
				else
					command << "," << arguments[a];
			command << ";";

			serial << command.str() << endl << std::flush;
			cout << "<< " << command.str() << endl;

			// while(1) {
				std::string line;
				std::getline(serial, line);
				cout << ">> " << line << endl;
			// }
		}
	}

	serial.Close();

	return 0;
}