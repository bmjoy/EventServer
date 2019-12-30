#include "MyServer.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"

#include <set>
#include<iostream>
#include "Timer/TimerLoop.h"

using namespace std;

void Print()
{
	cout << "test timer" << endl;
}

void RecvCb(const socket_t nFd, const int nMsgId, const char* pMsg, const uint32_t nLen)
{
	cout << pMsg << endl;
}

void EventCb(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	cout << nFd << endl;
	cout << nEvent << endl;
}

int main()
{
	
	INIT_SFLOG(true);
	/*signal(SIGPIPE, SIG_IGN);*/
	g_pSessionPool = std::make_unique<SessionPool>();
	NET_RECEIVE_FUNCTOR Refunctor = std::bind(RecvCb, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	NET_EVENT_FUNCTOR Evfunctor = std::bind(EventCb, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	SeNet server(Refunctor, Evfunctor);
	
	server.InitServer(88888);
	server.StartLoop(LOOP_RUN_TYPE::LOOP_RUN_BLOCK);
	server.StopLoop();

	TimerLoop timerLoop;

	timerLoop.RunAfter(10 * 1000, Print);
	timerLoop.RunAfter(15 * 1000, Print);
	timerLoop.RunAfter(20 * 1000, Print);

	TimeId tid = timerLoop.NearestTimer();
	cout << tid.GetSeq() << endl;

	while (1)
	{
		TimeId tid = timerLoop.NearestTimer();
		cout << tid.GetSeq() << endl;
		timerLoop.TimeLoop();
		SFSLEEP(100);
	}
	return 0;
}