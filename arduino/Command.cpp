#include "Command.h"

#include <assert.h>

Command::Command() : m_command({0,0}), m_size(0) {
}

const bool Command::isValid() const {
	return m_command[0] != 0;
}

const bool Command::operator == (char* cmd) const {
	return (cmd[0] == m_command[0]) && (cmd[1] == m_command[1]);
}

const unsigned Command::size() const {
	return m_size;
}

const int Command::operator[](unsigned char index) const {
	assert(index < m_size);
	return m_arguments[index];
}

////////////

namespace {
	char getc(HardwareSerial& s) {
		while(!s.available())
			;
		return s.read();
	}

	char peekc(HardwareSerial& s) {
		while(!s.available())
			;
		return s.peek();
	}

	void eatWhitespaces(HardwareSerial& s) {
		const char c = peekc(s);
		if((c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'))
			s.read();
	}
}

HardwareSerial& operator >> (HardwareSerial& s, Command& c) {
	// reset the command
	c.m_command[0] = 0;
	c.m_command[1] = 0;
	c.m_size = 0;

	// read the command characters
	eatWhitespaces(s);
	c.m_command[0] = getc(s);
	c.m_command[1] = getc(s);

	// read the command arguments
	eatWhitespaces(s);

	char val = 0;
	while(val != ';') {
		eatWhitespaces(s);

		val = peekc(s);

		if((val >= '0') && (val <= '9')) {
			c.m_arguments[c.m_size] = 0;
			c.m_size++;

			while((peekc(s) >= '0') && (peekc(s) <= '9'))
				c.m_arguments[c.m_size-1] = c.m_arguments[c.m_size-1] * 10 + getc(s)-'0';
		}
		else if(val == ',')
			getc(s);
		else if(val == ';')
			getc(s);
	}

	return s;
}

HardwareSerial& operator << (HardwareSerial& s, const Command& c) {
	if(!c.isValid())
		s.print("(invalid command)");
	else {
		s.print(c.m_command[0]);
		s.print(c.m_command[1]);

		if(c.m_size > 0)
			s.print(' ');

		for(unsigned char a=0;a<c.m_size;a++) {
			s.print(c.m_arguments[a]);
			if(a < c.m_size-1)
				s.print(',');
		}

		s.print(';');
	}

	return s;
}