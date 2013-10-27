#include "Motor.h"

Motor::Motor(unsigned char motorPins[4], unsigned char motorDelay) : 
	m_motorPins({motorPins[0],motorPins[1],motorPins[2],motorPins[3]}), m_motorDelay(motorDelay), m_motorState(0.0f) {
}

void Motor::moveTo(const float& value) {
	const float diff = (value > m_motorState) ? 1.0f : -1.0f;
	
	while(round(m_motorState) != round(value)) {
		// update the state
		m_motorState += diff;

		// figure out the motor values for given state
		const unsigned val = round(m_motorState + 640);
		const unsigned state = (1 << (val % 8 / 2)) | (1 << ((val+1) % 8 / 2));
		
		// write the motor values
		for(unsigned a=0;a<4;a++)
			digitalWrite(m_motorPins[a], state & (1 << a));
		
		delay(m_motorDelay);
	}

	m_motorState = value;
	
	// switch off the motor (no reason for it to be eating the power if its not moving)
	for(unsigned a=0;a<4;a++)
		digitalWrite(m_motorPins[a], LOW);
}

void Motor::move(const float& diff) {
	moveTo(m_motorState+diff);
}

const float& Motor::state() const {
	return m_motorState;
}
