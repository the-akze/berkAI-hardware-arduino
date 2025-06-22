#define m1dir1pin 2
#define m1dir2pin 3
#define m1ena 9

#define m2dir1pin 4
#define m2dir2pin 5
#define m2ena 10

#define DEFAULT_VIBRATION_SPEED 255
#define SHORT_VIB_DUR 200
#define LONG_VIB_DUR 700

long vibrationStartTime = 0;
long vibrationEndTime = 0;


#define ultsonEchoPin 12
#define ultsonTrigPin 13

void setVibration(int amount) {
  digitalWrite(m1dir1pin, HIGH);
  digitalWrite(m1dir2pin, LOW);

  analogWrite(m1ena, amount);
}

void startVibration(long durationMs) {
  vibrationStartTime = millis();
  vibrationEndTime = millis() + durationMs;
}

void updateVibration() {
  if (
    (vibrationEndTime >= vibrationStartTime) && (millis() > vibrationStartTime) && (millis() < vibrationEndTime)) {
    setVibration(DEFAULT_VIBRATION_SPEED);
  }
  else {
    setVibration(0);
  }

  // Serial.print(vibrationStartTime);
  // Serial.print(" ");
  // Serial.print(vibrationEndTime);
  // Serial.print(" ");
  // Serial.println(millis());
}

#define VIB_COUNT 4

bool useVibList = false;

int vibrations[VIB_COUNT][3] = {
  {1000, SHORT_VIB_DUR, false},
  {1400, SHORT_VIB_DUR, false},
  {1800, LONG_VIB_DUR, false},
  {4000, 3000, false},
};

long ultsonDuration;
int ultsonDistance;

void ultrasonicLoop() {
  // Clears the trigPin
  digitalWrite(ultsonTrigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(ultsonTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultsonTrigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  ultsonDuration = pulseIn(ultsonEchoPin, HIGH);
  // Calculating the distance
  ultsonDistance = ultsonDuration * 0.034 / 2;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(m1dir1pin, OUTPUT);
  pinMode(m1dir2pin, OUTPUT);
  pinMode(m1ena, OUTPUT);

  pinMode(m2dir1pin, OUTPUT);
  pinMode(m2dir2pin, OUTPUT);
  pinMode(m2ena, OUTPUT);

  pinMode(ultsonTrigPin, OUTPUT);
  pinMode(ultsonEchoPin, INPUT);

  Serial.begin(9600);
}

void updateAll() {
  updateVibration();
  ultrasonicLoop();
}

bool useVibDelay = true;
long vibrateDelay;
long lastVibTime;

void doVibDelay(long every, long duration) {
  if (every == 0) {
    setVibration(0);
    return;
  }
  if (millis() > (lastVibTime + every)) {
    startVibration(duration);
    lastVibTime = millis();
  }
}

void loop() {
  if (useVibList) {
    for (int i = 0; i < VIB_COUNT; i++) {
      if (millis() > vibrations[i][0] && !vibrations[i][2]) {
        startVibration(vibrations[i][1]);
        vibrations[i][2] = true;
      }
    }
  }

  updateAll();

  long every;

  if (ultsonDistance < 10) {
    every = 300;
  }
  else if (ultsonDistance < 50) {
    every = 500;
  }
  else if (ultsonDistance < 100) {
    every = 2000;
  }
  else {
    every = 0;
  }

  doVibDelay(every, 200);

  Serial.print("dist: ");
  Serial.print(ultsonDistance);

  Serial.print(" \tevery: ");
  Serial.print(every);

  Serial.print(" \tUD:");
  Serial.println(ultsonDistance);

}