// ConsoleApplication1.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <iostream>
#include <cstdio>
#include <Windows.h>


char* convert2char(TCHAR* Source){
	char AnsiBuffer[255] ;
	WideCharToMultiByte(CP_ACP,0,Source,wcslen(Source)+1,AnsiBuffer,sizeof(AnsiBuffer),NULL,NULL);
	return AnsiBuffer ;
}


int _tmain(int argc, _TCHAR* argv[])
{
	int a ;
	TCHAR* vmarg = _T("-jar");
	std::cout << vmarg[0]<<std::endl ;
	char* destination = convert2char(vmarg);
	std::cout << destination[0]<<std::endl ;
	std::cin >> a ;

	return 0;
}

