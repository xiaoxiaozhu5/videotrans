# videotrans
S3C6410平台下，基于QT的视频传输

## 环境：
	ubuntu 9.10
	gcc/g++:4.4.1
	arm-linux-gcc/arm-linux-g++:4.4.1
	qt:4.7.1

	s3c6410:linux 3.0.1 
	qt:4.7.1


## 简介：
	s3c6410运行服务器端程序，读取USB摄像头，
	然后进行H.264编码，最后通过WiFi发送到电脑，
	在电脑端运行客户端程序解码，并显示。
	服务器和客户端的程序均由qt编写。
