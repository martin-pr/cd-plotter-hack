#pragma once

#include "HBridgeMotor.h"
#include "SubstepMotor.h"

class Plotter {
	public:
		Plotter(unsigned char xPins[2], unsigned char yPins[2], unsigned char penPins[4]);

		void penUp();
		void penDown();
		void penIncrement();
		void penDecrement();

		void moveTo(float x, float y);
		const float x() const;
		const float y() const;

	protected:
	private:
		SubstepMotor m_x, m_y;
		HBridgeMotor m_pen;

		bool m_penState;
};
