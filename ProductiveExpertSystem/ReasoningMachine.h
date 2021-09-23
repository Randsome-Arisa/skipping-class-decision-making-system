#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

typedef struct Rule {
	vector<string> reason;	// 前件
	string result = "";	// 后件
};

class ReasoningMachine {
private:
	vector<Rule> rule;
	vector<string> reality;
public:
	void readRule();
	void readReality();
	double ratiocinate(vector<Rule> &path);	// 推理
};