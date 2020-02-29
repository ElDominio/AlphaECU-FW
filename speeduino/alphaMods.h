#ifndef __ALPHA_MODS_H__
#define __ALPHA_MODS_H__

#include "globals.h"

struct alphaMods{
//***
  byte carSelect = 9; //0 - generic car; 1 - Corolla XRS; 2 - Hyundai Tiburon; 
  //3 - Subaru WRX; 4 - Audi A4 1.8T; 5 - 1988 Mazda RX7; 6- 98 Mustang V6; 7 - DCwerx; 8 - 1994 BMW 328i; 9 - 1988 BMW 325e
  //255 - mods disabled
  
  
  
  /*bool DFCOwait; // waits to enable DFCO
  bool vvlOn = false;
  bool CELon = false;
  bool gaugeSweep = true;
  bool rollingALsoft = false;
  bool rollingALhard = false;
  bool rollingALtrigger = false;
  bool ACOn; //whether AC is on
  bool AcReq; // AC request
  bool highIdleReq;*/ //raises idle in open loop to evade stalling
  uint8_t highIdleCount = 0;// counts to wait for normal idle, basically a dashpot function; 
  //works by holding idle at an added value for a few seconds, then going back to what is in the idle duty table. Could be improved
  //by using a decay timer, and a "wait till decay" to hold at a value + addder for a second or two, then start decaying into the normal table
  uint8_t DFCOcounter = 0;// counts cycles till dfco
  //makes DFCO happen after a set amount of seconds, allowing for eeither more predictable throttle, or pops and bangs
  uint8_t alphaBools1 = 0;
  uint8_t alphaBools2 = 0;
  //these contain a few boolean values i use for checking certain states
  uint8_t vvlCorrection;
  //corrects fuel and timing when a certain output is enabled
  uint8_t alphaNcorrection;
  //used for cars with hybrid alphan, may not be useful anymore
  uint8_t gCamTime = 0;
  //ghost cam, diables ignition outputs for a set amount of time, under a certain amount of revs and TPS, as to not affect drivability, used on a rotary to make it sound "bridgeported". 
  //actually was pretty convincing
  uint8_t stallCount = 0;
  //used to try to force ignition outputs off
  uint32_t camElapsedTime = 0;//not used
  uint32_t lastCamAngleCalc = 0;//not used
  
  //**
};

//defines for alphaBools1
#define BIT_AC_ON         0 //Position Report enabled
#define BIT_AC_REQ        1 
#define BIT_HIGH_IDLE     2
#define BIT_DFCO_WAIT     3
#define BIT_CEL_STATE     4
#define BIT_GAUGE_SWEEP   5
#define BIT_VVL_ON        6

//defines for alphaBools2
#define BIT_RLING_SOFT    0
#define BIT_RLING_HARD    1 
#define BIT_RLING_TRIG    2
#define BIT_GCAM_STATE    3
#define BIT_CRK_ALLOW     4
#define BIT_SKIP_TOOTH    5

struct alphaMods alphaVars;
extern struct alphaMods alphaVars; // from speeduino.ino

void ACControl(); //controls an output for an ac compressor, using inputs from one or two sensors (sometimes both analog, pressure and evap panel temp
//sometimes digital, older cars with overpressure switches), to make sure the AC lines don't freeze
void CELcontrol();
//activates a certain output when the internal speeduino ERROR flag was not "no error", and a few other custom conditions.
void vvlControl();
//controlled an on/of output on 2zz engines
void fanControl2();
//fan control for cars with complicated PWM fan controllers, and provided for an additional fan
void highIdleFunc();
//dashpot function explained above
void DFCOwaitFunc();
//dfco wait function explained above
void XRSgaugeCLT();
//coolant gauge control for Toyota Corolla XRS
void alphaIdleMods();
// has an adder for when the A/C output is on, an adder for when ASE is on, and adder for when fans are on, and takes care of implementing dashpot
void RPMdance();
// sort of worked, pulses the tacho put with increasing and decreasing frrequencies to make the tacho needle sweep when powered on. 
//later on, i retried to implement this by unmasking the tacho pin from the ignition output and doing it in software, but i could never tie it back
//to the ignition output so the output just stayed at zero. maybe implment tacho control in software?
void initialiseAlphaPins();
void alpha4hz();
//a 4hz timer to check for AC request  and other mods (so they are checked ever .25 seconds)
void alphaCorrections();
void forceStallOff();
void forceStallOffTimer();
void ghostCam();
//explained above
void ghostCamTimer();
void cltTimer();
//coolant timer function for corolla XRS
void perMSfunc();
void maxStallTimeMod();

void alphaIgnMods();
// implemented the ghost cam feature, wanted to use it to implement an RPMdot correction to have it pull timing when 
//RPMdot is too slow, indicating a very high load condition (racing in higher gears)
uint16_t getCamAngle_VVT();

static inline uint8_t correctionVVL();
static inline uint8_t correctionAlphaN();
uint16_t WOTdwellCorrection(uint16_t); 
//lowered dwell time in high RPM conditions, where sometimes a car would be fine in idle but overdwelling WOT.
uint16_t boostAssist(uint16_t);
//kept boost duty at 80% until a certain boost threshold was met, to assist with spool up. Should check for TPS to make sure it is only in WOT
static inline int8_t correctionRollingAntiLag(int8_t);
//tried to implement rolling (meaning while moving) AL, but never quite worked. The idea is that when the user presses the button, a certain window of rpm activates
//where the ECU will slowly substract timing to increase boost, until it hit a rev limiter established at the moment of the button press
//for example, if the user hits the button at 3000RPM, and they set a window of 500RPM, then the car will slowly reach their set negative timing
//maximum until 3500RPM. probably needs a better rolling cut implemented to work proerly since a full cut would be very aggressive

static inline int8_t correctionZeroThrottleTiming(int8_t advance);
//already in latest speeduino code
static inline bool correctionDFCO2();
static inline int8_t correctionTimingAlphaN(int8_t advance);
static inline int8_t correctionAtUpshift(int8_t advance);
//detects WOT conditions where timing is going backwards, which typically means it is an automatic transmission vehicle that is shifting
//removed timing to help the transmission while shifting.
void alphaTableSetup();

// Custom pins
 byte pinAC; // pin for AC clutch
 byte pinAcReq;
 byte pinFan2;
 byte pinCEL;
 byte pinVVL;
 byte pinACpress;
 byte pinACtemp;
 byte pinOilPress;
 byte pinCLTgauge;
 byte pinRollingAL;

#endif // __ALPHA_MODS_H__
