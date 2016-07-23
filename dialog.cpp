#include "dialog.h"
#include "ui_dialog.h"
#include <QtNetwork>
#include <QTextCodec>
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("天气预报"));
    setFixedSize(width(), height());
    setWindowIcon(QIcon(QPixmap(":/rc/a_0.gif")));
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    ui->labelLogo->setPixmap(QPixmap(":/rc/r_t1.gif"));
    ui->labelCity->setPixmap(QPixmap(":/rc/r_b.gif"));
    showClear();

    cityManager_.getSupportProvince(std::bind(&Dialog::onProvinces, this, std::placeholders::_1));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onProvinces(const QStringList &strList)
{
    QStringList::const_iterator citer = strList.constBegin();
    for ( ; citer != strList.constEnd(); ++citer) {
        ui->provinces->addItem(*citer);
    }
    ui->provinces->setCurrentIndex(0);
    cityManager_.getSupportCity(ui->provinces->currentText(), std::bind(&Dialog::onCitys, this, std::placeholders::_1));
}

void Dialog::onCitys(const QStringList &strList)
{
    ui->citys->clear();
    QStringList::const_iterator citer = strList.constBegin();
    for ( ; citer != strList.constEnd(); ++citer) {
        QString str = citer->left(citer->indexOf(QChar(' ')));
        ui->citys->addItem(str);
    }
}

void Dialog::onWeather(const QStringList &strList)
{
    if (strList.count() < 23) {
        return;
    }

    QString strImage(":/rc/");
    ui->labelImage1->setPixmap(QPixmap(strImage + strList[8]));
    ui->labelImage2->setPixmap(QPixmap(strImage + strList[9]));
    ui->labelTemperature->setText(strList[5]);
    ui->labelWeather->setText(strList[6]);
    ui->labelWind->setText(strList[7]);

    ui->labelImage3->setPixmap(QPixmap(strImage + strList[15]));
    ui->labelImage4->setPixmap(QPixmap(strImage + strList[16]));
    ui->labelTemperature_2->setText(strList[12]);
    ui->labelWeather_2->setText(strList[13]);
    ui->labelWind_2->setText(strList[14]);

}

void Dialog::on_provinces_currentIndexChanged(const QString &provinceName)
{
    showClear();
    cityManager_.getSupportCity(provinceName, std::bind(&Dialog::onCitys, this, std::placeholders::_1));
}

void Dialog::on_findButton_clicked()
{
    cityManager_.getWeatherbyCityName(ui->citys->currentText(), std::bind(&Dialog::onWeather, this, std::placeholders::_1));
}

void Dialog::showClear()
{
    ui->labelImage1->clear();
    ui->labelImage2->clear();
    ui->labelTemperature->clear();
    ui->labelWeather->clear();
    ui->labelWind->clear();

    ui->labelImage3->clear();
    ui->labelImage4->clear();
    ui->labelTemperature_2->clear();
    ui->labelWeather_2->clear();
    ui->labelWind_2->clear();
}

void Dialog::on_citys_currentIndexChanged(const QString &)
{
    showClear();
}
