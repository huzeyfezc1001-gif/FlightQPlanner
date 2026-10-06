#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "FController.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    FController *m_controller;

    double m_currentLat, m_currentLong, m_currentRelAlt;

private slots:
    void armClicked();
    void forceDisarmClicked();
    void setTOAltClicked();
    void takeoffClicked();
    void execute2WPMissionClicked();
    void executeVelocityOffboardClicked();
    void landClicked();
    void setHomeClicked();
    void testMotorClicked();
    void testAllMotorsClicked();

    void updateGPSData(double lat, double log, double absAlt, double relAlt);
    void updateGPSInfo(int satCnt);
    void updateYPR(double y, double p, double r);

};
#endif // MAINWINDOW_H
