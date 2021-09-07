#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->VeloSlider, SIGNAL(valueChanged(int)), ui->RefVeloLcd, SLOT(display(int)));
    connect(ui->SWADegreeSlider, SIGNAL(valueChanged(int)), ui->RefSWALcd, SLOT(display(int)));

    ui->EngagedLabel->setText("Disengaged");
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
        ui->EngagePushButton->setText("Disengage");
    }
    else
    {
        Engaged = false;
        ui->EngagePushButton->setText("Engage");
    }
}

