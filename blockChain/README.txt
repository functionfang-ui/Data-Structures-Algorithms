进入程序

首先提示 用户选择要执行的功能 a b c d e f g h q

a——查找指定账号在一个时间段内的所有转入或转出记录，返回总记录数，交易金额最大的前k条记录

(e.g.  1BAjnzBx6jvuQJWxByQGtiYVJUCdjwiz37 5 0 1400000000)

b——查询某个账号在某个时刻的金额

(e.g.  1N8sGzWwXcLqN21yafyEmDCcbDjmWS88MT 1400000000)

c——输出在该时刻最有钱的前k个用户

(e.g.  10 1400000000)

d——构建交易关系图

e——统计交易关系图的平均出度、入度，显示出度 / 入度最高的前k个帐号

(e.g.  5)

f——检查交易关系图中是否存在环

g——求给定的账户到其他所有账号的最短路径

(e.g.  1CZyWWpJviTcfst7LykhCkWUUJK3zTzyrf)

h——从文件中读入新交易记录

(e.g.  tx_data_part2.csv)

q——退出程序