#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->armPushButton, &QPushButton::clicked, this, &MainWindow::armClicked);
    connect(ui->forceDisarmPushButton, &QPushButton::clicked, this, &MainWindow::forceDisarmClicked);
    connect(ui->setTOAltPushButton, &QPushButton::clicked, this, &MainWindow::setTOAltClicked);
    connect(ui->takeoffPushButton, &QPushButton::clicked, this, &MainWindow::takeoffClicked);
    connect(ui->startMissionButton, &QPushButton::clicked, this, &MainWindow::execute2WPMissionClicked);
    connect(ui->startVelMissionOffboardButton, &QPushButton::clicked, this, &MainWindow::executeVelocityOffboardClicked);
    connect(ui->landPushButton, &QPushButton::clicked, this, &MainWindow::landClicked);
    connect(ui->setHomePushButton, &QPushButton::clicked, this, &MainWindow::setHomeClicked);
    connect(ui->testMotorButton, &QPushButton::clicked, this, &MainWindow::testMotorClicked);
    connect(ui->testAllMotorsButton, &QPushButton::clicked, this, &MainWindow::testAllMotorsClicked);




    m_controller = new FController(false);
    //m_controller = new FController(true);

    connect(m_controller, &FController::sgn_pushGPSData, this, &MainWindow::updateGPSData);
    connect(m_controller, &FController::sgn_pushGPSInfo, this, &MainWindow::updateGPSInfo);
    connect(m_controller, &FController::sgn_pushYPR, this, &MainWindow::updateYPR);


    m_controller->connect();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::armClicked()
{
    //initilize for future mission...
    ui->targetLatLEdit->setText(QString::number(m_currentLat, 'g', 10));
    ui->targetLongLEdit->setText(QString::number(m_currentLong, 'g', 10));
    ui->targetRelAltLEdit->setText(QString::number(m_currentRelAlt));

    QApplication::processEvents();

    m_controller->armIfNeeded();
    QApplication::processEvents();

}

void MainWindow::forceDisarmClicked()
{    
    m_controller->forceDisarm();
    QApplication::processEvents();

}

void MainWindow::setTOAltClicked()
{
    float alt = ui->takeoffAltLineEdit->text().toFloat();
    if(alt > 50 || alt < 1.5){
        QMessageBox::warning(this, "Wrong Takeoff Altitude", "Wrong takeoff altitude: " + QString::number(alt));
        return;
    }

    if(m_controller->isArmed()){
        QMessageBox::warning(this, "Cannot set takeoff altitude", "Quadrone is armed. Cannot change the takeoff altitude.");
        ui->takeoffAltLineEdit->setText(QString::number(m_controller->getTakeoffAltitude()));
        return;
    }

    QApplication::processEvents();

    float assignedTOAlt = m_controller->setTakeoffAltitude(alt);
    ui->takeoffAltLineEdit->setText(QString::number(assignedTOAlt));
    QMessageBox::information(this, "Done", "Takeoff altitude has been set to: " + QString::number(assignedTOAlt));
    QApplication::processEvents();

}

void MainWindow::takeoffClicked()
{
    if(!m_controller->isArmed()){
        QMessageBox::warning(this, "Cannot takeoff", "Quadrone is not armed. Cannot change takeoff.");
        return;
    }
    QApplication::processEvents();

    m_controller->takeoffAsync();
    QApplication::processEvents();

}

void MainWindow::execute2WPMissionClicked()
{

    if(!m_controller->isArmed()){
        QMessageBox::warning(this, "Cannot start mission", "Quadrone is not armed. Cannot start mission.");
        return;
    }

    QApplication::processEvents();

    double targetLat = ui->targetLatLEdit->text().toDouble();
    double targetLong = ui->targetLongLEdit->text().toDouble();
    double targetRelAlt = ui->targetRelAltLEdit->text().toDouble();
    double speed = ui->missionSpeedLEdit->text().toDouble();
    QApplication::processEvents();

    //m_controller->execute2WPMissionFromCurrentGPS(targetLat, targetLong, targetRelAlt, speed);

    QtConcurrent::run(std::bind(&FController::execute2WPMissionFromCurrentGPS,
                                m_controller,
                                targetLat,
                                targetLong,
                                targetRelAlt,
                                speed));

    QApplication::processEvents();

}

void MainWindow::executeVelocityOffboardClicked()
{
    if(!m_controller->isArmed()){
        QMessageBox::warning(this, "Cannot start mission", "Quadrone is not armed. Cannot start mission.");
        return;
    }

    QApplication::processEvents();

    double targetLat = ui->targetLatLEdit->text().toDouble();
    double targetLong = ui->targetLongLEdit->text().toDouble();
    double targetRelAlt = ui->targetRelAltLEdit->text().toDouble();
    double speedPerc = ui->speedPercLEdit->text().toDouble();

    QApplication::processEvents();

    QtConcurrent::run(std::bind(&FController::goToGPSPointWithVelocity,
                                m_controller,
                                targetLat,
                                targetLong,
                                targetRelAlt,speedPerc));



    QApplication::processEvents();

}

void MainWindow::landClicked()
{
    if(!m_controller->isArmed()){
        QMessageBox::warning(this, "Cannot land", "Quadrone is not armed. Cannot land.");
        return;
    }


    m_controller->land();
}

void MainWindow::setHomeClicked()
{
    qDebug() << "setHomeClicked";
    m_controller->setCurrentPositionAsHome();
}

void MainWindow::testMotorClicked() {
    //Arayüzdeki değerleri al
    int motorIndex = ui->motorIndexSpinBox->value();
    float throttle = ui->throttleSpinBox->value();
    float timeout = ui->timeoutSpinBox->value();

    //Güvenlik kontrolü
    if(m_controller->isArmed()){
        QMessageBox::warning(this, "Güvenlik", "Drone Armed durumundayken test yapılamaz.");
        return;
    }

    // Arka planı tetikle
    m_controller->testMotor(motorIndex, throttle, timeout);

    if(throttle > 20) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "DİKKAT!",
                                      "Güç %20'nin üzerinde! Pervanelerin sökülü olduğundan emin misiniz?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return; // Kullanıcı iptal ederse arka plana komut gönderme
        }
    }
}

void MainWindow::testAllMotorsClicked() {
    float throttle = ui->throttleSpinBox->value();
    float timeout = ui->timeoutSpinBox->value();

    if(m_controller->isArmed()){
        QMessageBox::warning(this, "Güvenlik", "Drone Armed durumundayken test yapılamaz.");
        return;
    }

    m_controller->testAllMotors(throttle,timeout);

    if(throttle > 20) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "DİKKAT!",
                                      "Güç %20'nin üzerinde! Pervanelerin sökülü olduğundan emin misiniz?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return; // Kullanıcı iptal ederse arka plana komut gönderme
        }
    }
}

void MainWindow::updateGPSData(double lat, double log, double absAlt, double relAlt)
{
    m_currentLat = lat;
    m_currentLong = log;
    m_currentRelAlt = relAlt;

    ui->lat_label->setText(QString::number(lat, 'g', 10));
    ui->longLabel->setText(QString::number(log, 'g', 10));
    ui->absAlt_label->setText(QString::number(absAlt));
    ui->relAlt_label->setText(QString::number(relAlt));

}

void MainWindow::updateGPSInfo(int satCnt)
{
    ui->satCountLabel->setText(QString::number(satCnt));
}

void MainWindow::updateYPR(double y, double p, double r)
{
    ui->rollLabel->setText(QString::number(r));
    ui->yawLabel->setText(QString::number(y));
    ui->pitchLabel->setText(QString::number(p));

}



