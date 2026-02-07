#ifndef IMGCROPCONFIG_H
#define IMGCROPCONFIG_H
#include <opencv2/core.hpp>
#include <QObject>

struct ImgCropConfig {
    cv::Rect matRect;//局部图像定位
    int thresholdType;//二值化方式
    int threshType;//二值化取值方式
    int erosionSize;//侵蚀强度

};

Q_DECLARE_METATYPE(
    ImgCropConfig)  //通过Q_DECLARE_METATYPE声明后，就可以让自定义的类型设置到QVariant

#endif // IMGCROPCONFIG_H
