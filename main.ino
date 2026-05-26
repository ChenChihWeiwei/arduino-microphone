const int micPin1 = A0;        // 麥克風 1 接 A0
const int micPin2 = A1;        // 麥克風 2 接 A1
const int noiseThreshold = 50; 
const int sampleWindow = 50;   // 採樣時間窗口 (ms)

// 【校正參數】請根據實際狀況調整
// Arduino 類比輸入的最大理論差值是 1023。
// 如果你發現對著麥克風大叫，輸出也到不了 100，就把這個數值調小（例如改為 600 或 800）
const int maxRawVolume = 1023; 

void setup() {
  Serial.begin(9600); 
}

void loop() {
  unsigned long startMillis = millis(); 
  
  int signalMax1 = 0; int signalMin1 = 1024;
  int signalMax2 = 0; int signalMin2 = 1024;

  while (millis() - startMillis < sampleWindow) {
    int val1 = analogRead(micPin1);
    int val2 = analogRead(micPin2);
    
    if (val1 < 1024) {
      if (val1 > signalMax1) signalMax1 = val1;
      if (val1 < signalMin1) signalMin1 = val1;
    }
    if (val2 < 1024) {
      if (val2 > signalMax2) signalMax2 = val2;
      if (val2 < signalMin2) signalMin2 = val2;
    }
  }

  int volume1 = signalMax1 - signalMin1;
  int volume2 = signalMax2 - signalMin2;

  // 1. 噪音過濾（小於門檻值的直接歸零）
  if (volume1 <= noiseThreshold) volume1 = 0;
  if (volume2 <= noiseThreshold) volume2 = 0;

  // 2. 關鍵修改：將原始數值等比例對應到 0 - 100
  int outVol1 = map(volume1, 0, maxRawVolume, 0, 100);
  int outVol2 = map(volume2, 0, maxRawVolume, 0, 100);

  // 3. 安全防護：確保數值絕對不會低於 0 或 超過 100
  outVol1 = constrain(outVol1, 0, 100);
  outVol2 = constrain(outVol2, 0, 100);

  // 輸出至 Serial Plotter
  Serial.print("Mic1_0-100:");
  Serial.print(outVol1);
  Serial.print(",");
  Serial.print("Mic2_0-100:");
  Serial.println(outVol2);
}