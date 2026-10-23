
#include <Arduino.h>

// ピン設定
constexpr uint8_t LED_PIN = 3;
constexpr uint8_t SW_PIN  = 2;

// 動作設定
constexpr unsigned long LED_INTERVAL = 1000;  // 1秒ごと反転
constexpr unsigned long DEBOUNCE_MS = 30;

unsigned long lastLedTime = 0;
unsigned long lastSwChangeTime = 0;

bool ledState = false;
bool lastRawSw = HIGH;
bool stableSw = HIGH;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(SW_PIN, INPUT_PULLUP);

  digitalWrite(LED_PIN, LOW);

  lastRawSw = digitalRead(SW_PIN);
  stableSw = lastRawSw;

  Serial.begin(115200);
}

void loop() {
  unsigned long now = millis();

  // --------------------------------
  // LED : 2秒周期点滅
  // --------------------------------
  if (now - lastLedTime >= LED_INTERVAL) {
    lastLedTime = now;

    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }

  // --------------------------------
  // SW : ONエッジ検出
  // --------------------------------
  bool rawSw = digitalRead(SW_PIN);

  // 入力レベルの変化
  if (rawSw != lastRawSw) {
    lastSwChangeTime = now;
    lastRawSw = rawSw;
  }

  // チャタリング除去
  if ((now - lastSwChangeTime) >= DEBOUNCE_MS) {

    if (rawSw != stableSw) {
      stableSw = rawSw;

      // HIGH -> LOW = SW ON
      if (stableSw == LOW) {
        Serial.println("SW_ON");
      }
    }
  }
}
