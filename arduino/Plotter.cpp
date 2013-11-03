#include "Plotter.h"

namespace {
	// ratio of the scale of the HPGL language to the stepper motor microstep
	static const float SCALE = 0.75f;

	static const float ACCURACY = 0.25f;
}

Plotter::Plotter(unsigned char xPins[2], unsigned char yPins[2], unsigned char penPins[4]) :
	m_x(xPins[0], xPins[1], 500), m_y(yPins[0], yPins[1], 500), m_pen(penPins, 65),
	m_penState(false) {
}

void Plotter::penUp() {
	if(m_penState) {
		m_pen.move(-16);
		m_penState = false;
	}
}

void Plotter::penDown() {
	if(!m_penState) {
		m_pen.move(16);
		m_penState = true;
	}
}

void Plotter::penIncrement() {
	m_pen.move(1);
}

void Plotter::penDecrement() {
	m_pen.move(-1);
}

void Plotter::moveTo(float x, float y) {
	// limit the maximal reach
	if(x > 1500.0f)
		x = 1500.0f;
	if(y > 1500.0f)
		y = 1500.0f;
	if(x < 0.0f)
		x = 0.0f;
	if(y < 0.0f)
		y = 0.0f;

	x /= SCALE;
	y /= SCALE;

	const float xDiff = m_x.state() < x ? ACCURACY : -ACCURACY;
	const float yDiff = m_y.state() < y ? ACCURACY : -ACCURACY;

	const float a[2] = {m_x.state(), m_y.state()};
	float n[2] = {x - m_x.state(), y - m_y.state()};
	const float nLen = sqrt(n[0]*n[0] + n[1]*n[1]);

	if(nLen == 0.0f)
		return;

	n[0] /= nLen;
	n[1] /= nLen;

	while(1) {
		float p1[2] = {m_x.state() + xDiff, m_y.state()};
		float p2[2] = {m_x.state(), m_y.state() + yDiff};

		const float t1 = ((a[0]-p1[0])*n[0] + (a[1]-p1[1])*n[1]);
		const float t2 = ((a[0]-p2[0])*n[0] + (a[1]-p2[1])*n[1]);

		const float v1[2] = {(a[0]-p1[0]) - t1*n[0], (a[1]-p1[1]) - t1*n[1]};
		const float v2[2] = {(a[0]-p2[0]) - t2*n[0], (a[1]-p2[1]) - t2*n[1]};

		const float d1 = v1[0]*v1[0] + v1[1]*v1[1];
		const float d2 = v2[0]*v2[0] + v2[1]*v2[1];

		float* p;
		if(d1 < d2)
			p = p1;
		else
			p = p2;

		if(((xDiff < 0) && (p[0] < x)) || ((xDiff > 0) && (p[0] > x)))
			break;

		if(((yDiff < 0) && (p[1] < y)) || ((yDiff > 0) && (p[1] > y)))
			break;

		m_x.moveTo(p[0]);
		m_y.moveTo(p[1]);
	}

	m_x.moveTo(x);
	m_y.moveTo(y);
}

const float Plotter::x() const {
	return m_x.state() * SCALE;
}

const float Plotter::y() const {
	return m_y.state() * SCALE;
}
