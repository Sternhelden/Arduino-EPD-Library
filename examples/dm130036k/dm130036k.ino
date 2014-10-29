#include <epd.h>

//const int data = 8;
//const int clock = 9;
EPD epd(9,8);
byte displayData[5] = {0,0,0,0,0};
int seg[36] = {0};
int i;
int j;

void setup(){
  Serial.begin(57600);
}

void loop() {
  byte nodeID;
  byte address;
  int funcSelec;
  Serial.print("Input node ID (0~3):");
  while(Serial.available() == 0){} // wait input
  nodeID = Serial.read();
  nodeID = asciiToHex(nodeID);
  //checkNodeId   <= future function
  Serial.println(nodeID, DEC);
  Serial.print("Please select function: [0] white [1] black [2] input number:");
  while(Serial.available() == 0){} // wait input
  funcSelec = Serial.read();
  funcSelec = asciiToHex(funcSelec);
  Serial.println(funcSelec);
  switch (funcSelec){
    case 0:
      epd.allBlack(nodeID);
      epd.allWhite(nodeID);
      break;
    case 1:
      epd.allWhite(nodeID);
      epd.allBlack(nodeID);
      break;
    case 2:
      inputNum(nodeID);
      break;
  }
}

void inputNum(byte _nodeID) {
  int temp[5] = {-1,-1,-1,-1,-1};
  Serial.print("Please input display number:");
  while(Serial.available() == 0){} // wait input
  readSerialData(temp, 5);
  resort(temp, 5);
  for (j=0;j<5;j++) {
    Serial.print(temp[j]);
  }
  Serial.println();
  numericToRegister(temp, 5);
  //Serial.println("Begin");
  //Serial.println("Initialize");
  initialize(_nodeID);
  //Serial.println("Phase 1");
  phaseOne(_nodeID);
  //Serial.println("Phase 2");  
  phaseTwo(_nodeID);
  //Serial.println("Phase 3");
  phaseThree(_nodeID);
  epd.powerSaving(_nodeID);
  for (i=0;i<5;i++) {
    displayData[i] = 0;
  }
}

int asciiToHex(int _target) {
  if ((47 < _target) && (_target < 58)) {
    _target = _target-'0';
  } else if ((64 < _target) && (_target < 71)) {
    _target = _target-55;
  } else if ((96 < _target) && (_target < 103)) {
    _target = _target-87;
  }
  return (_target);
}

int readSerialData(int target[], int arraySize) {
   for (i=0; i<arraySize; i++) {
     target[i]=Serial.read();
     target[i]=asciiToHex(target[i]);
     delay(1);
   }
}

int resort(int target[], int arraySize) {
  for (i=0;i<arraySize;i++) {
    if (target[i]==-1) {
      for (j=i;j>=1;j--) {
        target[j]=target[j-1];
      }
      target[0]=0;
    }
  }
}

void initialize(byte _nodeID) {
  for (j=0;j<5;j++) {
    epd.writeRegister(_nodeID, j, 0);
  }
  epd.writeRegister(_nodeID, 5, 0x88);
  epd.writeRegister(_nodeID, 5, 0x80);
  delay(100);
}

void phaseOne(byte _nodeID) {
  for (j=0;j<4;j++) {
    epd.writeRegister(_nodeID, j, 0xFF);
  }
  epd.writeRegister(_nodeID, 4, 0x3F);
  
  epd.writeRegister(_nodeID, 5, 0x88);
  epd.writeRegister(_nodeID, 5, 0x80);
  delay(100);
}

void phaseTwo(byte _nodeID) {
  for (j=0;j<4;j++) {
    epd.writeRegister(_nodeID, j, 0x00);
  }
  epd.writeRegister(_nodeID, 4, 0xC0);

  epd.writeRegister(_nodeID, 5, 0x88);
  epd.writeRegister(_nodeID, 5, 0x80);
  delay(100);
}

void phaseThree(byte _nodeID) {
  for (j=0;j<5;j++) {
    epd.writeRegister(_nodeID, j, displayData[j]);
  }
//  epd.writeRegister(_nodeID, 4, 0x0F);

  epd.writeRegister(_nodeID, 5, 0x88);
  epd.writeRegister(_nodeID, 5, 0x80);
  delay(80);
}

void numericToRegister(int _target[], int arraySize) {
  int _temp[7] = {false};
  int k;
  int a;
  int b;
  for (j=0;j<arraySize;j++) {
    switch (_target[j]) {
      case 0:
        _temp[0]=1;
        _temp[1]=1;
        _temp[2]=1;
        _temp[3]=0;
        _temp[4]=1;
        _temp[5]=1;
        _temp[6]=1;
        break;
      case 1:
        _temp[0]=0;
        _temp[1]=0;
        _temp[2]=0;
        _temp[3]=0;
        _temp[4]=0;
        _temp[5]=1;
        _temp[6]=1;
        break;
      case 2:
        _temp[0]=1;
        _temp[1]=0;
        _temp[2]=1;
        _temp[3]=1;
        _temp[4]=1;
        _temp[5]=0;
        _temp[6]=1;
        break;
      case 3:
        _temp[0]=0;
        _temp[1]=0;
        _temp[2]=1;
        _temp[3]=1;
        _temp[4]=1;
        _temp[5]=1;
        _temp[6]=1;
        break;
      case 4:
        _temp[0]=0;
        _temp[1]=1;
        _temp[2]=0;
        _temp[3]=1;
        _temp[4]=0;
        _temp[5]=1;
        _temp[6]=1;
        break;
      case 5:
        _temp[0]=0;
        _temp[1]=1;
        _temp[2]=1;
        _temp[3]=1;
        _temp[4]=1;
        _temp[5]=1;
        _temp[6]=0;
        break;
      case 6:
        _temp[0]=1;
        _temp[1]=1;
        _temp[2]=1;
        _temp[3]=1;
        _temp[4]=0;
        _temp[5]=1;
        _temp[6]=0;
        break;
      case 7:
        _temp[0]=0;
        _temp[1]=0;
        _temp[2]=0;
        _temp[3]=0;
        _temp[4]=1;
        _temp[5]=1;
        _temp[6]=1;
        break;
      case 8:
        _temp[0]=1;
        _temp[1]=1;
        _temp[2]=1;
        _temp[3]=1;
        _temp[4]=1;
        _temp[5]=1;
        _temp[6]=1;
        break;
      case 9:
        _temp[0]=0;
        _temp[1]=1;
        _temp[2]=0;
        _temp[3]=1;
        _temp[4]=1;
        _temp[5]=1;
        _temp[6]=1;
        break;
    }
    for (k=0;k<7;k++) {
      a=j*7+k;
      if (j>2) {
        a=a+1;
      }
      seg[a]=_temp[k];
//      Serial.println(seg[a]);
    }
  }
   for (k=0;k<36;k++) {
//     Serial.print("k=");
//     Serial.println(k);
//     Serial.print("seg[");
//     Serial.print(k);
//     Serial.print("]=");
//     Serial.println(seg[k]);
     a = k/8;
     b = k%8;
//     Serial.println(displayData[a], BIN);
     if (seg[k]==1) {
       displayData[a]=displayData[a]+(1<<b);
     }
//     Serial.println(displayData[a], BIN);
  }
//  for (k=0;k<5;k++) {
//     Serial.println(displayData[k], BIN);
//  }
}

