#include "pch.h"
#include "ReasoningMachine.h"

void ReasoningMachine::readRule() {
	ifstream fin;
	fin.open("rule.txt");
	if (!fin.is_open()) {
		cout << "��������ļ�ʧ��" << endl;
		return;
	}
	string line;
	vector<string> r;	// �洢ÿһ��
	Rule R;
	while (getline(fin, line, '\n'))
		r.push_back(line);

	//������ļ�����Ĺ���  ��ֶ����ÿ�й�������
	int p1, p2;
	for (int i = 0; i < r.size(); i++) {
		p1 = -1;
		p2 = -1;
		R.reason.clear();
		R.result = "";
		for (int j = 0; j < r[i].length(); j++) {
			// |ǰ�����ǰ���������ǽ��
			if (r[i][j] == '|') {
				R.reason.push_back(r[i].substr(p2 + 1, j - p2 - 1));
				R.result = r[i].substr(j + 1, r[i].size() - j - 1);
				break;
			}
			// �ո���ǰ���ķָ���
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
		cout << "����֪ʶ�ļ�ʧ��" << endl;
		return;
	}
	string line;
	while (getline(fin, line, '\n'))
		reality.push_back(line);
}
double ReasoningMachine::ratiocinate(vector<Rule> &path) {
	vector<double> conflict;	// ��ͻ��
	vector<string> preReality;	// �����һ��ƥ������ʵ��
	preReality.push_back(" ");	// �ȷ�һ��Ԫ��ʹ�õ�һ��ƥ��˳������
	bool flag = true;	// �Ƿ����ƥ��ı�־
	// ƥ��
	while (flag) {
		// ��ʵ��Ϊ�գ��˳�ƥ��
		if (reality.empty())
			break;
		// ��������ʵ������һ����ʵ����ͬʱ��˵��һ��ƥ��û��ȡ�ý�չ������˳�ƥ��
		// ע���ж�������ʵ���Ƿ����ʱҪ�Ե�ǰ��ʵ��Ϊ׼����Ϊ��ǰ��ʵ���Ĵ�С��<=��һ����ʵ���ģ��Ͳ����������Խ��
		for (int i = 0; i < reality.size(); i++) {
			// ���ֲ�ͬ���˳���鲢����ƥ��
			if (reality[i] != preReality[i])
				break;
			// �Ѿ���鵽���һ����ÿ����ͬ���˳�ƥ��
			else if (i == reality.size() - 1)
				flag = false;
		}
		// ��ÿ��ƥ��֮ǰ�����ε���ʵ��д���ϴε���ʵ����
		preReality.clear();
		for (int i = 0; i < reality.size(); i++)
			preReality.push_back(reality[i]);
		// ƥ��
		for (int i = 0; i < rule.size(); i++) { // iΪ�������
			vector<int> resn;	// �洢һ������ĸ���ǰ����ƥ�����
			// �ȶ���ƥ�䣬��-1
			for (int k = 0; k < rule[i].reason.size(); k++)
				resn.push_back(-1);
			int cnt = 0;	// ͬһ��ǰ���Ĳ�ͬ������ҵ�Ҫ���ƥ��ɹ�����
			// jΪ��ʵ���������� ������ʵ���е�ÿһ��
			for (int j = 0; j < reality.size(); j++) {
				for (int k = 0; k < rule[i].reason.size(); k++) {
					// ƥ�䵽������Ϊ0
					if (rule[i].reason[k] == reality[j])
						resn[k] = 0;
				}
			}
			for (int k = 0; k < rule[i].reason.size(); k++)
				cnt += resn[k];
			// f == 0��ʾû��һ����ƥ�䣨��-1����
			if (cnt == 0) {	// ƥ��ɹ�
				// ����·����
				path.push_back(rule[i]);
				// ����˹���Ľ����Ȩֵ��������ͻ��
				if (rule[i].result[0] == '-' || rule[i].result[0] == '+')
					conflict.push_back(stod(rule[i].result));
				// �����������ʵ��
				else
					reality.push_back(rule[i].result);
			}
			// ɾ������ƥ�����ʵ��
			for (int k = 0; k < rule[i].reason.size(); k++) {
				for (int j = 0; j < reality.size(); j++) {
					if (reality[j] == rule[i].reason[k])
						reality.erase(reality.begin() + j);
				}
			}

		}
	}

	// ��ͻ����
	double skipclassWeight = -1;	// Ȩֵ
	// ��Ȩֵ�����ۼӣ��õ����ս��
	for (double x : conflict)
		skipclassWeight += x;
	return skipclassWeight;
}