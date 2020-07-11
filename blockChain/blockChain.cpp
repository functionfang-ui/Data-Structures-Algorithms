#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <time.h>
#include <vector>
#include <string>
#include <stack>
#include <queue>

#define INF		99999999
#define MAXLINE 256

using namespace std;

int degree=0;//交易关系图 弧的数量
int num=0;//交易关系图 顶点的数量

typedef struct Block{//区块的结构体声明
    int         blockID;//区块ID
    long int    blockTimeStamp;//区块时间戳
}Block;

vector<Block> blockChain;//全局变量存储所有区块

typedef struct Tx{//交易的结构体声明
    long int    txID;//交易ID
    int     	blockID;//交易所在的区块ID
    string  	from;//交易转出方
    double  	amout;//金额
    string  	to;//交易转入方
}Tx;

struct Arc;//提前声明弧

typedef struct User{//用户结构体声明
	string			name;//名字
	double 			money;//金额
	vector<Tx*>     in;//所有以该用户为转入方的交易
	vector<Tx*>     out;//所有以该用户为转出方的交易
	vector<Arc*> 	_in;//所有以该用户为转入方的用户
	vector<Arc*> 	_out;//所有以该用户为转出方的用户
	struct User    *left;//左孩子
	struct User    *right;//右孩子
	//辅助信息
	int 			inDegree;//入度 拓扑排序 判读有没有环
	double 			dist=INF;//距离 权重
	string 			help="";//存储从指定账户到该账户的最短路径
}userNode,*userTree;

typedef struct Arc{//弧的结构体声明
	User	   *partner;//交易的伙伴
	double		sum;//累计交易的金额——权重
}Arc;

struct cmp1{//比较函数——交易金额
    bool operator()(Tx *&a, Tx *&b) const{return a->amout < b->amout;}
};

struct cmp2{//比较函数——账户余额
	bool operator()(User *&a, User *&b) const{return a->money < b->money;}
};

struct cmp3{//比较函数——账户入度
	bool operator()(User *&a, User *&b) const{return a->_in.size() < b->_in.size();}
};

struct cmp4{//比较函数——交易出度
	bool operator()(User *&a, User *&b) const{return a->_out.size() < b->_out.size();}
};

bool cmp5(User *&a, User *&b) {return a->dist > b->dist;}//比较函数——路径权重

userTree T;//全局变量——用户树

userNode* searchUserTree(userTree T, string name){//从用户树中返回名字为name的用户指针
	if(!T){return NULL;}
	else if(!name.compare(T->name)){return T;}
	else if(name.compare(T->name)<0) return searchUserTree(T->left,name);
	else return searchUserTree(T->right,name);
}

bool inseartUserTree(userTree &T, userNode* u){//将一个用户指针插入用户树
	if(!T){T=u; return true;}
	if(u->name.compare(T->name)<0) inseartUserTree(T->left, u);
	else inseartUserTree(T->right,u);
}

void readInfo(){//读取区块信息和交易信息
	clock_t start, end;
	start = clock();

	FILE  *blockPath, *txPath;
	struct Block temp1;
	struct Tx temp2;

	blockPath = fopen("block_part1.csv","r");
	char str[MAXLINE], hash[MAXLINE];
	fgets(str, MAXLINE+1, blockPath);
	while(fgets(str, MAXLINE, blockPath)!=NULL){
		sscanf(str, "%d,%[0-9 a-z A-Z],%ld\n", &temp1.blockID, hash, &temp1.blockTimeStamp);
		blockChain.push_back(temp1);
	}
	fclose(blockPath);	

	char s1[50],s2[50];
	txPath = fopen("tx_data_part1_v2.csv","r");
	fgets(str, MAXLINE+1, txPath);
	while(fgets(str, MAXLINE, txPath)!=NULL){		
		sscanf(str, "%ld,%d,%[0-9 a-z A-Z],%lf,%[0-9 a-z A-Z]\n", &temp2.txID, &temp2.blockID, s1, &temp2.amout, s2);

		Tx *t=new Tx;
		t->txID=temp2.txID;t->blockID=temp2.blockID;t->from=s1;t->amout=temp2.amout;t->to=s2;
		
		User *u1=searchUserTree(T, t->from);
		if(!u1){
			u1 = new User;
			u1->name=s1;u1->left=NULL;u1->right=NULL;u1->money=0;	
			inseartUserTree(T,u1);
		}
		u1->out.push_back(t);
		
		User *u2=searchUserTree(T, t->to);	
		if(!u2){
			u2 = new User;
			u2->name=s2;u2->left=NULL;u2->right=NULL;u2->money=0;
			inseartUserTree(T,u2);
		}
		u2->in.push_back(t);
	}
	fclose(txPath);

	end = clock();
	cout<<"Load the file successfully!"<<endl<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
}

int findBlock(long int time){//给定时间查找所在的下一个block
	for(int i=0;i<blockChain.size();i++){
		if(blockChain[i].blockTimeStamp>time){
			return blockChain[i].blockID;
		}
	}
	return INF;
}

int findRecord(string name,int k,long int lower,long int upper){//查找所有时间在lower和upper之间的交易，打印出交易额前k大的交易
	clock_t start, end;
	start = clock();
	User* u=searchUserTree(T, name);
	if(!u){
		cout<<"This user does not exist. Please try another one."<<endl;
		return 0;
	}
	
	Tx *t;
	priority_queue<Tx*, vector<Tx *>, cmp1> q;
	
	int count=0;	
	int up=findBlock(upper);
	int low=findBlock(lower);
	cout<<endl<<name<<endl;
	for(int i=0;i<u->in.size();i++){
		if(low<=u->in[i]->blockID&&u->in[i]->blockID<up){
			q.push(u->in[i]);
			cout<<" <- "<<u->in[i]->from<<"\t"<<fixed<<setprecision(4)<<u->in[i]->amout<<endl;
			count++;
		}		
	}
	for(int i=0;i<u->out.size();i++){
		if(low<=u->out[i]->blockID&&u->out[i]->blockID<up){
			q.push(u->out[i]);
			cout<<" -> "<<u->out[i]->to<<"\t"<<fixed<<setprecision(4)<<u->out[i]->amout<<endl;
			count++;
		}
	}
	cout<<endl<<"There are "<<count<<" in total"<<endl<<endl;

	if(k>count) k=count;
	cout<<"They are "<< k <<" records with the largest transaction amount."<<endl<<endl;
	while(k--){
		t=q.top();
		q.pop();
		cout<<t->from<<" -> "<<t->to<<" "<< t->amout<<endl;
	}
	end = clock();
	cout<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
	return count;
}

double findMoney(string name, long int time){//查找time时间前name用户的金额
	clock_t start, end;
	start = clock();
	User* u=searchUserTree(T, name);
	if(!u){
		cout<<endl<<"This user does not exist. Please try another one."<<endl;
		return 0;
	}
	
	u->money=0;
	int blockLimit=findBlock(time);
	for(int i=0;i<u->in.size();i++){
		if(u->in[i]->blockID<blockLimit)
			u->money+=u->in[i]->amout;
	}
	for(int i=0;i<u->out.size();i++){
		if(u->out[i]->blockID<blockLimit)
			u->money-=u->out[i]->amout;
	}
	
	double d=u->money;
	u->money=0;
	cout<<endl<<"Total money before "<<time<<" of "<<name<<" is "<<d<<endl<<endl;
	end = clock();
	cout<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
	return d;
} 

void preOrderCalc(userTree &user, int blockID, priority_queue<User*, vector<User *>, cmp2> &q){//先序遍历计算所有用户的金额
	for(int i=0;i<user->in.size();i++){
		if(user->in[i]->blockID<blockID) 
			user->money+=user->in[i]->amout;
	}
	for(int i=0;i<user->out.size();i++){
		if(user->out[i]->blockID<blockID)
			user->money-=user->out[i]->amout;
	}
	q.push(user);
    if(user->left!=NULL)
        preOrderCalc(user->left, blockID, q);
    if(user->right!=NULL)
        preOrderCalc(user->right, blockID, q); 
}

void preOrderClear(userTree &user){//先序遍历将金额 权重 路径信息
	user->money=0;
	user->dist=0;
	user->help="";
	if(user->left!=NULL)
        preOrderClear(user->left);
    if(user->right!=NULL)
        preOrderClear(user->right); 
}

void findRich(int n, long int time){//查找出time时间前 富豪榜前n名
	clock_t start, end;
	start = clock();
	
	User* u;
	priority_queue<User*, vector<User *>, cmp2> q;
	
	int blockLimit=findBlock(time);
	preOrderCalc(T, blockLimit, q);	
	while(n--){
		u=q.top();
		q.pop();
		cout<<u->name<<"\t"<<fixed<<setprecision(4)<<u->money<<endl;
	}
	end = clock();
	cout<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
	preOrderClear(T);	
} 

void creatGraphOut(userTree &user){//创建邻接表
	for(int i=0;i<user->out.size();i++){
		int flag=1;
		for(int j=0;j<user->_out.size();j++){
			if(user->_out[j]->partner->name==user->out[i]->to){
				user->_out[j]->sum+=user->out[i]->amout;
				flag=0;break;
			}
		}
		if(flag){
			Arc* a=new Arc;
			a->partner=searchUserTree(T, user->out[i]->to);
			a->sum=user->out[i]->amout;
			user->_out.push_back(a);
		}	
	}
	if(user->left!=NULL)
        creatGraphOut(user->left);
    if(user->right!=NULL)
        creatGraphOut(user->right); 
}

void creatGraphIn(userTree &user){//创建逆邻接表
	for(int i=0;i<user->_out.size();i++){
		Arc* b=new Arc;
		b->partner=user;
		b->sum=user->_out[i]->sum;
		User* u=user->_out[i]->partner;
		u->_in.push_back(b);
	}
	if(user->left!=NULL)
        creatGraphIn(user->left);
    if(user->right!=NULL)
        creatGraphIn(user->right); 
}

void creatGraph(userTree &T){//创建交易图
	clock_t start, end;
	start = clock();
	creatGraphOut(T);
	creatGraphIn(T);
	end = clock();
	cout<<"Creat market graph successfully!"<<endl<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
}

//将所有用户放入优先级对列 依据出度入度排序 计算弧的数量和点的数量
void preOrderPush(userTree user, priority_queue<User*, vector<User *>, cmp3> &q1, priority_queue<User*, vector<User *>, cmp4> &q2){
	degree+=user->_out.size();num++;
	q1.push(user);
	q2.push(user);
	if(user->left!=NULL)
        preOrderPush(user->left, q1, q2);
    if(user->right!=NULL)
        preOrderPush(user->right, q1, q2);
}

void findInOut(int k){//返回交易图的平均入/出度 查找出度入度前k高的用户
	clock_t start, end;
	start = clock();
	
	User* u;
	priority_queue<User*, vector<User *>, cmp3> q1;
	priority_queue<User*, vector<User *>, cmp4> q2;
	degree=0;num=0;
	preOrderPush(T, q1, q2);
	cout<<endl<<"Total : "<<degree<<" degrees and "<<num<<" accounts"<<endl;
	cout<<"Average in/out degree: "<<fixed<<setprecision(6)<<(double)degree/(double)num<<endl<<endl;
	
	int n=k;
	cout<<"top "<<n<<" in degree account: "<<endl<<endl;
	while(k--){
		u=q1.top();
		q1.pop();
		cout<<u->name<<" "<<u->_in.size()<<endl;
	}
	cout<<endl<<"top "<<n<<" out degree account: "<<endl<<endl;
	while(n--){
		u=q2.top();
		q2.pop();
		cout<<u->name<<" "<<u->_out.size()<<endl;
	}
	
	end = clock();
	cout<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
}

bool hasAdded(User* user, Tx* t){//判断一个交易是否已被加入
	for(int i=0;i<user->in.size();i++){
		if(user->in[i]->txID==t->txID) return true;
	}
	for(int i=0;i<user->out.size();i++){
		if(user->out[i]->txID==t->txID) return true;
	}
	return false;
}

void addInfo(char* file){//从指定路径加入额外交易信息
	clock_t start, end;
	start = clock();
	
	struct Tx temp;
	char s1[50],s2[50],str[MAXLINE];
	FILE *txPath = fopen(file,"r");
	if(!txPath){
		cout<<"This file does not exist, please try another one."<<endl;
		return ;
	}
	fgets(str, MAXLINE+1, txPath);
	while(fgets(str, MAXLINE, txPath)!=NULL){	
		sscanf(str, "%ld,%d,%[0-9 a-z A-Z],%lf,%[0-9 a-z A-Z]\n", &temp.txID, &temp.blockID, s1, &temp.amout, s2);

		Tx *t=new Tx;
		t->txID=temp.txID;t->blockID=temp.blockID;t->from=s1;t->amout=temp.amout;t->to=s2;
		
		User *u1=searchUserTree(T, t->from);
		User *u2=searchUserTree(T, t->to);
		if(hasAdded(u1,t)||hasAdded(u2,t)) break;
		u1->out.push_back(t);
		u2->in.push_back(t);
		
		int flag1=1;
		for(int i=0;i<u1->_out.size();i++){ 
			if(u1->_out[i]->partner->name==t->to){
				u1->_out[i]->sum+=t->amout;
				flag1=0;
			}
		}
		if(flag1){
			Arc* a =new Arc;
			a->partner=u2;
			a->sum=t->amout;
			u1->_out.push_back(a);
		}
		int flag2=1;
		for(int i=0;i<u2->_in.size();i++){
			if(u2->_in[i]->partner->name==t->from){
				u2->_in[i]->sum+=t->amout;
				flag2=0;
			}
		}
		if(flag2){
			Arc* b =new Arc;
			b->partner=u1;
			b->sum=t->amout;
			u2->_in.push_back(b);
		}		
	}
	fclose(txPath);
	end = clock();
	cout<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
}

void reCount(userTree user){//更新交易图的弧和点的数量
	num++;degree+=user->_out.size();
	if(user->left!=NULL)
        reCount(user->left);
    if(user->right!=NULL)
        reCount(user->right);
}

void findInDegree(userTree &user, stack<User*> &s){//查找所有用户的入度
	user->inDegree=user->_in.size();
	if(user->inDegree<=0) s.push(user);
	if(user->left!=NULL)
        findInDegree(user->left, s);
    if(user->right!=NULL)
        findInDegree(user->right, s);
}

bool hasCircle(userTree &T){//拓扑排序判断交易图中是否有环
	clock_t start, end;
	start = clock();
	
	num=0;degree=0;reCount(T);	
	stack<User*> s;
	findInDegree(T, s);
	int count=0;
	while(!s.empty()){
		User *u=s.top();s.pop();count++;
		for(int i=0;i<u->_out.size();i++){
			u->_out[i]->partner->inDegree--;
			if(u->_out[i]->partner->inDegree<=0) s.push(u->_out[i]->partner);
		}
	}
	if(count<num){
		cout<<"Result: YES"<<endl;
		end = clock();
		cout<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
		return true;
	}
	else{
		cout<<"Result: NO"<<endl;
		end = clock();
		cout<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
		return false;
	}	
}

bool hasVisited(User *u, vector<string> s){//判断该节点有无被访问过
	for(int i=0;i<s.size();i++){
		if(u->name==s[i]){
			return true;
		}
	}
	return false;
}

void shortestPath(string name){//迪杰斯特拉算法求最短路
	preOrderClear(T);
	clock_t start, end;
	start = clock();
	vector<string> s;
	
	User* u=searchUserTree(T, name);
	if(!u){
		cout<<endl<<"This user does not exist. Please try another one."<<endl;
		return ;
	}
	
	vector<User*> v;
	v.push_back(u);
	u->dist=0;
	int count=0;
	while(!v.empty()){
		User *t=v[v.size()-1];
		if(count){
			cout<<endl<<"Path: "<<count<<endl;
			cout<<t->help<<t->name<<endl;
			cout<<"Weight: "<<t->dist<<endl;
		}
		count++;		
		v.pop_back();
		
		for(int i=0;i<t->_out.size();i++){
			User *temp=t->_out[i]->partner;
			if(!hasVisited(temp, s)){
				s.push_back(temp->name);
				v.push_back(temp);
				temp->dist=t->dist+t->_out[i]->sum;
				temp->help=t->help+t->name+" ->\n";
			}
			else{
				if(t->dist+t->_out[i]->sum<temp->dist){
					temp->dist=t->dist+t->_out[i]->sum;
					temp->help=t->help+t->name+" ->\n";
				}
			}
		}
		sort(v.begin(), v.end(), cmp5);
	}
	end = clock();
	cout<<"Time = "<<fixed<<setprecision(6)<<(double)(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
}

void printHelp(){//打印帮助信息
	cout<<endl<<"----------------------------------------------------------------------------------"<<endl<<endl;
	cout<<"Please choose the module(e.g. a):"<<endl<<endl;
	cout<<"Query:"<<endl;
	cout<<"	 a. Top k biggest Txs of an account between two timetamps."<<endl; 
	cout<<"	 b. Total money of an account before a certain timetamp."<<endl;
	cout<<"	 c. Top k most valuable accounts before a certain timetamp."<<endl;
	cout<<"Analyze:"<<endl;
	cout<<"	 d. Market graph."<<endl;
	cout<<"	 e. Average in/out degree and top k in/out degree account."<<endl;
	cout<<"	 f. Are there circles in the market graph?"<<endl;
	cout<<"	 g. Shortest path from a given account."<<endl;
	cout<<"Update:"<<endl;
	cout<<"	 h. Update Txs from a given file."<<endl;
	cout<<"Others:"<<endl;
	cout<<"	 q. Quit"<<endl<<endl;
	cout<<"----------------------------------------------------------------------------------"<<endl<<endl<<endl;	
} 

int main(){
	readInfo();
	creatGraph(T);

	while(1){
		printHelp();
		char ch;cout<<">>>";cin>>ch;
		switch(ch){
			case 'a':
			{
				cout<<"Please input one user's name, number k, time lower bound and upper bound"<<endl;
				string name;int n;long int upper;long int lower;
				cin>>name>>n>>lower>>upper;
				findRecord(name, n, lower, upper);//1Q9yKkNSoBBKSNwA8AeyPcC8Cs31iVZ2UF,2,1284753029,1301152781
			}	
				break;
			case 'b':
			{
				cout<<"Please input one user's name and timeTamp"<<endl<<endl;
				string name;long int time;
				cin>>name>>time;
				findMoney(name, time);
			}
				break;
			case 'c':
			{
				cout<<"Please input number k and timeTamp"<<endl;
				int k;long int timeTamp;
				cin>>k>>timeTamp;
				findRich(k, timeTamp);	
			}
				break;
			case 'd':
			{
				cout<<"Ready to creat market graph..."<<endl;
				cout<<"Job done."<<endl;
			}
				break;
			case 'e':
			{
				int k;
				cout<<"Please input number k"<<endl;
				cin>>k;
				findInOut(k);
			}
				break;
			case 'f':
			{
				cout<<"Are there any circles in the market graph?"<<endl;
				hasCircle(T);
			}
				break;
			case 'g':
			{
				cout<<"Please input a user's name"<<endl;
				string user;
				cin>>user;
				shortestPath(user);
			} 
				break;
			case 'h':
			{
				char ch=getchar();
				cout<<"Please input the path of the given file"<<endl<<endl;
				char str[MAXLINE];gets(str);
				addInfo(str);
			}
				break;
			case 'q':
			{
				cout<<"The program has been shut down"<<endl;
			}
				return 0;
		}		
	}
}
