#include <Arduino.h>

class SubstepMotor {
	public:
		SubstepMotor(unsigned char stepPin, unsigned char dirPin, unsigned motorDelay = 5, bool reverse = false);

		void moveTo(const float& value);
		void move(const float& diff);

		const float& state() const;
		const unsigned& delay() const;
		void setDelay(const unsigned& d);

	protected:
	private:
		SubstepMotor(const SubstepMotor& m);
		const SubstepMotor& operator = (const SubstepMotor& m);

		unsigned char m_stepPin, m_dirPin;
		unsigned m_motorDelay;
		bool m_reverse;
		float m_motorState;
};
