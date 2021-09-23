#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

typedef struct Rule {
	vector<string> reason;	// ǰ��
	string result = "";	// ���
};

class ReasoningMachine {
private:
	vector<Rule> rule;
	vector<string> reality;
public:
	void readRule();
	void readReality();
	double ratiocinate(vector<Rule> &path);	// ����
};