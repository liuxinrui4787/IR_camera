#ifndef STRUCTDEFINE_H
#define STRUCTDEFINE_H
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMessageBox>
#include <QMetaType>
#include <QString>
#include <QVector>

struct gate
{
    QString gatetypename;
    // int typenum;
    uint gatetype;
    int gatenamenum;
    bool isinput;
    bool isoutput;
    QVector<int> outputvector;
    QVector<int> inputvector;
    QVector<int> ckvector;
    QVector<int> dvector;
    QVector<int> rstvector;
    QVector<int> pregate;
    QVector<int> cktorf;
    QVector<int> dtorf;
    QVector<int> rsttorf;
    /**
     * OAI21BA    A0 ->
     *                   nor ->
     *            A1 ->
     *
     *            B0 ->  no  ->  NAND
     *
     *            C0         ->
     *
     *            此处B0需要添加一个NO门
     * @brief noPortVector
     */
    QStringList noPortVector; //反向门的序号 针对OAI21BA
    QVector<int> gateArray; //复合门的组合逻辑
    QVector<QList<int>> nextgate;
    QStringList unionPortNames; //
    bool isUnionMap = false;
    //针对NetUnion 当为true时
    //需要使用inputvector按顺序保存对应的序号
    //unionPortNames保存对应顺序的端口名
    //复合门的创建方式
    QJsonObject m_unionConf;
};

enum GateTypeEnum
{
    NONE = 0, //未知
    AND = 1,
    NAND = 2, //与非
    OR = 3,
    NOR = 4, //或非
    NO = 5, //NOT 非
    XNOR = 6, //同或
    XOR = 7, //异或
    ON = 8, //ONE
    ZER = 9, //ZERO
    BU = 10, //BUFFER门
    MUX = 11, //多路选择器
    MUXI = 12, //反向多路选择器
    NETINPUT = 13,
    FD = 14,
    FJK = 15,
    FJK12S = 16,
    FJK34S = 17,
    LD = 18,
    AND_NO = 19,
    OR_NO = 20,
    RAM = 21, //之后的门全部是复合门，会被拆成基本门的组合
    AO = 22,
    AOI = 23,
    AOIN = 24,
    OA = 25,
    OAI = 26,
    OAIN = 27,
    ADD = 28,
    FDS = 29,
    IOA = 30,
    UNION = 31
};

enum CdlKWordsEnum
{
    INCLUDE = 1,
    SUBCKT = 2,
    ENDS = 3,
    GLOBAL = 4
};

enum MRCLnum
{
    MOS = 1,
    RE = 2,
    CA = 3,
    INDUCTOR = 4
};

enum ParaEnum
{
    M = 1,
    L = 2,
    W = 3,
    F = 4,
    A = 5,
    P = 6
};

enum UnitEnum
{
    u = -6,
    n = -9,
    e = 1
};

struct TJconfig
{
    bool TypeRand;
    int victimSize;
    int victimMaxInedx;
    int triggersize;
    int triggerMaxIndex;
};

struct Para
{
    int paraID;
    double para;
    int unitID;
};
// QMap <QString,uint> VGateNameList;

// typedef QMap<int,int> WIRE_GATE;

struct NetLoopState
{
    void *net; //用于保存用户指针
    uint8_t num; //当前时间的值 0或者1
    uint32_t time;
    //    RoadType rtype = RoadType::ALL; //路径类型
    uint loopstartidx; // 当前路径的上一个节点所在list中的序号
    uint16_t rid = 0; //当前起始位置的编号 高8位为当前信息 低8位为总长度
    bool moreroad = false; //是否有多条路
    void *loop_next; //在相同环路中，上一次出现的路径，如果连续出现相同的走法，在判断时，将下一个路径纪录下来
    // eg G10-G29-G10-G29 在第二次遇到G29时会发现，连续走了相同的路，则会将loop_next纪录为G10
};

struct NetLoopIdx
{
    uint myIdx; //结点当前所在位置
    uint sIdx = 0; //当次循环起始序号
    uint myNextIdx = 0; //我的下一个目标的位置,如果是retry时，myIdx不为0
    //    bool isReTry = false; //是否是重试
};

enum ActivityState
{
    UNKNOW = 0, //未知
    ENABLE = 1, //可激活
    UNABLE = 2, //不可激活
    LOOP = 3, //循环过多跳出
    TIMEOUT = 4,
    SAME = 5 //DFS算法中，用于标记要求相同的值，当值相同时，不需要再添加新的节点
};

enum Signal
{
    V0,
    V1,
    X,
};

namespace Run
{
enum class Step
{
    NONE, //未运行                0
    READVFILE, //读取网表文件     1
    SCAN, //扫描文件 网表解析       2
    BUILD, //建立网表文件         3
    LINK, //建立连接关系          4
    INSETTJ, //插入木马         5
    CHECKCC, //计算可控制性               6
    CHECKJH, //判断是否可激活              7
    DATAINTEREST, //少态点筛选            8
    OUTPUT, //导出数据，送给神经网络学习     9
    OVER //结束,结束时变成可用           10
};

enum class CDLStep
{
    NONE, //未运行
    READCDLFILE, //读取网表文件
    SCAN, //扫描文件 网表解析
    BUILD, //建立网表文件
    LINK, //建立连接关系
    INSETTJ, //插入木马
    CIRCUITCC, //电路分析
    PARAMETERCC, //参数分析
    PARAMETERYCCC, //参数异常分析
    OUTPUT, //导出数据，送给网络学习
    OVER //结束,结束时变成可用
};
} // namespace Run

struct InfoBundle
{
    QString input; //文件输入
    QString output; //保存路径
    bool isLocal; //是否在本地
    QString server_url; //服务器IP
    int server_port; //服务器端口
    int type = 0; //当前连接的网络类型  0为gnn 1为gat
    QString feature; //feature文件路径
    QString edge; //edge文件路径
    QString featureSample;
    QString edgeSample;
};

//插入木马的数组结构
struct TrojanInfo
{
    QStringList trigger;
    QStringList payload;
    QJsonObject type;
};

struct TrojanItem
{
    QString tjName;
    int type;
    int times; //当前木马插入数量
    int inputStep;
    int linkStep;
    int outputStep;
    bool isCreateNormal; //是否生成正常子图
    QString rndInfo;
};


Q_DECLARE_METATYPE(TrojanItem)




#endif // STRUCTDEFINE_H
