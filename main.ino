const int micPin1 = A0;        // 麥克風 1 接 A0
const int micPin2 = A1;        // 麥克風 2 接 A1
const int noiseThreshold = 50; 
const int sampleWindow = 50;   // 採樣時間窗口 (ms)

// 【校正參數】可依實際狀況調整
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

  // 噪音過濾
  if (volume1 <= noiseThreshold) volume1 = 0;
  if (volume2 <= noiseThreshold) volume2 = 0;

  // 對應到 0 - 100
  int outVol1 = map(volume1, 0, maxRawVolume, 0, 100);
  int outVol2 = map(volume2, 0, maxRawVolume, 0, 100);

  // 限制範圍
  outVol1 = constrain(outVol1, 0, 100);
  outVol2 = constrain(outVol2, 0, 100);

  // 關鍵修改：純逗號分隔輸出 (格式如：52,38)
  Serial.print(outVol1);
  Serial.print(",");
  Serial.println(outVol2);
}