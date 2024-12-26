
#include <RtcDS1302.h>
#include <Wire.h>

#define I2C_ADDRESS 0x08  // I2C address for start command
//#define PIN_VALVE_IRRIGATION 7    // Pin for irrigation valve 7

#define VALVE_1_PIN 9
#define SPLIT_VALVE_1_PIN 11
#define VALVE_2_PIN 10
#define SPLIT_VALVE_2_PIN 12
#define FEEDER_VALVE_PIN 7


#define CMD_OPEN_VALUE_1() (setDigitalOutputPin(VALVE_1_PIN))
#define CMD_CLOSE_VALUE_1() (resetDigitalOutputPin(VALVE_1_PIN))
#define CMD_LEFT_SPLIT_VALUE_1() (setDigitalOutputPin(SPLIT_VALVE_1_PIN))
#define CMD_RIGHT_SPLIT_VALUE_1() (resetDigitalOutputPin(SPLIT_VALVE_1_PIN))
#define CMD_OPEN_VALUE_2() (setDigitalOutputPin(VALVE_2_PIN))
#define CMD_CLOSE_VALUE_2() (resetDigitalOutputPin(VALVE_2_PIN))
#define CMD_LEFT_SPLIT_VALUE_2() (setDigitalOutputPin(SPLIT_VALVE_2_PIN))
#define CMD_RIGHT_SPLIT_VALUE_2() (resetDigitalOutputPin(SPLIT_VALVE_2_PIN))
#define CMD_OPEN_FEEDER_VALVE() (setDigitalOutputPin(FEEDER_VALVE_PIN))
#define CMD_CLOSE_FEEDER_VALVE() (resetDigitalOutputPin(FEEDER_VALVE_PIN))

#define CMD_CHAR_OPEN_VALUE_1 'a'
#define CMD_CHAR_CLOSE_VALUE_1 'z'
#define CMD_CHAR_LEFT_SPLIT_VALUE_1 's'
#define CMD_CHAR_RIGHT_SPLIT_VALUE_1 'x'
#define CMD_CHAR_OPEN_VALUE_2 'd'
#define CMD_CHAR_CLOSE_VALUE_2 'c'
#define CMD_CHAR_LEFT_SPLIT_VALUE_2 'f'
#define CMD_CHAR_RIGHT_SPLIT_VALUE_2 'v'
#define CMD_CHAR_OPEN_FEEDER_VALVE 'g'
#define CMD_CHAR_CLOSE_FEEDER_VALVE 'b'
#define CMD_PRINT_COMMANDS_INFO 'i'

#define FEEDER_TIME 30000  // 30 second


void initDigitalOutputPin(int pin) {
  pinMode(pin, OUTPUT);
}
void setDigitalOutputPin(int pin) {
  digitalWrite(pin, HIGH);
}
void resetDigitalOutputPin(int pin) {
  digitalWrite(pin, LOW);
}

void setup() {
  Serial.begin(9600);

  // Initialize the pins as OUTPUT
  initDigitalOutputPin(VALVE_1_PIN);
  initDigitalOutputPin(SPLIT_VALVE_1_PIN);
  initDigitalOutputPin(VALVE_2_PIN);
  initDigitalOutputPin(SPLIT_VALVE_2_PIN);
  initDigitalOutputPin(FEEDER_VALVE_PIN);

  // setDigitalOutputPin(SPLIT_VALVE_2_PIN);
  // setDigitalOutputPin(SPLIT_VALVE_1_PIN);
  // testPin(VALVE_1_PIN);
  // testPin(VALVE_2_PIN);

  // testPin(FEEDER_VALVE_PIN);
}


void loop() {

  int command = Serial.read();
  if (command == -1)  // return if no command or new line
  {
    // so stuff when no command given
  }
  if (command != -1 && command != 10)
    Serial.println("Recieved command: " + String(command));

  switch (command) {
    case CMD_CHAR_OPEN_VALUE_1:
      CMD_OPEN_VALUE_1();
      break;
    case CMD_CHAR_CLOSE_VALUE_1:
      CMD_CLOSE_VALUE_1();
      break;
    case CMD_CHAR_LEFT_SPLIT_VALUE_1:
      CMD_LEFT_SPLIT_VALUE_1();
      break;
    case CMD_CHAR_RIGHT_SPLIT_VALUE_1:
      CMD_RIGHT_SPLIT_VALUE_1();
      break;
    case CMD_CHAR_OPEN_VALUE_2:
      CMD_OPEN_VALUE_2();
      break;
    case CMD_CHAR_CLOSE_VALUE_2:
      CMD_CLOSE_VALUE_2();
      break;
    case CMD_CHAR_LEFT_SPLIT_VALUE_2:
      CMD_LEFT_SPLIT_VALUE_2();
      break;
    case CMD_CHAR_RIGHT_SPLIT_VALUE_2:
      CMD_RIGHT_SPLIT_VALUE_2();
      break;
    case CMD_CHAR_OPEN_FEEDER_VALVE:
      CMD_OPEN_FEEDER_VALVE();
      break;
    case CMD_CHAR_CLOSE_FEEDER_VALVE:
      CMD_CLOSE_FEEDER_VALVE();
      break;
    case CMD_PRINT_COMMANDS_INFO: 
      printCommandsInfo();
      break;
    default:
      break;
  }
}

void printCommandsInfo() {
  Serial.println("CMD_CHAR_OPEN_VALUE_1: " + String('a'));
  Serial.println("CMD_CHAR_CLOSE_VALUE_1: " + String('z'));
  Serial.println("CMD_CHAR_LEFT_SPLIT_VALUE_1: " + String('s'));
  Serial.println("CMD_CHAR_RIGHT_SPLIT_VALUE_1: " + String('x'));
  Serial.println("CMD_CHAR_OPEN_VALUE_2: " + String('d'));
  Serial.println("CMD_CHAR_CLOSE_VALUE_2: " + String('c'));
  Serial.println("CMD_CHAR_LEFT_SPLIT_VALUE_2: " + String('f'));
  Serial.println("CMD_CHAR_RIGHT_SPLIT_VALUE_2: " + String('v'));
  Serial.println("CMD_CHAR_OPEN_FEEDER_VALVE: " + String('g'));
  Serial.println("CMD_CHAR_CLOSE_FEEDER_VALVE: " + String('b'));
}
