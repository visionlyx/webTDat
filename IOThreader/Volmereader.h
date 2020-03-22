#ifndef VOLUMEREADER_H
#define	VOLUMEREADER_H
#include <QThread>
#include <iostream>
#include <tiffio.h>
#include <QtNetwork>  
#include "../TDatypes/ROI.h"
#include"../IOThreader/webtiffreader.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <omp.h>  
#define BLOCK_SIZE 128.0
#define DEPTH_SIZE 128
class VolumeReader :public QThread
{
	Q_OBJECT
private:
	//QString path;
	unsigned char * pROI;
	int roi_x;
	int roi_y;
	int roi_z;

	double res_x;
	double res_y;
	double res_z;

	int x_index_b;
	int x_index_e;
	int y_index_b;
	int y_index_e;
	int z_index_b;
	int z_index_e;

	int z_shift;   //z���������blockС��ԭ���ƫ������
	int y_shift;  //y���������blockС��ԭ���ƫ������
	int x_shift;  //x���������blockС��ԭ���ƫ������

	int x_block_b ;
	int x_block_e;
	int y_block_b ;
	int y_block_e ;
	int z_block_b ;
	int z_block_e ;
	int half_bit_state;

	int level;

	QList<QStringList> path;
	QString URL;
	ROI* roi;
	
public:

	


	VolumeReader()
	{
		roi_x = 0;
		roi_y = 0;
		roi_z = 0;

		 res_x = 0;
		 res_y = 0;
		 res_z = 0;
		 half_bit_state = 0;

		roi = new ROI();
		URL = "G:/data/test_tiff_data_tdat";
	
	}
	~VolumeReader(){ wait(); pROI = NULL; }

	void set_xyz_size(int x, int y, int z){ roi_x = x; roi_y = y; roi_z = z; }
	void set_xyz_res(double rex, double rey, double rez);
	//void access_ROI(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level, QString URL_path);

protected:
	void run();//��ʼ�ָ�
	void get_block_path_all_bit();
	bool write_buffer(const char filename[], unsigned char * buffer, int x_size, int y_size, int z_size);

signals:
	void emit_finish_read(ROI *roi);//�ָ���ɺ���еĲ��������������GUI
//	void emit_finish_read2(unsigned char *pROI);//�ָ���ɺ���еĲ��������������GUI�ĵ�һЩ�źŲ���
public slots:
void access_ROI(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level, int ishalfbit);
	
	

	
};



#endif