#include <Arduino.h>

class HBridgeMotor {
	public:
		HBridgeMotor(unsigned char motorPins[4], unsigned motorDelay = 5);

		void moveTo(const float& value);
		void move(const float& diff);
		const float& state() const;

	protected:
	private:
		HBridgeMotor(const HBridgeMotor& m);
		const HBridgeMotor& operator = (const HBridgeMotor& m);

		unsigned char m_motorPins[4];
		unsigned m_motorDelay;
		float m_motorState;
};
