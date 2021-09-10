#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QHostAddress *ServerIp)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->EngagedLabel->setText("Disengaged");

    Com = new ComModule("0.0.0.0", 9999, *ServerIp, 9999);
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

