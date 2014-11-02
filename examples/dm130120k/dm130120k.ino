#include <epd.h>

//const int data = 8;
//const int clock = 9;
EPD epd(1,9,8);
byte displayData[12] = {0};
unsigned int driverModel;
int arraySize;
int seg[120] = {0};
int i;
int j;

void setup(){
  Serial.begin(57600);
  arraySize=sizeof(displayData);
  /*Serial.print("Driver model: [0]DM130036 [1]DM130120:");
  while(Serial.available()==0){};
  do {
    driverModel=Serial.read();
  } while(driverModel < '0' or driverModel > '2');
  driverModel = asciiToHex(driverModel);
  Serial.println(driverModel, DEC);*/
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
  Serial.println("Please select function: [0] white [1] black [2] input number [3] input number with decimal point");
  Serial.print("[4] input number (white type):");
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
      epd.allBlack(nodeID);
      epd.allWhite(nodeID);
      epd.allBlack(nodeID);
      break;
    case 2:
//      seg[21] = 0;
      inputNum(nodeID);
      break;
    case 3:  
      seg[21] = 1;
      seg[43] = 1;
      seg[58] = 1;
      seg[66] = 1;
      seg[74] = 1;
      seg[82] = 1;
      inputNumPoint(nodeID);
      break;
    case 4:
//      seg[21] = 0;
      inputNumWhite(nodeID);
      break;
  }
  for (i=0;i<12;i++) {
    displayData[i] = 0;
  }
  for (i=0;i<120;i++) {
    seg[i]=0;
  }
}

void variableArray(int _arraySize) {
//  byte temp[_arraySize] = {-1};
}

void inputNum(byte _nodeID) {
  int temp[] = {-1};
//  variableArray(arraySize);
  Serial.print("Please input display number:");
  while(Serial.available() == 0){} // wait input
  readSerialData(temp, arraySize);
  resort(temp, arraySize);
  for (j=0;j<arraySize;j++) {
    Serial.print(temp[j]);
  }
  Serial.println();
  numericToRegister(temp, arraySize);
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
//  for (i=0;i<5;i++) {
//    displayData[i] = 0;
//  }
}

void inputNumPoint(byte _nodeID) {
  int temp[12] = {-1};
  Serial.print("Please input display number:");
  while(Serial.available() == 0){} // wait input
  readSerialData(temp, arraySize);
  resort(temp, arraySize);
  for (j=0;j<arraySize;j++) {
    Serial.print(temp[j]);
  }
  Serial.println();
  numericToRegister(temp, arraySize);
  //Serial.println("Begin");
  //Serial.println("Initialize");
  initialize(_nodeID);
  //Serial.println("Phase 1");
  phaseOne(_nodeID);
  //Serial.println("Phase 2");  
  phaseTwo(_nodeID);
  //Serial.println("Phase 3");
  phaseThreePoint(_nodeID);
  epd.powerSaving(_nodeID);
//  for (i=0;i<5;i++) {
//    displayData[i] = 0;
//  }
}

void inputNumWhite(byte _nodeID) {
  int temp[] = {-1};
  Serial.print("Please input display number:");
  while(Serial.available() == 0){} // wait input
  readSerialData(temp, arraySize);
  resort(temp, arraySize);
  for (j=0;j<arraySize;j++) {
    Serial.print(temp[j]);
  }
  Serial.println();
  numericToRegister(temp, arraySize);
  //Serial.println("Begin");
  //Serial.println("Initialize");
  initialize(_nodeID);
  //Serial.println("Phase 1");
  phaseOne(_nodeID);
  //Serial.println("Phase 2");  
  phaseTwo(_nodeID);
  phaseOneWhite(_nodeID);
  //Serial.println("Phase 3");
  phaseThreeWhite(_nodeID);
  epd.powerSaving(_nodeID);
//  for (i=0;i<5;i++) {
//    displayData[i] = 0;
//  }
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

int readSerialData(int _target[], int _arraySize) {
   for (i=0; i<_arraySize; i++) {
     _target[i]=Serial.read();
     _target[i]=asciiToHex(_target[i]);
     delay(1);
   }
}

int resort(int _target[], int _arraySize) {
  for (i=0;i<_arraySize;i++) {
    if (_target[i]==-1) {
      for (j=i;j>=1;j--) {
        _target[j]=_target[j-1];
      }
      _target[0]=0;
    }
  }
}

void initialize(byte _nodeID) {
  for (j=0;j<17;j++) {
    epd.writeRegister(_nodeID, j, 0);
  }
  epd.writeRegister(_nodeID, 0x10, 0x88);
  epd.writeRegister(_nodeID, 0x10, 0x80);
  delay(100);
}

void phaseOne(byte _nodeID) {
  for (j=0;j<16;j++) {
    epd.writeRegister(_nodeID, j, 0xFF);
  }
  epd.writeRegister(_nodeID, 0x0F, 0x02);
  
  epd.writeRegister(_nodeID, 0x1F, 0x88);
  epd.writeRegister(_nodeID, 0x1F, 0x80);
  delay(100);
}

void phaseOneWhite(byte _nodeID) {
  for (j=0;j<15;j++) {
    epd.writeRegister(_nodeID, j, 0xFF);
  }
  epd.writeRegister(_nodeID, 0x0F, 0x02);
  
  epd.writeRegister(_nodeID, 0x10, 0x88);
  epd.writeRegister(_nodeID, 0x10, 0x80);
  delay(100);
}

void phaseTwo(byte _nodeID) {
  for (j=0;j<15;j++) {
    epd.writeRegister(_nodeID, j, 0x00);
  }
  epd.writeRegister(_nodeID, 0x0F, 0x01);

  epd.writeRegister(_nodeID, 0x10, 0x88);
  epd.writeRegister(_nodeID, 0x10, 0x80);
  delay(100);
}

void phaseThree(byte _nodeID) {
  for (j=0;j<16;j++) {
    epd.writeRegister(_nodeID, j, displayData[j]);
  }
  epd.writeRegister(_nodeID, 0x10, 0x88);
  epd.writeRegister(_nodeID, 0x10, 0x80);
  delay(80);
}

void phaseThreePoint(byte _nodeID) {
  for (j=0;j<16;j++) {
    epd.writeRegister(_nodeID, j, displayData[j]);
  }
  epd.writeRegister(_nodeID, 0x10, 0x88);
  epd.writeRegister(_nodeID, 0x10, 0x80);
  delay(80);
}

void phaseThreeWhite(byte _nodeID) {
  for (j=0;j<16;j++) {
    displayData[j] = ~displayData[j];
    epd.writeRegister(_nodeID, j, displayData[j]);
  }
  epd.writeRegister(_nodeID, 0x10, 0x88);
  epd.writeRegister(_nodeID, 0x10, 0x80);
  delay(80);
}

void numericToRegister(int _target[], int _arraySize) {
  int _temp[7] = {0};
  int k;
  int a;
  int b;
  for (j=0;j<_arraySize;j++) {
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
      if (j>5) {
        a=a+1;
      } 
      if (j>7) {
        a=a+1;
      }
      if (j>8) {
        a=a+1;
      }
      if (j>9) {
        a=a+1;
      }
      if (j>10) {
        a=a+1;
      }
      seg[a]=_temp[k];
//      Serial.println(seg[a]);
    }
  }
   for (k=0;k<120;k++) {
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
//  Serial.println(seg[21]);
//  for (k=0;k<5;k++) {
//     Serial.println(displayData[k], BIN);
//  }
}

