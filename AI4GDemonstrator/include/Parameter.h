#pragma once

struct Parameter
{
	enum {INT, FLOAT} tag;
	union
	{
		float floatValue;
		int intValue;
	};
};

static const char* MAXACCELERATION_PARAM	= "MAXACCELERATION";
static const char* MAXSPEED_PARAM			= "MAXSPEED";
static const char* MAXROTATION_PARAM		= "MAXROTATTION";
static const char* MAXPREDICTION_PARAM		= "MAXPREDICTION";
static const char* TURNRATE_PARAM			= "TURNRATE";
static const char* TARGETRADIUS_PARAM		= "TARGETRADIUS";
static const char* SLOWRADIUS_PARAM			= "SLOWRADIUS";
static const char* TIMETOTARGET_PARAM		= "TIMETOTARGET";
static const char* DISTANCE_PARAM			= "DISTANCE";
static const char* DECAY_PARAM				= "DECAY";
static const char* ENTITYRADIUS_PARAM		= "ENTITYRADIUS";
static const char* RAYLENGTH_PARAM			= "RAYLENGTH";


