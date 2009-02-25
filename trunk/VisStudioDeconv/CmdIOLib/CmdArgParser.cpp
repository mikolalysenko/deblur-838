#include "CmdArgParser.h"

void collectAllArgs(vector <string> &allArgv, int argc, const char *argv[])
{	
	int i = 0; 
	while (i < argc)
	{
		if (strcmp(argv[i], "-af") == 0)
		{
			i++;
			if (i == argc)
			{
				cerr << "collectAllArgs: empty argument filename after -af" << endl;
				exit(0);
			}
			appendCommandArgFromFile(allArgv, argv[i]);
			i++;
		}
		else
		{
			allArgv.push_back(string(argv[i]));
			i++;
		}
	}
}

void appendCommandArgFromFile(vector <string> &allArgv, const char *filename)
{
	ifstream ifs(filename);
	if (ifs.fail())
	{
		cerr << "appendCommandArgFromFile: fail to open " << filename << endl;
		exit(0);
	}

	string s;
	while (true)
	{
		ifs >> s;
		if (ifs.fail())
		{
			break;
		}
		allArgv.push_back(s);		
	}

	ifs.close();
}

bool extractParaFromCommandArg(bool &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size();
	for (int i = 0; i < argc; i ++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			para = true;
			return true;
		}
	}
	return false;
}

bool extractParaFromCommandArg(int &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size();
	for (int i = 0; i < argc; i ++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			sscanf(argv[i+1].c_str(), "%d", &para);
			return true;
		}
	}
	return false;
}

bool extractParaFromCommandArg(unsigned long &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size();
	for (int i = 0; i < argc; i ++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			sscanf(argv[i+1].c_str(), "%u", &para);
			return true;
		}
	}
	return false;
}

bool extractParaFromCommandArg(float &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size();
	for (int i = 0; i < argc; i ++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			sscanf(argv[i+1].c_str(), "%f", &para);				
			return true;
		}
	}
	return false;
}

bool extractParaFromCommandArg(double &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size();
	for (int i = 0; i < argc; i ++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			sscanf(argv[i+1].c_str(), "%lf", &para);				
			return true;
		}
	}
	return false;
}

bool extractParaFromCommandArg(string &para, const char flag[], const vector <string> &argv)
{
	int argc = (int)argv.size();
	for (int i = 0; i < argc; i ++)
	{
		if (strcmp(argv[i].c_str(), flag) == 0)
		{
			para = argv[i+1];
			return true;
		}
	}
	return false;
}

bool extractParaFromCommandArg(vector <string> &para, const char flag[], const vector <string> &argv)
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
				para[j] = argv[i+j];
			}
			return true;
		}
	}
	return false;
}			


