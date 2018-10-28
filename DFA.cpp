#include<iostream>
#include<fstream>
#include<string>
#include<set>
#include<map>
#include<vector>
#include<cstring>
using namespace std;

ifstream input;       //输入文件
ofstream output;      //输出文件
set<char>char_set;    //字符集
set<char>state_set;   //状态集
set<char>end_state;   //接受状态集
map<char, int>change;  //下标转换
char table[100][100]; //装换表
char example[100];    //合法字符集合
int id = 0;             //下标
int min1 = 100000;      //最小状态编号
int N;                //最大长度N
int max1 = 0;           //最大状态编号
char start_state = 0;   //开始状态
vector<string>v;      //暂存转换表

bool read_and_show_DFA()
{
	 //input.open("./DFA_in1.dfa.txt");
	//input.open("./DFA_in2.dfa.txt");
	input.open("./DFA_in3.dfa.txt"); //输入文件
	output.open("./DFA_out.dfa.txt"); //输出文件
	string in, tem;
	memset(table, 0, sizeof(table));//初始化转移表
	int sum = 0;
	while (input>>in){
		int len = in.length();
		if (sum == 0)                     //读取字符集
		{
			cout << "字符集" << endl;
			output << "字符集" << endl;
			for (int x = 0; x<len; x++){
				if (char_set.count(in[x]) != 0) return 0;    //含义重复字符，DFA不正确
				char_set.insert(in[x]);                   //字符集
				cout << in[x] << " ";
				output << in[x] << " ";
			}
			cout << endl;
			output << endl;
			set<char>::iterator it = char_set.begin();
			while (it != char_set.end()){                     //下标转换
				change[*it] = id;
				id++;
				it++;
			}
		}
		else if (sum == 1)               //读取状态集
		{
			cout << "状态集" << endl;
			output << "状态集" << endl;
			for (int x = 0; x<len; x++)
			{
				if (state_set.count(in[x]) != 0) return 0;   //含有重复状态，DFA不正确
				state_set.insert(in[x]);                  //状态集
				cout << in[x] << " ";
				output << in[x] << " ";
				min1 = min1 > (in[x] - '0') ? in[x] - '0' : min1;     //最小状态编号
				max1 = max1 < (in[x] - '0') ? in[x] - '0' : max1;     //最大状态编号
			}
			cout << endl;
			output << endl;
		}
		else if (sum == 2)             //读取开始状态
		{
			if ((len != 1) || (state_set.count(in[0]) == 0)) return 0;  //开始状态不唯一或者不包含在状态集合里面，DFA不正确
			start_state = in[0];                         //开始状态
			cout << "开始状态" << endl;
			cout << in[0] << endl;
			output << "开始状态" << endl;
			output << in[0] << endl;
		}
		else if (sum == 3)          //读取接受状态集
		{
			cout << "接受状态集" << endl;
			output << "接受状态集" << endl;
			for (int x = 0; x<len; x++)
			{
				//接受状态为空或者接受状态有重复或者接受状态不在状态集合里面，DFA不正确
				if ((len == 0) || (end_state.count(in[x]) != 0) || (state_set.count(in[x]) == 0)) return 0;
				end_state.insert(in[x]);   //接受集合
				cout << in[x] << " ";
				output << in[x] << " ";
			}
			cout << endl;
			output << endl;
		}
		else              //读取状态转移关系
		{
			//状态表不满足DFA要求，DFA不正确
			if ((state_set.count(in[0]) == 0) || (state_set.count(in[2]) == 0) || (char_set.count(in[1]) == 0)) return 0;
			table[in[0] - '0'][change[in[1]]] = in[2];  //状态表
			output << in << endl;
		}
		sum++;
	}
	cout << "状态转换表" << endl;
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
{   //判断是否能识别输入字符串
	char state = start_state;
	string temp;
	cout<<"请输入识别字符串："<<endl;
	getline(cin, temp);
	int x = 0;
	while (temp[x] != '\0')
	{
		int id = change[temp[x]];
		if (char_set.count(temp[x]) == 0 && char_set.count('|') == 1)  //如果字符是other
			id = change['|'];
		state = table[state - '0'][id];
		if (state == ' ') return 0;
		x++;
	}
	if (end_state.find(state) != end_state.end())  
		return 1;
	return 0;
}


void DFA_show(int N, int x1, char sta){   //打印所有合法的规则字符串
	if (x1 == N){
		if (end_state.count(sta) != 1) return;  //如果找到最后没找到结束状态
		for (int x = 0; x < N; x++)
			cout<<example[x];
		cout<<endl;
		return;
	}
	for (int y = 0; y<id; y++)          //id是字符集数量
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
		cout << "DFA输入不正确" << endl; return 0;
	}
	cout<<"输入N的大小：";
	cin>>N;
	cout<<"所有可能的合法字符串："<<endl;
	for (int x = 1; x <= N; x++)
	{
		DFA_show(x, 0, start_state);
	}
	getchar();//吸收回车防止后面的getline出错
	if (DFA_judge())
	{
		cout<<"accept!!!!"<<endl;
	}
	else
		cout<<"error!!!!"<<endl;
	system("pause");
	return 0;
}
