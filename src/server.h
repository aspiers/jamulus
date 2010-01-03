/******************************************************************************\
 * Copyright (c) 2004-2010
 *
 * Author(s):
 *  Volker Fischer
 *
 ******************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
\******************************************************************************/

#if !defined ( SERVER_HOIHGE7LOKIH83JH8_3_43445KJIUHF1912__INCLUDED_ )
#define SERVER_HOIHGE7LOKIH83JH8_3_43445KJIUHF1912__INCLUDED_

#include <qobject.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qhostaddress.h>
#include "celt.h"
#include "global.h"
#include "socket.h"
#include "channel.h"
#include "util.h"
#include "serverlogging.h"


/* Definitions ****************************************************************/
// no valid channel number
#define INVALID_CHANNEL_ID                  ( MAX_NUM_CHANNELS + 1 )

// minimum timer precision
#define MIN_TIMER_RESOLUTION_MS             1 // ms


/* Classes ********************************************************************/
class CHighPrecisionTimer : public QObject
{
    Q_OBJECT

public:
    CHighPrecisionTimer();

    void start();
    void stop();
    bool isActive() const { return Timer.isActive(); }

protected:
    QTimer       Timer;
    CVector<int> veciTimeOutIntervals;
    int          iCurPosInVector;
    int          iIntervalCounter;

public slots:
    void OnTimer();

signals:
    void timeout();
};


class CServer : public QObject
{
    Q_OBJECT

public:
    CServer ( const QString& strLoggingFileName,
              const quint16  iPortNumber,
              const QString& strHTMLStatusFileName,
              const QString& strHistoryFileName,
              const QString& strServerNameForHTMLStatusFile );
    virtual ~CServer() {}

    void Start();
    void Stop();
    bool IsRunning() { return HighPrecisionTimer.isActive(); }

    bool GetTimingStdDev ( double& dCurTiStdDev );

    bool PutData ( const CVector<uint8_t>& vecbyRecBuf,
                   const int iNumBytesRead, const CHostAddress& HostAdr );

    void GetConCliParam ( CVector<CHostAddress>& vecHostAddresses,
                          CVector<QString>& vecsName,
                          CVector<int>& veciJitBufNumFrames,
                          CVector<int>& veciNetwFrameSizeFact );

protected:
    // access functions for actual channels
    bool IsConnected ( const int iChanNum )
        { return vecChannels[iChanNum].IsConnected(); }

    void StartStatusHTMLFileWriting ( const QString& strNewFileName,
                                      const QString& strNewServerNameWithPort );

    int CheckAddr ( const CHostAddress& Addr );
    int GetFreeChan();
    CVector<CChannelShortInfo> CreateChannelList();
    void CreateAndSendChanListForAllConChannels();
    void CreateAndSendChanListForThisChan ( const int iCurChanID );
    void CreateAndSendChatTextForAllConChannels ( const int iCurChanID,
                                                  const QString& strChatText );
    void WriteHTMLChannelList();

    CVector<int16_t> ProcessData ( CVector<CVector<int16_t> >& vecvecsData,
                                   CVector<double>& vecdGains );

    virtual void     customEvent ( QEvent* Event );

    // do not use the vector class since CChannel does not have appropriate
    // copy constructor/operator
    CChannel            vecChannels[MAX_NUM_CHANNELS];
    QMutex              Mutex;

    // audio encoder/decoder
    CELTMode*           CeltMode[MAX_NUM_CHANNELS];
    CELTEncoder*        CeltEncoder[MAX_NUM_CHANNELS];
    CELTDecoder*        CeltDecoder[MAX_NUM_CHANNELS];

    CVector<QString>    vstrChatColors;

    // HTML file server status
    bool                bWriteStatusHTMLFile;
    QString             strServerHTMLFileListName;
    QString             strServerNameWithPort;

    CHighPrecisionTimer HighPrecisionTimer;
    CVector<short>      vecsSendData;

    // actual working objects
    CSocket             Socket;

    CCycleTimeVariance  CycleTimeVariance;

    // logging
    CServerLogging      Logging;

public slots:
    void OnTimer();
    void OnSendProtMessage ( int iChID, CVector<uint8_t> vecMessage );

    // CODE TAG: MAX_NUM_CHANNELS_TAG
    // make sure we have MAX_NUM_CHANNELS connections!!!
    // send message
    void OnSendProtMessCh0 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 0, mess ); }
    void OnSendProtMessCh1 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 1, mess ); }
    void OnSendProtMessCh2 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 2, mess ); }
    void OnSendProtMessCh3 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 3, mess ); }
    void OnSendProtMessCh4 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 4, mess ); }
    void OnSendProtMessCh5 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 5, mess ); }
    void OnSendProtMessCh6 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 6, mess ); }
    void OnSendProtMessCh7 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 7, mess ); }
    void OnSendProtMessCh8 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 8, mess ); }
    void OnSendProtMessCh9 ( CVector<uint8_t> mess ) { OnSendProtMessage ( 9, mess ); }

    void OnNewConnectionCh0() { vecChannels[0].CreateReqJitBufMes(); }
    void OnNewConnectionCh1() { vecChannels[1].CreateReqJitBufMes(); }
    void OnNewConnectionCh2() { vecChannels[2].CreateReqJitBufMes(); }
    void OnNewConnectionCh3() { vecChannels[3].CreateReqJitBufMes(); }
    void OnNewConnectionCh4() { vecChannels[4].CreateReqJitBufMes(); }
    void OnNewConnectionCh5() { vecChannels[5].CreateReqJitBufMes(); }
    void OnNewConnectionCh6() { vecChannels[6].CreateReqJitBufMes(); }
    void OnNewConnectionCh7() { vecChannels[7].CreateReqJitBufMes(); }
    void OnNewConnectionCh8() { vecChannels[8].CreateReqJitBufMes(); }
    void OnNewConnectionCh9() { vecChannels[9].CreateReqJitBufMes(); }

    void OnReqConnClientsListCh0() { CreateAndSendChanListForThisChan ( 0 ); }
    void OnReqConnClientsListCh1() { CreateAndSendChanListForThisChan ( 1 ); }
    void OnReqConnClientsListCh2() { CreateAndSendChanListForThisChan ( 2 ); }
    void OnReqConnClientsListCh3() { CreateAndSendChanListForThisChan ( 3 ); }
    void OnReqConnClientsListCh4() { CreateAndSendChanListForThisChan ( 4 ); }
    void OnReqConnClientsListCh5() { CreateAndSendChanListForThisChan ( 5 ); }
    void OnReqConnClientsListCh6() { CreateAndSendChanListForThisChan ( 6 ); }
    void OnReqConnClientsListCh7() { CreateAndSendChanListForThisChan ( 7 ); }
    void OnReqConnClientsListCh8() { CreateAndSendChanListForThisChan ( 8 ); }
    void OnReqConnClientsListCh9() { CreateAndSendChanListForThisChan ( 9 ); }

    void OnNameHasChangedCh0() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh1() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh2() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh3() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh4() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh5() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh6() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh7() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh8() { CreateAndSendChanListForAllConChannels(); }
    void OnNameHasChangedCh9() { CreateAndSendChanListForAllConChannels(); }

    void OnChatTextReceivedCh0 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 0, strChatText ); }
    void OnChatTextReceivedCh1 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 1, strChatText ); }
    void OnChatTextReceivedCh2 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 2, strChatText ); }
    void OnChatTextReceivedCh3 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 3, strChatText ); }
    void OnChatTextReceivedCh4 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 4, strChatText ); }
    void OnChatTextReceivedCh5 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 5, strChatText ); }
    void OnChatTextReceivedCh6 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 6, strChatText ); }
    void OnChatTextReceivedCh7 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 7, strChatText ); }
    void OnChatTextReceivedCh8 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 8, strChatText ); }
    void OnChatTextReceivedCh9 ( QString strChatText ) { CreateAndSendChatTextForAllConChannels ( 9, strChatText ); }

    void OnPingReceivedCh0 ( int iMs ) { vecChannels[0].CreatePingMes ( iMs ); }
    void OnPingReceivedCh1 ( int iMs ) { vecChannels[1].CreatePingMes ( iMs ); }
    void OnPingReceivedCh2 ( int iMs ) { vecChannels[2].CreatePingMes ( iMs ); }
    void OnPingReceivedCh3 ( int iMs ) { vecChannels[3].CreatePingMes ( iMs ); }
    void OnPingReceivedCh4 ( int iMs ) { vecChannels[4].CreatePingMes ( iMs ); }
    void OnPingReceivedCh5 ( int iMs ) { vecChannels[5].CreatePingMes ( iMs ); }
    void OnPingReceivedCh6 ( int iMs ) { vecChannels[6].CreatePingMes ( iMs ); }
    void OnPingReceivedCh7 ( int iMs ) { vecChannels[7].CreatePingMes ( iMs ); }
    void OnPingReceivedCh8 ( int iMs ) { vecChannels[8].CreatePingMes ( iMs ); }
    void OnPingReceivedCh9 ( int iMs ) { vecChannels[9].CreatePingMes ( iMs ); }
};

#endif /* !defined ( SERVER_HOIHGE7LOKIH83JH8_3_43445KJIUHF1912__INCLUDED_ ) */
