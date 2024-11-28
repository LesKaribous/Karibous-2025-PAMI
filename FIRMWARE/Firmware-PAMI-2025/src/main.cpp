#include <Arduino.h>
#include "pins.h"
#include "ihm.h"
#include "sensors.h"
#include "actuators.h"
#include "motion.h"
#include "match.h"

void waitStart();
void datumPosition(int robotNumber, int teamColor);
void match();
void strategiePAMI();

void setup() {

  initBuzzer();
  initIHM();
  initSensor();
  initMotion();
  initActuators();

  drawSplashScreen();
  //pairingScreen();
  drawBackLcd();

  disableMotors();
  armsDown();

  waitStart();
}

void loop() 
{
  updateMatchTime();
  match();
}

void waitStart(){
  // Attendre que la tirette ne soit plus présente
  infoLCD("Remove Tirette");
  while(getTirette()) {
    delay(250); 
    checkColorTeam();
  }
  playTirette();
  // Attendre que la tirette soit insérée
  infoLCD("Insert Tirette");
  while(!getTirette()) {
    delay(500);
    checkColorTeam();
  }
  playTirette();
  // Datum position du PAMI
  delay(1000);
  datumPosition(getRobotNumber(), getTeamColor());
  setRobotState(READY);
  infoLCD("Robot Ready");
  delay(2000);
  // Attendre que la tirette soit bien insérée pour éviter les faux-départs
  infoLCD("Insert Tirette");
  while(!getTirette()) delay(500);
  playTirette();
  // Attendre que la tirette soit retirée pour débuter le match
  infoLCD("Wait Start");
  while(getTirette()) delay(250);
  playTirette();
  // Le match commence
  setRobotState(MATCH_STARTED);
  infoLCD("Go Match !");
  // Démarrage du compteur !
  startMatch();
}

void datumPosition(int robotNumber, int teamColor){
  enableMotors();
  if (teamColor == TEAM_BLUE){
    // Datum at low Speed
    setMaxSpeed(DATUM_SPEED);
    setAcceleration(DATUM_ACCELERATION);
    // Datum X
    go(-100);
    // Save X position and orientation
    setCurrentX(3000-CENTER_POSITION_MM);
    setCurrentRot(0);
    // Orientate robot
    goTo(0,80,180);
    go(-100);
    // SaveX position and orientation
    setCurrentX(1050+CENTER_POSITION_MM);
    setCurrentRot(180);

    if(robotNumber == 0) goTo(60,80,0);        // Go to safe position
    else if(robotNumber == 1) goTo(180,80,0);  // Go to safe position
    else if(robotNumber == 2) goTo(300,80,0);  // Go to safe position
    else if(robotNumber == 3) goTo(420,80,0);  // Go to safe position
    else debug("ERROR robot number");
  }
  else if (teamColor == TEAM_YELLOW){
    // Datum at low Speed
    setMaxSpeed(DATUM_SPEED);
    setAcceleration(DATUM_ACCELERATION);

    // Datum X
    go(-100);
    // Save X position and orientation
    setCurrentX(CENTER_POSITION_MM);
    setCurrentRot(0);
    // Orientate robot
    goTo(100,0,90);

    go(-100);
    // SaveY position
    setCurrentY(CENTER_POSITION_MM);
    setCurrentRot(90);

    if(robotNumber == 0) goTo(100,100,0);        // Go to safe position
    else if(robotNumber == 1) goTo(100,200,0);  // Go to safe position
    else if(robotNumber == 2) goTo(100,300,0);  // Go to safe position
    else if(robotNumber == 3) goTo(100,400,0);  // Go to safe position
    else debug("ERROR robot number");
  }

  setMaxSpeed(MAX_SPEED);
  setAcceleration(MAX_ACCELERATION);
}

void match(){
  if(getMatchState() == PAMI_RUN){
    enableMotors();
    strategiePAMI();
    setMatchState(PAMI_STOP);
  }
  else if (getMatchState() == PAMI_STOP){
    disableMotors(); // Desactive les moteurs
    while(1) armsFiesta(); // Fin de match
  }
  else {
    disableMotors(); // Desactive les moteurs
  }  
}

void strategiePAMI(){

  setOpponentChecking(false);
  if(getRobotNumber() == 1){
    if(getTeamColor() == TEAM_BLUE){
      goTo(750,180);
      setOpponentChecking(true);
      goTo(750,0);
      armsDown();
    }
    else{
      goTo(3000-750,180);
      setOpponentChecking(true);
      goTo(3000-750,0);
      armsDown();
    }
  }
  else if(getRobotNumber() == 2){
    if(getTeamColor() == TEAM_BLUE){
      goTo(1200,300);
      goTo(600,300);
      setOpponentChecking(true);
      goTo(400,300);
      armsDown();
    }
    else{
      goTo(3000-1200,300);
      goTo(3000-600,300);
      setOpponentChecking(true);
      goTo(3000-400,300);
      armsDown();
    }
  }
  else if(getRobotNumber() == 3){
    if(getTeamColor() == TEAM_BLUE){
      goTo(1350,450);
      goTo(400,550);
      setOpponentChecking(true);
      goTo(0,550);
      armsDown();
    }
    else{
      goTo(3000-1350,450);
      goTo(3000-400,550);
      setOpponentChecking(true);
      goTo(3000-0,550);
      armsDown();
    }
  }
  else if(getRobotNumber() == 0){
    setOpponentChecking(true);
    if(getTeamColor() == TEAM_BLUE){
      go(500);
    }
    else{
      setOpponentChecking(true);
      goTo(500,100);
      setOpponentChecking(false);
      setMaxSpeed(MAX_SPEED*0.75f);
      setAcceleration(MAX_ACCELERATION*0.75f);
      goTo(1300,100,90);
      setMaxSpeed(MAX_SPEED);
      setAcceleration(MAX_ACCELERATION);
      go(-100);
      setCurrentY(CENTER_POSITION_MM);
      goTo(1300,400);
    }
  }
}