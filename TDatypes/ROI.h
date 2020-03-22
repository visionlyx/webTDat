/*********************************************************************

most_image类，平台读取的三维图像数据保存在该类中，主要为一些set
和get方法。

*************************************************************************/

#ifndef ROI_H
#define ROI_H
#include <vector>
#include <tiffio.h>
#include <QtGui>
#include <QApplication>
#include<QDialog>

class ROI : public QObject

{
	Q_OBJECT
public:
	int ROI_type;           //图像类型，区分血管数据，胞体数据，荧光数据
	unsigned char * pROI; //指针，指向图像的像素点
	int xROI;            //三个方向的大小，像素
	int yROI;
	int zROI;
	double res_xROI, res_yROI, res_zROI; //三个方向分辨率
	int origin_x, origin_y, origin_z; //数据块的起始坐标，暂时未用
	int progress;
	int bit_all_read;

private:


public:

								 //	bool *visited_pr;
								 //构造函数
	ROI()
	{
	
		ROI_type = 0;
		pROI = NULL;
		//		visited_pr = NULL;
		xROI = yROI = zROI = 0;
		res_xROI = res_yROI = res_zROI = 1;
		origin_x = origin_y = origin_z = 0;
		progress = 0;
		bit_all_read = 0;
	}
	virtual ~ROI()
	{

	}
	//清除所有指针和数组

	int gettype() { return ROI_type; }
	void settype(int i) { ROI_type = i; }
	//main interface to the data
	unsigned char * getRawData() { return pROI; }
	//	bool * getVisited_pr() {return visited_pr;}
	const unsigned char * getRawData() const { return pROI; } // const version 25-Apr 2011 CMB
	long getXDim() const { return xROI; }
	long getYDim() const { return yROI; }
	long getZDim() const { return zROI; }
	double getRezX() const { return res_xROI; }
	double getRezY() const { return res_yROI; }
	double getRezZ() const { return res_zROI; }
	double getOriginX() const { return origin_x; }
	double getOriginY() const { return origin_y; }
	double getOriginZ() const { return origin_z; }
//	int* getCenterX(){return }
	bool setNewRawDataPointer(unsigned char *p) { if (!p) return false; if (pROI) delete[]pROI; pROI = p; return true; }
	uint8 getVexel(int x, int y, int z) { return pROI[xROI*yROI*z + xROI*y + x]; }

	//设置图像块的指针
	bool setData(unsigned char *p, int x , int y , int z)
	{
		if (p && x > 0 && y > 0 && z > 0)
			if (setNewRawDataPointer(p))
			{
				xROI = x;
				yROI = y;
				zROI = z;
				return true;
			}
		return false;
	}

};
#endif