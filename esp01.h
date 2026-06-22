#ifndef _ESP01_H_
#define _ESP01_H_

void esp01_connectAP(void);
void esp01_sendToThingspeak(char *);
void esp01_sendsetpoint(char *);
void ReadSetpointFromESP_01(char *);

#endif
