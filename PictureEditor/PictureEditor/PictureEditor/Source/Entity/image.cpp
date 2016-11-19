#include "../../Header/Entity/image.h"




Image::Image()
{
	loc = new Location();
	url = "test.jpg";    //Ĭ��Ϊ��ǰ·��
	dirUrl = "";  //Ĭ���ļ���·��
	picName = "";
	description = "";
}

Image::Image(string url, string picName, string des) {
	this->url = url;
	this->picName = picName;
	this->description = des;
}


Image::~Image()
{
	delete loc;
}


//���ص���ͼƬ
bool Image::read(string u) {
	CImage* Image = new CImage();
	Image->Load(u.c_str);

	try {
		if (Image->IsNull) {
			throw 1;
			return false;
		}

		map<string, vector<CImage> >::iterator it = ImageMap.find(u);
		if (it != ImageMap.end()) {
			it->second.push_back(*Image);
		}
		else {
			ImageMap.insert(make_pair(u, vector<CImage>()));
		}

		
	}
	catch (int i) {
		cerr << "��Ƭ�����ڣ�����������·��" << endl;
		string newUrl;
		cin >> newUrl;
		url = newUrl;
	}
	
	delete Image;
}

//�����ļ����������ļ�
bool Image::readDir(string dUrl, int& sucRate) {
	//��ȡ�ļ����������ļ���
	vector<string> files;
	getFiles(dUrl, files);
	int countLoad = 0;
	for (int i = 0; i < files.size(); ++i) {
		if (read(files[i])) {
			countLoad++;
		}
	}
	sucRate = countLoad;
	if (sucRate == files.size())
	{
		return true;
	}
	return false;

}

void Image::getFiles(string path, vector<string>& files)
{
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

bool Image::save(string dUrl) {
	CImage* Image = new CImage();
	string name = "test.jpg";    //�ļ��� Ĭ��Ϊ"test.jpg"

	string comb = dUrl + name;
	Image->Save(comb.c_str);

	//�鿴�Ƿ񱣴�ɹ�
	Image->Load(comb.c_str);

	if (Image->IsNull) {
		return false;
	}
	return true;

	delete Image;
}

bool Image::del(string url) {
	remove(url.c_str);

	//�鿴�Ƿ�ɾ���ɹ�

	CImage* Image = new CImage();

	if (Image->IsNull) {
		return true;
	}
	return false;

	delete Image;
}

bool Image::getSolution(CImage* Image,int& width, int& height) {
	width = Image->GetWidth;
	height = Image->GetHeight;
	return true;
}

bool Image::setSolution(CImage *pImage, CImage *ResultImage, int StretchHeight, int StretchWidth) {
	if (pImage->IsDIBSection) {	
		// ȡ�� pImage �� DC
		CDC* pImageDC1 = CDC::FromHandle(pImage->GetDC);
		CBitmap* bitmap1 = pImageDC1->GetCurrentBitmap();
		BITMAP bmpInfo;
		bitmap1->GetBitmap(&bmpInfo);

		// �����µ� CImage
		ResultImage->Create(StretchWidth, StretchHeight, bmpInfo.bmBitsPixel);
		CDC* ResultImageDC = CDC::FromHandle(ResultImage->GetDC());

		ResultImageDC->SetStretchBltMode(HALFTONE);

		SetBrushOrgEx(ResultImageDC->m_hDC, 0, 0, NULL);

		StretchBlt(*ResultImageDC, 0, 0, StretchWidth, StretchHeight, *pImageDC1, 0, 0, pImage->GetWidth(), pImage->GetHeight(), SRCCOPY);

		pImage->ReleaseDC();
		ResultImage->ReleaseDC();
	}
}