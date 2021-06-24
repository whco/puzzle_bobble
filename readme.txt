Puzzle Bobble이라는 아케이드 게임을 c++와 openGL 라이브러리를 활용해 
제작한 코드모음(.cpp 및 h파일) 및 실행을 위해 필요한 openGL파일들입니다.

Windows 10 Home 에디션 2004버전 64비트 운영 체제에서
Microsoft Visual Studio 2019를 사용해 실행하는 것을 기준으로 작성되었습니다.

구성요소 : 

main.cpp
Light.cpp		Light.h  // 빛 생성을 통한 입체감 표현
Material.cpp	Material.h // 질감 표현
SolidShape3D.cpp	SolidShape3D.h // 3D물체 표현하는 super class
SolidSphere.cpp	SolidSphere.h // SolidShape3D 상속하여 구 표현
Vector3.cpp	Vector3.h // 3차원좌표 생성 및 연산에 필요한 기능 제공
reame.txt

실행 전 설정 방법은 다음과 같습니다.
/*
* made in windows10 visual studio 19 environment
*
* zip-off "opengl.zip"
* Copy “include” and “lib” folder included in "opengl.zip" to your project folder
* Copy “glut32.dll” included in "opengl.zip" > "dll" to your project folder or exe folder
*
* Project > Properties > C/C++ > General > Additional include directories > Type “./include"
* Project > Properties > Linker > General > Additional library directories > Type“./lib"
* Project > Properties > Linker > Input > Additional dependencies > Add“glut32.lib"
*
*/