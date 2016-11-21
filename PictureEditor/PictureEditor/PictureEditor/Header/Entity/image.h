#pragma once

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "location.h"
#include<string>
#include<vector>
#include<map>
#include<atlimage.h>
#include<iostream>
#include<io.h>
#include<Afxwin.h>

using namespace std;

class Image
{
public:
	Image();
	Image(string url, string picName, string des);
	virtual bool read(string url);
	void getFiles(string path, vector<string>& files);  //��ȡ�ļ���
	virtual bool readDir(string dUrl, int& sucRate);
	virtual bool save(string url);
	virtual bool del(string url);
	virtual Location* getLoc() { return loc; }
	virtual bool setLoc(Location* loca) { loc = loca; }
	virtual string getName() { return picName; }
	virtual bool setName(string name) { this->picName = name; }
	virtual bool getSolution(CImage* image, int& width, int& height);
	virtual bool setSolution(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);   //���÷ֱ���



	virtual bool undoSolution();  //��������
	virtual bool redoSolution();  //��������
	virtual bool setDiscription(string comment);  //��������
	virtual string getDiscription();  //��ȡ����

	virtual HBITMAP GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack);
	virtual void rotate(CImage* image);
	~Image();
private:
	Location* loc;
	string url;  //ͼƬurl
	string dirUrl;  //�ļ���url
	vector<string> urlPrev;  //��¼��ǰ�ķֱ��ʰ汾
	map<string, vector<CImage> >  imageMap;
	string picName;
	string description;
};


#endif



