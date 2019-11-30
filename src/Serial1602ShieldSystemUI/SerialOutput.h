#ifndef SerialOutput_H_
#define SerialOutput_H_

extern unsigned long lastSerialOutputTime;
extern long serialOutputIntervalInSeconds;

void serialPrintDeviceInfo();

void serialPrintData();

void forceSerialOutput();

#endif
