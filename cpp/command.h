#pragma once

#include <string>
#include <vector>
#include <iostream>

class command {
	public:
		command(const std::string& c);
		const std::string& type() const;

		void addArg(int a);
		const unsigned size() const;
		int& operator[](unsigned idnex);
		const int& operator[](unsigned idnex) const;

	protected:
	private:
		std::string m_command;
		std::vector<int> m_arguments;
};

std::ostream& operator << (std::ostream& out, const command& c);