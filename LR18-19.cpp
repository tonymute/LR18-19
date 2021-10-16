// LR18-19.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "pch.h"
#include "framework.h"
#include "LR18-19.h"
#pragma warning(disable : 4996)

#define BUF_SIZE 160															//объявление константы BUF_SIZE(размер буфера) равной 160
#define rmaxf1 40																//объявление константы rmaxf1(длина 1 строки) равной 40
#define rmaxf2 10																//объявление константы rmaxf2(длина 2 строки) равной 10

#include <process.h>															//подключение заголовочного файла, для использования функций управления сопроцессами
#include <stdarg.h>																//подключение заголовочного файла, для обрабатывания списока переменных аргументов
#include <string.h>																//подключение заголовочного файла, для работы с типом данных String
void KCompare(CHAR, CHAR, int);													//объявление функции с аргументами Kcompare, которая сравнивает строки 
void KCompare(CHAR string1[rmaxf1], CHAR string2[rmaxf2], int KEY)				//описание тела функции Kcompare с параметрами string1, string2, KEY
{
	char tmp[20];																//объявление массива символов tmp размерностью 20
	int result;																	//объявление целочисленной переменной result, хранящей результат сравнения 
	result = _tcsncmp((CHAR*)string1, (CHAR*)string2, KEY);						//сравниваем строки с помощью функции _tcsncmp, получившийся результат помещаем в result
	if (result == 0)															//если в результате сравнения в переменную result помещен 0, то ..
	{
		strcpy(tmp, "equal to");												//помещаем в массив tmp сообщение "equal to"("равно")
		printf("Result:\t\tString 1 is %s string 2\n\n", tmp);					//вывод на экран сообщения, что строки равны
	}
	else																		//иначе ..
	{
		strcpy(tmp, "noequal to");												//помещаем в массив tmp сообщение "noequal to"("неравно")
		printf("Result:\t\tString 1 is %s string 2\n\n", tmp);					//вывод на экран сообщения, что строки не равны
	}
}
int _tmain(int argc, LPTSTR argv[])												//главная функция
{
	HANDLE hFile, hFile2, hMap;													//объявление hFile - исходный файл, hFile2 - файл содержащий искомый фрагмент, hMap - переменная, для работы с дынными исходного файла
	LPCTSTR pFile;																//объявление строковой константы pFile - хрянящей текст исходного файла
	CHAR string1[rmaxf1], string2[rmaxf2];										//объявление символьных массивов (строк) string1 размерностью 40, string2 размерностью 10
	CHAR Buffer1[BUF_SIZE], Buffer2[BUF_SIZE];									//объявление символьных массивов буферов Buffer1 и Buffer2 размерностью 160
	DWORD FsLow, nf2;															//объявление беззнаковых целочисленных переменных FsLow - кол-во символов в файлах и nf2 - кол-во читаемых символов
	int i, j, k, KEY;															//объявление целочисленных i, j, k - параметров циклов, KEY - число искомого фрагмента
	BOOL f2;																	//объявление логической переменной f2
	LPCTSTR pF;																	//объявление строковой константы pF - хрянящей текст исходного файла
	hFile = CreateFile(argv[1], GENERIC_READ, 0, NULL,							//открываем исходный файл для чтения и помещаем его в hFile
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);							//
	FsLow = GetFileSize(hFile, NULL);											//присваиваем FsLow кол-во символов в исходном файле
	printf("Fs=%d\n", (int)FsLow);												//выводит на экран число символов в исходном файле
	hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY,						//помещает в файл объект отображенного в памяти файла для заданного файла
		0, 0, NULL);															//
	pFile = (char*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);					//создание представления файла в виртуальном адресном пространстве процесса
	pF = pFile;																	//помещаем pFile в pF
	for (i = 0; i < (int)FsLow; i++)											// |
	{																			// | цикл проходящий по всем символам исходного файла и
		Buffer1[i] = *pF;														// | заносящий их в буфер 1
		pF++;																	// |
	}																			// 
	for (i = 0; i < (int)FsLow; i++)											// | цикл проходящий по всем символам в буфере 1 и
		printf("%c", Buffer1[i]);												// | выводящий их на экран
	printf("\n");																//переход на новую строку на экране
	hFile2 = CreateFile(argv[2], GENERIC_READ, 0, NULL,							//открываем файл с фрагментом для чтения и помещаем его в hFile2
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);							//
	f2 = ReadFile(hFile2, Buffer2, BUF_SIZE, &nf2, NULL);						//
	KEY = GetFileSize(hFile2, NULL);											//определяет и помещает в переменную KEY кол-во символов искомого фрагмента
	printf("KEY=%d\n", (int)KEY);												//выводит на экран число символов искомого фрагмента
	for (i = 0; i < (int)KEY; i++)												// | цикл посимвольно выводящий на экран
		printf("%c", Buffer2[i]);												// | содержимое буфера 2
	printf("\n");																//переход на новую строку на экране
	for (i = 0; i < (int)KEY; i++)											    // | цикл посимвольно переносящий содержимое буфера 2
		string2[i] = Buffer2[i];												// | в строку 2
	k = 0;																		//присваиваем параметру k значение 0
	do																			// |
	{																			// |
		i = k;																	// |
		do																		// |
		{																		// |
			for (j = 0; j < (int)KEY; j++)										// | проход по всем сочетаниям символов размером KEY (кол-во символов
			{																	// | в искомом фрагменте) находящимся в исходном файле, сравнивает 
				string1[j] = Buffer1[j + i];									// | получившиеся строки и результаты выводт на экран (равны они или нет)
				printf("%c", string1[j]);										// |
			}																	// |
			printf("\n");														// |
			getchar();															// |
			KCompare(string1, string2, (int)KEY);								// |
			i = i + KEY;														// |
		} while (i < (int)FsLow - (int)KEY + 1);								// |
		k = k + 1;																// |
	} while (k < (int)FsLow - (int)KEY + 1);									// |
	/* Выводим исходный файл. */
	_tprintf(_T("%s"), pFile);													//вывод исходного файла
	UnmapViewOfFile(pFile);														// |
	CloseHandle(hMap);															// | очистка памяти
	CloseHandle(hFile);															// | от заданных ранее объектов
	CloseHandle(hFile2);														// |
	return 0;																	//возращает 0, программа успешно завершена
}
