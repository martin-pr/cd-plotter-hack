#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

#include <SerialStream.h>

#include "command.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main(void) {
	// process all the commands and stick them into a vector
	std::vector<command> commands;

	while(cin.good()) {
		// get rid of any white characters
		while(((cin.peek() == ' ') || (cin.peek() == '\t') || (cin.peek() == '\n') || (cin.peek() == '\r')) && (cin.good()))
			cin.get();

		if(cin.good()) {
			// first read the command
			char commandArr[3] = {'\0', '\0', '\0'};
			cin.read(commandArr, 2);
			command c(commandArr);

			// arguments (ints only)
			while((cin.peek() != ';') && (cin.good())) {
				int tmp;
				cin >> tmp;
				c.addArg(tmp);

				if(cin.peek() == ',')
					cin.get();
			}

			// final character
			if(cin.get() != ';') {
				cerr << "[ERR] Cannot find command's terminating character." << endl;
				return 1;
			}
			cin.get();

			// exit on SP 0,0 (end of the file)
			if((c.type() == "SP") && (c.size() == 1) && (c[0] == 0))
				break;

			// and stick the command onto the commands string
			if((c.type() == "PA") || (c.type() == "PU") || (c.type() == "PD"))
				commands.push_back(c);
		}
	}

	// get the min and max values for PA commands
	int xMin = std::numeric_limits<int>::max(), yMin = std::numeric_limits<int>::max();
	int xMax = std::numeric_limits<int>::min(), yMax = std::numeric_limits<int>::min();

	for(std::vector<command>::const_iterator i = commands.begin(); i != commands.end(); i++)
		if(i->type() == "PA") {
			if(i->size() != 2) {
				cerr << "[ERR] Wrong number of arguments for a PA command - '" << *i << "'" << endl;
				return 1;
			}

			if((*i)[0] < xMin)
				xMin = (*i)[0];
			if((*i)[1] < yMin)
				yMin = (*i)[1];

			if((*i)[0] > xMax)
				xMax = (*i)[0];
			if((*i)[1] > yMax)
				yMax = (*i)[1];
		}

	// test that there are any PA commands
	if((xMax < xMin) || (yMax < yMin)) {
		cerr << "[ERR] No PA commands found in the file!" << endl;
		return 1;
	}

	// test the maximum size of the drawing
	if(xMax - xMin > 1500) {
		cerr << "[ERR] Maximum size in X exceeded - 1500 allowed, " << (xMax - xMin) << " found." << endl;
		return 1;
	}

	if(yMax - yMin > 1500) {
		cerr << "[ERR] Maximum size in Y exceeded - 1500 allowed, " << (yMax - yMin) << " found." << endl;
		return 1;
	}

	// convert all commands to make the origin at [0,0]
	for(std::vector<command>::iterator i = commands.begin(); i != commands.end(); i++)
		if(i->type() == "PA") {
			(*i)[0] -= xMin;
			(*i)[1] -= yMin;
		}

	// and add the final "go home"
	command end("PA");
	end.addArg(0);
	end.addArg(0);
	commands.push_back(end);


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


	// and, finally, send everything to the plotter
	for(std::vector<command>::iterator i = commands.begin(); i != commands.end(); i++) {
		serial << *i << endl << std::flush;
		cout << "<< " << *i << endl;

		// while(1) {
			std::string line;
			std::getline(serial, line);
			cout << ">> " << line << endl;
		// }

	}

	serial.Close();

	return 0;
}