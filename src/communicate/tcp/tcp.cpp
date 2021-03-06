/*
Tencent is pleased to support the open source community by making 
PhxPaxos available.
Copyright (C) 2016 THL A29 Limited, a Tencent company. 
All rights reserved.

Licensed under the BSD 3-Clause License (the "License"); you may 
not use this file except in compliance with the License. You may 
obtain a copy of the License at

https://opensource.org/licenses/BSD-3-Clause

Unless required by applicable law or agreed to in writing, software 
distributed under the License is distributed on an "AS IS" basis, 
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
implied. See the License for the specific language governing 
permissions and limitations under the License.

See the AUTHORS file for names of contributors. 
*/

#include "tcp.h"
#include "phxpaxos/network.h"

namespace phxpaxos
{

TcpIOThread :: TcpIOThread(NetWork * poNetWork)
    : m_oTcpAcceptor(&m_oEventLoop, poNetWork),
    m_oTcpClient(&m_oEventLoop, poNetWork)
{
    m_bIsStarted = false;
    m_oEventLoop.SetTcpAcceptor(&m_oTcpAcceptor);
    m_oEventLoop.SetTcpClient(&m_oTcpClient);

    assert(signal(SIGPIPE, SIG_IGN) != SIG_ERR);
    assert(signal(SIGALRM, SIG_IGN) != SIG_ERR);
    assert(signal(SIGCHLD, SIG_IGN) != SIG_ERR);
}

TcpIOThread :: ~TcpIOThread()
{
}

void TcpIOThread :: Stop()
{
    m_oTcpAcceptor.Stop();

    if (m_bIsStarted)
    {
        m_oEventLoop.Stop();
        join();
    }

    PLHead("TcpIOThread [END]");
}

int TcpIOThread :: Init(const std::string & sListenIp, const int iListenPort)
{
    m_oTcpAcceptor.Listen(sListenIp, iListenPort);
    return m_oEventLoop.Init(20480);
}

void TcpIOThread :: run()
{
    m_bIsStarted = true;
    m_oTcpAcceptor.start();
    m_oEventLoop.StartLoop();
}

int TcpIOThread :: AddMessage(const std::string & sIP, const int iPort, const std::string & sMessage)
{
    return m_oTcpClient.AddMessage(sIP, iPort, sMessage);
}

}


