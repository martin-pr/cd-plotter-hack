#pragma once

#include "HardwareSerial.h"

#define MAX_COMMAND_SIZE 8

class Command {
	public:
		Command();

		// true if a valid command
		const bool isValid() const;

		// comparison with the command string
		const bool operator == (char* cmd) const;

		// returns number of parameters
		const unsigned size() const;
		// returns a parameter
		const int operator[](unsigned char index) const;

	protected:
	private:
		char m_command[2];
		int m_arguments[MAX_COMMAND_SIZE];
		unsigned char m_size;

		// noncopyable
		Command(const Command& c);
		const Command& operator = (const Command& c);

	friend HardwareSerial& operator >> (HardwareSerial& s, Command& c);
	friend HardwareSerial& operator << (HardwareSerial& s, const Command& c);
};

HardwareSerial& operator >> (HardwareSerial& s, Command& c);
HardwareSerial& operator << (HardwareSerial& s, const Command& c);