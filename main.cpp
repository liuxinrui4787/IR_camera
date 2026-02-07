#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
#include <appwidget.h>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#endif
    //    QFileInfo file(__FILE__);
    //    Log4Qt::PropertyConfigurator::configure(QDir::toNativeSeparators(":/conf/logs.conf"));

    //    VerLogFilePreProcess verLogPreProcess;
    //    verLogPreProcess.ReadFile("D:\workspace\TrustHub-Benchmarks\EthernetMAC10GE-T710\EthernetMAC10GE-T710\src\TrojanInserted\xge_mac_scan.v");

    //    QString words = "vga_enh_top_DW01_dec_2 pixel_generator_vtgen_ver_gen_sub_103";
    //    QString check = "vga_enh_top_DW01_inc_2";

    //    if (words.split(" ")[0] == check)
    //    {
    //        qDebug() << "sotp!";
    //    }
    //    else
    //    {
    //        qDebug() << "nonono";
    //    }
    //    int index = words.indexOf(check + " ");

    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 9));
    a.setWindowIcon(QIcon(":/main.ico"));

    AppWidget qui;
    MainWindow *creator = new MainWindow;

    //设置主窗体
    qui.setMainWidget(creator);
    qui.setStyleWithFile(":/qss/silvery.css");
    qui.setTitle(u8"红外图像恶意逻辑分析");
    creator->winTitle(qui.getWinTitle());
    creator->winLabIcon(qui.getLabIco());

    //    //设置标题文本居中
    //    qui.setAlignment(Qt::AlignCenter);

    //设置窗体可拖动大小
    qui.setSizeGripEnabled(false);

    qui.show();

    return a.exec();
}
