#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QString>
#include <string>
#include <QTextCodec>
#include <QCoreApplication>
#include <queue>
#include <stack>
using namespace std;

QString line1[100]={"行政楼","经管楼","逸夫馆","工程训练中心","航空宇航馆","综合教学楼","图书馆","机械馆","艺术馆","体育馆","综合体育场","大学生活动中心","蓝天剧场","大学生飞行器创新实践基地","创新创业楼","国际教育学院","南区足球场","南区食堂","友谊会馆","南区篮球场","校医院","南区学生公寓","网球场","北区食堂","北区学生公寓","北区运动场","通航实验楼","重点学科实验楼","热能实验楼","蒲新教师公寓"};
int visited[100];
int begin0;
stack<int> sta;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, QColor(176, 196, 222));
    this->setPalette(palette);
    this->setStyleSheet("background-color:blue;");
    ui->setupUi(this);
    setWindowTitle("校园导航系统");
    ui->min_line->hide();
    ui->all_line->hide();
    ui->cx->hide();
    ui->cx_edit->hide();
    ui->cx_label->hide();
    ui->dh_begin->hide();
    ui->dh_end->hide();
    ui->cx_label->hide();
    ui->cx_begin_na->hide();
    ui->cx_end_na->hide();
    ui->cx_label->hide();
    ui->all_na->hide();
    ui->min_na->hide();
    ui->map->hide();


    connect(this->ui->pu_1,SIGNAL(click),this,SLOT(on_pu_1_clicked));
    connect(this->ui->pu_2,SIGNAL(click),this,SLOT(on_pu_2_clicked));
    connect(this->ui->pu_3,SIGNAL(click),this,SLOT(on_pu_3_clicked));
    connect(this->ui->pu_4,SIGNAL(click),this,SLOT(on_pu_4_clicked));
    connect(this->ui->pu_5,SIGNAL(click),this,SLOT(on_pu_5_clicked));

}

/*
本程序是使用Dijkstra算法实现求解最短路径的问题
采用的邻接矩阵来存储图
*/
//记录起点到每个顶点的最短路径的信息
struct Dis {
    queue<int> path;
    int value;
    bool visit;
    Dis() {
        visit = false;
        value = 0;
        //path = "";
    }
};

class Graph_DG {
public:
    int vexnum;   //图的顶点个数
    int edge;     //图的边数
    int **arc;   //邻接矩阵
    Dis * dis;   //记录各个顶点最短路径的信息
public:
    //构造函数
    Graph_DG(int vexnum, int edge);
    //析构函数
    ~Graph_DG();
    // 判断我们每次输入的的边的信息是否合法
    //顶点从1开始编号
    bool check_edge_value(int start, int end, int weight);
    //打印邻接矩阵
    void print();
    //求最短路径
    void Dijkstra(int begin);
    //求所有路径
    void dfs_all_line(int start,int end);
    //打印最短路径
    void print_path(int end);
};

//构造函数
Graph_DG::Graph_DG(int vexnum, int edge) {
    //初始化顶点数和边数
    this->vexnum = vexnum;
    this->edge = edge;
    //为邻接矩阵开辟空间和赋初值
    arc = new int*[this->vexnum];
    dis = new Dis[this->vexnum];
    for (int i = 0; i < this->vexnum; i++) {
        arc[i] = new int[this->vexnum];
        for (int k = 0; k < this->vexnum; k++) {
            //邻接矩阵初始化为无穷大
            arc[i][k] = INT_MAX;
        }
    }
}
//析构函数
Graph_DG::~Graph_DG() {
    delete[] dis;
    for (int i = 0; i < this->vexnum; i++) {
        delete this->arc[i];
    }
    delete arc;
}

Graph_DG read_file()
{
    int vexnum=30; //顶点数
    int edge=900; //边数
    FILE *file;
    if ((file = fopen("C://Users/hyb/Desktop/Work/map.txt", "r+")) == NULL)
    {
        cout<<"123\n";
        exit(0);
    }
    Graph_DG Graph(vexnum, edge);
    for(int i=0;i<vexnum;i++)
        for(int j=0;j<vexnum;j++)
        {
            if(j==29)
                fscanf(file,"%d\n",&Graph.arc[i][j]);
            else
                fscanf(file,"%d ",&Graph.arc[i][j]);
        }
    return Graph;
}

Graph_DG G=read_file();

// 判断我们每次输入的的边的信息是否合法
//顶点从1开始编号
bool Graph_DG::check_edge_value(int start, int end, int weight) {
    if (start<1 || end<1 || start>vexnum || end>vexnum || weight < 0) {
        return false;
    }
    return true;
}


void Graph_DG::print() {
    //cout << "图的邻接矩阵为：" << endl;
    int count_row = 0; //打印行的标签
    int count_col = 0; //打印列的标签
    //开始打印
    while (count_row != this->vexnum) {
        count_col = 0;
        while (count_col != this->vexnum) {
            if (arc[count_row][count_col] == INT_MAX)
                cout << "∞" << " ";
            else
                cout << arc[count_row][count_col] << " ";
            ++count_col;
        }
        cout << endl;
        ++count_row;
    }
}

void Graph_DG::Dijkstra(int begin) {
    //首先初始化我们的dis数组
    int i;
    for (i = 0; i < this->vexnum; i++) {
        //设置当前的路径
        //dis[i].path = "v" + QString(begin) + "-->v" + QString(i + 1);
        dis[i].path.push(i+1);
        dis[i].value = arc[begin - 1][i];
    }
    //设置起点的到起点的路径为0
    dis[begin - 1].value = 0;
    dis[begin - 1].visit = true;

    int count = 1;
    //计算剩余的顶点的最短路径（剩余this->vexnum-1个顶点）
    while (count != this->vexnum) {
        //temp用于保存当前dis数组中最小的那个下标
        //min记录的当前的最小值
        int temp = 0;
        int min = INT_MAX;
        for (i=0;i<G.vexnum;i++) {
            if (!dis[i].visit && dis[i].value < min) {
                min = dis[i].value;
                temp = i;
            }
        }
        //cout << temp + 1 << "  "<<min << endl;
        //把temp对应的顶点加入到已经找到的最短路径的集合中
        dis[temp].visit = true;
        ++count;
        for (i = 0; i < this->vexnum; i++) {
            //注意这里的条件arc[temp][i]!=INT_MAX必须加，不然会出现溢出，从而造成程序异常
            if (!dis[i].visit && arc[temp][i] != INT_MAX && (dis[temp].value + arc[temp][i]) < dis[i].value) {
                //如果新得到的边可以影响其他为访问的顶点，那就就更新它的最短路径和长度
                dis[i].value = dis[temp].value + arc[temp][i];
                queue<int> q=dis[temp].path;
                q.push(i+1);
                dis[i].path=q;
            }
        }
    }

}
void Graph_DG::print_path(int end) {
    QString str;
    str = "v" + QString(end);
    int i=0;
    while(i!=this->vexnum)
    {
        if(i==end)
        {
            QString min_line;
            while(!dis[i].path.empty())
            {
                qDebug()<<dis[i].path.front();
                min_line+=line1[dis[i].path.front()-1];
                dis[i].path.pop();
            }
            qDebug()<<min_line;
            break;
        }
        i++;
    }
}

//检验输入边数和顶点数的值是否有效，可以自己推算为啥：
//顶点数和边数的关系是：((Vexnum*(Vexnum - 1)) / 2) < edge
bool check(int Vexnum, int edge) {
    if (Vexnum <= 0 || edge <= 0 || ((Vexnum*(Vexnum - 1)) / 2) < edge)
        return false;
    return true;
}

void init(bool *visit)
{
    for(int i=0;i<100;i++)
        visit[i]=false;
}

QString all;

void Graph_DG::dfs_all_line(int start,int end)//深搜入栈查询所有路径
{
    visited[start] = 1;//visited数组存储各定点的遍历情况，true为已遍历（标记）
    sta.push(start);//入栈
    for (int j = 0; j < G.vexnum; j++)
    {
        if (start== end) {//找到终点
        stack<int> s=sta;
        while(!s.empty())
        {
            qDebug()<<s.top();
            s.pop();
        }
        sta.pop();//出栈
        visited[start] = false;
        break;
        }
        if (!visited[j]) {//该顶点未被访问过
            dfs_all_line(j,end);
        }
        if (j == G.vexnum - 1 ) {//如果该顶点无其它出度
            sta.pop();
            visited[start] = 0;
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


//主界面
void MainWindow::on_pu_1_clicked()
{
    ui->main_pic->show();
    ui->min_line->hide();
    ui->all_line->hide();
    ui->cx->hide();
    ui->cx_edit->hide();
    ui->cx_label->hide();
    ui->dh_begin->hide();
    ui->dh_end->hide();
    ui->cx_label->hide();
    ui->cx_begin_na->hide();
    ui->cx_end_na->hide();
    ui->cx_label->hide();
    ui->all_na->hide();
    ui->min_na->hide();
    ui->map->hide();
}
//导航界面
void MainWindow::on_pu_3_clicked()
{
    ui->main_pic->hide();
    ui->cx_begin_na->show();
    ui->cx_end_na->show();
    ui->dh_begin->show();
    ui->dh_end->show();
    //ui->all_line->show();
    //ui->all_na->show();
    ui->min_line->show();
    ui->min_na->show();

    //查询界面关掉
    ui->cx->hide();
    ui->cx_label->hide();
    ui->cx_edit->hide();
    ui->map->hide();
}
//查询界面
void MainWindow::on_pu_2_clicked()
{
    ui->cx->show();
    ui->cx_label->show();
    ui->cx_edit->show();

    //导航的界面关掉
    ui->main_pic->hide();
    ui->cx_begin_na->hide();
    ui->cx_end_na->hide();
    ui->dh_begin->hide();
    ui->dh_end->hide();
    ui->all_line->hide();
    ui->all_na->hide();
    ui->min_line->hide();
    ui->min_na->hide();

    ui->map->hide();
}
//电子地图
void MainWindow::on_pu_4_clicked()
{

    ui->min_line->hide();
    ui->all_line->hide();
    ui->cx->hide();
    ui->cx_edit->hide();
    ui->cx_label->hide();
    ui->dh_begin->hide();
    ui->dh_end->hide();
    ui->cx_label->hide();
    ui->cx_begin_na->hide();
    ui->cx_end_na->hide();
    ui->cx_label->hide();
    ui->all_na->hide();
    ui->min_na->hide();
    ui->map->hide();
    ui->main_pic->hide();
    ui->map->show();
}

//退出键
void MainWindow::on_pu_5_clicked()
{
    this->close();
}

//查询下拉按钮
void MainWindow::on_cx_currentIndexChanged(int index)
{
    //将当前索引赋值给变量index
    index = ui->cx->currentIndex();
    qDebug()<<"Index"<< index;
    if(index==0)
    {
        ui->cx_edit->setText("位于学校正门，是学校部分机关部门，直属附属单位办公场所，涉及到学生事务的部门有教务处，计财处等");
    }
    else if(index==1)
    {
        ui->cx_edit->setText("位于行政楼北侧，经济与管理学院，安全工程学院，马克思主义学院在此办公");
    }
    else if(index==2)
    {
        ui->cx_edit->setText("位于学校西北角经管楼北侧，电子信息工程学院在此办公，二楼为理学院的物理实验室");
    }
    else if(index==3)
    {
        ui->cx_edit->setText("位于逸夫馆北侧，三楼为计算机机房，计算机相关作业、选课等事项可在此进行");
    }
    else if(index==4)
    {
        ui->cx_edit->setText("位于逸夫馆南侧，航空宇航学院，航空发动机学院，民用航空学院办公楼");
    }
    else if(index==5)
    {
        ui->cx_edit->setText("位于学校中心，分为A,B,C座，外国语学院，理学院在此办公，是学生集中上课地点，大多数“小班制管理模式”的自习教室分布在此楼");
    }
    else if(index==6)
    {
        ui->cx_edit->setText("位于学校展翼大道音乐喷泉的后面，是书刊查阅、借阅、资料索引、下载、读书学习集中地，各类学术讲座及学术论坛经常在二楼国内报告厅及国际报告厅举行");
    }
    else if(index==7)
    {
        ui->cx_edit->setText("位于图书馆的西侧，机电工程学院、自动化学院、材料工程学院在此办公");
    }
    else if(index==8)
    {
        ui->cx_edit->setText("位于体育馆西侧，设计艺术学院（一到四楼）、计算机学院（五楼）在此办公");
    }
    else if(index==9)
    {
        ui->cx_edit->setText("主要有羽毛球馆、篮球馆、游泳馆、健身房、台球馆、乒乓球馆等");
    }
    else if(index==10)
    {
        ui->cx_edit->setText("室内有百米跑道、形体馆、乒乓球馆等");
    }
    else if(index==11)
    {
        ui->cx_edit->setText("学生处、招生就业处、校团委、校史馆、校学生会联合会、校社团联合会、校大学生艺术团等在此办公");
    }
    else if(index==12)
    {
        ui->cx_edit->setText("我校大型文艺活动举办场所，可容纳1000人左右");
    }
    else if(index==13)
    {
        ui->cx_edit->setText("该基地是学校对外展示形象的窗口，通过开展和参与各类科普教育和科研创新活动，为社会输送了大量的优秀人才");
    }
    else if(index==14)
    {
        ui->cx_edit->setText("位于大学生飞行器创新实践基地的南侧，创新创业学院，继续教育学院在此办公");
    }
    else if(index==15)
    {
        ui->cx_edit->setText("留学生教学与管理等办公场所");
    }
    else if(index==16)
    {
        ui->cx_edit->setText("位于3号教学楼东侧，南区生活区西侧楼下，是晨练及比赛的好去处");
    }
    else if(index==17)
    {
        ui->cx_edit->setText("位于南生活区中心，汇聚南北各类风味菜肴，学生就餐采用一卡通或者微信支付消费，一楼有一卡通自动充值机，四楼为教职工餐厅");
    }
    else if(index==18)
    {
        ui->cx_edit->setText("位于南生活区南10栋西侧，一楼友谊餐厅融汇各国风味美食及传统美食与一体，是外教楼、留学生宿舍楼");
    }
    else if(index==19)
    {
        ui->cx_edit->setText("位于友谊会馆北侧，是平时锻炼身体、团体比赛的好去处");
    }
    else if(index==20)
    {
        ui->cx_edit->setText("位于学校南生活区南二栋北侧，8点准时开门，22:30以后看病需按门铃，星期一至星期五可做B超，心电图等");
    }
    else if(index==21)
    {
        ui->cx_edit->setText("这里是学生日常生活的重要场所，同时也是对学生进行思想政治教育工作和素质教育工作的重要阵地");
    }
    else if(index==22)
    {
        ui->cx_edit->setText("位于大学生活动中心的东侧，这里免费向同学们开放");
    }
    else if(index==23)
    {
        ui->cx_edit->setText("位于学校网球场的北侧，一楼设有自助餐厅、超市、电信营业厅，二楼和三楼设有很多地方风味小吃档口");
    }
    else if(index==24)
    {
        ui->cx_edit->setText("由学校和龙源物业公司合作管理，其他与南区学生公寓一样");
    }
    else if(index==25)
    {
        ui->cx_edit->setText("这里有篮球场、排球场，也是运动会大部分田赛的比赛场地");
    }
    else if(index==26)
    {
        ui->cx_edit->setText("辽宁通用航空研究院位于这里，由学校与沈阳飞机设计研究所、空气动力研究院等单位组建的开放式科研单位");
    }
    else if(index==27)
    {
        ui->cx_edit->setText("航空制造工艺数字化国防重点学科实验室位于此处，是航空制造企业的应用技术研究平台和创新性人才培养基地，能源与环境学院在此B座办公");
    }
    else if(index==28)
    {
        ui->cx_edit->setText("学校总务部在此办公");
    }
    else if(index==29)
    {
        ui->cx_edit->setText("这里已建设成为温馨、舒适的教职工之家");
    }
}



//导航起点下拉按钮
void MainWindow::on_dh_begin_activated(int begin)
{
    //将当前索引赋值给变量index
    begin = ui->dh_begin->currentIndex();
    begin0=begin;
    G.Dijkstra(begin+1);
    qDebug()<<"begin"<< begin;
    //ui->all_line->setText(a);
}



//导航终点下拉按钮
void MainWindow::on_dh_end_activated(int end)
{
    //将当前索引赋值给变量index
    end = ui->dh_end->currentIndex();

    //最短路径
    int i=0;
    QString min_line=line1[begin0]+"-->";

    while(i!=G.vexnum)
    {
        int flag=0;
        if(i==end)
        {
            while(!G.dis[i].path.empty())
            {
                qDebug()<<G.dis[i].path.front();
                if(G.dis[i].path.front()-1==end&&flag!=1)
                {
                    flag=1;
                    min_line+=line1[G.dis[i].path.front()-1];
                }
                if(flag!=1)
                    min_line+=line1[G.dis[i].path.front()-1];
                G.dis[i].path.pop();
                if(!G.dis[i].path.empty()&&flag!=1)
                    min_line+="-->";
            }
            if(flag==0) min_line+=line1[end];
            ui->min_line->setText(min_line);
            break;
        }
        i++;
    }

    //所有路径
    int num=0;
    //G.dfs_all_line(begin0,end);
    //ui->all_line->setText(all);

    qDebug()<<"end"<< end;
}
