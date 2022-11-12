# IPv6 Socket Programming

🌟[ wiki](https://github.com/gywn-dev/socketIPv6/wiki)
<br/><br/>

### ⚙️ **2021년 2학기 최형기 교수님 컴퓨터네트웍개론 프로젝트**

- Socket programming을 통해 IPv4 및 IPv6 통신 구현

	- [1. `IPv4 클라이언트`로 IPv4 서버에 접속하여 자신의 IPv6 서버 IP 및 포트번호 전달](#1-ipv4-client가-ipv4-서버에-접속)

	- [2. `IPv6 서버`는 concurrent하게 구현되어 IPv6 클라이언트(총 5개, multi clients)가 전달하는 20바이트짜리 난수 5개를 수신 후, 이를 `IPv4 클라이언트`에 전달](#2-ipv6-서버가-multi-clients로부터-5개의-토큰을-수신-후-전달)

	- [3. `IPv4 클라이언트`는 전달받은 토큰 5개를 IPv4 서버에 전송](#3-ipv4-클라이언트가-전달받은-토큰-5개를-ipv4-서버에-전송)

- multi clients에 대응하는 concurrent 서버 구현 필요

- IPv6 서버를 위한 tunneling 필요
<br/>

## 1. Protocol and Program details
<img width="85%" src="https://user-images.githubusercontent.com/94846990/144199378-92fcc5a1-3638-4cf2-9fd8-86fe0d000d02.png">
<br/>

## 2. Used Environment
- Host OS: macOS Monterey 12.0.1
- Guest OS: Ubuntu 20.0 on VMware Fusion
- Language: C
- IDE: VScode on Ubuntu (20.04)
<br/>

## 3. Programming
### 1\) IPv4 client가 IPv4 서버에 접속
- **IPv6 서버 IP 및 포트번호 전달**
	- 예시로 주어진 example.pcap 파일을 확인해보면, 먼저 터미널 상의 색을 지정하는 패킷이 한 번 수신된 다음 메세지가 전송됨. 따라서 패킷을 두 번 연속으로 읽어와야 할 것
	- 주어진 문항에 답변하는 패킷을 보낼 때, `scanf()`를 사용하면 개행문자까지 버퍼에 저장되고, 이 함수는 개행문자 직전까지만 버퍼에서 읽어들여 변수에 저장하므로 다음번 문항에 답변하려 할 때 버퍼에 남아있던 개행문자로 인해 사용자의 입력을 받지 않고 넘어가는 문제가 발생할 것
		- `read()`함수를 사용하여 파일 디스크립터 인자로 `0`을 주면 표준 입력으로부터 읽어들이므로 해결 가능
	- `read()` 함수를 통해 파일의 len 바이트 만큼 읽으라고 요청하였지만 읽은 데이터가 없다면, `read()`는 읽은 바이트가 생길 때 까지 `block`된다. 이 시스템 콜로 인한 블록은 곧 프로세스가 블록됨을 의미하므로, IPv4 클라이언트는 `fork()` 등의 방법을 사용하지 않고 단일 프로세스로 작동되고 있음을 고려했을 때 `read()` 아랫부분의 코드는 작동하지 않고 같이 블록된다. 즉 `read()` 함수를 무분별하게 쓰면 프로그램이 중간에 멈춘 것 처럼 보일 것.
<br/><br/>

### 2\) IPv6 서버가 multi clients로부터 5개의 토큰을 수신 후 전달
**(1) Concurrent IPv6 서버 구현**
- concurrent하게 구현하지 않으면 `accept()` 요청 자체가 수신되지 않음
- 자식 프로세스가 읽어들인 토큰 값을 부모 프로세스에게 전달하는 IPC 통신 필요
	- 자식 프로세스가 데이터를 전달하면 부모 프로세스가 읽어들이는 단방향 통신이므로 간단하게 `PIPE` 방식 사용
<br/><br/>

**(2) IPv6 서버와 IPv4 클라이언트 간 통신**
- 앞서 IPv6 서버 내에서의 IPC와 다르게 IPv6 서버와 IPv4 클라이언트 간 IPC의 경우 두 프로세스의 PPID가 다르므로 다른 방식의 IPC 필요
- `fopen()` 등의 방식으로 파일을 생성해서 쓰고 읽는 방식의 통신은 금지됨
	- 앞선 `PIPE` 방식의 경우 이름을 붙이면 (`named PIPE`) PPID를 공유하지 않는 프로세스 간 통신도 가능하지만, `PIPE` 방식 자체가 `temporal FIFO 파일`을 만들어 파일 디스크립터를 통해 접근 및 읽고 쓰는 방식이므로 `PIPE` 대신 `Message Queue` 방식을 사용함
<br/><br/>

### 3\) IPv4 클라이언트가 전달받은 토큰 5개를 IPv4 서버에 전송
- 전송 시 데이터의 마지막임을 알리는 문자(혹은 숫자) 추가 필요
<br/><br/>

## 4. Result
<img src="https://user-images.githubusercontent.com/94846990/146135823-73d8871f-9dd0-4dcc-ac7c-19ffd081af34.png">
<br/>

## 5. Further Discussion
- IPv6 서버의 부모 프로세스가 모든 토큰 수신이 끝난 후 `listen()`에 사용되었던 소켓을 닫아주지 못했다(`close(listen_sock_fd)` 필요). 제출이 끝나고 알아차려서 너무 아쉽다😭
- trials의 IPv6서버 코드와 최종 제출한 IPv6 서버 코드와의 차이점은 <u>(1) 자식 클라이언트가 `if (pid==0)` 문 안에서 `listen`에 사용된 소켓을 닫느냐 닫지 않느냐</u> 하는 부분과 <u>(2) 자식 클라이언트가 `accept`된 소켓으로부터 토큰을 읽어올 때 `read()` 함수로 그냥 한 번 읽어오느냐 또는 읽을 바이트가 없을 때까지 `while`문으로 읽어오느냐</u> 하는 부분의 두 가지 뿐인데, trials의 코드는 작동하지 않았어서 정확히 어느 부분이 문제를 발생시켰는지 제대로 파악하고 싶다.
- 소켓 프로그래밍인 만큼 IPv6 서버와 IPv4 클라이언트 간의 통신도 소켓으로 해보고 싶었는데 머리가 복잡해져서 그러지 못했다. 이미 하나의 소켓이 열려서 다른 프로그램과 통신하고 있을 때 또다른 소켓을 열어 하나의 통신을 더 하는 방법을 공부해봐야 할 듯 싶다.
