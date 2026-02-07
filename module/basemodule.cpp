#include "basemodule.h"
#include "components/qtmaterialstyle.h"
#include "ui_basemodule.h"

BaseModule::BaseModule(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::BaseModule)
{
    ui->setupUi(this);
}

BaseModule::~BaseModule()
{
    delete ui;
}

void BaseModule::init()
{
}

void BaseModule::updateButton(QtMaterialFlatButton *btn, QString iconPath)
{
    btn->setBackgroundMode(Qt::TransparentMode);
    btn->setIcon(QIcon(iconPath));
    btn->setIconSize(QSize(25, 25));
    btn->setIconPadding(5);
    btn->setFontSize(15);
    btn->setFontBold(true);
    btn->setIconPlacement(Material::TopIcon);
    btn->setForegroundColor(QtMaterialStyle::instance().themeColor("balck"));
    btn->setOverlayColor(QtMaterialStyle::instance().themeColor("primary1"));
}
