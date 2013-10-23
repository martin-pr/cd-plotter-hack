#include "Plotter.h"

Plotter::Plotter(unsigned char xPins[4], unsigned char yPins[4], unsigned char penPins[4]) :
	m_x(xPins, 10), m_y(yPins, 10), m_pen(penPins, 30),
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
	// the plotter's step is 0.075mm; HPGL's step is 0.025 -> divide by 3
	x /= 3.0f;
	y /= 3.0f;

	if(x > 500.0f)
		x = 500.0f;
	if(y > 500.0f)
		y = 500.0f;
	if(x < 0.0f)
		x = 0.0f;
	if(y < 0.0f)
		y = 0.0f;

	const float xDiff = m_x.state() < x ? 1.0f : -1.0f;
	const float yDiff = m_y.state() < y ? 1.0f : -1.0f;

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

const float& Plotter::x() const {
	// the plotter's step is 0.075mm; HPGL's step is 0.025 -> multiply by 3
	return m_x.state() * 3.0f;
}

const float& Plotter::y() const {
	// the plotter's step is 0.075mm; HPGL's step is 0.025 -> multiply by 3
	return m_y.state() * 3.0f;
}
