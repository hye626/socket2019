#include <stdio.h>
#include <netinet/in.h> //네트워크 통신
#include <sys/socket.h>
#include <string.h>

#define PORT 10000 //포트변수 9000으로 할당

char buffer[100] = "HI. I'm Server.";
char rcvBuffer[100];

int main() {
    int c_socket, s_socket; //서버소켓, 클라이언트소켓 int로 할당
    struct sockaddr_in s_addr, c_addr; //서버어드레스, 클라이언트어드레스 구조체 생성
    int len;
    int n;
   
    // 1. 서버 소켓 생성(소켓 함수 사용)
    //서버 소켓 = 클라이언트의 접속 요청을 처리(허용)해 주기 위한 소켓
    s_socket = socket(PF_INET, SOCK_STREAM, 0); //어떤 통신방법을 쓸지 인자값으로 넘겨줌=TCP/IP통신
   
    // 2. 서버 소켓 주소 설정
    memset(&s_addr, 0, sizeof(s_addr)); //s_addr의 값을 모두 0으로 초기화
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP 주소 설정
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    // 3. 서버 소켓 바인딩 (대표번호(IP)와 내선번호(PORT)를 연결하는 작업)
    if(bind(s_socket, (struct sockaddr *) &s_addr, sizeof(s_addr)) == -1) { //바인딩 작업 실패 시,
        printf("Cannot Bind\n");
        return -1; //프로그램 종료
    }


    // 4.listen() 함수 실행
    if(listen(s_socket, 5) == -1) { //5는 동접자를 의미
        printf("listen Fail\n");
        return -1;
    }

    // 5. 클라이언트 요청 처리
    // 요청 허용 후, Hello World 메세지를 전송함
    while(1) { //무한 루프
        len = sizeof(c_addr);
        printf("클라이언트 접속을 기다리는 중 ...\n");
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len); //클라이언트 소켓 생성
        //클라이언트의 요청이 오면 허용(accept)해 주고, 해당 클라이언트와 통신할 수 있도록 클라이언트 소켓(c_socket)을 반환
        printf("클라이언트 접속 허용\n");
		 read(c_socket, rcvBuffer, sizeof(rcvBuffer));
		 printf("rcvBuffer:%s\n", rcvBuffer);
        n = strlen(buffer);
        write(c_socket, buffer, n); //클라이언트에게 buffet의 내용을 전송함

        close(c_socket);
    }

    close(s_socket);
    return 0;
}
