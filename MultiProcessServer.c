//에코서버 서버

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#define PORT 10000
#define BUFSIZE 10000

char buffer[BUFSIZE] = "Hi. I'm Server. \n";
char rcvBuffer[BUFSIZE];
int numClient = 0;

void do_service(int c_socket);
void sig_handler(int signo);

int main(){
	int c_socket, s_socket;
	struct sockaddr_in s_addr, c_addr;
	int len;
	int n;

	signal(SIGCHLD, sig_handler); //첫번째 인자 : 시그널번호, 두번째 인자: 첫번째 인자의 시그널이 발생했을 때 실행되는 함수명

	// 1. 서버 소켓 생성
	s_socket = socket(PF_INET, SOCK_STREAM, 0);

	//2. 서버 소켓 주소 설정
	memset(&s_addr, 0, sizeof(s_addr));
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);

	//3. 서버 소켓바인딩
	if(bind(s_socket,(struct sockaddr *) &s_addr, sizeof(s_addr)) == -1){
		printf("Cannot Bind\n");
		return -1;
	}

	//4.listen() 함수 실행
	if(listen(s_socket, 5) == -1){
		printf("listen Fail\n");
		return -1;
	}

	//5. 클라이언트 요청 처리
	while(1){
		char *token;
		char *str[3];
		int i = 0;
		len = sizeof(c_addr);
		printf("클라이언트 접속을 기다리는 중....\n");
		c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
		printf("/client is connected\n");
		numClient++;
		printf("현재 접속 중인 클라이언트 수 : [%d]\n", numClient);
		

		int pid = fork();

		if(pid > 0) {
			continue;		
		} else if(pid == 0) {
			do_service(c_socket);
			numClient--;
			exit(0);
		} else {
		printf("fork() failed \n");
		exit(0);
		}
	}
	close(s_socket);
	return 0;	
}

void do_service(int c_socket) {
	int n;
	while(1) {
		n = read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		printf("메시지 받음 : %s\n", rcvBuffer);
		rcvBuffer[n] = '\0'; //개행 문자 삭제
		if(strncasecmp(rcvBuffer, "quit", 4) == 0 || strncasecmp(rcvBuffer, "kill server", 11) == 0)
				break;
		else if(!strncmp(rcvBuffer, "안녕하세요", strlen("안녕하세요")))
			strcpy(buffer, "안녕하세요. 만나서 반가워요.");
		else if(!strncmp(rcvBuffer, "이름이 머야?", strlen("이름이 머야?")))
				strcpy(buffer, "내 이름은 강혜경이야.");
		else if(!strncmp(rcvBuffer, "몇 살이야?", strlen("몇 살이야?")))
				strcpy(buffer, "나는 22살이야.");
		else if(!strncasecmp(rcvBuffer, "strlen ", 7))
				sprintf(buffer, "문자열의 길이 : %d", strlen(rcvBuffer)-7);
		else if(!strncasecmp(rcvBuffer, "strcmp ", 7)){
				char *token;
				char *str[3];				
				int i = 0;
				token = strtok(rcvBuffer, " ");
			while(token != NULL){
					str[i++] = token;
					token = strtok(NULL, " ");
			}
			if(i<3)
				sprintf(buffer, "문자열 비교를 위해서는 두 문자열이 필요합니다.");
			else if(!strcmp(str[1], str[2]))
				sprintf(buffer, "%s %s : 같은 문자열 입니다.", str[1], str[2]);
			else
				sprintf(buffer, "%s %s : 다른 문자열 입니다.", str[1], str[2]);
			}
		else
			strcpy(buffer, "무슨 말인지 모르겠습니다.");

		n = strlen(buffer);
		write(c_socket, buffer, n);
	}

	close(c_socket);
}

void sig_handler(int signo) {
	int pid;
	int status;
	pid = wait(&status);
	printf("pid[%d] is terminated.status = %d\n", pid, status);
	numClient--;
	printf("현재 접속 중인 클라이언트 수 : %d\n", numClient);
}
