#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->EngagedLabel->setText("Disengaged");

    Com = new ComModule("192.168.0.10", 9999, "192.168.0.2", 9999);
    connect(ui->VeloSlider, SIGNAL(valueChanged(int)), ui->RefVeloLcd, SLOT(display(int)));
    connect(ui->SWADegreeSlider, SIGNAL(valueChanged(int)), ui->RefSWALcd, SLOT(display(int)));
    connect(ui->VeloSlider, SIGNAL(valueChanged(int)), Com, SLOT(SetRefVelo(int)));
    connect(ui->SWADegreeSlider, SIGNAL(valueChanged(int)), Com, SLOT(SetRefSWA(int)));
    connect(Com, SIGNAL(UpdateVelo(double)), ui->CurrVeloLcd, SLOT(display(double)));
    connect(Com, SIGNAL(UpdateSWA(double)), ui->CurrSWALcd, SLOT(display(double)));
    connect(Com, SIGNAL(UpdateLabel(QString)), ui->EngagedLabel, SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_EngagePushButton_clicked()
{
    qDebug() << "button is pushed";

    if(Engaged == false)
    {
        Engaged = true;
        Com->EngageReq();
        ui->EngagePushButton->setText("Disengage");
    }
    else
    {
        Engaged = false;
        Com->DisengageReq();
        ui->EngagePushButton->setText("Engage");
    }
}

