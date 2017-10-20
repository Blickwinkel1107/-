#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

enum classes //�γ�ö�����ͣ�����������ʳɼ�����
{
	Ӣ��,����,
	���ݽṹ,����ϵͳ,�����ϵͳ,
	�������,����˼ά,����ԭ��,�������,��Ϣ��ȫ,���������,���ݿ����,�����ھ�,��ý�弼��,�˹�����
};

vector<string> info //�γ������飬���ڱ���������ʲ���ӡ�γ���
{
	"Ӣ��","����",
	"���ݽṹ","����ϵͳ","�����ϵͳ",
	"�������","����˼ά","����ԭ��","�������","��Ϣ��ȫ","���������","���ݿ����","�����ھ�","��ý�弼��","�˹�����"
};

struct TMP	//����ĳ�γ̳ɼ���ʱ���ڽ�����Ϣ�Ľṹ��
{
	string number;	//ѧ��
	string name;	//����
	int score;	//�γ̷���
	int pos;	//�γ�����ö�٣�
	TMP(string,string,int,int);	//ѧ��+����+�γ̷���+�γ�����ö�٣��Ĺ��캯��
};

struct STU	//ѧ����Ϣ�ṹ��
{
	string number;	//ѧ��
	string name;	//����
	vector<int> score;	//���Ƴɼ�
	double gpa;	//ѧ�ּ�
	int credit;	//ѧ��
	int rank;	//����
	STU()=default; //Ĭ�Ϲ��캯��
	STU(string,string);	//ѧ��+�����Ĺ��캯��
	STU(string,string,int,int); //ѧ��+����+�γ̷���+�γ�����ö�٣��Ĺ��캯��
	void merge(TMP&); //����ɼ�
	void calc(); //����ѧ�ּ���ѧ��
};

vector<STU> list_14; //14��ѧ���ɼ����ܱ�
vector<STU> list_15; //15��ѧ���ɼ����ܱ�
string fname; //��ǰ���ļ����ļ���

void OutCredit(int);	//����int A����ӡA�꼶ѧ���ɼ���������d://A�꼶�ɼ�����.txt���ļ�
void ReadIn();	//���������ı��ļ������ɼ����ϵĴ��󣬿����󱣴���꼶ѧ�����������ܳɼ�
bool CheckUp(vector<TMP>&);	//����ĳ�γɼ������飬��������ʾУ�������д�������ԭ�ɼ����ļ�
void Refresh(vector<TMP>&,string);	//����ԭ�ɼ����ļ�
void Asm(int);	//����int A����ӡA�꼶ѧ�����ܳɼ�����d://A�꼶�ɼ�����.txt���ļ�
void VerifyA(); //��ѯ�ɼ���
void VerifyB(); //��ѯĳ�γ��޳ɼ���ѧ������
void VerifyC(); //��ѯѧ�ֲ���27��ѧ������


int main()
{
	ReadIn();
	Asm(14);
	Asm(15);
	OutCredit(14);
	OutCredit(15);
	cout<<string(10,'-')<<"��ѯ����"<<string(10,'-')<<endl;
	while(true)
	{
		bool ed=false;
		char choice;
		cout<<"����Ҫ��ʲô��"<<endl
			<<"a.��ѯ�ɼ���"<<endl
			<<"b.��ѯĳ�γ��޳ɼ���ѧ������"<<endl
			<<"c.��ѯѧ�ֲ���27��ѧ������"<<endl
			<<"d.�˳�����"<<endl
			<<"�������������";
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
		filename="d://14���ɼ�����.txt";
	}
	else
	{
		p=&list_15;
		filename="d://15���ɼ�����.txt";
	}
	vector<STU>& tbl=*p;
	fout.open(filename);
	for(int i=0;i<tbl.size();i++)
		tbl[i].calc();
	sort(tbl.begin(),tbl.end(),[](STU a,STU b){ return (a.gpa-b.gpa)>=1e-6; });
	fout<<setw(9)<<"ѧ��"<<setw(11)<<"����"<<setw(11)<<"ѧ�ּ�����"<<setw(9)<<"ѧ�ּ�"<<endl;
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

	fin.open("d://14��ѧ������.txt");
	while(fin>>number>>name)
		list_14.push_back(STU(number,name));
	fin.close();

	fin.open("d://15��ѧ������.txt");
	while(fin>>number>>name)
		list_15.push_back(STU(number,name));
	fin.close();

	for(int i=0;i<info.size();i++)
	{
		string filename;
		string number,name;
		vector<TMP> tmp;
		int score;

		if(Ӣ��<=i && i<=����)
		{
			fname=string("14��"+info[i]+"�ɼ�");
			filename="d://"+fname+".txt";
			fin.open(filename);
			while(fin>>number>>name>>score)
				tmp.push_back(TMP(number,name,score,i));
			fin.close();
			if(!CheckUp(tmp))
				Refresh(tmp,filename);
			tmp.clear(); //���
			fname=string("15��"+info[i]+"�ɼ�");
		}
		else
			fname=string(info[i]+"�ɼ�");
		filename="d://"+fname+".txt";
		fin.open(filename);
		while(fin>>number>>name>>score)
			tmp.push_back(TMP(number,name,score,i));
		fin.close();
		if(!CheckUp(tmp))
			Refresh(tmp,filename);
		tmp.clear(); //���
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
			cout<<fname<<".txt��"<<endl
				<<"δ�ҵ�ѧ��Ϊ"<<num<<",����Ϊ"<<name<<"��ѧ����"<<endl
				<<"����ҪУ���ɼ����ڸ�ѧ������Ϣ��"<<endl
				<<"a.ɾ����ѧ����Ϣ"<<endl
				<<"b.�޸ĸ�ѧ����ѧ�Ż�������"<<endl
				<<"�������������";
			cin>>choice;
			if(choice=='a')
			{
				it=vec.erase(it);
				cout<<"ɾ���ɹ���"<<endl<<endl;
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
					cout<<"��������ͬѧ�Ż�������ѧ�����Զ�ɾ��"<<endl
						<<"ɾ���ɹ���"<<endl<<endl;
					it=vec.erase(it);
					continue;
				}
				cout<<"Ϊ���Ƽ��޸�Ϊ���µ�ѧ����Ϣ"<<endl
					<<"ѡ��󣬸�ѧ����Ϣ�����ԭ�еĴ���ѧ����Ϣ��"<<endl;
				if(index1!=-1)
				{
					cout<<"ѧ����ͬ��������ͬ��ѧ����"<<endl;
					for(int i=0;i<=index1;i++)
						cout<<i+1<<".ѧ�ţ�"<<t[i].number<<"  ������"<<t[i].name<<endl;
				}
				if(index2!=index1)
				{
					cout<<"������ͬ��ѧ�Ų�ͬ��ѧ����"<<endl;
					for(int i=index1+1;i<=index2;i++)
						cout<<i+1<<".ѧ�ţ�"<<t[i].number<<"  ������"<<t[i].name<<endl;
				}
				cout<<"�������Ƽ�ѧ������ţ�";
				cin>>choice;
				choice--;
				STU& tar=t[choice];
				findit=lower_bound(_list.begin(),_list.end(),tar,[](STU a,STU b){	return a.number<b.number;	});
				findit->merge(*it);
				it->name=tar.name;
				it->number=tar.number;
				++it;
				cout<<"�޸ĳɹ���"<<endl<<endl;
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
		filename="d://14���ɼ�����.txt";
		p=&list_14;
	}
	else
	{
		filename="d://15���ɼ�����.txt";
		p=&list_15;
	}
	vector<STU>& table=*p;
	fout.open(filename);
	fout<<setw(9)<<"ѧ��"<<setw(11)<<"����";
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
	cout<<"\n�����������ѯ��ѧ�Ż�������";
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
		cout<<"�޸�ѧ����"<<endl;
		return;
	}
	for(auto it=t.begin();it!=t.end();++it)
	{
		cout<<"ѧ��:"<<it->number<<"   ����:"<<it->name<<endl
			<<setw(11)<<"�γ���"<<setw(10)<<"�γ̳ɼ�"<<setw(10)<<"�γ�ѧ��"<<endl;
		for(auto p=it->score.begin();p!=it->score.end();++p)
		{
			int index=p-(it->score.begin()),t=0;
			if(Ӣ��<=index && index<=����)
				t=4;
			else if(index<=�����ϵͳ)
				t=3;
			else
				t=2;
			cout<<setw(11)<<info[index]<<setw(10)<<*p<<setw(10)<<t<<endl;
		}
		cout<<"������ѧ��:"<<it->credit<<"  �꼶����:"<<it->rank<<endl;
	}
}

void VerifyB()
{
	int grade;
	string classname;
	vector<STU>* p=&list_15;
	cout<<"�������꼶��14��15��:";
	cin>>grade;
	if(grade==14)
		p=&list_14;
	vector<STU>& tbl=*p;
	vector<STU> t;
	cout<<"������γ�����";
	cin>>classname;
	int index=find(info.begin(),info.end(),classname)-info.begin();
	for(auto& x:tbl)
	{
		if(x.score[index]<60)
			t.push_back(x);
	}
	if(t.size()==0)
	{
		cout<<"����ѧ�����иÿγ�ѧ�֣�"<<endl;
		return;
	}
	cout<<setw(9)<<"ѧ��"<<string(5,' ')<<"����"<<endl;
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
		cout<<"��ѧ�ֲ���27��ѧ����"<<endl;
		return ;
	}
	cout<<setw(9)<<"ѧ��"<<string(5,' ')<<"����"<<endl;
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
	for(int i=Ӣ��;i<=�˹�����;i++)
	{
		if(score[i]>=60)
		{
			if(Ӣ��<=i && i<=����)
				credit+=4;
			else if(i<=�����ϵͳ)
				credit+=3;
			else
				credit+=2;
		}
	}
	for(int i=Ӣ��;i<=�����ϵͳ;i++)
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
