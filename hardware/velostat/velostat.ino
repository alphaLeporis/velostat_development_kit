#include <Arduino_PortentaBreakout.h>
#include "settings.h"
#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

SDMMCBlockDevice block_device;
mbed::FATFileSystem fs("fs");

uint8_t (*seq)[4];
int ln;
bool recording = 0;

uint8_t mode = 0;
volatile uint8_t current[4] = {0,1,2,3};

int err;
FILE *myFile;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:
  for (int x = 0; x < 5; x++) {
    pinMode(SELECT[x], OUTPUT);
    Breakout.digitalWrite(SELECT[x], LOW);
  }
  for (int x = 0; x < 3; x++) {
    pinMode(MUX0[x], OUTPUT);
    Breakout.digitalWrite(MUX0[x], HIGH);
  }
  for (int x = 0; x < 3; x++) {
    pinMode(MUX1[x], OUTPUT);
    Breakout.digitalWrite(MUX1[x], HIGH);
  }
  for (int x = 0; x < 3; x++) {
    pinMode(MUX2[x], OUTPUT);
    Breakout.digitalWrite(MUX2[x], HIGH);
  }
  for (int x = 0; x < 3; x++) {
    pinMode(MUX3[x], OUTPUT);
    Breakout.digitalWrite(MUX3[x], HIGH);
  }

  Serial.begin(115200);
  Breakout.digitalWrite(MUX0[1], LOW);
  Breakout.digitalWrite(MUX1[1], LOW);
  Breakout.digitalWrite(MUX2[1], LOW);
  Breakout.digitalWrite(MUX3[1], LOW);

  Breakout.digitalWrite(MUX0[2], LOW);
  Breakout.digitalWrite(MUX1[2], LOW);
  Breakout.digitalWrite(MUX2[2], LOW);
  Breakout.digitalWrite(MUX3[2], LOW);


  delay(1000);
  

  delay(10);
  pinSelect(MUX3, PINMAP[5]);
  delay(10);
  pinSelect(MUX2, PINMAP[3]);
  Breakout.analogReadResolution(ANALOG_RESOLUTION);

  // Init SD
  delay(1000);
  Serial.println("Mounting SDCARD...");
  err =  fs.mount(&block_device);
  if (err) {
     Serial.println("No SD Card filesystem found, please check SD Card on computer and manually format if needed.");
  } else {
    mkdir("fs/reading",0777);                     // 0777 full access permissions linux style 
  }

}

void loop() {
  switch (mode) {
    case 0:
      modeSelect();

    break;

    case 1:
      mode1();
      mode1init();
    
    break;

    case 2:
      mode2();
    
    break;
  
  }


}

void modeSelect(){
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    switch (inByte) {
      // Status return
      case 's':
        mode = 0;
        break;

      // Single mode
      case 'i':
        mode = 1;
        break;
        

      // 
      case 'q':
        mode = 2;

        char myFileName[] = "fs/sequence/seq.txt";
        unsigned char c; 
        FILE *fp = fopen(myFileName, "r");

        fgetc(fp);
        unsigned char c1 = fgetc(fp)-48;
        unsigned char c2 = fgetc(fp)-48;
        unsigned char c3 = fgetc(fp)-48;
        ln = c1*100+c2*10+c3;

        seq = (uint8_t (*)[4])malloc(sizeof(uint8_t[4])*ln);

        for (int x = 0; x<ln; x++){
          fgetc(fp);
          for (int i = 0; i < 4; i++) {
            int inByte1 = fgetc(fp)-48;
            int inByte2 = fgetc(fp)-48;
            seq[x][i] = inByte1*10+inByte2;
            //Serial.print(seq[x][i]);
            //Serial.print(",");
          }          
        }
        fclose(fp);
        
        break;

    }
  }
}

void mode1() {
  pinSelect(MUX0, PINMAP[current[0]]);
  pinSelect(MUX1, PINMAP[current[1]]);
  pinSelect(MUX2, PINMAP[current[2]]);
  pinSelect(MUX3, PINMAP[current[3]]);
  uint16_t w = Breakout.analogRead(ANALOG_A1);

  uint8_t from = 0b10100000 | (0b00011111 & current[2]);
  uint8_t to = 0b11100000 | (0b00011111 & current[3]);
  struct Message msg = {0b10101010,from,to,w}; 
  //Serial.println(current[2] << 6);
  //Serial.println(current[3] << 11);
  //Serial.println(header);

  char foo[sizeof(struct Message)];
  memcpy(foo, &msg, sizeof msg);
  Serial.write(foo, sizeof msg);

  if (recording & !err) {
    char vp[8];
    char vm[8];
    char from[8];
    char to[8];
    char data[16];
    itoa(current[0], vp, 10);
    itoa(current[1], vm, 10);
    itoa(current[2], from, 10);
    itoa(current[3], to, 10);
    itoa(w, data, 10);
    fprintf(myFile,vp);
    fprintf(myFile,",");
    fprintf(myFile,vm);
    fprintf(myFile,",");
    fprintf(myFile,from);
    fprintf(myFile,",");
    fprintf(myFile,to);
    fprintf(myFile,",");
    fprintf(myFile,data);
    fprintf(myFile,"\r\n");
  }

  //Serial.println(header);
  //Serial.println(w);
}

void mode1init() {
  if (Serial.available() >= 9) {
    Serial.read();
    for (int i = 0; i < 4; i++) {
      int inByte1 = Serial.read()-48;
      int inByte2 = Serial.read()-48;
    
      current[i] = inByte1*10+inByte2; 
      //Serial.println(current[i]);
    }
    return;
  }

  if (Serial.available() >= 2) {
    Serial.read();
    int inByte1 = Serial.read()-48;
    if (inByte1 == 66) {
      recording = 1;
      Serial.println("Recording started");
      char myFileName[] = "fs/reading/test2.txt";   // Would be nice if filename is counter
      myFile = fopen(myFileName, "a");
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (inByte1 == 67) {
      recording = 0;
      Serial.println("Recording stopped");
      digitalWrite(LED_BUILTIN, HIGH);
      fclose(myFile);
    }
      

  }

  //Serial.println(current[0]);
}

void mode2() {
  for (int i = 0; i < ln; i++) {
    pinSelect(MUX0, PINMAP[seq[i][0]]);
    pinSelect(MUX1, PINMAP[seq[i][1]]);
    if (seq[i][2] != seq[i-1][2]) pinSelect(MUX2, PINMAP[seq[i][2]]);
    if (seq[i][3] != seq[i-1][3]) pinSelect(MUX3, PINMAP[seq[i][3]]);
    //delay(50);

    uint16_t w = Breakout.analogRead(ANALOG_A1);
    uint8_t from = 0b10100000 | (0b00011111 & seq[i][2]);
    uint8_t to = 0b11100000 | (0b00011111 & seq[i][3]);
    struct Message msg = {0b10101010,from,to,w}; 

    char foo[sizeof(struct Message)];
    memcpy(foo, &msg, sizeof msg);
    Serial.write(foo, sizeof msg);


    if (recording & !err) {
      char vp[8];
      char vm[8];
      char from[8];
      char to[8];
      char data[16];
      itoa(current[0], vp, 10);
      itoa(current[1], vm, 10);
      itoa(current[2], from, 10);
      itoa(current[3], to, 10);
      itoa(w, data, 10);
      fprintf(myFile,vp);
      fprintf(myFile,",");
      fprintf(myFile,vm);
      fprintf(myFile,",");
      fprintf(myFile,from);
      fprintf(myFile,",");
      fprintf(myFile,to);
      fprintf(myFile,",");
      fprintf(myFile,data);
      fprintf(myFile,"\r\n");
    }
    //delay(50);
  }
}

void mode2store(int length) {
  ln = length;
  Serial.println(length);
  seq = (uint8_t (*)[4])malloc(sizeof(uint8_t[4])*ln);
  for (int x = 0; x<ln; x++){
    if (Serial.available() >= 9) {
      Serial.read();
      for (int i = 0; i < 4; i++) {
        int inByte1 = Serial.read()-48;
        int inByte2 = Serial.read()-48;
        seq[x][i] = inByte1*10+inByte2;
        //Serial.println(inByte1);
        //Serial.println(inByte2);
      }
      //Serial.println();
    }
  }
  for (int x = 0; x<ln; x++){
    Serial.println(seq[x][0]);
    Serial.println(seq[x][1]);
    Serial.println(seq[x][2]);
    Serial.println(seq[x][3]);
    Serial.println();
  }
}


void pinSelect(const breakoutPin *MUX, int pinnum){
    for (int x = 0; x<5; x++){
      byte state = bitRead(pinnum, x);
      if (state) {
        Breakout.digitalWrite(SELECT[x], HIGH);
        //Serial.println("High");
      } else {
        Breakout.digitalWrite(SELECT[x], LOW);
        //Serial.println("Low");
      }
    }
    //Serial.println();
    Breakout.digitalWrite(MUX[0], LOW);
    delay(1);
  Breakout.digitalWrite(MUX[0], HIGH);
}

