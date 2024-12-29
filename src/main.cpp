#include <Arduino.h>

#define GPIO_LEFT_STICK 18
#define GPIO_RIGHT_STICK 5

#define MARIO GPIO_RIGHT_STICK
#define LUIGI GPIO_LEFT_STICK

#define RELAY_ON LOW
#define RELAY_OFF HIGH

#define GAME_TWISTEE_ISLAND 1

#define GAME_NUMBER GAME_TWISTEE_ISLAND

enum A_Or_B
{
  A, // Luigi
  B, // Mario
  _NONE // None
};

int intervalMs = 1000;
int lastTime = 0;
A_Or_B state = A_Or_B::_NONE;

int startTime = 0;

bool hasPrinted = false;

int hits = 0;

void setup()
{
  Serial.begin(115200);

  while (!Serial)
  {
  }

  delay(200);

  Serial.println("Starting...");

  esp_reset_reason_t reason = esp_reset_reason();

  bool skipFirstPress = reason == ESP_RST_POWERON;

  // put your setup code here, to run once:
  pinMode(GPIO_LEFT_STICK, OUTPUT);
  pinMode(GPIO_RIGHT_STICK, OUTPUT);
  digitalWrite(GPIO_LEFT_STICK, RELAY_OFF);
  digitalWrite(GPIO_RIGHT_STICK, RELAY_OFF);

  Serial.println("Press any key to start");

  while (Serial.available() == 0 && !skipFirstPress)
  {
  }

  Serial.setTimeout(50);

  Serial.println("End of setup()");

  startTime = millis();

  Serial.println("Waiting 5 seconds before starting");

  digitalWrite(MARIO, RELAY_ON);
  delay(50);
  digitalWrite(MARIO, RELAY_OFF);

  // clear the serial buffer
  while (Serial.available())
  {
    Serial.read();
  }

  delay(5150);
}

#define BETWEEN(from, to, value)                     \
  if (hits >= from && hits < to && intervalMs > 200) \
  {                                                  \
    intervalMs = value;                              \
  }

#define XBETWEEN(from, to, subtract)

void loop()
{
  if (!hasPrinted)
  {
    hasPrinted = true;
    Serial.println("Start of loop()");
  }

  if (hits >= 100)
  {
    Serial.printf("100 hits reached, good bye! (took %is)\n", (millis() - startTime) / 1000);
    Serial.flush();
    esp_restart();
  }

  // put your main code here, to run repeatedly:
  bool condition = ((millis() - lastTime) > intervalMs);

  if (Serial.available())
  {
    String recv = Serial.readString();
    if (recv.indexOf('l') != -1)
    { // luigi
      state = A_Or_B::A;
    }
    else if (recv.indexOf('m') != -1)
    { // mario
      state = A_Or_B::B;
    }
    else if (recv.indexOf('r') != -1)
    { // reset
      esp_restart();
    }
  }

  if (intervalMs < 200)
  {
    Serial.println("Oops, something went wrong!");
    Serial.flush();
    esp_restart();
  }

  if (condition)
  {
    if (state == A_Or_B::_NONE)
    {
      Serial.println("State is _NONE, pls press l or m");
      Serial.flush();
      esp_restart();
    }

    lastTime = millis();
    hits += 1;

    if (hits == 1)
    {
      intervalMs -= 100;
    }

#if GAME_NUMBER == GAME_TWISTEE_ISLAND
    BETWEEN( 0, 10, 1280);
    BETWEEN(10, 12, 1200);
    BETWEEN(12, 20, 1150);
    BETWEEN(20, 28, 1050);
    BETWEEN(28, 36,  995);
    BETWEEN(36, 40,  940);
    BETWEEN(40, 50,  880);
    BETWEEN(52, 60,  750);
    BETWEEN(60, 67,  680);
    BETWEEN(67, 77,  638);
    BETWEEN(77, 85,  590);
    BETWEEN(85, 90,  550);
    BETWEEN(90, 93, 492);
    BETWEEN(93, 100, 460);
#elif
#error "No game selected"
#endif

    Serial.printf("Pressing %s\n", state == A_Or_B::A ? "Luigi (Y)" : "Mario (X)");

    digitalWrite(GPIO_LEFT_STICK, state == A_Or_B::A ? RELAY_ON : RELAY_OFF);
    digitalWrite(GPIO_RIGHT_STICK, state == A_Or_B::B ? RELAY_ON : RELAY_OFF);

    delay(75);

    digitalWrite(GPIO_LEFT_STICK, RELAY_OFF);
    digitalWrite(GPIO_RIGHT_STICK, RELAY_OFF);

    Serial.printf("Hit #: %i, waiting %ims\n", hits, intervalMs);

    state = state == A_Or_B::A ? A_Or_B::B : A_Or_B::A;
  }
}
