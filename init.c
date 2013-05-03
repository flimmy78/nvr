#include "init.h"
void
clearInactiveStreams()
{
	syslog(LOG_DEBUG,  "clearing inactive download streams...");
	int i = 0; 
	for (; i < MAX_STREAMS; i++) { 
		if (pDInfo[i] && (time(NULL) - pDInfo[i]->lastReadTime > 30))
		{
			syslog(LOG_INFO, "closing a inactive download stream, IPC:%s, handle:0x%x.", pDInfo[i]->CameraID, pDInfo[i]->dHandle); 
    		if(CloseRecordSeg(pDInfo[i]->dHandle, NULL) < 0){
    			syslog(LOG_ERR,  "close the download stream fail, IPC:%s, handle:0x%x, errno:%u.",pDInfo[i]->CameraID, pDInfo[i]->dHandle, ErrorFlag);
    		} else {
                syslog(LOG_INFO,  "close the download stream success, IPC:%s, handle:0x%x.",pDInfo[i]->CameraID, pDInfo[i]->dHandle);
            }
		    releaseDownloadInfo(&pDInfo[i]);
		}
	}
    syslog(LOG_DEBUG,  "clear inactive download streams sucess.");
}

void *
WriteTnodeThread(void *arg)
{
	syslog(LOG_INFO,  "start write tnode thread.");
	int i = 0;
	for (;; i = (i + 1) % MAX_STREAMS) {
		if (i == 0)
			sleep(20);
		pthread_rwlock_rdlock(&SInfo_PRW);
		if (streamInfos[i] == NULL || streamInfos[i]->count < WriteLen) {
			pthread_rwlock_unlock(&SInfo_PRW);
			continue;
		}

		if (write_back_tnodes(streamInfos[i]->sbinfo, streamInfos[i]->v, streamInfos[i]->vi, streamInfos[i]) < 0) {
			syslog(LOG_ERR,  "write back tnodes fail, IPC:%s, errno:%u.", streamInfos[i]->cameraID, ErrorFlag);
		}
		streamInfos[i]->count = 0;
		if (put_vnode(streamInfos[i]->sbinfo, streamInfos[i]->v, NULL, streamInfos[i]->ID) < 0) {
			syslog(LOG_ERR, "put vnode fail, IPC:%s, errno:%u.", streamInfos[i]->cameraID, ErrorFlag);
		}
		pthread_rwlock_unlock(&SInfo_PRW);

	}
}
 
void *
VolOpThread(void *arg)
{
	syslog(LOG_INFO,  "start volume operation thread.");
	CreatRecVolMsg creatMsg;
	DelRecVolMsg delMsg;
	DelVideoVolMsg delVideoMsg;
	ResMsg resMsg;
	//key_t sendKey,recvKey;
	int sendQID, recvQID;
	ssize_t ret;

	//recvKey = ftok("/opt/HTRaid/conf/camera.conf",1);
	//sendKey = ftok("/opt/HTRaid/conf/msgfile",1);

	sendQID = msgget((key_t) 706504, 0666 | IPC_CREAT);
	recvQID = msgget((key_t) 706507, 0666 | IPC_CREAT);
	if (sendQID == -1 || recvQID == -1) {
		syslog(LOG_ERR,  "msgget failed with error: %d. *exit*", errno);
		exit(1);
	}
    
	while (1) {
		ret = msgrcv(recvQID, (void *)&creatMsg, sizeof(CreatRecVolMsg) - sizeof(long int), CREAT_REC_VOL_ARGS, IPC_NOWAIT);
		if (ret != -1) {
			resMsg.msgType = CREAT_REC_VOL_RES;
			resMsg.result = CreateRecordVol(creatMsg.volumeid,
							creatMsg.name,
							creatMsg.alias, creatMsg.savedDays, creatMsg.delPolicy, creatMsg.encodeType, creatMsg.blocks);
			if(resMsg.result < 0)
			{
				syslog(LOG_ERR,  "create record volume %s in %s faile.(errno:%u).", creatMsg.name, creatMsg.volumeid, ErrorFlag);
			}
			if (msgsnd(sendQID, (void *)&resMsg, sizeof(int), 0) == -1) {
				syslog(LOG_ERR, "send msg failed when create record volume %s in %s.", creatMsg.name, creatMsg.volumeid);
				break;
			}
			continue;
		}

		if (msgrcv(recvQID, (void *)&delMsg, sizeof(DelRecVolMsg) - sizeof(long int), DEL_REC_VOL_ARGS, IPC_NOWAIT) != -1) {
			resMsg.msgType = DEL_REC_VOL_RES;
			resMsg.result = DeleteRecordVol(delMsg.cameraID, delMsg.mode);
			if (msgsnd(sendQID, (void *)&resMsg, sizeof(int), 0) == -1) {
				fprintf(stderr, "send msg failed\n");
				syslog(LOG_ERR, "send msg failed when delete record volume %s.", delMsg.cameraID);
				break;
			}
			continue;
		}

		if (msgrcv(recvQID, (void *)&delVideoMsg, sizeof(DelVideoVolMsg) - sizeof(long int), DEL_VIDEO_VOL_ARGS, IPC_NOWAIT) != -1) {
			resMsg.msgType = DEL_VIDEO_VOL_RES;
			resMsg.result = DeleteVideoVol(delVideoMsg.volName);
			if (msgsnd(sendQID, (void *)&resMsg, sizeof(int), 0) == -1) {
				fprintf(stderr, "send msg failed\n");
				syslog(LOG_ERR, "send msg failed when delete videovol %s.", delVideoMsg.volName);
				break;
			}
			continue;
		}

		sleep(1);
	}
	syslog(LOG_ERR, "VolOPThread exit!");
	pthread_exit(0);
}

void *
UpdateThread(void *arg)
{
    syslog(LOG_INFO,  "start update thread.");
    while(1) {
        sleep(120);
        clearInactiveStreams(); 
    }
}

int isRunning()
{
    pid_t pid = -1, file_pid = -1;
    FILE *fp = NULL;
    pid = getpid();
    char cmd[256];
    bzero(cmd, 256);
    if(access(PID_FILE, 0)<0)
    { 
        sprintf(cmd, "%u\0", pid);
        syslog(LOG_INFO, "start nvrd (pid:%u) success! ",pid);
        fp = fopen(PID_FILE, "w+");
        fputs(cmd, fp);
        fclose(fp);
        return 0;
    }
    if ((fp = fopen(PID_FILE, "r")) == NULL) 
        
    {
        syslog(LOG_ERR, "open pidfile error!");
        return -1;
	}

    if(fgets(cmd, 255, fp) == NULL)
    {
        fclose(fp);
        return -1;
    } 
    fclose(fp);
    sscanf(cmd, "%u", &file_pid); 
    sprintf(cmd, "ps -ef|grep nvrd|awk '{print $2}'|grep %u\0", file_pid);
    if(NULL == (fp= popen(cmd, "r")))
    {
        syslog(LOG_ERR, "call popen failed!");
        return -1;
    }
    else
    { 
        while(fgets(cmd, 255, fp) != NULL)
        {
            
            sscanf(cmd, "%u", &file_pid);
            syslog(LOG_ERR, "start failed! nvrd (pid:%u) is exist!",file_pid );
            fclose(fp);
            return 1;
        }
        fclose(fp); 
        sprintf(cmd, "%u\0", pid);
        fp = fopen(PID_FILE, "w+");
        fputs(cmd, fp);
        fclose(fp);
        syslog(LOG_INFO, "start nvrd (pid:%u) success! ",pid);
        return 0;
    }
}

int tcp_create()
{
    int listenfd;
    struct sockaddr_in servaddr;
    if((listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
		close(listenfd);
		return -1;
	}
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(SERV_PORT);
	if(bind(listenfd,(struct sockaddr *) &servaddr,sizeof(servaddr))<0){
		printf("error in binding\n");
		close(listenfd);
		return -1;
	}    
	if(listen(listenfd,10)<0){
		printf("error in listening\n");
		close(listenfd);
		return -1;
	}
    return listenfd;
}

int tcp_send(int fd, char *buf, int len, int flags)
{
    int nsend=0,send_count=0;
    int length =len;
 
    if(buf==NULL)
        return -1;
  
    while(length > 0)
    {
        nsend =send(fd, buf+send_count, length, flags);
        if(nsend == 0)
            return 0;
         
        if(nsend < 0)
        {
             perror("Failed send(),error code:");
             return -1;
        }
        else
        {
         length -= nsend;
         send_count += nsend;
        }
    }
    return send_count;
}

int tcp_recv(int fd, char *buf, int len, int flags)
{
     
    int nrec=0,recv_count=0;
    int length =len;
 
    if(buf==NULL)
        return -1;
  
    while(length > 0)
    {
        nrec =recv(fd, buf+recv_count, length, flags);
        if(nrec == 0)
            return 0;
         
        if(nrec < 0)
        {
             perror("Failed recv(),error code:");
             return -1;
        }
        else
        {
         length -= nrec;
         recv_count += nrec;
        }
    }
    return recv_count;
}
int enc_rec_result(char *presult, int* pret, unsigned int* perron)
{
    char headchar = 'h';
    char tailchar  = 't';
    memcpy(presult, &headchar, sizeof(char));
    presult += sizeof(char);
    memcpy(presult, pret, sizeof(int));
    presult += sizeof(int);
    memcpy(presult, perron, sizeof(unsigned int));
    presult += sizeof(unsigned int);
    memcpy(presult, &tailchar, sizeof(char)); 
    return 0;
} 
int dec_rec_head(char *phead, unsigned int* precordhandle, unsigned int* pdatalen, unsigned int* pbegintime)
{
    char headchar = '\0';
    char tailchar  = '\0';
    memcpy(&headchar, phead, sizeof(char));
    if (headchar != 'h') 
        return -1;
    phead += sizeof(char); 
    memcpy(precordhandle, phead, sizeof(unsigned int));
    phead += sizeof(unsigned int);     
    memcpy(pdatalen, phead, sizeof(unsigned int));
    phead += sizeof(unsigned int); 
    memcpy(pbegintime, phead, sizeof(unsigned int));
    phead += sizeof(unsigned int);
    memcpy(&tailchar, phead, sizeof(char)); 
    if (tailchar != 't') 
        return -1;
    return 0;

}

int record (int sockfd ) 
{ 
    int rc, ret;
    unsigned int erron = 0;
    char phead[REC_HEAD_SIZE];
    char presult[REC_RESULT_SIZE];
    char data[MAX_RECORD_DATA_SIZE];
    unsigned int datalen=0, handle=0, begintime=0;
    WRITEargs writeargs;
    bzero(phead, REC_HEAD_SIZE);
    bzero(presult, REC_RESULT_SIZE); 
    rc = tcp_recv(sockfd, phead, REC_HEAD_SIZE, 0);
    if(rc <= 0)
    { 
         return -1;
    }
    if (dec_rec_head(phead, &handle, &datalen, &begintime) < 0) 
    {
        syslog(LOG_ERR, "dec_rec_head error!");
        return -1;
    }
    rc = tcp_recv(sockfd, data, datalen, 0);
    if(rc <= 0)
    {
        return -1;
    }
    writeargs.recordHandle = handle;
    writeargs.beginTime = begintime;
    writeargs.data.data_val = data;
    writeargs.data.data_len = datalen;
    ret =  nvrproc_write(writeargs);
    if(ret < 0)
        erron = ErrorFlag;
    enc_rec_result(presult, &ret, &erron);
    rc = tcp_send(sockfd, presult, REC_RESULT_SIZE, 0);
    if(rc <= 0)
    { 
         return -1;
    } 
    return ret; 
}  

void *recordthread(void* arg)
{
      int socket=(int)arg;
      int ready;
      unsigned int last_request_time;
      struct pollfd pfd;
      pfd.events = POLLIN;
	  pfd.fd = socket;
      while(1) {
        ready = poll(&pfd, 1, 3000); 
        if (ready < 0) {
            break;
        }
        if (ready == 0) {

            if ((time(NULL) - last_request_time) > 30) { //if no data come for 30s, close connect
                syslog(LOG_ERR, "no data come exceed 30 seconds, sockfd:%d", socket);
                break;
            }
            continue;  
        }
        last_request_time = time(NULL);
        if(record(socket) < 0) {
            break;
        }
      }
      close(socket);
      syslog(LOG_INFO, "close record socket %d.", socket);
}

void *SerialRecordThread(void* arg)
{
    syslog(LOG_INFO,  "start record thread.");

    int i, maxi, listenfd, connfd, sockfd;
    int nready;
    ssize_t n; 
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cliaddr;
    listenfd = (int)arg;
    client[0].fd = listenfd;
    client[0].events = POLLIN;
    for (i =1; i < OPEN_MAX; i++)
        client[i].fd = -1;
    maxi = 0;
    while(1) { 
        nready = poll(client, maxi + 1, -1); 
        if (nready == 0) {
            continue;
        }
        if (client[0].revents == POLLIN) { 
            clilen = sizeof(cliaddr);
            connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
            if (connfd < 0) {
                perror("server accept:");
                continue;
            }
            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    syslog(LOG_DEBUG,  "create new record socket.");
                    break;
                }
            }
            if (i == OPEN_MAX)
                syslog(LOG_INFO,  "too many record socket.defused.");
            client[i].events = POLLIN;
            if (i > maxi)
                maxi = i;  /*max index in client[] array*/
            if (--nready <=0)
                continue;       /*no more readable descriptors*/
        } 
        for (i = 1; i <= maxi; i++) { 
            if ( client[i].revents == POLLIN) {
                if (record(client[i].fd) < 0) {
                    close(client[i].fd);
                    client[i].fd = -1;
                }
            }
            else
                continue;
            if (--nready <= 0)
                break;
        }
    }
}

void *ParallelRecordThread(void* arg)
{
    syslog(LOG_INFO, "start parallel record thread.");
    int server_socket,client_socket,result,len;
    struct sockaddr_in clientaddr;
    pthread_t pt;  
    server_socket = (int)arg; 
    while(1)
    {
        if((client_socket = accept(server_socket, (struct sockaddr*)&clientaddr, (socklen_t*)&len))==0)
        {
            perror("server accept:");
            continue; 
        }
 
        if((result=pthread_create(&pt, NULL, recordthread, (void *)client_socket))!=0)
        {
            perror("pthread create:");
            continue; 
        }
        pthread_detach(pt);
        syslog(LOG_DEBUG, "create new record thread, sockfd:%d.", client_socket);
    }    
}

