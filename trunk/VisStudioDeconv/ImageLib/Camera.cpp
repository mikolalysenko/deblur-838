#include <cstdio>
#include <cfloat>

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

#include "CmdArgParser.h"
#include "Camera.h"

void Camera::Parameter2Matrix(void)
{
	_intrinsicMat[0][0] = _focalLength[0];
	_intrinsicMat[1][1] = _focalLength[1];

	_intrinsicMat[0][1] = _skew * _focalLength[0];
	_intrinsicMat[1][0] = 0.0;

	_intrinsicMat[0][2] = _principalPnt[0];
	_intrinsicMat[1][2] = _principalPnt[1];

	_intrinsicMat[2].makeunit(2);
}

void Camera::Matrix2Parameter(void)
{
	_focalLength[0] = _intrinsicMat[0][0];
	_focalLength[1] = _intrinsicMat[1][1];

	_skew = _intrinsicMat[0][1] / _intrinsicMat[0][0];

	_principalPnt[0] = _intrinsicMat[0][2];
	_principalPnt[1] = _intrinsicMat[1][2];
}

void Camera::LoadFromFile(const char name[])
{
	vector<string> allArgv;

	appendCommandArgFromFile(allArgv, name);

	if (!extractUnknownParaFromCommandArg(_focalLength, "focal length", "-focal", allArgv)) exit(0);
	
	if (!extractUnknownParaFromCommandArg(_principalPnt, "principal point", "-principal", allArgv)) exit(0);
	
	_skew = 0;
	extractDefaultParaFromCommandArg(_skew, "skew", "-skew", allArgv);

	_distortion.makezero();
	extractDefaultParaFromCommandArg(_distortion, "distortion", "-distortion", allArgv);

	Vec<9,double> tmpRot;
	tmpRot[0] = 1; tmpRot[1] = 0; tmpRot[2] = 0;
	tmpRot[3] = 0; tmpRot[4] = 1; tmpRot[5] = 0;
	tmpRot[6] = 0; tmpRot[7] = 0; tmpRot[8] = 1;
	extractDefaultParaFromCommandArg(tmpRot, "rotation", "-rotation", allArgv);
	_rotation[0][0] = tmpRot[0]; _rotation[0][1] = tmpRot[1]; _rotation[0][2] = tmpRot[2]; 
	_rotation[1][0] = tmpRot[3]; _rotation[1][1] = tmpRot[4]; _rotation[1][2] = tmpRot[5]; 
	_rotation[2][0] = tmpRot[6]; _rotation[2][1] = tmpRot[7]; _rotation[2][2] = tmpRot[8]; 
	
	_translation.makezero();
	extractDefaultParaFromCommandArg(_translation, "translation", "-translation", allArgv);
}

void Camera::SaveToFile(const char name[])
{
	FILE *fp = fopen(name, "w");
	if (fp==NULL)
	{
		printf("Camera::SaveToFile fail to open %s!\n", name);
		exit(0);
	}

	fprintf(fp, "-focal %lf %lf\n", _focalLength[0], _focalLength[1]);
	fprintf(fp, "-principal %lf %lf\n", _principalPnt[0], _principalPnt[1]);
	fprintf(fp, "-skew %lf\n", _skew);
	fprintf(fp, "-distortion %lf %lf %lf %lf %lf\n", 
		_distortion[0], _distortion[1], _distortion[2], _distortion[3], _distortion[4]);
	

	fclose(fp);
}

