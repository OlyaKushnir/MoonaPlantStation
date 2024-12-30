bool dir = false;
int speed = 50;
int pulses = 3333;
int pulses_value = 3333;

// Constants for delays
const int PRIME_DELAY = 10000; // Delay for priming system in milliseconds
const int FERTILIZE_DELAY = 10000; // Delay for fertilizing in milliseconds
const int FLUSH_DELAY = 10000; // Delay for flushing system in milliseconds


/// Pumps
const int SINGLE_PUMP_PULSES = 33333; //3333 //pulses count for a specific measured pumping
const int PUMP_SPEED = 50; //delay between pulse up and down
bool eng1dir = false;
bool eng2dir = false;

// pins for pulse engines
#define ENG_1_DIR_PIN 13    // Direction Pin
#define ENG_1_STEP_PIN 6   // Step Pin
#define ENG_2_DIR_PIN 8   // Direction Pin
#define ENG_2_STEP_PIN 3   // Step Pin
#define ENG_ENABLE_ALL_PIN 4 // Enable Pin

// pins for pulse engine
#define DIR_PIN 2    // Direction Pin   1=yellow
#define STEP_PIN 3   // Step Pin
#define ENABLE_PIN 4 // Enable Pin (optional)

#define DIR2_PIN 5    // Direction Pin   2=yellow
#define STEP2_PIN 6   // Step Pin

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

#define CMD_CHAR_OPEN_VALUE_1 'a'               //valve1 close steper1 vavle (Fertilizer1)
#define CMD_CHAR_CLOSE_VALUE_1 'z'              //valve1 open steper1 vavle (Fertilizer1)
#define CMD_CHAR_LEFT_SPLIT_VALUE_1 's'         //valve3 close steper2 vavle (Fertilizer2)
#define CMD_CHAR_RIGHT_SPLIT_VALUE_1 'x'        //valve3 open steper2 vavle (Fertilizer2) 
#define CMD_CHAR_OPEN_VALUE_2 'd'               //valve2 back Fertilizer1 to Fertilizer tank1 
#define CMD_CHAR_CLOSE_VALUE_2 'c'              //valve2 back Fertilizer1 to Watter tank 
#define CMD_CHAR_LEFT_SPLIT_VALUE_2 'f'         //valve4 back Fertilizer2 to Fertilizer tank2 
#define CMD_CHAR_RIGHT_SPLIT_VALUE_2 'v'        //valve4 back Fertilizer2 to Watter tank 
#define CMD_CHAR_OPEN_FEEDER_VALVE 'g'          //valve5 close watter to the plants
#define CMD_CHAR_CLOSE_FEEDER_VALVE 'b'         //valve5 release watter tank to the plants
#define CMD_PRINT_COMMANDS_INFO 'i'

const int valvePins[] = {VALVE_1_PIN, SPLIT_VALVE_1_PIN, VALVE_2_PIN, SPLIT_VALVE_2_PIN};
const int numValves = sizeof(valvePins) / sizeof(valvePins[0]);

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

void closeValves() {
  // Initialize vales pins
  resetDigitalOutputPin(VALVE_1_PIN);
  resetDigitalOutputPin(SPLIT_VALVE_1_PIN);
  resetDigitalOutputPin(VALVE_2_PIN);
  resetDigitalOutputPin(SPLIT_VALVE_2_PIN);
  resetDigitalOutputPin(FEEDER_VALVE_PIN);
}
/// Values

void setup() {
  Serial.begin(9600);

  initValues();
  initPumps();
  initializeMotors();

  openTwoValvesReturnFertilizer(); 

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
    
    // //pumps
    // case CMD_CHAR_MOVE_PUMP_1:
    //   CMD_MOVE_PUMP_1();
    //   break;
    // case CMD_CHAR_MOVE_PUMP_2:
    //   CMD_MOVE_PUMP_2();
    //   break;
    
    //misc
    case CMD_PRINT_COMMANDS_INFO: 
      printCommandsInfo();
      break;

    case CMD_CHAR_MOVE_PUMP_1:  // command = 'q' - single pulse series
      movePump(pulses, speed); 
      movePump2(pulses, speed);
      break;
    case 119: // command = 'w' - start/stop continous
      stopMotors();
      break;
    case 101: // command = 'e' - flip direction
      dir = !dir;
      (dir ? digitalWrite(DIR_PIN, HIGH) : digitalWrite(DIR_PIN, LOW));
      (dir ? digitalWrite(DIR2_PIN, HIGH) : digitalWrite(DIR2_PIN, LOW));
      break;

    case 49:  //press '1'
        Serial.println("Starting priming phase...");
        openTwoValvesReturnFertilizer();
        movePump(pulses, speed); 
        movePump2(pulses, speed);
        delay(PRIME_DELAY);

        Serial.println("Starting fertilization phase...");
        fertilizeSystem(); //swich fertilezer to watter tank 
        movePump(pulses_value, speed); 
        movePump2(pulses_value, speed);
        stopMotors();
        delay(FERTILIZE_DELAY);

        Serial.println("Starting flushing phase...");
        setDigitalOutputPin(FEEDER_VALVE_PIN);
        delay(FLUSH_DELAY);

        Serial.println("Process completed.");
        closeValves(); // Reset all valves to closed state
        break;
    
    default:
      break;
  }
}

void printCommandsInfo() {
  Serial.println("valve1 close steper1 vavle (Fertilizer1)" + String('a'));
  Serial.println("valve1 open steper1 vavle (Fertilizer1)" + String('z'));
  Serial.println("valve3 close steper2 vavle (Fertilizer2)" + String('s'));
  Serial.println("valve3 open steper2 vavle (Fertilizer2) " + String('x'));
  Serial.println("valve2 back Fertilizer1 to Fertilizer tank1 " + String('d'));
  Serial.println("valve2 back Fertilizer1 to Watter tank " + String('c'));
  Serial.println("valve4 back Fertilizer2 to Fertilizer tank2 " + String('f'));
  Serial.println("valve4 back Fertilizer2 to Watter tank " + String('v'));
  Serial.println("valve5 close watter to the plants" + String('g'));
  Serial.println("valve5 release watter tank to the plants" + String('b'));
}

// Function to open all valves except the 5th feeder
void openValvesExcept5th() {
  for (int i = 0; i < numValves; i++) {
    if (i == 4) { // Skip 5th feeder (index 4)
      digitalWrite(valvePins[i], LOW); // Ensure the 5th valve stays closed
    } else {
      digitalWrite(valvePins[i], HIGH); // Open the other valves
    }
  }
}

// Function to open only VALVE_1_PIN and SPLIT_VALVE_1_PIN
void openTwoValvesReturnFertilizer() {
  digitalWrite(VALVE_1_PIN, LOW); // Open valve 1
  digitalWrite(SPLIT_VALVE_1_PIN, LOW); // Open split valve 1
  digitalWrite(VALVE_2_PIN, HIGH); // Ensure valve 2 is closed
  digitalWrite(SPLIT_VALVE_2_PIN, HIGH); // Ensure split valve 2 is closed
}

// Function to open only VALVE_2_PIN and SPLIT_VALVE_2_PIN for fertilization
void fertilizeSystem() {
  digitalWrite(VALVE_1_PIN, HIGH); // Close valve 1
  digitalWrite(SPLIT_VALVE_1_PIN, HIGH); // Close split valve 1
  digitalWrite(VALVE_2_PIN, HIGH); // Open valve 2
  digitalWrite(SPLIT_VALVE_2_PIN, HIGH); // Open split valve 2
}

void movePump(int pulseCount, int pulseDelay) {
  // Generate steps
  for (int i = 0; i < pulseCount; i++) {
    digitalWrite(STEP_PIN, HIGH);   // Send a pulse
    delayMicroseconds(pulseDelay);        // Adjust speed
     digitalWrite(STEP_PIN, LOW);    // End pulse
    delayMicroseconds(pulseDelay);        // Adjust speed
  }
}

void movePump2(int pulseCount, int pulseDelay) {
  // Generate steps
  for (int i = 0; i < pulseCount; i++) {
    digitalWrite(STEP2_PIN, HIGH);   // Send a pulse
    delayMicroseconds(pulseDelay);        // Adjust speed
    digitalWrite(STEP2_PIN, LOW);    // End pulse
    delayMicroseconds(pulseDelay);        // Adjust speed
  }
}

// Function to initialize motor pins
void initializeMotors() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(STEP2_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  // Optionally enable the driver
  digitalWrite(ENABLE_PIN, LOW);  // Enable driver (LOW or HIGH based on your driver)

  // Set initial direction (HIGH or LOW)
  digitalWrite(DIR_PIN, LOW);  // Rotate clockwise
  digitalWrite(DIR2_PIN, LOW);  // Rotate clockwise
}

// Function to stop motors
void stopMotors() {
  // Optionally enable the driver
  digitalWrite(ENABLE_PIN, HIGH);  // Enable driver (LOW or HIGH based on your driver)

  // Set initial direction (HIGH or LOW)
  digitalWrite(DIR_PIN, LOW);  // Rotate clockwise
  digitalWrite(DIR2_PIN, LOW);  // Rotate clockwise
}
