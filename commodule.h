#ifndef COMMODULE_H
#define COMMODULE_H

#include <QObject>
#include <QString>
#include <QUdpSocket>
#include <QTimer>

enum eMsgType
{
    Msg_StateReq,
    Msg_State,
    Msg_EngageReq,
    Msg_SetRef,
};

class ComModule : public QObject
{
    Q_OBJECT
public:
    ComModule(QString SrcIp, unsigned int SrcPort, QString DstIp, unsigned int DstPort);
    void EngageReq(void);
    void DisengageReq(void);
    ~ComModule();

signals:
    void UpdateSWA(double Degree);
    void UpdateVelo(double Velo);
    void UpdateLabel(QString Text);

private slots:
    void QuerrySimState(void);
private:
    unsigned char SeqCntr = 1;
    QUdpSocket *socket;
    QHostAddress *Src;
    QHostAddress *Dst;
    unsigned int DstPort;
    QTimer *Timer;

    void IncSeqCntr(void);

};

#endif // COMMODULE_H
