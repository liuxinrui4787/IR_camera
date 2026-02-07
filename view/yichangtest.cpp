#include "yichangtest.h"
#include<QDebug>
#include<QApplication>
#include<cstdio>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<string>
#include<vector>
#include<set>
#include<qstring.h>
using namespace std;

yichangtest::yichangtest(QWidget *parent) : QWidget(parent)
{
}


//20241025粘贴adressbook的main函数到这里面
void yichangtest::init()
{
//    fstream green("F:\\1014test\\xingneng\\zongtu\\yichangdian.csv", ios::in);
//    fstream red("F:\\1014test\\xingneng\\zongtu\\cankaotu\\log\\log.csv", ios::in);
    fstream green("F:\\1026test\\data_5500\\yichangdian.csv", ios::in);//data1112
    fstream red("F:\\1026test\\data_5500\\ref_del\\log\\log.csv", ios::in);//data1112
    vector<pair<int,int>> g,r;
    vector<pair<int,int>> rres,gres;

    if(green)
    {
        while(!green.eof())
        {
            string in,temp1,temp2;
            getline(green, in);
            stringstream ss(in);
            getline(ss, temp1,',');
            getline(ss, temp2,',');
            if(temp1 == "")
            {
                continue;
            }
            int a = atoi(temp1.c_str());
            int b = atoi(temp2.c_str());
//            cout<<a<<endl;
            g.push_back(make_pair(a,b));
        }
    }
    if(red)
    {
        while(!red.eof())
        {
            string in,temp1,temp2;
            getline(red, in);
            stringstream ss(in);
            getline(ss, temp1,',');
            getline(ss, temp2,',');
            if(temp1 == "")
            {
                continue;
            }
            int a = atoi(temp1.c_str());
            int b = atoi(temp2.c_str());
            r.push_back(make_pair(a,b));
        }
    }
    for(int i = 0; i<g.size(); i++)
    {
        for(int j=0; j<r.size(); j++)
        {

            if((r[j].first) == (g[i].first-1) && (r[j].second) == (g[i].second-1))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
            else if((r[j].first) == (g[i].first-1) && (r[j].second) == (g[i].second))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
            else if((r[j].first) == (g[i].first-1) && (r[j].second) == (g[i].second+1))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
            else if((r[j].first) == (g[i].first) && (r[j].second) == (g[i].second-1))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
            else if((r[j].first) == (g[i].first) && (r[j].second) == (g[i].second))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
            else if((r[j].first) == (g[i].first) && (r[j].second) == (g[i].second+1))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
            else if((r[j].first) == (g[i].first+1) && (r[j].second) == (g[i].second-1))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
            else if((r[j].first) == (g[i].first+1) && (r[j].second) == (g[i].second))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
            else if((r[j].first) == (g[i].first+1) && (r[j].second) == (g[i].second+1))
            {
                rres.push_back(make_pair(r[j].first,r[j].second));
                gres.push_back(make_pair(g[i].first,g[i].second));

            }
        }
    }
//    fstream out("F:\\1014test\\xingneng\\zongtu\\jieguo.txt", ios::out);
    fstream out("F:\\1026test\\data_5500\\jieguo.txt", ios::out);//data1112
    for(int o = 0; o<rres.size(); o++)
    {
        out<<u8"删除点坐标："<<rres[o].first<<","<<rres[o].second<<endl;
        out<<u8"检测到异常："<<gres[o].first<<","<<gres[o].second<<endl;
        QString yichangstr = QString::fromStdString(u8"检测到异常点: ") + QString::number(gres[o].first)+ " " + QString::number(gres[o].second)+"\n";
        emit deliveryichangToMain(yichangstr);
        out<<endl;
    }
//    2024年10月25日
    for(int oo = 0; oo<r.size(); oo++)
    {
        int count = 0;
        for(int ooo = 0;ooo<rres.size();ooo++){
            if(rres[ooo].first == r[oo].first && rres[ooo].second == r[oo].second){
                break;
            }
            count++;
        }
        if(count==rres.size()){
            QString yichangstr = QString::fromStdString("未检测到: ") + QString::number(r[oo].first)+ " " + QString::number(r[oo].second)+"\n";
            emit deliveryichangToMain(yichangstr);
        }
    }
}
