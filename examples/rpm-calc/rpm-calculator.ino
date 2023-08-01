#include <SPI.h>
#include <mcp_can.h>

#define SPEED_SENSOR_PIN 2
MCP_CAN CAN(10);

/** values to calculate RPM */
#define PPR 20
#define WheelDiameter 0.65
#define SPeedMeterDiameter 0.20

double elapsedTimeAvg = 0.0;
double elapsedTimeSum = 100000.0;
double elapsedTime = 0;
double prevTime = 0;
unsigned long frqRaw = 0;
unsigned long RPM_s = 0;
unsigned long RPM_w = 0;
int readingCnt = 1;
int purseCnt = 1;


void setup() {
  Serial.begin(115200);
  pinMode(SPEED_SENSOR_PIN, INPUT);

  CAN.begin(MCP_ANY, CAN_125KBPS, MCP_8MHZ);
  CAN.setMode(MCP_NORMAL);

  attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), purseCounter, RISING);
}

void loop() {
  uint8_t data[8];
  int can_id = 0x125;
  int can_dlc = 8;
  memcpy(data, &count, 8);

  frqRaw = 1000000 * 1000 / elapsedTimeAvg;
  RPM_s = (frqRaw * 60 / PPR) / 1000;
  RPM_w = RPM_s * (SPeedMeterDiameter / WheelDiameter);

  Serial.print("RPM_w: ", RPM_w, "RPM");


  int status = CAN.sendMsgBuf(can_id, 0, can_dlc, data);
  if (status == CAN_OK) {
    Serial.println("Success");
   } else
    Serial.println("Error");
}

void purseCounter() {
  elapsedTime = micros() - prevTime;
  prevTime = micros();

  if (purseCnt >= readingCnt) {
    elapsedTimeAvg = elapsedTimeSum / readingCnt;
    purseCnt = 1;
    elapsedTimeSum = elapsedTime;

    int tmpReadCnt = map(elapsedTime, 0, 100000, 1, 10);
    readingCnt = constrain(tmpReadCnt, 1, 10);
  } else {
    purseCnt++;
    elapsedTimeSum += elapsedTime;
  }
}
