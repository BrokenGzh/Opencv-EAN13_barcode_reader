# Opencv-EAN13_barcode_reader
opencv4.0+vs2017 （vs14） win 64位

基于opencv的一个读取ean13条形码的MFC程序，控件自适应窗口大小，选择图片自适应控件大小，支持JPG,BMP等格式。能够进行条形码边缘检测并将结果显示在控件，可识别出ISSN和ISBN两种码制，对图片质量要求较高。解码部分用ZXING或者ZBAR库会有更好效果。主要内容在showimageDlg.cpp中。

预处理部分采用形态学处理的办法，对不同背景条件的图片效果不同，译码采用平均值译码，相似距离译码的效果也会更好一些

图片预处理部分待改善，对于较高分辨率图片处理较慢，背景复杂情况下对条形码部分的选取也存在问题，可改善阈值的选取

包含可用的例图

数字图像处理课程设计
