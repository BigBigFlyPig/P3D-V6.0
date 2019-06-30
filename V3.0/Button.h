#pragma once
#ifndef BUTTON_H
#define BUTTON_H


#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>
#include <math.h>

enum TRIGGER_TYPE {
	TRIGGER_ONCE,
	TRIGGER_LAST
};


enum BUTTON_TYPE {
	BUTTON_THROTTLE,
	BUTTON_MIX,
	BUTTON_ELEVATOR_UP,
	BUTTON_ELEVATOR_DOWN,
	BUTTON_FLAP_UP,
	BUTTON_FLAP_DOWN,
	BUTTON_ENGIEN_START,
	BUTTON_DOOR,
	BUTTON_BATTARY,
	BUTTON_BRAKE,
	BUTTON_NOTHING,
	BUTTON_CALIBARATE
};

const double CHUFAJULI = 0.002;


class Button {
private:
	SIMCONNECT_DATA_XYZ position;
	BUTTON_TYPE b_type;
	double chufajuli;
	TRIGGER_TYPE t_type;



public:
	Button(SIMCONNECT_DATA_XYZ p, BUTTON_TYPE b,double chufajuli,TRIGGER_TYPE t)
	{
		this->b_type = b;
		this->position = p;
		this->chufajuli = chufajuli;
		this->t_type = t;
		
	}
	Button(double x=0, double y=0, double z=0, TRIGGER_TYPE t= TRIGGER_ONCE, BUTTON_TYPE b= BUTTON_NOTHING, double chufajuli = CHUFAJULI )
	{
		this->b_type = b;
		this->chufajuli = chufajuli;
		this->t_type = t;
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
	}

	SIMCONNECT_DATA_XYZ getPosition(){
		return this->position;
	}

	void setPosition(SIMCONNECT_DATA_XYZ p){
		this->position = p;
	}
	void setPosition(double x, double y, double z) {
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
	}
	BUTTON_TYPE getbType(){
		return this->b_type;
	}

	TRIGGER_TYPE gettType(){
		return this->t_type;
	}

	void setbType(BUTTON_TYPE b){
		this->b_type = b;
	}

	void settType(TRIGGER_TYPE t){
		this->t_type = t;
	}
	double getchufajuli(){
		return this->chufajuli;
	}

	void setchufajuli(double chufajuli){
		this->chufajuli = chufajuli;
	}
	
	double triggered_dis(SIMCONNECT_DATA_XYZ finger_pos){
		 return pow(this->position.x - finger_pos.x, 2) + pow(this->position.y - finger_pos.y, 2) + pow(this->position.z - finger_pos.z, 2);	
	}
};





























#endif // !BUTTON_H