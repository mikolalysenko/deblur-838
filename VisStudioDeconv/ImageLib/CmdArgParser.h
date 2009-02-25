#ifndef ComArgParser_h
#define ComArgParser_h

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;
using std::stringstream;

#include "../MatrixLib/TVecMat.h"

void collectAllArgs(vector <string> &allArgv, int argc, const char *argv[]);
void appendCommandArgFromFile(vector <string> &allArgv, const char *filename);

bool extractParaFromCommandArg(bool &para, const char flag[], const vector <string> &argv);
bool extractParaFromCommandArg(int &para, const char flag[], const vector <string> &argv);
bool extractParaFromCommandArg(unsigned long &para, const char flag[], const vector <string> &argv);
bool extractParaFromCommandArg(float &para, const char flag[], const vector <string> &argv);
bool extractParaFromCommandArg(double &para, const char flag[], const vector <string> &argv);
bool extractParaFromCommandArg(string &para, const char flag[], const vector <string> &argv);
bool extractParaFromCommandArg(vector <string> &para, const char flag[], const vector <string> &argv);

template <int N, class floating>
bool extractParaFromCommandArg(Vec<N, floating> &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size();
	for (int i = 0; i < argc; i ++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			i++;
			if (i+N > argc)
			{
				cerr << "Not enough arguments for " << flag << endl;
				return false;
			}
			for (int j = 0; j < N; j++)
			{			
				stringstream ss;
				ss << argv[i+j];
				ss >> para[j];
			}
			return true;
		}
	}
	return false;
}

template <int D, class floating>
bool extractParaFromCommandArg(vector < Vec<D,floating> > &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size(); 
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			i++;
			int N = 0;
			stringstream ss;
			ss << argv[i];
			ss >> N;
			i++;
			if (i+N*D>argc)
			{
				cerr << "Not enough arguments for " << flag << endl;
				return false;
			}
			para.resize(N);
			for (int j = 0; j<N; j++)
			{		
				for (int d = 0; d < D; ++d)
				{
					stringstream ss;
					ss << argv[i+j*D+d];
					ss >> para[j][d];
				}
			}
			return true;
		}
	}
	return false;
}			

template <class floating>
bool extractParaFromCommandArg(vector < floating > &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size(); 
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			i++;
			int N = 0;
			stringstream ss;
			ss << argv[i];
			ss >> N;
			i++;
			if (i+N>argc)
			{
				cerr << "Not enough arguments for " << flag << endl;
				return false;
			}
			para.resize(N);
			for (int j = 0; j<N; j++)
			{		
				stringstream ss;
				ss << argv[i+j];
				ss >> para[j];
			}
			return true;
		}
	}
	return false;
}			

template <class T>
ostream & operator << (ostream & cout, const vector <T> &v)
{
	int N = (int)v.size();
	cout << N;
	for (int i = 0; i < N; i++) cout << endl << v[i]; 
	return cout;
}

template <class T>
bool extractUnknownParaFromCommandArg(T &para, const char name[], const char flag[], const vector <string> &argv)
{
	if (extractParaFromCommandArg(para, flag, argv))
	{
		cout << name << ": " << para << endl;
		return true;
	}
	else
	{
		cout << name << " not provided, terminated!" << endl;
		return false;
	}
}

template <class T>
void extractDefaultParaFromCommandArg(T &para, const char name[], const char flag[], const vector <string> &argv)
{
	if (extractParaFromCommandArg(para, flag, argv))
	{
		cout << name << ": " << para << endl;
	}
	else
	{
		cout << name << " not provided, use default value: " << para << endl;
	}
}

#endif
