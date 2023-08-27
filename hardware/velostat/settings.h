// Data types
struct Message {
  uint8_t preamble;
  uint8_t from;
  uint8_t to;
  uint16_t val;
} __attribute__((__packed__));

struct Pinout {
  uint8_t vPlus;
  uint8_t vMinus;
  uint8_t sPlus;
  uint8_t sMinus;
};

struct Mode {
  bool working;
  bool sdPresent;
};

// Pins
const uint8_t preamble = 0b101010;
const int PINMAP[24] = {0, 17, 19, 21, 23, 25, 15, 14, 13, 12, 11, 10, 9, 24, 22, 20, 18, 16, 6, 5, 4, 3, 2, 1};

const breakoutPin SELECT[5] = {PWM4, PWM3, PWM2, PWM1, PWM0}; //A0, A1, A2, A3, A4 Respectively

const breakoutPin MUX0[3] = {PWM5, PWM6, PWM7};
const breakoutPin MUX1[3] = {PWM8, PWM9, GPIO_0};
const breakoutPin MUX2[3] = {GPIO_1, GPIO_2, GPIO_3};
const breakoutPin MUX3[3] = {GPIO_4, GPIO_5, GPIO_6};

const breakoutPin ADC_PIN = ANALOG_A1;
const int ANALOG_RESOLUTION = 16;