#pragma once

#include "Motor.h"

class Plotter {
	public:
		Plotter(unsigned char xPins[4], unsigned char yPins[4], unsigned char penPins[4]);

		void penUp();
		void penDown();
		void penIncrement();
		void penDecrement();

		void moveTo(float x, float y);
		const float& x() const;
		const float& y() const;

	protected:
	private:	
		bool m_penState;

		Motor m_pen, m_x, m_y;
};