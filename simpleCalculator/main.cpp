#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <math.h>
#include <queue>
#include <stack>

using namespace std;

#define MAXSIZE 100
#define ERROR   -1

//运算数 运算符栈
double opds[1000]={0};
int opstop,opdstop;
char ops[1000]={'\0'};

typedef int ElemType;

//之所以将LNode 独立成一个struct 是为了priority_queue<LNode> q 方便排序
typedef struct LNode{//链表实现多项式运算
    int             coef;
    int             exp;
    bool operator <(const LNode &a)const//重载<运算符实现排序
    {
        return exp<a.exp;
    }
}LNode, *ListPtr;

priority_queue<LNode> q;//优先级队列实现自动排序
LNode p;

typedef struct Node{//链表实现一元多项式
    LNode        elem;
    struct Node *next;
}Node, *LinkList;

Node* creat(){
    Node *head,*end,*node;
    head = (Node*)malloc(sizeof(Node));
    end = head;
    end->next=NULL;
    while(!q.empty()){
         LNode m=q.top(); q.pop();
         node = (Node*)malloc(sizeof(Node));
         node->elem=m;
         end->next=node;
         end=node;
    }
    end->next=NULL;
    return head;
}

typedef struct{//顺序表实现一元多项式
    LNode 	    *elem;
    int 		length;
}LNodeList;

void CreateList(LNodeList &L){//顺序创建一元多项式
    L.elem = new LNode[MAXSIZE];
    L.length=0;
    while(!q.empty()){
         LNode m=q.top(); q.pop();
         L.elem[L.length]=m;
         L.length++;
    }
}

void display(LNodeList L){//根据顺序表 格式化 打印一元多项式
    if(L.elem[0].coef==0||L.length==0){
        cout<<'0'<<endl;
        return ;
    }
    for(int i=0;i<L.length;i++){
        if(i==0&&L.elem[i].coef>1&&L.elem[i].exp==0) cout<<L.elem[i].coef;
        if(i==0&&L.elem[i].coef>1&&L.elem[i].exp==1) cout<<L.elem[i].coef<<"*x";
        if(i==0&&L.elem[i].coef>1&&L.elem[i].exp>1) cout<<L.elem[i].coef<<"*x^"<<L.elem[i].exp;
        if(i==0&&L.elem[i].coef==1&&L.elem[i].exp==0) cout<<"1";
        if(i==0&&L.elem[i].coef==1&&L.elem[i].exp==1) cout<<"x";
        if(i==0&&L.elem[i].coef==1&&L.elem[i].exp>1) cout<<"x^"<<L.elem[i].exp;
        if(i==0&&L.elem[i].coef==-1&&L.elem[i].exp==0) cout<<"-1";
        if(i==0&&L.elem[i].coef==-1&&L.elem[i].exp==1) cout<<"-x";
        if(i==0&&L.elem[i].coef==-1&&L.elem[i].exp>1) cout<<"-x^"<<L.elem[i].exp;
        if(i==0&&L.elem[i].coef<-1&&L.elem[i].exp==0) cout<<L.elem[i].coef;
        if(i==0&&L.elem[i].coef<-1&&L.elem[i].exp==1) cout<<L.elem[i].coef<<"*x";
        if(i==0&&L.elem[i].coef<-1&&L.elem[i].exp>1) cout<<L.elem[i].coef<<"*x^"<<L.elem[i].exp;

        if(i!=0&&L.elem[i].coef>1&&L.elem[i].exp==0) cout<<"+"<<L.elem[i].coef;
        if(i!=0&&L.elem[i].coef>1&&L.elem[i].exp==1) cout<<"+"<<L.elem[i].coef<<"*x";
        if(i!=0&&L.elem[i].coef>1&&L.elem[i].exp>1) cout<<"+"<<L.elem[i].coef<<"*x^"<<L.elem[i].exp;
        if(i!=0&&L.elem[i].coef==1&&L.elem[i].exp==0) cout<<"+1";
        if(i!=0&&L.elem[i].coef==1&&L.elem[i].exp==1) cout<<"+x";
        if(i!=0&&L.elem[i].coef==1&&L.elem[i].exp>1) cout<<"+x^"<<L.elem[i].exp;
        if(i!=0&&L.elem[i].coef==-1&&L.elem[i].exp==0) cout<<"-1";
        if(i!=0&&L.elem[i].coef==-1&&L.elem[i].exp==1) cout<<"-x";
        if(i!=0&&L.elem[i].coef==-1&&L.elem[i].exp>1) cout<<"-x^"<<L.elem[i].exp;
        if(i!=0&&L.elem[i].coef<-1&&L.elem[i].exp==0) cout<<L.elem[i].coef;
        if(i!=0&&L.elem[i].coef<-1&&L.elem[i].exp==1) cout<<L.elem[i].coef<<"*x";
        if(i!=0&&L.elem[i].coef<-1&&L.elem[i].exp>1) cout<<L.elem[i].coef<<"*x^"<<L.elem[i].exp;
    }
    puts("");
}

void display(LinkList L){//根据链表 格式化 打印一元多项式
    Node *p=L->next;
    if(p==NULL||p->elem.coef==0){
        cout<<'0'<<endl;
        return ;
    }
    int i=0;
    while(p){
        if(i==0&&p->elem.coef>1&&p->elem.exp==0) cout<<p->elem.coef;
        if(i==0&&p->elem.coef>1&&p->elem.exp==1) cout<<p->elem.coef<<"*x";
        if(i==0&&p->elem.coef>1&&p->elem.exp>1) cout<<p->elem.coef<<"*x^"<<p->elem.exp;
        if(i==0&&p->elem.coef==1&&p->elem.exp==0) cout<<"1";
        if(i==0&&p->elem.coef==1&&p->elem.exp==1) cout<<"x";
        if(i==0&&p->elem.coef==1&&p->elem.exp>1) cout<<"x^"<<p->elem.exp;
        if(i==0&&p->elem.coef==-1&&p->elem.exp==0) cout<<"-1";
        if(i==0&&p->elem.coef==-1&&p->elem.exp==1) cout<<"-x";
        if(i==0&&p->elem.coef==-1&&p->elem.exp>1) cout<<"-x^"<<p->elem.exp;
        if(i==0&&p->elem.coef<-1&&p->elem.exp==0) cout<<p->elem.coef;
        if(i==0&&p->elem.coef<-1&&p->elem.exp==1) cout<<p->elem.coef<<"*x";
        if(i==0&&p->elem.coef<-1&&p->elem.exp>1) cout<<p->elem.coef<<"*x^"<<p->elem.exp;

        if(i!=0&&p->elem.coef>1&&p->elem.exp==0) cout<<"+"<<p->elem.coef;
        if(i!=0&&p->elem.coef>1&&p->elem.exp==1) cout<<"+"<<p->elem.coef<<"*x";
        if(i!=0&&p->elem.coef>1&&p->elem.exp>1) cout<<"+"<<p->elem.coef<<"*x^"<<p->elem.exp;
        if(i!=0&&p->elem.coef==1&&p->elem.exp==0) cout<<"+1";
        if(i!=0&&p->elem.coef==1&&p->elem.exp==1) cout<<"+x";
        if(i!=0&&p->elem.coef==1&&p->elem.exp>1) cout<<"+x^"<<p->elem.exp;
        if(i!=0&&p->elem.coef==-1&&p->elem.exp==0) cout<<"-1";
        if(i!=0&&p->elem.coef==-1&&p->elem.exp==1) cout<<"-x";
        if(i!=0&&p->elem.coef==-1&&p->elem.exp>1) cout<<"-x^"<<p->elem.exp;
        if(i!=0&&p->elem.coef<-1&&p->elem.exp==0) cout<<p->elem.coef;
        if(i!=0&&p->elem.coef<-1&&p->elem.exp==1) cout<<p->elem.coef<<"*x";
        if(i!=0&&p->elem.coef<-1&&p->elem.exp>1) cout<<p->elem.coef<<"*x^"<<p->elem.exp;

        p=p->next;i++;
    }
    puts("");
}

void ListPlus(LNodeList L1, LNodeList L2, LNodeList &ans){//顺序表实现加法
    int i=0,j=0;
    LNode t;
    while(i<L1.length&&j<L2.length){
        if(L1.elem[i].exp==L2.elem[j].exp){
            if(L1.elem[i].coef==-1*L2.elem[j].coef){
                i++;j++;continue;
            }
            else{
                t.coef=L1.elem[i].coef+L2.elem[j].coef;
                t.exp=L1.elem[i].exp;
                ans.elem[ans.length]=t;
                ans.length++;
                i++;j++;continue;
            }
        }
        else if(L1.elem[i].exp>L2.elem[j].exp){
            t.exp=L1.elem[i].exp;
            t.coef=L1.elem[i].coef;
            ans.elem[ans.length]=t;
            ans.length++;
            i++;continue;
        }
        else{
            t.exp=L2.elem[j].exp;
            t.coef=L2.elem[j].coef;
            ans.elem[ans.length]=t;
            ans.length++;
            j++;continue;
        }
    }
    for(int k=i;k<L1.length;k++){
        ans.elem[ans.length]=L1.elem[k];
        ans.length++;
    }
    for(int k=j;k<L2.length;k++){
        ans.elem[ans.length]=L2.elem[k];
        ans.length++;
    }
}

void ListPlus(LinkList L1, LinkList L2, LinkList &ans){//链表实现加法
    Node *p1=L1->next,*p2=L2->next,*end=ans,*temp;
    while(p1&&p2){
        if(p1->elem.exp==p2->elem.exp){
            if(p1->elem.coef==-1*p2->elem.coef){
                p1=p1->next;p2=p2->next;continue;
            }
            else{
                temp=(Node*)malloc(sizeof(Node));
                temp->elem.coef=p1->elem.coef+p2->elem.coef;
                temp->elem.exp=p1->elem.exp;
                end->next=temp;
                end=temp;
                p1=p1->next;p2=p2->next;continue;
            }
        }
        else if(p1->elem.exp>p2->elem.exp){
            temp=(Node*)malloc(sizeof(Node));
            temp->elem.coef=p1->elem.coef;
            temp->elem.exp=p1->elem.exp;
            p1=p1->next;
            end->next=temp;
            end=temp;continue;
        }
        else{
            temp=(Node*)malloc(sizeof(Node));
            temp->elem.coef=p2->elem.coef;
            temp->elem.exp=p2->elem.exp;
            p2=p2->next;
            end->next=temp;
            end=temp;continue;
        }
    }
    while(p1){
        temp=(Node*)malloc(sizeof(Node));
        temp->elem.coef=p1->elem.coef;
        temp->elem.exp=p1->elem.exp;
        p1=p1->next;
        end->next=temp;
        end=temp;
    }
    while(p2){
        temp=(Node*)malloc(sizeof(Node));
        temp->elem.coef=p2->elem.coef;
        temp->elem.exp=p2->elem.exp;
        p2=p2->next;
        end->next=temp;
        end=temp;
    }
}

void ListSubtract(LNodeList L1, LNodeList L2, LNodeList &ans){//顺序表实现减法
    for(int i=0;i<L2.length;i++){
        L2.elem[i].coef=-L2.elem[i].coef;
    }
    ListPlus(L1,L2,ans);
    for(int i=0;i<L2.length;i++){
        L2.elem[i].coef=-L2.elem[i].coef;
    }
}

void ListSubtract(LinkList L1, LinkList L2, LinkList &ans){//链表实现减法
    Node* p2=L2->next;
    while(p2){
        p2->elem.coef=-p2->elem.coef;
        p2=p2->next;
    }
    ListPlus(L1,L2,ans);
    p2=L2->next;
    while(p2){
        p2->elem.coef=-p2->elem.coef;
        p2=p2->next;
    }
}

void ListMultiple(LNodeList L1, LNodeList L2, LNodeList &ans){//顺序表实现乘法
    LNode t;
    int i=0,j=0;
    while(i<L1.length){
        while(j<L2.length){
            t.coef=L1.elem[i].coef*L2.elem[j].coef;
            t.exp=L1.elem[i].exp+L2.elem[j].exp;
            q.push(t);
            j++;
        }
        j=0;
        i++;
    }
    while(!q.empty()){
         LNode m=q.top(); q.pop();
         while(q.top().exp==m.exp){
             if(q.empty()) break;
             m.coef+=q.top().coef;
             q.pop();
         }
         ans.elem[ans.length]=m;
         ans.length++;
    }
}

void ListMultiple(LinkList L1, LinkList L2, LinkList &ans){
    LNode t;
    Node *p1=L1->next,*p2=L2->next;
    while(p1){
        while(p2){
            t.coef=p1->elem.coef*p2->elem.coef;
            t.exp=p1->elem.exp+p2->elem.exp;
            q.push(t);
            p2=p2->next;
        }
        p2=L2->next;
        p1=p1->next;
    }
    Node *end,*node;
    ans = (Node*)malloc(sizeof(Node));
    end = ans;
    end->next=NULL;
    while(!q.empty()){
         LNode m=q.top(); q.pop();
         while(q.top().exp==m.exp){
             if(q.empty()) break;
             m.coef+=q.top().coef;
             q.pop();
         }
         node = (Node*)malloc(sizeof(Node));
         node->elem=m;
         end->next=node;
         end=node;
    }
    end->next=NULL;
}

void takeDerivative(LNodeList L,int n){//顺序表实现求导
    for(int i=0;i<L.length;i++){
        if(n>L.elem[i].exp){
            L.elem[i].coef=0;
            L.elem[i].exp=0;
        }
        else{
            for(int j=1;j<=n;j++){
                L.elem[i].coef*=L.elem[i].exp;
                L.elem[i].exp--;
            }
        }
    }
}

void takeDerivative(LinkList L,int n){//链表实现求导
    Node *p=L->next;
    while(p){
        if(n>p->elem.exp){
            p->elem.exp=0;
            p->elem.coef=0;
        }
        else{
            for(int j=1;j<=n;j++){
                p->elem.coef*=p->elem.exp;
                p->elem.exp--;
            }
        }
        p=p->next;
    }
}

typedef struct{//顺序表实现向量运算
    ElemType 	*elem;
    int 		length;
}SqList;

int replaceStr(char *A, const char *B, const char *C){//用C替换A中所有B子串
    char stmp[101]={0};
    char *p1,*p2;
    p1=strstr(A,B);
    if(p1==NULL){
        strcat(A,C);
        return 0;
    }
    else{
        p2=p1+strlen(B);
        strcpy(stmp,p2);
        *p1='\0';
        strcat(A,C);
        strcat(A,stmp);
        return 1;
    }
}

void CreateList(SqList &L, char str[]){//顺序表创建多维向量
    L.elem=new ElemType[MAXSIZE];
    if(!L.elem) cout<<"OVERFLOW"<<endl;
    int temp=0;
    int flag=1;
    int count=0;
    for(unsigned int i=0;i<strlen(str);i++){
        if(str[i]=='-'){
            flag=-1;
            continue;
        }
        if(i==strlen(str)-1){
            temp=temp*10+str[i]-'0';
            L.elem[count]=temp*flag;
            flag=1;
            count++;
            L.length=count;
            break;
        }
        if('0'<=str[i]&&str[i]<='9'){
            temp=temp*10+str[i]-'0';
        }
        else{
            L.elem[count]=temp*flag;
            temp=0;
            flag=1;
            count++;
        }
    }
}

void display(SqList L){//打印顺序表 多维向量所有节点的值
    for(int i=0;i<L.length;i++)	cout<<L.elem[i]<<' ';
    puts("");
}

char relation(char q1,char q2){ //判断运算符之间的优先关系
    if (q1=='+'||q1=='-'){
        if (q2=='+'||q2=='-'||q2==')'||q2=='#')
            return '>';
        else return '<';
    }
    else if (q1=='*'||q1=='/'){
        if (q2=='(')
            return '<';
        else return '>';
    }
    else if (q1=='('){
        if (q2==')')
            return '=';
        else if (q2=='#')
            return 'E';
        else return '<';
    }
    else if (q1==')'){
        if (q2=='(')
            return 'E';
        else return '>';
    }
    else if (q1=='#'){
        if (q2==')')
            return 'E';
        else
            return '<';
    }
    return 'E';
}

double operate(double a,char ch,double b) //四则运算
{
    if (ch=='+') return a+b;
    else if (ch=='-') return a-b;
    else if (ch=='*') return a*b;
    else if (ch=='/'){
        if (b!=0) return a/b;
        else return -999999999.9;
    }
    return -999999999.9;
}

int calculate(string str){//统一的四则运算接口
    char symb,ch;
    double x,a,b;
    int k=0;

    opstop=0;opdstop=0;
    opstop++;
    ops[opstop]='#'; //'#'运算符栈

    symb=str[k];
    k++;

    int flag=1;
    while (symb!='#'||ops[opstop]!='#'){

        //判断运算数的正负
        if((k==1&&symb=='-')||(str[k-1]=='-'&&isdigit(str[k])&&str[k-2]=='(')){
            flag=-1;
            symb=str[k];
            k++;
            continue;
        }

        //运算数的解析
        if (symb>='0'&&symb<='9'){
            x=0;
            while (symb>='0'&&symb<='9'){
                x=x*10+double(symb-'0');
                symb=str[k];
                k++;
            }
            opdstop++; opds[opdstop]=x*flag;flag=1;
        }
        //运算符的解析
        else if (relation(ops[opstop],symb)=='<'){//栈顶元素优先权低
               opstop++;  ops[opstop]=symb;
               symb=str[k];
               k++;
        }
        else if (relation(ops[opstop],symb)=='>'){//栈顶元素优先权高
            ch=ops[opstop]; opstop--;
            b=opds[opdstop]; opdstop--;
            a=opds[opdstop];
            opds[opdstop]=operate(a,ch,b);
        }
        else if (relation(ops[opstop],symb)=='='){//与栈顶元素优先权相等
            ch=ops[opstop]; opstop--;
            symb=str[k];
            k++;
        }
        else{
            printf("error\n");
            return ERROR;
        }
    }
    //输出答案
    if (opstop==1&&opdstop==1)
        cout<<"The ans would be : "<<endl<<opds[opdstop]<<endl;
    else
        cout<<"error"<<endl;
}

void analysis(string str){//解析输入的一元多项式 将解析出来的值以节点的形式存入 priority_queue来自动排序
    int sign=1;
    int flag=0;
    int temp=0;
    int count=0;
    char symb=str[count];
    count++;
    while(symb!='#'&&count<=str.size()){
        if(symb=='-'){
            sign=-1;
            symb=str[count];
            count++;
        }
        if(symb=='+'){
            sign=1;
            symb=str[count];
            count++;
        }
        if(isdigit(symb)){
            flag=1;
            while(isdigit(symb)){
                temp=temp*10+double(symb-'0');
                symb=str[count];
                count++;
            }
        }

        if(flag) p.coef=temp*sign;
        else p.coef=sign;

        if(symb=='x'){

            if(str[count]!='^'){
                p.exp=1;
                q.push(p);
                symb=str[count];
                count++;
                flag=0;temp=0;
                continue;
            }else{
                symb=str[count];
                count++;
                symb=str[count];
                count++;
                if(isdigit(symb)){
                    temp=0;
                    while(isdigit(symb)){
                        temp=temp*10+double(symb-'0');
                        symb=str[count];
                        count++;
                    }
                }
                p.exp=temp;
                q.push(p);
                flag=0;temp=0;
                continue;
            }
        }

        if(symb!='*'&&symb!='x'){
            p.exp=0;
            q.push(p);
            flag=0;temp=0;
            continue;
        }else{
            symb=str[count];
            count++;
        }
        if(str[count]!='^'){
            p.exp=1;
            q.push(p);
            symb=str[count];
            count++;
            flag=0;temp=0;
            continue;
        }else{
            symb=str[count];
            count++;
            symb=str[count];
            count++;
            if(isdigit(symb)){
                temp=0;
                while(isdigit(symb)){
                    temp=temp*10+double(symb-'0');
                    symb=str[count];
                    count++;
                }
            }
            p.exp=temp;
            q.push(p);
            flag=0;temp=0;
            continue;
        }
    }
}

int main(){

    //打印表头
    puts("Welcome to simple calculator!");
    puts("-------------------------------------------");
    puts("Please choose what you want to do with it:");
    puts("");
    puts("a.Four arithmetic expression evaluation");//四则运算表达式求值 done
    puts("b.Expression evaluation with univariate");//含单变量的表达式求值 done
    puts("c.Calculation of the same dimension vector");//同维度向量的计算 done
    //顺序表实现
    puts("d.Calculation of unary polynomials by Sequence table");//一元多项式的加减乘 done
    puts("e.Nth derivative of a mononomial polynomial by Sequence table");//一元多项式的n阶导数 done
    //链表实线
    puts("f.Calculation of unary polynomials by Linked list");//一元多项式的加减乘
    puts("g.Nth derivative of a mononomial polynomial by Linked list");//一元多项式的n阶导数 done
    puts("");

    //根据用户输入的选择执行不同的功能
    char choice = getchar();

    if(choice == 'a'){

        //打印表头
        puts("Please input one four arithmetic expression(end it with '#')");

        //接收表达式
        string str;
        cin>>str;

        //计算
        calculate(str);

    }
    else if(choice == 'b'){

        //打印表头
        puts("Please input one four arithmetic expression with univariate 'x' (end it with '#')");
        char str[201];
        char ch=getchar();
        gets(str);
        puts("Please assign a value to the univariable 'x'");
        cout<<"x = ";

        //接收变量值 并在变量值两端分别加上'('')'
        char num[15];
        gets(num);
        int len=strlen(num);
        num[len+2]='\0';
        num[len+1]=')';
        for(int i=len;i>=1;i--){
            num[i]=num[i-1];
        }
        num[0]='(';

        //用处理后的变量替换'x'
        while(strstr(str,"x")){
            replaceStr(str,"x",num);
        }

        //计算
        calculate(str);
    }
    else if(choice == 'c'){

        //打印表头
        char str1[201];char str2[201];
        char temp;
        puts("Please input one vector");
        temp=getchar();
        gets(str1);
        puts("Please input another vector");
        gets(str2);

        //用两个list接受两个向量
        SqList list1,list2;
        CreateList(list1,str1);
        CreateList(list2,str2);
        if(list1.length!=list2.length) return ERROR;

        //为结果分配空间
        SqList ans;
        ans.length=list1.length;
        ans.elem=new ElemType[MAXSIZE];
        if(!ans.elem) cout<<"OVERFLOW"<<endl;

        //相加
        puts("vector1+vector2:");
        for(int i=0;i<ans.length;i++){
            ans.elem[i]=list1.elem[i]+list2.elem[i];
        }
        display(ans);

        //相减
        puts("vector1-vector2:");
        for(int i=0;i<ans.length;i++){
            ans.elem[i]=list1.elem[i]-list2.elem[i];
        }
        display(ans);

        //求夹角（变相向量乘积）
        puts("cos<vector1, vector2>:");
        double a=0,b=0,c=0;
        for(int i=0;i<ans.length;i++){
            a+=list1.elem[i]*list2.elem[i];
            b+=list1.elem[i]*list1.elem[i];
            c+=list2.elem[i]*list2.elem[i];
        }
        printf("%lf",a/sqrt(b*c));
    }
    else if(choice == 'd'){

        string str1,str2;

        puts("Please input one unary polynomials L1");
        cin>>str1;
        analysis(str1);
        LNodeList L1;
        CreateList(L1);

        puts("Please input another unary polynomials L2");
        cin>>str2;
        analysis(str2);
        LNodeList L2;
        CreateList(L2);

        LNodeList ans;
        ans.elem=new LNode[MAXSIZE];
        ans.length=0;

        ListPlus(L1,L2,ans);
        cout<<"The ans of L1 + L2 ="<<endl;
        display(ans);

        ans.length=0;
        ListSubtract(L1,L2,ans);
        cout<<"The ans of L1 - L2 ="<<endl;
        display(ans);

        ans.length=0;
        ListMultiple(L1,L2,ans);
        cout<<"The ans of L1 * L2 ="<<endl;
        display(ans);
    }
    else if(choice == 'e'){
        string str;
        puts("Please input one unary polynomials L");
        cin>>str;
        analysis(str);
        LNodeList L;
        CreateList(L);

        int n;
        cout<<"Please input n---Nth derivative"<<endl;
        cin>>n;

        takeDerivative(L,n);
        cout<<"L's "<<n<<"th derivative ="<<endl;
        display(L);
    }
    else if(choice == 'f'){
        string str1,str2;

        puts("Please input one unary polynomials L1");
        cin>>str1;
        analysis(str1);
        LinkList L1;
        L1=creat();

        puts("Please input another unary polynomials L2");
        cin>>str2;
        analysis(str2);
        LinkList L2;
        L2=creat();

        LinkList ans;
        ans=(Node*)malloc(sizeof(Node));
        ans->next=NULL;

        ListPlus(L1,L2,ans);
        cout<<"The ans of L1 + L2 ="<<endl;
        display(ans);
        free(ans);

        ans=(Node*)malloc(sizeof(Node));
        ans->next=NULL;
        ListSubtract(L1,L2,ans);
        cout<<"The ans of L1 - L2 ="<<endl;
        display(ans);
        free(ans);

        ans=(Node*)malloc(sizeof(Node));
        ListMultiple(L1,L2,ans);
        cout<<"The ans of L1 * L2 ="<<endl;
        display(ans);
    }
    else if(choice == 'g'){
        string str;
        puts("Please input one unary polynomials L");
        cin>>str;
        analysis(str);
        LinkList L;
        L=creat();

        int n;
        cout<<"Please input n---Nth derivative"<<endl;
        cin>>n;

        takeDerivative(L,n);
        cout<<"L's "<<n<<"th derivative ="<<endl;
        display(L);
    }
    return 0;
}
