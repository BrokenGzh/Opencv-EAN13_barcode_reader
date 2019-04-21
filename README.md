# Opencv-EAN13_barcode_reader
opencv4.0+vs2017 （vs14）

基于opencv的一个读取ean13条形码的MFC程序，能够进行条形码边缘检测，可识别出ISSN和ISBN两种码制，对图片质量要求较高。解码部分用ZXING或者ZBAR库会有更好效果。主要内容在showimageDlg.cpp中。

图片预处理部分待改善，对于较高分辨率图片处理较慢，背景复杂情况下对条形码部分的选取也存在问题，可改善阈值的选取

包含可用的例图

数字图像处理课程设计
