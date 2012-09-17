#pragma once

#include "XnCppWrapper.h"
#include <iostream>
#include "ofMain.h"
#include "CLNUIDevice.h"

using namespace xn;

class Kinect
{

private:
	XnStatus status;
	Context context;

	const XnDepthPixel * depthMap;
	const XnUInt8 * imageMap;
	DepthMetaData depthMD;
	SceneMetaData * escena;
	CLNUIMotor motor;
	
	bool checkStatusOK(const XnStatus status, char * entorno);


public:

	enum GeneratorType {
		IMAGE_GENERATOR, 
		DEPTH_GENERATOR, 
		USER_GENERATOR, 
		HAND_GENERATOR,
		GESTURE_GENERATOR,
		ALL_GENERATORS
	};

	enum Joint{
		HEAD			= 1,
		NECK			= 2,
		TORSO			= 3,
		WAIST			= 4,

		LEFT_COLLAR		= 5,
		LEFT_SHOULDER	= 6,
		LEFT_ELBOW		= 7,
		LEFT_WRIST		= 8,
		LEFT_HAND		= 9,
		LEFT_FINGERTIP	=10,

		RIGHT_COLLAR	=11,
		RIGHT_SHOULDER	=12,
		RIGHT_ELBOW		=13,
		RIGHT_WRIST		=14,
		RIGHT_HAND		=15,
		RIGHT_FINGERTIP	=16,

		LEFT_HIP		=17,
		LEFT_KNEE		=18,
		LEFT_ANKLE		=19,
		LEFT_FOOT		=20,

		RIGHT_HIP		=21,
		RIGHT_KNEE		=22,
		RIGHT_ANKLE		=23,
		RIGHT_FOOT		=24	
	};

	Kinect(void);
	~Kinect(void);

	void setup();
	void update();
	bool enableGenerator(GeneratorType generator);
	bool disableGenerator(GeneratorType generator);
	bool setMotorPosition(short position);
	XnChar * getActiveGenerators();

	inline const XnDepthPixel * getDepthMap() { return depthMap; }
	inline const XnUInt8 * getImageMap() { return imageMap; }
	const int getXRes();
	const int getYRes();
	const XnPoint3D * getHand();
	bool getJoints(Joint * joints, XnSkeletonJointTransformation * jPositions, XnUInt8 nJoints);
	bool isTrackingPlayer(XnUserID player);
	bool isTracking();
	void setID(XnUserID nId);
	XnUserID getActiveID();
	const XnLabel * getPixelesUsuario();
	DepthGenerator * getDepthG();
};

