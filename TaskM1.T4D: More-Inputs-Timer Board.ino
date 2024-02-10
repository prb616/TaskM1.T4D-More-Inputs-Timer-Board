// TaskM1.T4D: More-Inputs-Timer Board
//Name : Prabhjot Singh
//Roll No. : 2210994884

// Define constants for the soil moisture sensor, PIR motion sensors, and LED pins
#define Soil_sense 3
#define PIR_1 2
#define P_2 4

#define Soil_sense_LED 13
#define PIR_2_LED 12
#define timer_LED 11
#define PIR_1_LED 10

// Initialize variables for storing the LED states
volatile bool PIR_1_LED_state = false;
volatile bool Soil_sense_LED_state = false;
volatile bool PIR_2_LED_state = false;

// Variable for the timer LED delay
int a = 2;

// Function to set up the timer interrupt
void starttimmerfunc()
{
 // Clear the timer counter
 TCCR1A = 0;
 TCCR1B = 0;
 TCNT1  = 0;

 // Set the timer compare value for 2ms
 OCR1A = 31249;

 // Configure the timer for CTC mode and set the prescaler to 1024
 TCCR1B |= (1 << WGM12);
 TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);

 // Enable the timer compare interrupt
 TIMSK1 |= (1 << OCIE1A);
}

// Interrupt service routine for the timer compare match
ISR(TIMER1_COMPA_vect)
{
 // Print the current LED timer value in milliseconds
 Serial.print("Led Timer: ");
 Serial.print(a);
 Serial.println(" milliseconds");

 // Toggle the timer LED state
 digitalWrite(timer_LED, digitalRead(timer_LED) ^ 1);

 // Increment the timer LED delay
 a = a + 2;
}

// Setup function
void setup()
{
 // Set the LED and sensor pins as output and input, respectively
 pinMode(PIR_1_LED, OUTPUT);
 pinMode(Soil_sense_LED, OUTPUT);
 pinMode(PIR_2_LED, OUTPUT);
 pinMode(timer_LED, OUTPUT);

 pinMode(PIR_1, INPUT);
 pinMode(PIR_2, INPUT);
 pinMode(Soil_sense, INPUT);

 // Initialize the serial communication
 Serial.begin(9600);

 // Configure the timer and interrupts
 starttimmerfunc();

 // Disable interrupts
 noInterrupts();

 // Set up the external pin change interrupt for the soil moisture and PIR sensors
 PCIFR |= bit(PCIE2);
 PCICR |= bit(PCIE2);
 PCMSK2 |= bit (PCINT18);
 PCMSK2 |= bit (PCINT19);
 PCMSK2 |= bit (PCINT20);

 // Enable interrupts
 interrupts();
}

// Loop function
void loop()
{
 // Main program loop
}

// Interrupt service routine for the external pin change interrupt
ISR(PCINT2_vect)
{
 // Check the soil moisture sensor state
 if (digitalRead(Soil_sense) == HIGH)
 {
   // Toggle the soil moisture LED state
   Soil_sense_LED_state = !Soil_sense_LED_state;
   digitalWrite(Soil_sense_LED, Soil_sense_LED_state);

   // Print a message indicating soil moisture detection
   Serial.println("Soil Moisture Detected");
 }

 // Check the first PIR sensor state
 if (digitalRead(PIR_1) == HIGH)
 {
   // Toggle the first PIR LED state
   PIR_1_LED_state = !PIR_1_LED_state;
   digitalWrite(PIR_1_LED, PIR_1_LED_state);

   // Print a message indicating motion detection by the first sensor
   Serial.println("Motion detected by 1st sensor ");
 }

 // Check the second PIR sensor state
 if (digitalRead(PIR_2) == HIGH)
 {
   // Toggle the second PIR LED state
   PIR_2_LED_state = !PIR_2_LED_state;
   digitalWrite(PIR_2_LED, PIR_2_LED_state);

   // Print a message indicating motion detection by the second sensor
   Serial.println("Motion detected by 2nd sensor ");
 }
}
