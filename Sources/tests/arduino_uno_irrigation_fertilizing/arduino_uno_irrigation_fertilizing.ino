
/// Pumps
#define SINGLE_PUMP_PULSES 33333 //3333 //pulses count for a specific measured pumping
#define PUMP_SPEED 50 //delay between pulse up and down
bool eng1dir = false;
bool eng2dir = false;

// pins for pulse engines
#define ENG_1_DIR_PIN 13    // Direction Pin
#define ENG_1_STEP_PIN 6   // Step Pin
#define ENG_2_DIR_PIN 8   // Direction Pin
#define ENG_2_STEP_PIN 3   // Step Pin
#define ENG_ENABLE_ALL_PIN 4 // Enable Pin

void CMD_MOVE_PUMP_1() { movePump(ENG_1_STEP_PIN, SINGLE_PUMP_PULSES, PUMP_SPEED);}
void CMD_MOVE_PUMP_2() { movePump(ENG_2_STEP_PIN, SINGLE_PUMP_PULSES, PUMP_SPEED);}

#define CMD_CHAR_MOVE_PUMP_1 'q'
#define CMD_CHAR_MOVE_PUMP_2 'w'

void movePump(int pin, int pulseCount, int pulseDelay) {
  // Generate steps
  for (int i = 0; i < pulseCount; i++) {
    digitalWrite(pin, HIGH);   // Send a pulse
    delayMicroseconds(pulseDelay);        // Adjust speed
    digitalWrite(pin, LOW);    // End pulse
    delayMicroseconds(pulseDelay);        // Adjust speed
  }
}
void initPumps() {
  // Initialize engine pins as outputs
  pinMode(ENG_1_DIR_PIN, OUTPUT);
  pinMode(ENG_1_STEP_PIN, OUTPUT);
  pinMode(ENG_2_DIR_PIN, OUTPUT);
  pinMode(ENG_2_STEP_PIN, OUTPUT);
  pinMode(ENG_ENABLE_ALL_PIN, OUTPUT);
  // set engine initial state
  digitalWrite(ENG_ENABLE_ALL_PIN, HIGH); //enables all engines
  digitalWrite(ENG_1_DIR_PIN, LOW);  // Rotate eng1 clockwise
  digitalWrite(ENG_2_DIR_PIN, LOW);  // Rotate eng2 clockwise
}
/// Pumps


/// Values

// pins for value system
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

void initDigitalOutputPin(int pin) {
  pinMode(pin, OUTPUT);
}
void setDigitalOutputPin(int pin) {
  digitalWrite(pin, HIGH);
}
void resetDigitalOutputPin(int pin) {
  digitalWrite(pin, LOW);
}

void initValues() {
  // Initialize vales pins
  initDigitalOutputPin(VALVE_1_PIN);
  initDigitalOutputPin(SPLIT_VALVE_1_PIN);
  initDigitalOutputPin(VALVE_2_PIN);
  initDigitalOutputPin(SPLIT_VALVE_2_PIN);
  initDigitalOutputPin(FEEDER_VALVE_PIN);
}
/// Values

void setup() {
  Serial.begin(9600);

  initValues();
  initPumps();

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

    // values
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
    
    //pumps
    case CMD_CHAR_MOVE_PUMP_1:
      CMD_MOVE_PUMP_1();
      break;
    case CMD_CHAR_MOVE_PUMP_2:
      CMD_MOVE_PUMP_2();
      break;
    
    //misc
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
