#include "SubstepMotor.h"

SubstepMotor::SubstepMotor(unsigned char stepPin, unsigned char dirPin, unsigned motorDelay, bool reverse) :
	m_stepPin(stepPin), m_dirPin(dirPin), m_motorDelay(motorDelay), m_reverse(reverse), m_motorState(0.0f) {

}

void SubstepMotor::moveTo(const float& value) {
	// compute the differential for movement
	const float diff = (value > m_motorState) ? 1.0f : -1.0f;

	// set the direction pin
	digitalWrite(m_dirPin, (diff > 0) != m_reverse);
	delayMicroseconds(m_motorDelay / 2);

	while(round(m_motorState + 1000.0f) != round(value + 1000.0f)) {
		// update the state
		m_motorState += diff;

		// and move the motor
		digitalWrite(m_stepPin, HIGH);
		delayMicroseconds(m_motorDelay / 2);
		digitalWrite(m_stepPin, LOW);
		delayMicroseconds(m_motorDelay / 2);
	}

	m_motorState = value;
}

void SubstepMotor::move(const float& diff) {
	moveTo(m_motorState + diff);
}

const float& SubstepMotor::state() const {
	return m_motorState;
}

const unsigned& SubstepMotor::delay() const {
	return m_motorDelay;
}

void SubstepMotor::setDelay(const unsigned& d) {
	m_motorDelay = d;
}
