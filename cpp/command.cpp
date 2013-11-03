#include "command.h"

#include <cassert>

using std::cout;
using std::endl;

command::command(const std::string& c) : m_command(c) {
}

const std::string& command::type() const {
	return m_command;
}

void command::addArg(int a) {
	m_arguments.push_back(a);
}
		
const unsigned command::size() const {
	return m_arguments.size();
}

int& command::operator[](unsigned index) {
	assert(index < m_arguments.size());
	return m_arguments[index];
}

const int& command::operator[](unsigned index) const {
	assert(index < m_arguments.size());
	return m_arguments[index];
}

std::ostream& operator << (std::ostream& out, const command& c) {
	out << c.type();
	for(unsigned a=0;a<c.size();a++) {
		if(a == 0)
			out << " ";
		else
			out << ",";
		out << c[a];
	}
	out << ";";

	return out;
}