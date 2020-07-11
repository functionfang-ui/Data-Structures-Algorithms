#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <queue>

using namespace std;

int n;//全局变量 记录文件中出现的字符种类数

typedef struct codeNode{//结构体声明
    char 	  ch;//字符
    int  	  weight;//权重
    codeNode *left, *right, *parent;//左孩子 右孩子 父亲
    string    code;//霍夫曼编码结果（01串）
}codeNode, *HuffmanTree;

struct cmp{//重写比较函数 将字符权重小的排列在队头
    bool operator()(codeNode *&a, codeNode *&b) const{    return a->weight > b->weight;		}
};

priority_queue<codeNode*, vector<codeNode *>, cmp> q;//优先级队列依据权重排序 
vector<codeNode*> p;//vector数组储存霍夫曼树叶子节点

void readInf(int count[]){
	for(int i=0;i<130;i++){
		if(count[i]!=0){//若某字符出现频数大于0
			codeNode *temp=new codeNode;//建立叶子节点
			temp->ch=i;temp->weight=count[i];
			temp->left=NULL;temp->right=NULL;
			q.push(temp);//将构建好的叶子节点存入优先级队列
        	p.push_back(temp);//vector数组里也存一份 方便操作
		}
	}
	cout<<endl;
}

codeNode* creatHuffmanTree(){
	while(1){
		codeNode* s1=q.top();//从队头弹出权重最小的两个节点
		q.pop();
		if(q.empty()){	return s1;	}//若队中只剩最后一个节点，即为根节点，返回之
		codeNode* s2=q.top();
		q.pop();
		codeNode* s=new codeNode;//构建新节点
		s->left=s1;s->right=s2;s->ch='X';//将弹出的两个节点设为新节点的左右孩子
		s->weight=s1->weight+s2->weight;//新节点权重为两个相加
		s1->parent=s;s2->parent=s;//将弹出的两个节点的父亲节点设为新节点
		q.push(s);//新节点入队
	}
}

void huffmanCoding(HuffmanTree HT){	
	for(int i=0;i<n;i++){//遍历所有的叶子节点
		string s="";
		codeNode* c=p[i];
		while(1){
			codeNode* pt=c->parent;//向上逆推
			if(!pt) break;//直至根节点停止
			if(c==pt->left) s='0'+s;//自己是父亲节点的左孩子 则在编码字串前+'0'
			else	s='1'+s;//否则+'1'
			c=pt;
			if(pt==HT){//发现根节点 将编码值储存
				p[i]->code=s;
			}
		}		
	} 
}

string huffmanDecode(string code, HuffmanTree HT){
	string s="";codeNode* t=HT;	
	for(int i=0;i<code.length();i++){//遍历电文信息
		if(code[i]=='0'){//出现'0'则访问左孩子
			t=t->left;
		}
		if(code[i]=='1'){//出现'1'则访问右孩子
			t=t->right;
		}
		if(t->left==NULL&&t->right==NULL){//直到访问至根结点
			s+=t->ch;//将译出的电文储存
			t=HT;//指针移至霍夫曼树根节点
		}
	}
	return s;
} 
/*
a 23 b 11 c 5 d 3 e 29 f 14 g 7 h 8
aaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbcccccdddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeffffffffffffffggggggghhhhhhhh
*/
int main(){
	int count[130]={0};//计数器 记录各类字符出现的频数 作为权重

	FILE* fp=fopen("message.txt","r");//从文件读入需要统计的段落
	char s[2];
	while((fgets(s,2,fp))!=NULL)
		count[s[0]]++;	//每次读一个字符 更新计数器
	fclose(fp);

	for(int i=0;i<130;i++)//统计字符种类数
		if(count[i]!=0) n++;
	
    readInf(count);//建立叶子节点 优先级队列 vector数组
	HuffmanTree HT = creatHuffmanTree();//建立霍夫曼树
	huffmanCoding(HT);//对每个字符进行编码
	
	string t;
	freopen("code.txt","r",stdin);
	freopen("codeToMessage.txt","w",stdout);
	cin>>t;
	string ss=huffmanDecode(t,HT);//将01串文件的译码结果输出至文件
	cout<<ss;
	fclose(stdout);
	fclose(stdin);
	
	freopen("huffmanCode.txt","w",stdout);
	for(int i=0;i<n;i++){//将出现的每个字符的编码结果输出至文件
    	cout<<p[i]->ch<<" "<<p[i]->code<<endl;
	}
	fclose(stdout);
	
}