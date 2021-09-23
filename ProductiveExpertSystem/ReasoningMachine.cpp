#include "pch.h"
#include "ReasoningMachine.h"

void ReasoningMachine::readRule() {
	ifstream fin;
	fin.open("rule.txt");
	if (!fin.is_open()) {
		cout << "读入规则文件失败" << endl;
		return;
	}
	string line;
	vector<string> r;	// 存储每一行
	Rule R;
	while (getline(fin, line, '\n'))
		r.push_back(line);

	//处理从文件读入的规则  拆分读入的每行规则数据
	int p1, p2;
	for (int i = 0; i < r.size(); i++) {
		p1 = -1;
		p2 = -1;
		R.reason.clear();
		R.result = "";
		for (int j = 0; j < r[i].length(); j++) {
			// |前面的是前件，后面是结果
			if (r[i][j] == '|') {
				R.reason.push_back(r[i].substr(p2 + 1, j - p2 - 1));
				R.result = r[i].substr(j + 1, r[i].size() - j - 1);
				break;
			}
			// 空格是前件的分隔符
			else if (r[i][j] == ' ') {
				p1 = p2;
				p2 = j;
				R.reason.push_back(r[i].substr(p1 + 1, p2 - p1 - 1));
			}
		}
		rule.push_back(R);
	}
}
void ReasoningMachine::readReality() {
	ifstream fin;
	fin.open("reality.txt");
	if (!fin.is_open()) {
		cout << "读入知识文件失败" << endl;
		return;
	}
	string line;
	while (getline(fin, line, '\n'))
		reality.push_back(line);
}
double ReasoningMachine::ratiocinate(vector<Rule> &path) {
	vector<double> conflict;	// 冲突集
	vector<string> preReality;	// 存放上一次匹配后的事实集
	preReality.push_back(" ");	// 先放一个元素使得第一次匹配顺利进行
	bool flag = true;	// 是否继续匹配的标志
	// 匹配
	while (flag) {
		// 事实集为空，退出匹配
		if (reality.empty())
			break;
		// 当本次事实集和上一次事实集相同时，说明一轮匹配没有取得进展，因此退出匹配
		// 注意判断两次事实集是否相等时要以当前事实集为准，因为当前事实集的大小是<=上一次事实集的，就不会出现数组越界
		for (int i = 0; i < reality.size(); i++) {
			// 出现不同，退出检查并继续匹配
			if (reality[i] != preReality[i])
				break;
			// 已经检查到最后一个且每次相同，退出匹配
			else if (i == reality.size() - 1)
				flag = false;
		}
		// 在每次匹配之前将本次的事实集写到上次的事实集中
		preReality.clear();
		for (int i = 0; i < reality.size(); i++)
			preReality.push_back(reality[i]);
		// 匹配
		for (int i = 0; i < rule.size(); i++) { // i为规则序号
			vector<int> resn;	// 存储一个规则的各个前件的匹配情况
			// 先都不匹配，即-1
			for (int k = 0; k < rule[i].reason.size(); k++)
				resn.push_back(-1);
			int cnt = 0;	// 同一个前件的不同情况的且的要求的匹配成功数量
			// j为事实库的条件序号 遍历事实库中的每一条
			for (int j = 0; j < reality.size(); j++) {
				for (int k = 0; k < rule[i].reason.size(); k++) {
					// 匹配到了则置为0
					if (rule[i].reason[k] == reality[j])
						resn[k] = 0;
				}
			}
			for (int k = 0; k < rule[i].reason.size(); k++)
				cnt += resn[k];
			// f == 0表示没有一个不匹配（即-1）的
			if (cnt == 0) {	// 匹配成功
				// 加入路径集
				path.push_back(rule[i]);
				// 如果此规则的结果是权值，则放入冲突集
				if (rule[i].result[0] == '-' || rule[i].result[0] == '+')
					conflict.push_back(stod(rule[i].result));
				// 否则将其加入事实集
				else
					reality.push_back(rule[i].result);
			}
			// 删除本次匹配的事实集
			for (int k = 0; k < rule[i].reason.size(); k++) {
				for (int j = 0; j < reality.size(); j++) {
					if (reality[j] == rule[i].reason[k])
						reality.erase(reality.begin() + j);
				}
			}

		}
	}

	// 冲突消解
	double skipclassWeight = -1;	// 权值
	// 把权值进行累加，得到最终结果
	for (double x : conflict)
		skipclassWeight += x;
	return skipclassWeight;
}