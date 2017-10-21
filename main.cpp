#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

enum classes //课程枚举类型，便于随机访问成绩数组
{
	英语,政治,
	数据结构,操作系统,计算机系统,
	程序设计,计算思维,编译原理,软件工程,信息安全,计算机网络,数据库概论,数据挖掘,多媒体技术,人工智能
};

vector<string> info //课程名数组，便于便于随机访问并打印课程名 and fuck you!!!!
{
	"英语","政治",
	"数据结构","操作系统","计算机系统",
	"程序设计","计算思维","编译原理","软件工程","信息安全","计算机网络","数据库概论","数据挖掘","多媒体技术","人工智能"
};

struct TMP	//读入某课程成绩单时用于接收信息的结构体fuck!
{
	string number;	//123123123
	string name;	//姓名
	int score;	//课程分数
	int pos;	//课程名（枚举）
	TMP(string,string,int,int);	//学号+姓名+课程分数+课程名（枚举）的构造函数
};

struct STU	//学生信息结构体
{
	string number;	//学号
	string name;	//姓名
	vector<int> score;	//各科成绩
	double gpa;	//学分绩
	int credit;	//学分
	int rank;	//排名
	STU()=default; //默认构造函数
	STU(string,string);	//学号+姓名的构造函数
	STU(string,string,int,int); //学号+姓名+课程分数+课程名（枚举）的构造函数
	void merge(TMP&); //导入成绩
	void calc(); //计算学分绩、学分
};

vector<STU> list_14; //14级学生成绩汇总表
vector<STU> list_15; //15级学生成绩汇总表
string fname; //当前打开文件的文件名

void OutCredit(int);	//接收int A，打印A年级学生成绩排名至“d://A年级成绩排名.txt”文件
void ReadIn();	//读入所有文本文件，检查成绩单上的错误，勘正后保存各年级学生名单并汇总成绩
bool CheckUp(vector<TMP>&);	//接收某课成绩单数组，检查错误并提示校正，若有错误会更新原成绩单文件
void Refresh(vector<TMP>&,string);	//更新原成绩单文件
void Asm(int);	//接收int A，打印A年级学生汇总成绩至“d://A年级成绩汇总.txt”文件
void VerifyA(); //查询成绩单
void VerifyB(); //查询某课程无成绩的学生名单
void VerifyC(); //查询学分不够27的学生名单


int main()
{
	ReadIn();
	Asm(14);
	Asm(15);
	OutCredit(14);
	OutCredit(15);
	cout<<string(10,'-')<<"查询功能"<<string(10,'-')<<endl;
	while(true)
	{
		bool ed=false;
		char choice;
		cout<<"您想要做什么？"<<endl
			<<"a.查询成绩单"<<endl
			<<"b.查询某课程无成绩的学生名单"<<endl
			<<"c.查询学分不够27的学生名单"<<endl
			<<"d.退出程序"<<endl
			<<"请输入操作符：";
		cin>>choice;
		cout<<string(50,'-')<<endl;
		switch(choice)
		{
		case 'a':VerifyA();break;
		case 'b':VerifyB();break;
		case 'c':VerifyC();break;
		case 'd':ed=true;break;
		}
		cout<<string(50,'-')<<endl;
		if(ed)
			break;
	}
	system("pause");
	return 0;
}

void OutCredit(int grade)
{
	ofstream fout;
	string filename;
	vector<STU>* p;
	if(grade==14)
	{
		p=&list_14;
		filename="d://14级成绩排名.txt";
	}
	else
	{
		p=&list_15;
		filename="d://15级成绩排名.txt";
	}
	vector<STU>& tbl=*p;
	fout.open(filename);
	for(int i=0;i<tbl.size();i++)
		tbl[i].calc();
	sort(tbl.begin(),tbl.end(),[](STU a,STU b){ return (a.gpa-b.gpa)>=1e-6; });
	fout<<setw(9)<<"学号"<<setw(11)<<"姓名"<<setw(11)<<"学分绩排名"<<setw(9)<<"学分绩"<<endl;
	int step=0;
	for(auto it=tbl.begin();it!=tbl.end();++it)
	{
		step++;
		if(it!=tbl.begin() && fabs(it->gpa-(it-1)->gpa)<=1e-6)
			it->rank=(it-1)->rank;
		else
			it->rank=step;
		fout.setf(ios::fixed);
		fout<<setw(9)<<it->number<<setw(11)<<it->name<<setw(11)<<it->rank<<setw(9)<<setprecision(2)<<it->gpa<<endl;
	}
	fout.close();
}

void ReadIn()
{
	ifstream fin;
	string number,name;
	bool perfect=false;

	list_14.reserve(100);
	list_15.reserve(100);

	fin.open("d://14级学生名单.txt");
	while(fin>>number>>name)
		list_14.push_back(STU(number,name));
	fin.close();

	fin.open("d://15级学生名单.txt");
	while(fin>>number>>name)
		list_15.push_back(STU(number,name));
	fin.close();

	for(int i=0;i<info.size();i++)
	{
		string filename;
		string number,name;
		vector<TMP> tmp;
		int score;

		if(英语<=i && i<=政治)
		{
			fname=string("14级"+info[i]+"成绩");
			filename="d://"+fname+".txt";
			fin.open(filename);
			while(fin>>number>>name>>score)
				tmp.push_back(TMP(number,name,score,i));
			fin.close();
			if(!CheckUp(tmp))
				Refresh(tmp,filename);
			tmp.clear(); //清空
			fname=string("15级"+info[i]+"成绩");
		}
		else
			fname=string(info[i]+"成绩");
		filename="d://"+fname+".txt";
		fin.open(filename);
		while(fin>>number>>name>>score)
			tmp.push_back(TMP(number,name,score,i));
		fin.close();
		if(!CheckUp(tmp))
			Refresh(tmp,filename);
		tmp.clear(); //清空
	}
}

bool CheckUp(vector<TMP>& vec)
{
	auto it=vec.begin();
	bool flag=true;
	while(it!=vec.end())
	{
		string &num=it->number,&name=it->name;
		vector<STU>* p=&list_15;
		STU stu(it->number,it->name);

		if(num[3]=='4')
			p=&list_14;
		vector<STU>& _list=*p;
		auto check=lower_bound(_list.begin(),_list.end(),stu,[](STU a,STU b){	return a.number<b.number; });
		if(check->number==num && check->name==name)
		{
			check->merge(*it);
			++it;
			continue;
		}
		else
		{
			flag=false;
			char choice;
			cout<<fname<<".txt中"<<endl
				<<"未找到学号为"<<num<<",姓名为"<<name<<"的学生！"<<endl
				<<"您需要校正成绩单内该学生的信息！"<<endl
				<<"a.删除该学生信息"<<endl
				<<"b.修改该学生的学号或者姓名"<<endl
				<<"请输入操作符：";
			cin>>choice;
			if(choice=='a')
			{
				it=vec.erase(it);
				cout<<"删除成功！"<<endl<<endl;
				continue;
			}
			else
			{
				int choice;
				vector<STU> t;
				int count=0;
				int index1=-1,index2=-1;

				auto findit=lower_bound(_list.begin(),_list.end(),stu,[](STU a,STU b){return a.number<b.number;});
				if(findit->number==num)
				{
					t.push_back(*findit);
					index1++;
				}
				int size=_list.size();
				int l(0),r(size-1),m(size/2-1+size%2);
				while(r-l>0 && _list[m].name!=name)
				{
					name<_list[m].name? r=m-1:l=m+1;
					m=(l+r)/2;
				}
				while(_list[m].name==name && m-1>=0 && _list[m-1].name==name)
					m--;
				while(_list[m].name==name)
				{
					t.push_back(_list[m]);
					index2++;
					if(m+1<size && _list[m+1].name==name)
						m++;
					else
						break;
				}
				index2+=index1+1;
				if(index1==-1 && index2==-1)
				{
					cout<<"不存在相同学号或姓名的学生，自动删除"<<endl
						<<"删除成功！"<<endl<<endl;
					it=vec.erase(it);
					continue;
				}
				cout<<"为您推荐修改为如下的学生信息"<<endl
					<<"选择后，该学生信息将替代原有的错误学生信息："<<endl;
				if(index1!=-1)
				{
					cout<<"学号相同，姓名不同的学生："<<endl;
					for(int i=0;i<=index1;i++)
						cout<<i+1<<".学号："<<t[i].number<<"  姓名："<<t[i].name<<endl;
				}
				if(index2!=index1)
				{
					cout<<"姓名相同，学号不同的学生："<<endl;
					for(int i=index1+1;i<=index2;i++)
						cout<<i+1<<".学号："<<t[i].number<<"  姓名："<<t[i].name<<endl;
				}
				cout<<"请输入推荐学生的序号：";
				cin>>choice;
				choice--;
				STU& tar=t[choice];
				findit=lower_bound(_list.begin(),_list.end(),tar,[](STU a,STU b){	return a.number<b.number;	});
				findit->merge(*it);
				it->name=tar.name;
				it->number=tar.number;
				++it;
				cout<<"修改成功！"<<endl<<endl;
			}
		}
	}
	if(!flag)
		return false;
	else
		return true;
}

void Refresh(vector<TMP>& tmp,string filename)
{
	ofstream fout(filename);
	for(auto& x:tmp)
		fout<<x.number<<"\t"<<x.name<<"\t"<<x.score<<endl;
	fout.close();
}

void Asm(int grade)
{
	ofstream fout;
	string filename;
	vector<STU>* p;
	if(grade==14)
	{
		filename="d://14级成绩汇总.txt";
		p=&list_14;
	}
	else
	{
		filename="d://15级成绩汇总.txt";
		p=&list_15;
	}
	vector<STU>& table=*p;
	fout.open(filename);
	fout<<setw(9)<<"学号"<<setw(11)<<"姓名";
	for(int i=0;i<info.size();i++)
	{
		fout<<setw(2+info[i].size())<<info[i];
	}
	fout<<endl;
	for(auto& x:table)
	{
		fout<<setw(9)<<x.number<<setw(11)<<x.name;
		for(int i=0;i<info.size();i++)
		{
			fout<<setw(2+info[i].size());
			if(x.score[i]<60)
				fout<<0;
			else
				fout<<x.score[i];
		}
		fout<<endl;
	}
}

void VerifyA()
{
	string input;
	cout<<"\n请输入您想查询的学号或姓名：";
	cin>>input;
	vector<STU> t;
	for(auto& x:list_14)
	{
		if(x.name==input || x.number==input)
			t.push_back(x);
	}
	for(auto& x:list_15)
	{
		if(x.name==input || x.number==input)
			t.push_back(x);
	}
	if(t.size()==0)
	{
		cout<<"无该学生！"<<endl;
		return;
	}
	for(auto it=t.begin();it!=t.end();++it)
	{
		cout<<"学号:"<<it->number<<"   姓名:"<<it->name<<endl
			<<setw(11)<<"课程名"<<setw(10)<<"课程成绩"<<setw(10)<<"课程学分"<<endl;
		for(auto p=it->score.begin();p!=it->score.end();++p)
		{
			int index=p-(it->score.begin()),t=0;
			if(英语<=index && index<=政治)
				t=4;
			else if(index<=计算机系统)
				t=3;
			else
				t=2;
			cout<<setw(11)<<info[index]<<setw(10)<<*p<<setw(10)<<t<<endl;
		}
		cout<<"已修总学分:"<<it->credit<<"  年级排名:"<<it->rank<<endl;
	}
}

void VerifyB()
{
	int grade;
	string classname;
	vector<STU>* p=&list_15;
	cout<<"请输入年级（14或15）:";
	cin>>grade;
	if(grade==14)
		p=&list_14;
	vector<STU>& tbl=*p;
	vector<STU> t;
	cout<<"请输入课程名：";
	cin>>classname;
	int index=find(info.begin(),info.end(),classname)-info.begin();
	for(auto& x:tbl)
	{
		if(x.score[index]<60)
			t.push_back(x);
	}
	if(t.size()==0)
	{
		cout<<"所有学生都有该课程学分！"<<endl;
		return;
	}
	cout<<setw(9)<<"学号"<<string(5,' ')<<"姓名"<<endl;
	for(auto& x:t)
		cout<<x.number<<string(5,' ')<<x.name<<endl;
}

void VerifyC()
{
	vector<STU> t;
	for(auto& x:list_14)
	{
		if(x.credit<27)
			t.push_back(x);
	}
	for(auto& x:list_15)
	{
		if(x.credit<27)
			t.push_back(x);
	}
	if(t.size()==0)
	{
		cout<<"无学分不够27的学生！"<<endl;
		return ;
	}
	cout<<setw(9)<<"学号"<<string(5,' ')<<"姓名"<<endl;
	for(auto& x:t)
		cout<<x.number<<string(5,' ')<<x.name<<endl;
}

STU::STU(string _number,string _name):
	number(_number),
	name(_name),
	score(15,0),
	credit(0),
	rank(0),
	gpa(0.0)
{}

STU::STU(string _number,string _name,int _score,int _pos):
	STU(_number,_name)
{
	score[_pos]=_score;
}

void STU::merge(TMP& b)
{
	score[b.pos]=b.score;
	if(b.score<60)
		score[b.pos]=0;
}

void STU::calc()
{
	int sum=0,count=0;
	for(int i=英语;i<=人工智能;i++)
	{
		if(score[i]>=60)
		{
			if(英语<=i && i<=政治)
				credit+=4;
			else if(i<=计算机系统)
				credit+=3;
			else
				credit+=2;
		}
	}
	for(int i=英语;i<=计算机系统;i++)
		sum+=score[i];
	int op[12]={};
	copy(score.begin()+5,score.end(),op);
	sort(op,op+10,[](int a,int b){ return a>b; });
	for(int i=0;op[i]!=0 && i<5;i++)
		sum+=op[i];
	gpa=double(sum)/10;
}

TMP::TMP(string a,string b,int c,int d):
	number(a),
	name(b),
	score(c),
	pos(d)
{}
