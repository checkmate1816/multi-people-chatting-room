#include<iostream>
#include<winsock2.h>
#include<stdio.h>
#include<process.h>
#include<time.h>
#include<Windows.h>
#include<string.h>
#include<stdlib.h>
#include <mstcpip.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define SEND_OVER 1                          //�Ѿ�ת����Ϣ
#define SEND_YET  0                          //��ûת����Ϣ

int g_Status = SEND_YET;
SOCKET s;//���ӿ�
//SOCKET newsock;//�½ӿ�
sockaddr_in remoteaddr;//�ͻ��˵�ַ
int raddrlen = sizeof(struct sockaddr);
HANDLE manage = NULL;
HANDLE AC = NULL;
typedef struct _Client
{
	int id;
	char username[20] = {'\0'};
	char password[20] = { '\0' };
	int group;//Ⱥ���
}Client;
typedef struct Sclient
{
	SOCKET sclient= INVALID_SOCKET;
	char buf[255] = {'\0'};
	char username[20] = {'\0'};
	char recvname[20] = { '\0' };
	char IP[20] = { '\0' };
	UINT_PTR flag;
	int group=0;
	int recvgroup=0;
	int sendorder = 0;//1�������ˣ�2����Ⱥ��
}SClient;
SClient *gclient=new SClient[10] ;//���ӵ��������Ŀͻ�����Ϣ
unsigned __stdcall ThreadSend(Sclient *a)//ת����Ϣ����
{
	char temp[255] = { '\0' };
	if (a->sendorder==1)//����Ϣת���������û�
	{
		for (int j = 0; j < 10; j++)
		{
			if (strcmp(gclient[j].username, a->recvname) == 0 && gclient[j].sclient != INVALID_SOCKET)
			{
				strncpy(temp, a->username, 20);//������Ϣ��Դ
				strcpy(temp + 20, a->buf);//������Ϣ����
				send(gclient[j].sclient, temp, sizeof(temp), 0);//������Ϣ
				return 1;
			}
		}
		cout << "no user" << endl;
		return -1;
	}
	else if(a->sendorder==2)//����Ϣת����Ⱥ��
	{
		for (int i = 0; i < 10; i++)
		{
			if (gclient[i].group == a->recvgroup&&gclient[i].sclient != INVALID_SOCKET)
			{
				strncpy(temp, a->username, 20);//������Ϣ��Դ
				strcpy(temp + 20, a->buf);//������Ϣ����
				send(gclient[i].sclient, temp, sizeof(temp), 0);//������Ϣ
			}
		}
		return 2;
	}
	cout << "send error" << endl;
	return 0;
}
 unsigned __stdcall ThreadRecvmessage(SOCKET newsock)//���ܿͻ�����Ϣ
{
	int flag = 0;//��ǰ�̵߳��õĿͻ������
	int i = 0;
	while (i <= 9)
	{
		if (gclient[i].sclient == newsock)
		{
			flag = i;
			break;
		}
		else
			i++;
	}
	char temp[255] = {'\0'};
	while (1)//ѭ�����ܿͻ��˷��ĸ���ָ��
	{
		memset(temp, '\0', sizeof(temp));
		int ret = recv(newsock, temp, sizeof(temp), 0);//��������
		if (ret == INVALID_SOCKET)
		{
			cout << "recv error" << endl;
			continue;
		}
		if (temp[0]=='4')//����Ⱥ�� creategroup
		{
			int tempgroup;
			char mes[] = { "Ⱥ���" };
			char temp1[3] = { '\0' };
			srand((unsigned)time(NULL));
			tempgroup= (rand() % 100) + 1;//��Ŷ�Ϊ1��100
			gclient[flag].group = tempgroup;//��¼��������Ⱥ���
			memset(temp, '\0', sizeof(temp));//��ջ�������
			sprintf(temp1, "%d", tempgroup);
			sprintf(temp, "%s", mes);
			strcpy(temp + 20, temp1);
			send(newsock, temp, sizeof(temp), 0);//��Ⱥ��ŷ��͸��ͻ���
			FILE *fp;
			fopen_s(&fp, "account.txt", "r+");
			if (fp != NULL)
			{
				Client *f = new Client();
				while (fread(f, sizeof(Client), 1, fp) == 1)
				{
					if (strcmp(gclient[flag].username, f->username) != 0)
					{
						continue;
					}
					else
					{
						fseek(fp, -(sizeof(Client)), SEEK_CUR);
						f->group = tempgroup;//�洢���
						fwrite(f, sizeof(Client), 1, fp);
						break;
					}
				}
				fclose(fp);//���´�������Ϣд�뵽�ļ�����
			}
		}
		else if (temp[0]=='2')//����Ϣת����ĳ��username��tousername"
		{
			gclient[flag].sendorder = 1;
			strncpy(gclient[flag].recvname, temp+1,20);//��������
			strcpy(gclient[flag].buf, temp + 21);//������Ϣ
			_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))  ThreadSend, &gclient[flag], NULL, 0);//��ʼת���߳�
		}
		else if (temp[0] =='3')//����Ϣת����Ⱥ�顱togroup"
		{
			gclient[flag].sendorder = 2;
			char a[3] = { '\0' };
			strncpy(a, temp + 1, 3);
			gclient[flag].recvgroup = atoi(a);
			strcpy(gclient[flag].buf,temp+4);//���ܴ���������Ϣ,���뵽�û�����Ϣ�洢��
			_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))  ThreadSend, &gclient[flag], NULL, 0);
		}
		else if (temp[0] == '1')//����Ⱥ��addgroup
		{
			char b[3] = { '\0' };//����Ⱥ���
			strncpy(b, temp + 1, 3);
			gclient[flag].group = atoi(b);//�������׽��ֵ�Ⱥ�ĺ��޸�
			FILE *fp;//���ļ�������Ϣ
			fopen_s(&fp, "account.txt", "r+");
			Client *f = new Client();
			if (fp != NULL)
			{
				fseek(fp, 0, SEEK_SET);
				/*int id=0;
				int group=0;
				char tempname[20] = { '\0' };
				char temppassword[20] = { '\0' };*/
				while (fread(f, sizeof(Client), 1, fp) == 1 && feof(fp) == 0 && ferror(fp) == 0)
				{
					if (strcmp(gclient[flag].username, f->username) != 0)
					{
						continue;
					}
					else
					{
						fseek(fp, -(sizeof(Client)), SEEK_CUR);
						f->group = gclient[flag].group;//�洢���
						fwrite(f,sizeof(Client),1,fp);
						break;
					}
				}

				fclose(fp);//���´�������Ϣд�뵽�ļ�����
			}
		}	
		else if (temp[0] == '5')
		{
			gclient[flag].sclient = INVALID_SOCKET;
			return 0;
		}
	}
	return -1;
}
void createaccount(SOCKET n)
{
	int flag = 0;//��ǰ�̵߳��õĿͻ������
	int i = 0;
	while (i <= 9)
	{
		if (gclient[i].sclient == n)
		{
			flag = i;
			break;
		}
		else
			i++;
	}
	FILE *fp;
	fopen_s(&fp, "account.txt", "a");
	Client temp;
	srand((unsigned)time(NULL));
	temp.id = (rand() % (2000000000 - 1000000000)) + 1000000000;
	char id[11] = {'\0'};
	itoa(temp.id, id, 10);
	char tem1[20] = {'\0'};//��������
	char a[] = { "please insert username and password" };
	send(n, a, sizeof(a), 0);//������ʾ
	recv(n, tem1, sizeof(tem1), 0);//������������
	strcpy(temp.username, tem1);//�������洢
	strcpy(gclient[flag].username, tem1);//�������洢���׽�����
	memset(tem1, '\0', sizeof(tem1));
	recv(n, tem1, sizeof(tem1), 0);//������������
	strcpy(temp.password, tem1);//������洢
	send(n, id, sizeof(id), 0);//���������ɵ�ID
	fwrite(&temp, sizeof(Client), 1, fp);
	fclose(fp);
}//�����˻�
int loginaccount(SOCKET n)
{
	FILE *fp;
	fopen_s(&fp, "account.txt", "r");
	Client *f=new Client();
	char tem2[30] = { '\0' };//�洢����
	char id[11] = {'\0'};
	char password[20] = {'\0'};
	char a[] = {"success"};
	char b[] = { "log in failed" };
	recv(n, tem2, sizeof(tem2), 0);//���ܷ�������id
	strncpy(id, tem2,10);//��ǰʮλ�洢Ϊid
	strcpy(password, tem2+10);//�洢����
	
	if (fp == NULL)
	{
		cout << "open file error" << endl;
		return -1;
	}
	else
	{
		
		while (fread(f, sizeof(Client), 1, fp) == 1&&feof(fp)==0&&ferror(fp)==0)
		{
			memset(tem2, '\0', sizeof(tem2));
			itoa(f->id, tem2, 10);
			if (strcmp(id,tem2)!=0)
			{
				continue;
			}
			else
			{
				if (strcmp(password, f->password) == 0)
				{
					send(n, a, strlen(a), 0);//��½�ɹ�
					int i = 0;
					while (i<=9)//��¼�û��������
					{
						if (gclient[i].flag == n)
						{
							strcpy(gclient[i].username, f->username);
							gclient[i].group = f->group;
							break;
						}
						else
							i++;
					}
					fclose(fp);
					return 1;
				}
				else
					break;
			}
		}
		send(n, b, strlen(b), 0);//��½ʧ��
		fclose(fp);
		return 0;
	}
}//��½�˻�
unsigned __stdcall ThreadManager(SOCKET a)
{
	int ret = 0;
	int keep_alive = 1;
		
			ret = setsockopt(a, SOL_SOCKET, SO_KEEPALIVE, (char*)&keep_alive, sizeof(keep_alive));
			if (ret == SOCKET_ERROR)

			{
				gclient->sclient = INVALID_SOCKET;
			}
			else {
				struct tcp_keepalive in_keep_alive = { 0 };
				struct tcp_keepalive out_keep_alive = { 0 };
				unsigned long ul_in_len = sizeof(struct tcp_keepalive);
				unsigned long ul_out_len = sizeof(struct tcp_keepalive);
				unsigned long ul_bytes_return = 0;
				in_keep_alive.keepalivetime = 100;
				in_keep_alive.keepaliveinterval = 200;
				in_keep_alive.onoff = true;
				ret = WSAIoctl(a, SIO_KEEPALIVE_VALS, (LPVOID)&in_keep_alive, ul_in_len, (LPVOID)&out_keep_alive, ul_out_len, &ul_bytes_return, NULL, NULL);
				if (ret == SOCKET_ERROR)
				{
					printf("WSAIoctl failed: %d \n", WSAGetLastError());
				}
			}

	
	return 0;
}
unsigned __stdcall ThreadRecv(SOCKET newsock)//���ܵ�½��Ϣ
{
	SOCKET t = newsock;
	char temp[255];
		memset(temp, '\0', sizeof(temp));
		int ret = recv(t, temp, sizeof(temp), 0); //���յ�½����
		if (strlen(temp) == 6)//"create"�����˻�
		{
			createaccount(t);
			_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))ThreadRecvmessage, (void*)t, NULL, 0);
			
		}
		else if (strlen(temp) == 5)//"login"��½�˻�
		{
			int x;
			x=loginaccount(t);
			if(x==1)
			_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))ThreadRecvmessage, (void*)t, NULL, 0);
		}
		return -3;
}


unsigned int __stdcall ThreadAccept()
{
	while (1)
	{
		int i = 0;
		while (i < 10)
		{
			if (gclient[i].sclient != INVALID_SOCKET)
			{
				i++;	
				continue;
			}
			else
			{
				gclient[i].sclient=accept(s, (sockaddr*)&remoteaddr, &raddrlen);
				if (gclient[i].sclient == INVALID_SOCKET)
				{
					cout << "accept failed" << WSAGetLastError() << endl;
					return -3;
				}
				else
				{
					cout << "link success" << endl;
					memcpy(gclient[i].IP, inet_ntoa(remoteaddr.sin_addr), sizeof(gclient[i].IP));//�洢���û���ip��ַ
					gclient[i].flag = gclient[i].sclient;//��Ǹÿͻ���
					_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *)) ThreadManager, (void*)gclient[i].sclient, 0, NULL);//���������߳�
					_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))ThreadRecv, (void*)gclient[i].sclient, NULL, 0);
					i++;
				}
			}
		}
	}
	return 0;
}//��������


int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "failed";
		return 1;
	}
	
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		cout << "failed" << WSAGetLastError() << endl;
		return -1;
	}
	struct sockaddr_in sin;//���˵�ַ
	sin.sin_family = AF_INET;
	sin.sin_port = htons(9999);
	sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (bind(s, (sockaddr*)&sin, sizeof(sockaddr)) == SOCKET_ERROR)
	{
		cout << "failed" << WSAGetLastError()<<endl;
		return -2;
	}//�����ַ
	if (listen(s, 5) == SOCKET_ERROR)
	{
		cout << "failed" << WSAGetLastError();
	}//����
	
	AC=(HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall *)(void *))ThreadAccept, NULL, NULL, 0);
	int k = 0;
	while (k < 1000)
	{
		Sleep(1000000);
		k++;
	}
	int m = 0;
	while (m < 10)
	{
		if (gclient[m].sclient != INVALID_SOCKET)
			closesocket(gclient[m].sclient);
	}
	closesocket(s);
	CloseHandle(AC);
	WSACleanup();
	return 0;
}