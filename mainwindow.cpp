#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QHttpMultiPart>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>

void MainWindow::deal(QNetworkReply* reply){
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        qDebug() << "statusCode:" << statusCode;

        if(reply->error() == QNetworkReply::NoError)
        {
            //官方文档显示返回的reply是json格式，所以我们采用json格式读取
            QByteArray allData = reply->readAll();
            QJsonParseError json_error;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
            if(json_error.error != QJsonParseError::NoError)
               {
                   qDebug() << "json error!";
                   return;
               }
            QJsonObject rootObj = jsonDoc.object();
              //我们只需要上传后的url，所以值提取了url，如果你需要其他内容，请提取方法类似
              if(rootObj.contains("data"))
                {
                   QJsonObject subObj = rootObj.value("data").toObject();
                   qDebug() << subObj["url"].toString();
                   /*
                    statusCode: 200
                    "https://i.loli.net/2020/11/14/9mSbRK1yx6jlpGX.png"
                   */
                }

        }
        else
        {
                qDebug() << "=========";
        }

        reply->deleteLater();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(deal(QNetworkReply*)));

    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"text\""));
    textPart.setBody("my text");

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/png"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("multipart/form-data; name=\"smfile\"; filename=\"image.png\""));
//    QString strFilePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择上传图片"), "./", tr("Image files(*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm *.jpeg);;All files (*.*)"));

    QFile *file = new QFile("/home/abelard/Pictures/2019-02-13_11-41-parker.png");
    if(!file->open(QIODevice::ReadOnly)){
       std::cout<<"------------------------------file open error";
    }
    imagePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart


    multiPart->append(textPart);
    multiPart->append(imagePart);

//    QUrl url("http://192.168.2.235:1901/ttank_admin/v2/api/uploads");
    QUrl url("https://sm.ms/api/v2/upload");
    QNetworkRequest request(url);
    request.setRawHeader("Authorization","ApyfgSlDmRMVrz5gi4VEMuDzq86vDZpw");
    manager->post(request, multiPart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

