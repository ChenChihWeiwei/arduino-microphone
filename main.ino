const int micPin1 = A0;        // 麥克風 1 接 A0
const int micPin2 = A1;        // 麥克風 2 接 A1
const int micPin3 = A2;        // 麥克風 3 接 A2
const int noiseThreshold = 10; 
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
  int signalMax3 = 0; int signalMin3 = 1024; // 新增麥克風 3 的極值

  // 在同一個時間窗口內，同時對三組麥克風進行高速採樣
  while (millis() - startMillis < sampleWindow) {
    int val1 = analogRead(micPin1);
    int val2 = analogRead(micPin2);
    int val3 = analogRead(micPin3); // 讀取 A2
    
    if (val1 < 1024) {
      if (val1 > signalMax1) signalMax1 = val1;
      if (val1 < signalMin1) signalMin1 = val1;
    }
    if (val2 < 1024) {
      if (val2 > signalMax2) signalMax2 = val2;
      if (val2 < signalMin2) signalMin2 = val2;
    }
    // 處理麥克風 3
    if (val3 < 1024) {
      if (val3 > signalMax3) signalMax3 = val3;
      if (val3 < signalMin3) signalMin3 = val3;
    }
  }

  int volume1 = signalMax1 - signalMin1;
  int volume2 = signalMax2 - signalMin2;
  int volume3 = signalMax3 - signalMin3; // 計算麥克風 3 的音量

  // 噪音過濾
  if (volume1 <= noiseThreshold) volume1 = 0;
  if (volume2 <= noiseThreshold) volume2 = 0;
  if (volume3 <= noiseThreshold) volume3 = 0;

  // 對應到 0 - 100
  int outVol1 = map(volume1, 0, maxRawVolume, 0, 100);
  int outVol2 = map(volume2, 0, maxRawVolume, 0, 100);
  int outVol3 = map(volume3, 0, maxRawVolume, 0, 100);

  // 限制範圍
  outVol1 = constrain(outVol1, 0, 100);
  outVol2 = constrain(outVol2, 0, 100);
  outVol3 = constrain(outVol3, 0, 100);

  // 用逗號分隔輸出三個麥克風的值 (格式如：52,38,15)
  Serial.print("mic1 ");
  Serial.print(outVol1);
  Serial.print(",");
  Serial.print("mic2 ");
  Serial.print(outVol2);
  Serial.print(",");
  Serial.print("mic3 ");
  Serial.println(outVol3); // 最後一個用 println 來換行
}