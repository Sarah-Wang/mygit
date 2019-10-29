/*************************************************************************
	> File Name: sig_test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年09月11日 星期二 15时55分27秒
 ************************************************************************/
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

#define handle_error_en(en, msg)\
        do { errno= en; perror(msg);exit(EXIT_FAILURE);}while(0)

void kill_handler(int signum)
{
	pthread_t	tid = pthread_self();
	printf("Thread %lu in signal kill handler\n",tid);
	return;
}

void sigusr1_handler(int signum)
{
	pthread_t	tid = pthread_self();
	printf("Thread %lu in signal sigusr1 handler\n",tid);
	return;
}

void sigusr2_handler(int signum)
{
	pthread_t	tid = pthread_self();
	printf("Thread %lu in signal sigusr2 handler\n",tid);
	return;
}

static void *thread_one()
{
	sigset_t set, set_cat;
	int s, sig;
	pthread_t	tid = pthread_self();

	sigemptyset(&set_cat);
	sigaddset(&set_cat, SIGQUIT);
	sigaddset(&set_cat, SIGUSR1);
	//s = pthread_sigmask(SIG_BLOCK,&set,NULL);
	for (;;){
		s = sigwait(&set_cat,&sig);
		if (s != 0)
			handle_error_en(s,"sigwait");
		sigdelset(&set_cat, SIGUSR1);
		pthread_sigmask(SIG_SETMASK, &set_cat, NULL);
		printf("thread_one tid:%lu\n",tid);
		printf("Signal handling thread_one got signal %d\n", sig);
	}
}

static void *thread_two()
{
	sigset_t set;
	int s, sig;
	pthread_t	tid = pthread_self();

	for (;;){
		sigemptyset(&set);
		sigaddset(&set, SIGQUIT);
		sigaddset(&set, SIGUSR2);
		//s = pthread_sigmask(SIG_BLOCK,&set,NULL);
		s = sigwait(&set,&sig);
		if (s != 0)
			handle_error_en(s,"sigwait");
		sigdelset(&set, SIGUSR2);
		pthread_sigmask(SIG_SETMASK, &set, NULL);
		printf("thread_one tid:%lu\n",tid);
		printf("Signal handling thread_two got signal %d\n", sig);
	}
}

int main()
{
	int fd = 0, ret = 0, s = 0;
	int oflags;
	sigset_t set;
	pthread_t id1,id2;

	printf("PID = %d\n",getpid());
	/* 打开设备 
	fd = open("",O_RDWR, S_IRUSR | S_IWUSR);
	if(fd < 0)
	{
		printf("DEVICE OPEN FAILURE ! \n");
	}*/

	sigemptyset(&set);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	s = pthread_sigmask(SIG_BLOCK,&set,NULL);

	/* 建立线程1 */
	ret = pthread_create(&id2,NULL,&thread_two,0);
	if(ret)
	{
		printf("Create pthread error!\n");
		return 1;
	}

	/* 建立线程2 */
	ret = pthread_create(&id1, NULL, &thread_one,0);
	if(ret)
	{
		printf("Create pthread error!\n");
		return 1;
	}

	/* 启动信号驱动 */
	signal(SIGKILL, kill_handler);
	signal(SIGUSR1,sigusr1_handler);
	signal(SIGUSR2,sigusr2_handler);

	//fcntl(fd, F_SETOWN, getpid());
	//oflags = fcntl(fd, F_GETFL);
	//fcntl(fd, F_SETFL, oflags | FASYNC);

	for(; ;);

	return 0;
}


