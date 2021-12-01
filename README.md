# IPv6 Socket Programming

⚙️ **2021년 2학기 최형기 교수님 컴퓨터네트웍개론 프로젝트**

- Socket programming을 통해 IPv4 및 IPv6 통신 구현

	- [`IPv4 클라이언트`로 IPv4 서버에 접속하여 자신의 IPv6 서버 IP 및 포트번호 전달](#1.-protocol-and-program-details)

	- `IPv6 서버`는 concurrent하게 구현되어 IPv6 클라이언트(총 5개, multi clients)가 전달하는 20바이트짜리 난수 5개를 수신 후,<br/>이를 `IPv4 클라이언트`에 전달

	- `IPv4 클라이언트`는 전달받은 토큰 5개를 IPv4 서버에 전송

- multi clients에 대응하는 concurrent 서버 구현 필요

- IPv6 서버를 위한 tunneling 필요
<br/>

## 1. protocol and program details
<img width="85%" src="https://user-images.githubusercontent.com/94846990/144199378-92fcc5a1-3638-4cf2-9fd8-86fe0d000d02.png">
<br/>

## 2. Used Environment
- Host OS: macOS Monterey 12.0.1
- Guest OS: Ubuntu 20.0 on VMware Fusion
- Language: C only
- IDE: VScode on Ubuntu
<br/>

## 3. Programming
### 1\) IPv4 client가 IPv4 서버에 접속
IPv6 서버 IP 및 포트번호 전달


