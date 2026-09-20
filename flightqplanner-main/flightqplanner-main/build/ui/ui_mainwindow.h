/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QPushButton *armPushButton;
    QHBoxLayout *horizontalLayout;
    QLineEdit *takeoffAltLineEdit;
    QPushButton *setTOAltPushButton;
    QPushButton *landPushButton;
    QPushButton *takeoffPushButton;
    QPushButton *setHomePushButton;
    QPushButton *forceDisarmPushButton;
    QGridLayout *gridLayout_4;
    QLineEdit *targetLatLEdit;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_12;
    QPushButton *startMissionButton;
    QLineEdit *targetRelAltLEdit;
    QLineEdit *targetLongLEdit;
    QLineEdit *missionSpeedLEdit;
    QGridLayout *gridLayout_5;
    QPushButton *startVelMissionOffboardButton;
    QLabel *label_14;
    QLineEdit *speedPercLEdit;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QLabel *label_9;
    QLabel *label_5;
    QLabel *label_10;
    QFrame *line_7;
    QLabel *label_2;
    QLabel *label_4;
    QFrame *line_3;
    QLabel *satCountLabel;
    QLabel *label_3;
    QLabel *label;
    QFrame *line_6;
    QLabel *rollLabel;
    QLabel *label_11;
    QLabel *pitchLabel;
    QLabel *longLabel;
    QLabel *lat_label;
    QLabel *absAlt_label;
    QLabel *relAlt_label;
    QLabel *yawLabel;
    QFrame *line_5;
    QFrame *line_4;
    QFrame *line_2;
    QFrame *line;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(741, 539);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setHorizontalSpacing(30);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        armPushButton = new QPushButton(centralwidget);
        armPushButton->setObjectName("armPushButton");

        gridLayout_2->addWidget(armPushButton, 2, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        takeoffAltLineEdit = new QLineEdit(centralwidget);
        takeoffAltLineEdit->setObjectName("takeoffAltLineEdit");

        horizontalLayout->addWidget(takeoffAltLineEdit);

        setTOAltPushButton = new QPushButton(centralwidget);
        setTOAltPushButton->setObjectName("setTOAltPushButton");

        horizontalLayout->addWidget(setTOAltPushButton);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 2);

        landPushButton = new QPushButton(centralwidget);
        landPushButton->setObjectName("landPushButton");

        gridLayout_2->addWidget(landPushButton, 4, 0, 1, 1);

        takeoffPushButton = new QPushButton(centralwidget);
        takeoffPushButton->setObjectName("takeoffPushButton");

        gridLayout_2->addWidget(takeoffPushButton, 3, 0, 1, 1);

        setHomePushButton = new QPushButton(centralwidget);
        setHomePushButton->setObjectName("setHomePushButton");

        gridLayout_2->addWidget(setHomePushButton, 5, 0, 1, 1);

        forceDisarmPushButton = new QPushButton(centralwidget);
        forceDisarmPushButton->setObjectName("forceDisarmPushButton");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(forceDisarmPushButton->sizePolicy().hasHeightForWidth());
        forceDisarmPushButton->setSizePolicy(sizePolicy);
        forceDisarmPushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(237, 51, 59);"));

        gridLayout_2->addWidget(forceDisarmPushButton, 2, 1, 2, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 1, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_4->setContentsMargins(-1, -1, -1, 0);
        targetLatLEdit = new QLineEdit(centralwidget);
        targetLatLEdit->setObjectName("targetLatLEdit");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(targetLatLEdit->sizePolicy().hasHeightForWidth());
        targetLatLEdit->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(targetLatLEdit, 0, 1, 1, 1);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");

        gridLayout_4->addWidget(label_6, 0, 0, 1, 1);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");

        gridLayout_4->addWidget(label_7, 1, 0, 1, 1);

        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");

        gridLayout_4->addWidget(label_8, 2, 0, 1, 1);

        label_12 = new QLabel(centralwidget);
        label_12->setObjectName("label_12");

        gridLayout_4->addWidget(label_12, 3, 0, 1, 1);

        startMissionButton = new QPushButton(centralwidget);
        startMissionButton->setObjectName("startMissionButton");
        sizePolicy1.setHeightForWidth(startMissionButton->sizePolicy().hasHeightForWidth());
        startMissionButton->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(startMissionButton, 4, 1, 1, 1);

        targetRelAltLEdit = new QLineEdit(centralwidget);
        targetRelAltLEdit->setObjectName("targetRelAltLEdit");
        sizePolicy1.setHeightForWidth(targetRelAltLEdit->sizePolicy().hasHeightForWidth());
        targetRelAltLEdit->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(targetRelAltLEdit, 2, 1, 1, 1);

        targetLongLEdit = new QLineEdit(centralwidget);
        targetLongLEdit->setObjectName("targetLongLEdit");
        sizePolicy1.setHeightForWidth(targetLongLEdit->sizePolicy().hasHeightForWidth());
        targetLongLEdit->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(targetLongLEdit, 1, 1, 1, 1);

        missionSpeedLEdit = new QLineEdit(centralwidget);
        missionSpeedLEdit->setObjectName("missionSpeedLEdit");
        sizePolicy1.setHeightForWidth(missionSpeedLEdit->sizePolicy().hasHeightForWidth());
        missionSpeedLEdit->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(missionSpeedLEdit, 3, 1, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName("gridLayout_5");
        startVelMissionOffboardButton = new QPushButton(centralwidget);
        startVelMissionOffboardButton->setObjectName("startVelMissionOffboardButton");

        gridLayout_5->addWidget(startVelMissionOffboardButton, 1, 1, 1, 1);

        label_14 = new QLabel(centralwidget);
        label_14->setObjectName("label_14");

        gridLayout_5->addWidget(label_14, 0, 0, 1, 1);

        speedPercLEdit = new QLineEdit(centralwidget);
        speedPercLEdit->setObjectName("speedPercLEdit");
        sizePolicy1.setHeightForWidth(speedPercLEdit->sizePolicy().hasHeightForWidth());
        speedPercLEdit->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(speedPercLEdit, 0, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_5, 5, 0, 1, 2);


        gridLayout_3->addLayout(gridLayout_4, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 2, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName("label_9");

        gridLayout->addWidget(label_9, 6, 5, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 4, 1, 1, 1);

        label_10 = new QLabel(centralwidget);
        label_10->setObjectName("label_10");

        gridLayout->addWidget(label_10, 4, 5, 1, 1);

        line_7 = new QFrame(centralwidget);
        line_7->setObjectName("line_7");
        line_7->setFrameShape(QFrame::Shape::HLine);
        line_7->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line_7, 5, 5, 1, 2);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 2, 5, 1, 1);

        line_3 = new QFrame(centralwidget);
        line_3->setObjectName("line_3");
        line_3->setFrameShape(QFrame::Shape::HLine);
        line_3->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line_3, 1, 5, 1, 2);

        satCountLabel = new QLabel(centralwidget);
        satCountLabel->setObjectName("satCountLabel");

        gridLayout->addWidget(satCountLabel, 2, 6, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 2, 1, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 5, 1, 1);

        line_6 = new QFrame(centralwidget);
        line_6->setObjectName("line_6");
        line_6->setFrameShape(QFrame::Shape::HLine);
        line_6->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line_6, 3, 5, 1, 2);

        rollLabel = new QLabel(centralwidget);
        rollLabel->setObjectName("rollLabel");

        gridLayout->addWidget(rollLabel, 6, 6, 1, 1);

        label_11 = new QLabel(centralwidget);
        label_11->setObjectName("label_11");

        gridLayout->addWidget(label_11, 6, 1, 1, 1);

        pitchLabel = new QLabel(centralwidget);
        pitchLabel->setObjectName("pitchLabel");

        gridLayout->addWidget(pitchLabel, 4, 6, 1, 1);

        longLabel = new QLabel(centralwidget);
        longLabel->setObjectName("longLabel");

        gridLayout->addWidget(longLabel, 0, 6, 1, 1);

        lat_label = new QLabel(centralwidget);
        lat_label->setObjectName("lat_label");

        gridLayout->addWidget(lat_label, 0, 2, 1, 1);

        absAlt_label = new QLabel(centralwidget);
        absAlt_label->setObjectName("absAlt_label");

        gridLayout->addWidget(absAlt_label, 2, 2, 1, 1);

        relAlt_label = new QLabel(centralwidget);
        relAlt_label->setObjectName("relAlt_label");

        gridLayout->addWidget(relAlt_label, 4, 2, 1, 1);

        yawLabel = new QLabel(centralwidget);
        yawLabel->setObjectName("yawLabel");

        gridLayout->addWidget(yawLabel, 6, 2, 1, 1);

        line_5 = new QFrame(centralwidget);
        line_5->setObjectName("line_5");
        line_5->setFrameShape(QFrame::Shape::HLine);
        line_5->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line_5, 5, 1, 1, 2);

        line_4 = new QFrame(centralwidget);
        line_4->setObjectName("line_4");
        line_4->setFrameShape(QFrame::Shape::HLine);
        line_4->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line_4, 3, 1, 1, 2);

        line_2 = new QFrame(centralwidget);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::Shape::HLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line_2, 1, 1, 1, 2);

        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::VLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout->addWidget(line, 0, 3, 7, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 2, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "FlightQPlanner", nullptr));
        armPushButton->setText(QCoreApplication::translate("MainWindow", "Arm", nullptr));
        setTOAltPushButton->setText(QCoreApplication::translate("MainWindow", "Set Takeoff Altitude", nullptr));
        landPushButton->setText(QCoreApplication::translate("MainWindow", "Land", nullptr));
        takeoffPushButton->setText(QCoreApplication::translate("MainWindow", "Takeoff Async", nullptr));
        setHomePushButton->setText(QCoreApplication::translate("MainWindow", "Set Home", nullptr));
        forceDisarmPushButton->setText(QCoreApplication::translate("MainWindow", "Force Disarm", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Target Lat:", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Target Long:", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Target Rel Alt:", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Speed (m/s)", nullptr));
        startMissionButton->setText(QCoreApplication::translate("MainWindow", "Go Via 2 WP Mission", nullptr));
        missionSpeedLEdit->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        startVelMissionOffboardButton->setText(QCoreApplication::translate("MainWindow", "Go With Velocity", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "speed: perc:", nullptr));
        speedPercLEdit->setText(QCoreApplication::translate("MainWindow", "30", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Roll:", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Rel. Altitude:", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Pitch:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Latitude:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Sat Count:", nullptr));
        satCountLabel->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "Abs Altitude:", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Longitude:", nullptr));
        rollLabel->setText(QString());
        label_11->setText(QCoreApplication::translate("MainWindow", "Yaw:", nullptr));
        pitchLabel->setText(QString());
        longLabel->setText(QString());
        lat_label->setText(QString());
        absAlt_label->setText(QString());
        relAlt_label->setText(QString());
        yawLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
