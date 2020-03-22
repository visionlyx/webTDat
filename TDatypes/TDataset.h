#ifndef TDATASET_H
#define TDATASET_H
#include <tiffio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <ctime>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <omp.h>  
#include "../IOThreader/Volmereader.h"
#include "ROI.h"
using std::map;
using std::vector;
using std::string;
using std::list;
#define SIZE_TEST 10
#define BLOCK_SIZE 128.0
#define DEPTH_SIZE 128
#define BUFFERNUM 50
#define BLOCK_SIZE_INT 128
#define HALF_WINDOW_SIZE_2D 250
#define SLICE_SIZE 500

class TDataset  : public QObject
{
	Q_OBJECT

public:
	int id;               //ID 号
	int img_type;           //图像类型，区分血管数据，胞体数据，荧光数据
	int sz0;            //三个方向的大小，像素   sz0 x方向
	int sz1;            //sz1  y方向
	int sz2;          //sz2 Z方向
	int level_size;         //图像分辨率等级
	double rez_x, rez_y, rez_z; //the resolution of a image pixel along the 3 axes
	double rez_ROI_x, rez_ROI_y, rez_ROI_z; //the resolution of a image pixel along the 3 axes
	double origin_ROI_x, origin_ROI_y, origin_ROI_z;       //感兴趣区域相对应原点偏移
	string file_path;

	QList<ROI*> roi_list;


	int ROI_size[3];

	unsigned char * ROIdata; //选取的感兴趣区域
	QString URL_path;


	TDataset()
	{
		id = 0;
		img_type = 0;
		sz0 = sz1 = sz2 = 0;
		rez_x = rez_y = rez_z = 1;
		rez_ROI_x = rez_ROI_y = rez_ROI_z = 0;
		origin_ROI_x = origin_ROI_y = origin_ROI_z = 0;
		level_size = 0;
		ROI_size[0] = ROI_size[1] = ROI_size[2] = 0;
		ROIdata = NULL;
	}

	virtual ~TDataset()
	{
	}
	bool loadTDataset(const char *path);       //实现
	bool select_ROI_file_with_level_xyz_8bit_2(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level);   //实现

	
	bool readtifftobuffer2D(string path_for_read, uint8 *slice_in, int slice_index); //不实现
	bool readtifftobuffer_direct(const char filename[], unsigned char *block_in); //实现


	vector<string> get_file_list_path(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level);

	QList<QStringList> get_block_path_all_bit(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level);

	int * get_original_crood(string file_name);

	
	void write8bitImage(const char *path, unsigned char* buffer, uint32 width, uint32 height);
	bool load3Dtiff(QString path);


	public slots:
	void update_ROI(ROI * roi);
	void update_mostd(QStringList list);
	void access_ROI(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level);
	//void update_pROI(unsigned char * p);
signals:
	void emit_ask_actor(ROI * roi);//分割完成后进行的操作，将结果加入GUI


};



#endif