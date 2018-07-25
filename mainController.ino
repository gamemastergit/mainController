

/*

  PROJECT     :     TOY ROBOT FACTORY
  NAME        :     MAIN CONTROLLER
  BOARD       :     ARDUINO MEGA
  PROCESSOR   :     AT mega 2560
  DATE        :     7 / 16 / 2018
  PROGRAMMERS :     SERGEI DINES / JACK

  r1_1 =   relay1_1   = 53  = Maglock #1 = Robot Chicken Drops       - Active Low
  r1_2                = 51  = Ball Maglock Drop                      - Active Low
  r1_3                = 49  = Slide Maglock Door                     - Active Low
  r1_4                = 47  = Toy Room Maglock Door   / Waffle Door  - Active Low

  r2_1                = 45  = Machine / Simon Maglock Door           - Active Low
  r2_2                = 43  = Portal Maglock Door                    - Active Low
  r2_3                = 41
  r2_4                = 39

  r3_1                = 37  = Power for Gift Shop controllers       - Active Low
  r3_2                = 35
  r3_3                = 33
  r3_4                = 31  = Gift Shop  Maglock Hidden Door        - Active Low

*/

#define start_button 22    // Digital Pin 22
int start_button_state = 0;
int stage = 0;
int boss_battle_state = 0;
#define boss_battle_success_pin 4

#define chickenDrop 53
#define ballDrop 51
#define slideDoor 49
#define waffleDoor 47
#define r2_1 45
#define r2_2 43
#define r2_3 41
#define r2_4 39
#define r3_1 37
#define r3_2 35
#define r3_3 33
#define r3_4 31

boolean start = false;
int bolt_signal                = 24;    // Digital Pin 22 - Active High
int colorSensorPWR     = 26;    // Digital Pin 24 - Active Low Relay
int colorSensor   = 28;    // Digital Pin 26 - Active High

int waffleDoor   = 28;    //

int simon_signal_pin           = 30;
int simon_state;                        // Read if Simon Puzzle is successful or not - data from 3 Puzzle Mega

int tangram_pin                = 32;    // Continuity Puzzle
int tangram_state;

int pot_1_pin                  = A0;    // God Potentiometer to progress game manually
int pot_1_val;
int pot_2_pin                  = A1;
int pot_2_val;

/*

  "GOD" POT STATES :
  1] Ball Drop [ After Bolt Puzzle ]
  2]


  KEY : MC   =   Main Controller
      BC   =   Battle Controller
      3C   =   3 Puzzle Controller

      FULL PUZZLE LOGIC FLOW :

      [MC]             1]      Start Button > Lights & Background Music & Video (Teddy)
      [MC]             2]      Maglock #1 - Robot Chicken Drops > Press its button to talk > get Key > open 1st Aid with Key > Get Bolts
      [MC]             3]      Bolt Puzzle (Continuity)
      [MC]             4]      Small Button Puzzle Light Up (Continuity)
      [MC]             5]      Small Button Puzzle Solved > Balls Drop & Color Sensor Puzzle Active > Color Solved Opens Slide Door Maglock
      [MC]             6]      Slide Door Maglock Lock Opens > Toy Room Accessed
      [MC]             7]      Perfection > RF Signal to Main Controller
                       8]      Monkeys are dropped from dump truck > Monkeys open cabinet
                       9]      21 Colors found in Toy Room
      [MC]            10]      "OPEN" Blocks for Toy Room Waffle Wall
      [MC Powers/3C]  11]      Back into Robot Room > SIMON Puzzle (On 3 Puzzle Mega )
      [3C]            12]      SIMON solved > Machine Door Opens(On 3 Puzzle Mega) & Triominos (On 3 Puzzle Mega Maglock #2) Drops Pieces
      [3C]            13]      3 Puzzle Stage [ Hex Bugs + Triominos + Patch Bay on 3 Puzzle Mega ] & Main Panel Lights / Dials / Voltmeters > 3 successful
                      14]      Big Button Puzzle Active (On 3 Puzzle Mega-Lights up / can accept inputs)> Big Button Puzzle Solved > Knife Switch LEDs on/Green & Active
                      15]      Knife Switch Flipped (To 3 Puzzle Mega) then Tangram Pieces Fall (Maglock #3 On 3 Puzzle Mega) & Portal Door Lights On (To 3 Puzzle Mega)
                      16]      Tangram Into Door > Opens Portal Door Maglock (Located on Main Controller Relay)
      [BC]            17]      Laser Gun Cabinet Door Open > Signal to Robot Battle Controller > Lights Change & Audio / Boss Talks
      [BC]            18]      Robot Battle (Has own controller called Robot Battle) > Small Robots Defeated > Boss Sound
      [BC]            19]      Boss Robot Battle
      [BC]            20]      Boss Robot Defeated - signal to Main controller [MC]
                      21]      Maglock to Gift Shop (relay on Main Controller) & Power or Signal to Gift Shop Controllers
                      22]      Gift shop "shopping" sequence (Standalone controller gets activated from Main Controller) - Scan items, robot moves, sounds
                               Master Button Flips Power to Main Controller
*/

void setup() {

  Serial.begin(9600);
  pinMode(chickenDrop, OUTPUT);
  pinMode(ballDrop, OUTPUT);
  pinMode(slideDoor, OUTPUT);
  pinMode(waffleDoor, OUTPUT);
  pinMode(r2_1, OUTPUT);
  pinMode(r2_2, OUTPUT);
  pinMode(r2_3, OUTPUT);
  pinMode(r2_4, OUTPUT);
  pinMode(r3_1, OUTPUT);
  pinMode(r3_2, OUTPUT);
  pinMode(r3_3, OUTPUT);
  pinMode(r3_4, OUTPUT);
  pinMode(waffleDoor, OUTPUT);

  digitalWrite(chickenDrop, HIGH);  // Active Low Relay Boards For R1,2,3
  digitalWrite(ballDrop, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(slideDoor, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(waffleDoor, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(r2_1, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(r2_2, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(r2_3, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(r2_4, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(r3_1, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(r3_2, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(r3_3, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(r3_4, HIGH); // Active Low Relay Boards For R1,2,3
  digitalWrite(waffleDoor, HIGH);

  pinMode(bolt_signal,    INPUT);                 // Digital Pin 22
  pinMode(colorSensor, INPUT);       // Digital Pin 24
  pinMode(simon_signal_pin, INPUT);               // Digital Pin 30
  pinMode(tangram_pin, INPUT);                    // Digital Pin 32
}

void loop() {

  if (digitalRead(start_button) && stage == 0) {
    stage = 1;
  }

  if (stage == 1) {

    Serial.println("Game started!");
    Serial.println("Waiting 5 seconds and dropping chicken");

    delay(5000);
    digitalWrite(chickenDrop, LOW);

    stage = 2;

  }

  if (stage == 2) {

    if (digitalRead(bolt_signal)) {
      digitalWrite(ballDrop, LOW);
      digitalWrite(colorSensorPWR, LOW);
      stage = 3;

    }
  }

  if (stage == 3) {

    if (digitalRead(colorSensor)) {
      digitalWrite(slideDoor, LOW);
      digitalWrite(colorSensorPWR, HIGH);
      stage = 4;
    }

  }

  if (stage == 4) {

    if (digitalRead(waffleDoorOpen(){
      digitalWrite(waffleDoor, LOW);
      digitalWrite(
      stage = 5;
    }

  }


}




