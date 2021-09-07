#include "commodule.h"

ComModule::ComModule(QString SrcIp, unsigned int SrcPort, QString DstIp, unsigned int DstPort)
{
    socket = new QUdpSocket();
    this->DstPort = DstPort;
    Src = new QHostAddress(SrcIp);
    Dst = new QHostAddress(DstIp);
    socket->bind(*Src, SrcPort);

    StateTimer = new QTimer();
    connect(StateTimer, SIGNAL(timeout()), this, SLOT(QuerrySimState()));
    StateTimer->start(100);

    RefTimer = new QTimer();
    connect(RefTimer, SIGNAL(timeout()), this, SLOT(SendRefs()));
}

void ComModule::EngageReq(void)
{
    /* Building the data */
    QByteArray Data(3, 0);
    Data[0] = Msg_EngageReq;
    Data[1] = SeqCntr;
    Data[2] = 1;
    socket->writeDatagram(Data, *Dst, DstPort);
    IncSeqCntr();
    RefTimer->start(100);
}

void ComModule::DisengageReq(void)
{
    /* Building the data */
    QByteArray Data(3, 0);
    Data[0] = Msg_EngageReq;
    Data[1] = SeqCntr;
    Data[2] = 0;
    socket->writeDatagram(Data, *Dst, DstPort);
    IncSeqCntr();
    RefTimer->stop();
}

ComModule::~ComModule()
{
    delete(Src);
    delete(Dst);
    delete(StateTimer);
}

void ComModule::IncSeqCntr(void)
{
    /* Incrementing the sequence counter */
    SeqCntr++;
    if(SeqCntr == 0) SeqCntr++;
}

void ComModule::QuerrySimState(void)
{
    /* Building the data */
    QByteArray Data(2, 0);
    Data[0] = Msg_StateReq;
    Data[1] = SeqCntr;
    socket->writeDatagram(Data, *Dst, DstPort);

    Data.resize(socket->pendingDatagramSize());
    int retVal = socket->readDatagram(Data.data(), Data.size(), 0, 0);

    if(retVal == 7)
    {
        if(Data[2] != 0) emit UpdateLabel("Engaged");
        else emit UpdateLabel("Disengaged");

        QString debug = QString::number((unsigned int)Data[0], 16);
        debug += QString::number((unsigned int)Data[0], 16);

        /* Building Velocity from bytes */
        int Velo = ((int)Data[3]&0xFF);
        Velo <<= 8;
        Velo |= ((int)Data[4]&0xFF);

        /* Shifting to expand to negative values if needed */
        Velo <<= 16;
        Velo >>= 16;

        /* Building RawSWA from bytes */
        int RawSWA = ((int)Data[5]&0xFF);
        RawSWA <<= 8;
        RawSWA |= ((int)Data[6]&0xFF);

        /* Shifting to expand to negative values if needed */
        RawSWA <<= 16;
        RawSWA >>= 16;

        emit UpdateVelo((double)Velo*9/250);
        emit UpdateSWA((double)RawSWA/M_PI*180/1000);
    }
    IncSeqCntr();
}

void ComModule::SendRefs(void)
{
    /* Building the data */
    QByteArray Data(6, 0);
    Data[0] = Msg_SetRef;
    Data[1] = SeqCntr;
    Data[2] = (unsigned char)(RefVelo>>8);
    Data[3] = (unsigned char)(RefVelo&0xFF);
    Data[4] = (unsigned char)(RefSWA>>8);
    Data[5] = (unsigned char)(RefSWA&0xFF);
    socket->writeDatagram(Data, *Dst, DstPort);
    IncSeqCntr();
}

void ComModule::SetRefVelo(int Velo)
{
    RefVelo = roundf((float)Velo*250/9);
    qDebug() << "Velo: " << RefVelo;
}

void ComModule::SetRefSWA(int SWA)
{
    RefSWA = roundf((float)SWA*M_PI/180*1000);
}
