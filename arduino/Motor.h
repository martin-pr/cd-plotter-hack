#include <Arduino.h>

class Motor {
  public:
    Motor(unsigned char motorPins[4], unsigned char motorDelay = 5);
    
    void moveTo(const float& value);
    void move(const float& diff);
    const float& state() const;
    
  protected:
  private:
    Motor(const Motor& m);
    const Motor& operator = (const Motor& m);
   
    const unsigned char m_motorPins[4];
    const unsigned char m_motorDelay;
    float m_motorState;
};