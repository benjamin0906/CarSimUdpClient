#include "commodule.h"

ComModule::ComModule(QString SrcIp, unsigned int SrcPort, QString DstIp, unsigned int DstPort)
{
    socket = new QUdpSocket();
    this->DstPort = DstPort;
    Src = new QHostAddress(SrcIp);
    Dst = new QHostAddress(DstIp);
    socket->bind(*Src, SrcPort);

    Timer = new QTimer();
    connect(Timer, SIGNAL(timeout()), this, SLOT(QuerrySimState()));
    Timer->start(1000);
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
}

ComModule::~ComModule()
{
    delete(Src);
    delete(Dst);
    delete(Timer);
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
