#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<map>
#include<vector>
#include<cstring>
using namespace std;

ifstream input;       //�����ļ�
ofstream output;      //����ļ�
set<char>char_set;    //�ַ���
set<char>state_set;   //״̬��
set<char>end_state;   //����״̬��
map<char, int>change;  //�±�ת��
char table[100][100]; //װ����
char example[100];    //�Ϸ��ַ�����
int id = 0;             //�±�
int min1 = 100000;      //��С״̬���
int N;                //��󳤶�N
int max1 = 0;           //���״̬���
char start_state = 0;   //��ʼ״̬
vector<string>v;      //�ݴ�ת����

bool read_and_show_DFA()
{
	 //input.open("./DFA_in1.dfa.txt");
	//input.open("./DFA_in2.dfa.txt");
	input.open("./DFA_in3.dfa.txt"); //�����ļ�
	output.open("./DFA_out.dfa.txt"); //����ļ�
	string in, tem;
	memset(table, 0, sizeof(table));//��ʼ��ת�Ʊ�
	int sum = 0;
	while (input>>in){
		int len = in.length();
		if (sum == 0)                     //��ȡ�ַ���
		{
			cout << "�ַ���" << endl;
			output << "�ַ���" << endl;
			for (int x = 0; x<len; x++){
				if (char_set.count(in[x]) != 0) return 0;    //�����ظ��ַ���DFA����ȷ
				char_set.insert(in[x]);                   //�ַ���
				cout << in[x] << " ";
				output << in[x] << " ";
			}
			cout << endl;
			output << endl;
			set<char>::iterator it = char_set.begin();
			while (it != char_set.end()){                     //�±�ת��
				change[*it] = id;
				id++;
				it++;
			}
		}
		else if (sum == 1)               //��ȡ״̬��
		{
			cout << "״̬��" << endl;
			output << "״̬��" << endl;
			for (int x = 0; x<len; x++)
			{
				if (state_set.count(in[x]) != 0) return 0;   //�����ظ�״̬��DFA����ȷ
				state_set.insert(in[x]);                  //״̬��
				cout << in[x] << " ";
				output << in[x] << " ";
				min1 = min1 > (in[x] - '0') ? in[x] - '0' : min1;     //��С״̬���
				max1 = max1 < (in[x] - '0') ? in[x] - '0' : max1;     //���״̬���
			}
			cout << endl;
			output << endl;
		}
		else if (sum == 2)             //��ȡ��ʼ״̬
		{
			if ((len != 1) || (state_set.count(in[0]) == 0)) return 0;  //��ʼ״̬��Ψһ���߲�������״̬�������棬DFA����ȷ
			start_state = in[0];                         //��ʼ״̬
			cout << "��ʼ״̬" << endl;
			cout << in[0] << endl;
			output << "��ʼ״̬" << endl;
			output << in[0] << endl;
		}
		else if (sum == 3)          //��ȡ����״̬��
		{
			cout << "����״̬��" << endl;
			output << "����״̬��" << endl;
			for (int x = 0; x<len; x++)
			{
				//����״̬Ϊ�ջ��߽���״̬���ظ����߽���״̬����״̬�������棬DFA����ȷ
				if ((len == 0) || (end_state.count(in[x]) != 0) || (state_set.count(in[x]) == 0)) return 0;
				end_state.insert(in[x]);   //���ܼ���
				cout << in[x] << " ";
				output << in[x] << " ";
			}
			cout << endl;
			output << endl;
		}
		else              //��ȡ״̬ת�ƹ�ϵ
		{
			//״̬������DFAҪ��DFA����ȷ
			if ((state_set.count(in[0]) == 0) || (state_set.count(in[2]) == 0) || (char_set.count(in[1]) == 0)) return 0;
			table[in[0] - '0'][change[in[1]]] = in[2];  //״̬��
			output << in << endl;
		}
		sum++;
	}
	cout << "״̬ת����" << endl;
	map<char, int>::iterator it1 = change.begin();
	cout<<" ";
	while (it1 != change.end()){ cout<<" "<<it1->first; it1++; }
	cout<<endl;
	set<char>::iterator it = state_set.begin();
	for (int x = min1; x <= max1; x++){
		cout<<*it<<" "; it++;
		for (int y = 0; y<id; y++){
			cout<<table[x][y]<<" ";
		}
		cout << endl;
	}
	return 1;

}

bool DFA_judge()
{   //�ж��Ƿ���ʶ�������ַ���
	char state = start_state;
	string temp;
	cout<<"������ʶ���ַ�����"<<endl;
	getline(cin, temp);
	int x = 0;
	while (temp[x] != '\0')
	{
		int id = change[temp[x]];
		if (char_set.count(temp[x]) == 0 && char_set.count('|') == 1)  //����ַ���other
			id = change['|'];
		state = table[state - '0'][id];
		if (state == ' ') return 0;
		x++;
	}
	if (end_state.find(state) != end_state.end())  
		return 1;
	return 0;
}


void DFA_show(int N, int x1, char sta){   //��ӡ���кϷ��Ĺ����ַ���
	if (x1 == N){
		if (end_state.count(sta) != 1) return;  //����ҵ����û�ҵ�����״̬
		for (int x = 0; x < N; x++)
			cout<<example[x];
		cout<<endl;
		return;
	}
	for (int y = 0; y<id; y++)          //id���ַ�������
	{
		if (table[sta - '0'][y] != ' ')
		{
			map<char, int>::iterator it1 = change.begin();
			for (int z = 0; z<y; z++) it1++;
			example[x1] = it1->first;    //
			DFA_show(N, x1 + 1, table[sta - '0'][y]);
		}
	}
}
int main()
{
	if (!read_and_show_DFA())
	{
		cout << "DFA���벻��ȷ" << endl; return 0;
	}
	cout<<"����N�Ĵ�С��";
	cin>>N;
	cout<<"���п��ܵĺϷ��ַ�����"<<endl;
	for (int x = 1; x <= N; x++)
	{
		DFA_show(x, 0, start_state);
	}
	getchar();//���ջس���ֹ�����getline����
	if (DFA_judge())
	{
		cout<<"accept!!!!"<<endl;
	}
	else
		cout<<"error!!!!"<<endl;
	system("pause");
	return 0;
}
