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

void setup()
{

  initBuzzer();
  initIHM();
  initSensor();
  initMotion();
  initActuators();

  drawSplashScreen();
  // pairingScreen();
  drawBackLcd();

  disableMotors();
  armsDown();
  selectMelody(getRobotNumber());

  //while(1) readSensors(true); // TODO : Test sensors

  waitStart();
}

void loop()
{
  updateMatchTime();
  match();
}

void waitStart()
{
  // Attendre que la tirette ne soit plus présente
  infoLCD("Remove Tirette");
  while (getTirette())
  {
    delay(250);
    checkColorTeam();
  }
  playTirette();
  // Attendre que la tirette soit insérée
  infoLCD("Insert Tirette");
  while (!getTirette())
  {
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
  while (!getTirette())
    delay(500);
  playTirette();
  // Attendre que la tirette soit retirée pour débuter le match
  infoLCD("Wait Start");
  while (getTirette())
    delay(250);
  playTirette();
  // Le match commence
  setRobotState(MATCH_STARTED);
  infoLCD("Go Match !");
  // Démarrage du compteur !
  startMatch();
}



void datumPosition(int robotNumber, int teamColor)
{
  enableMotors();
  if (teamColor == TEAM_BLUE)
  {
    // Datum at low Speed
    setMaxSpeed(DATUM_SPEED);
    setAcceleration(DATUM_ACCELERATION);

    // Datum X
    go(-100);
    // Save X position and orientation
    setCurrentX(3000 - CENTER_POSITION_MM);
    setCurrentRot(180);
    // Orientate robot
    goTo(3000 - 250, 0, 90);

    go(-100);
    // SaveY position
    setCurrentY(CENTER_POSITION_MM);
    setCurrentRot(90);

    // Go to safe position
    if (robotNumber == 0)
    {
      goTo(3000 - 250, 100, 180);
      go(-150);
      setCurrentX(2900);
    }
    else if (robotNumber == 1)
    {
      goTo(3000 - 250, 200, 180);
      go(-150);
      setCurrentX(2900);
    }
    else if (robotNumber == 2)
    {
      goTo(3000 - 250, 300, 180);
      go(-150);
      setCurrentX(2900);
    }
    else if (robotNumber == 3)
    {
      goTo(3000 - 250, 400, 180);
      go(-150);
      setCurrentX(2900);
    }
    else
      debug("ERROR robot number");
  }
  else if (teamColor == TEAM_YELLOW)
  {
    // Datum at low Speed
    setMaxSpeed(DATUM_SPEED);
    setAcceleration(DATUM_ACCELERATION);

    // Datum X
    go(-100);
    // Save X position and orientation
    setCurrentX(CENTER_POSITION_MM);
    setCurrentRot(0);
    // Orientate robot
    goTo(250, 0, 90);

    go(-100);
    // SaveY position
    setCurrentY(CENTER_POSITION_MM);
    setCurrentRot(90);

    // Go to safe position
    if (robotNumber == 0)
    {
      goTo(250, 100, 0);
      go(-150);
      setCurrentX(100);
    }
    else if (robotNumber == 1)
    {
      goTo(250, 200, 0);
      go(-150);
      setCurrentX(100);
    }
    else if (robotNumber == 2)
    {
      goTo(250, 300, 0);
      go(-150);
      setCurrentX(100);
    }
    else if (robotNumber == 3)
    {
      goTo(250, 400, 0);
      go(-150);
      setCurrentX(100);
    }
    else
      debug("ERROR robot number");
  }

  setMaxSpeed(MAX_SPEED);
  setAcceleration(MAX_ACCELERATION);
}

void match()
{
  if (getMatchState() == PAMI_RUN)
  {
    enableMotors();
    strategiePAMI();
    setMatchState(PAMI_STOP);
  }
  else if (getMatchState() == PAMI_STOP)
  {
    disableMotors(); // Desactive les moteurs
    infoLCD("Fin Match !");
    while (1)
      armsFiesta(); // Fin de match
  }
  else
  {
    disableMotors(); // Desactive les moteurs
  }
}

void strategiePAMI()
{

  setOpponentChecking(true);
  if (getRobotNumber() == 0)
  {
    if (getTeamColor() == TEAM_BLUE)
    {
      goTo(3000-500, 100);
      setOpponentChecking(false);
      setMaxSpeed(MAX_SPEED * 0.40f);
      setAcceleration(MAX_ACCELERATION * 0.40f);
      goTo(3000-1300, 100, 90);
      setMaxSpeed(MAX_SPEED * 0.10f);
      setAcceleration(MAX_ACCELERATION * 0.10f);
      go(-200);
      setCurrentY(CENTER_POSITION_MM);
      setMaxSpeed(MAX_SPEED * 0.30f);
      setAcceleration(MAX_ACCELERATION * 0.30f);
      goTo(3000-1300, 430);
    }
    else
    {
      goTo(500, 100);
      setMaxSpeed(MAX_SPEED * 0.40f);
      setAcceleration(MAX_ACCELERATION * 0.40f);
      goTo(1300, 100, 90);
      setMaxSpeed(MAX_SPEED * 0.10f);
      setAcceleration(MAX_ACCELERATION * 0.10f);
      go(-200);
      setCurrentY(CENTER_POSITION_MM);
      setMaxSpeed(MAX_SPEED * 0.30f);
      setAcceleration(MAX_ACCELERATION * 0.30f);
      goTo(1300, 430);
    }
  }
  else if (getRobotNumber() == 1)
  {
    if (getTeamColor() == TEAM_BLUE)
    {
      pause(5000);
      goTo(3000-200, 200);
      goTo(3000-900, 500,-90);
    }
    else
    {
      pause(5000);
      goTo(200, 200);
      goTo(900, 500,-90);
    }
  }
  else if (getRobotNumber() == 2)
  {
    if (getTeamColor() == TEAM_BLUE)
    {
      pause(3000);
      goTo(3000-200, 300);
      goTo(3000-1220, 640,-90);
    }
    else
    {
      pause(3000);
      goTo(200, 300);
      goTo(1220, 640,-90);
    }
  }
  else if (getRobotNumber() == 3)
  {
    if (getTeamColor() == TEAM_BLUE)
    {
      goTo(3000-475, 400);
      goTo(3000-1030, 900);
      goTo(3000-1730, 900);
      goTo(3000-1800, 700, -90);
      
      /*
      goTo(3000-200, 400);
      setOpponentChecking(true);
      goTo(3000-1800, 700,-90);
      */
    }
    else
    {
      goTo(475, 400);
      goTo(1030, 900);
      goTo(1730, 900);
      goTo(1800, 700, -90);
      
      /*
      goTo(200, 400);
      setOpponentChecking(true);
      goTo(1800, 700,-90);
      */
    }
  }
}