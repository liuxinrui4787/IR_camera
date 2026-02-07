<<<<<<< HEAD
#ifndef TIPSVIEW_H
#define TIPSVIEW_H

#include <QWidget>

namespace Ui
{
class TipsView;
}

class TipsView : public QWidget
{
    Q_OBJECT

  public:
    explicit TipsView(QWidget *parent = 0);
    ~TipsView();

  public:
    void setTipsWords(QString);
    /**
     * @brief 强制设置坐标
     */
    void setGloblPos(QPoint);
  signals:
    void showTipsWords(QString, QPoint);
    void hideTipsWords();
    //    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
  protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;

  private:
    Ui::TipsView *ui;
    QString m_tipsWords;
    bool m_isSetGloblPos;
    QPoint m_globlPos;
};

#endif // TIPSVIEW_H
=======
#ifndef TIPSVIEW_H
#define TIPSVIEW_H

#include <QWidget>

namespace Ui
{
class TipsView;
}

class TipsView : public QWidget
{
    Q_OBJECT

  public:
    explicit TipsView(QWidget *parent = 0);
    ~TipsView();

  public:
    void setTipsWords(QString);
    /**
     * @brief 强制设置坐标
     */
    void setGloblPos(QPoint);
  signals:
    void showTipsWords(QString, QPoint);
    void hideTipsWords();
    //    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
  protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;

  private:
    Ui::TipsView *ui;
    QString m_tipsWords;
    bool m_isSetGloblPos;
    QPoint m_globlPos;
};

#endif // TIPSVIEW_H
>>>>>>> 3a7b4324667a235e742ed5952d47d3ea6fd845cb
