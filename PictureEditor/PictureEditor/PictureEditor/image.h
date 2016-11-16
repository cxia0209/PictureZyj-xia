#pragma once
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
<<<<<<< HEAD
	Image();
	virtual bool read() = 0 ;
	virtual bool save(string url) = 0;
	virtual bool del(string url) = 0;
	virtual location* getLoc() = 0;
	virtual bool setLoc( Location* loc ) = 0;
=======
	image();
	image(string url,string picName,string des);
	virtual bool read(string url);
	void getFiles(string path, vector<string>& files);  //��ȡ�ļ���
	virtual bool readDir(string dUrl, int& sucRate);
	virtual bool save(string url);
	virtual bool del(string url);
	virtual location* getLoc() { return loc; }
	virtual bool setLoc(location* loca) { loc = loca; }
>>>>>>> origin/master
	virtual string getName() { return picName; }
	virtual bool setName(string name) { this->picName = name; }
	virtual bool getSolution(CImage* image,int& width, int& height);
	virtual bool setSolution(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth);   //���÷ֱ���
	virtual bool undoSolution();  //��������
	virtual bool redoSolution();  //��������
	virtual bool setDiscription();  //��������
	virtual string getDiscription(string url);  //��ȡ����
	virtual void zoomOut();   //�Ŵ�
	virtual void zoomIn();    //��С
	virtual void rotate();
	~Image();
private:
	location* loc;
	string url;  //ͼƬurl
	string dirUrl;  //�ļ���url
	vector<string> urlPrev;  //��¼��ǰ�ķֱ��ʰ汾
	map<string, vector<CImage> >  imageMap;
	string picName;
	string description;
};
