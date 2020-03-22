#include "TDataset.h"


bool TDataset::loadTDataset(const char *path)
{

	int level_size_t;
	int block_num_t[30][3];
	double rez_x_t, rez_y_t, rez_z_t; //the resolution of a image pixel along the 3 axes
	char  file_path_t[150];

	FILE *fp = fopen(path, "r");
	if (!feof(fp))
	{

		fscanf(fp, "%d\n", &id);
		fscanf(fp, "%d\n", &img_type);
		fscanf(fp, "%d\n", &sz0);
		fscanf(fp, "%d\n", &sz1);
		fscanf(fp, "%d\n", &sz2);
		fscanf(fp, "%d\n", &level_size);

	
		fscanf(fp, "%f\n", &rez_x);
		fscanf(fp, "%f\n", &rez_y);
		fscanf(fp, "%f\n", &rez_z);
		fscanf(fp, "%s\n", file_path_t);
		this->file_path = file_path_t;

	}

	fclose(fp);
	return true;

}



vector<string> TDataset::get_file_list_path(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level)
{

	vector<string> list_of_index;

	int x_index_b = xbegin;
	int x_index_e = xend;
	int y_index_b = ybegin;
	int y_index_e = yend;
	int z_index_b = zbegin;
	int z_index_e = zend;

	int x_temp_b;
	int x_temp_e;
	int y_temp_b;
	int y_temp_e;
	int z_temp_b;
	int z_temp_e;


	int z_shift;
	int y_shift;
	int x_shift;


	for (int i = 1; i<level; i++)
	{
		x_index_b /= 2;
		x_index_e /= 2;
		y_index_b /= 2;
		y_index_e /= 2;
		z_index_b /= 2;
		z_index_e /= 2;
	}


	//	uint8 *buffer = new uint8[width_ROI*height_ROI*depth_ROI];  //申请总的内存开销


	//uint32 *slice = new uint32[int(BLOCK_SIZE*BLOCK_SIZE)];   //申请一张的内存开销，即每个slice的内存

	//	uint8 *block_temp = new uint8[int(BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE)];

	int x_block_b = x_index_b / int(BLOCK_SIZE);
	int x_block_e = x_index_e / int(BLOCK_SIZE);
	int y_block_b = y_index_b / int(BLOCK_SIZE);
	int y_block_e = y_index_e / int(BLOCK_SIZE);
	int z_block_b = z_index_b / int(DEPTH_SIZE);
	int z_block_e = z_index_e / int(DEPTH_SIZE);


	//theMsg->printf("%d\t%d\t%d\t%d\t%d\t%d\n",x_block_b,x_block_e,y_block_b,y_block_e,z_block_b,z_block_e);

	//std::cout<<x_block_b<<" "<<x_block_e<<" "<<y_block_b<<" "<<y_block_e<<" "<<z_block_b<<" "<<z_block_e<<std::endl;



	string str;
	int * temp_temp_be;
	char temppp[6];
	str.clear();

	int index_of_xy;
	for (int i = y_block_b; i <= y_block_e; i++)
	{
		for (int j = z_block_b; j <= z_block_e; j++)
		{
			for (int k = x_block_b; k <= x_block_e; k++)
			{


				str = this->file_path;
				str.append("/level");

				std::stringstream a8;
				a8 << int(level);
				str.append(a8.str().data());

				str.append("_data/z");




				//index_of_xy = j*(this->block_num[level][0])+k;

				std::stringstream a9;
				a9 << int(j);
				str.append(a9.str().data());
				str.append("/y");

				std::stringstream a11;
				a11 << int(i);
				str.append(a11.str().data());
				str.append("/");


				std::stringstream a12;
				a12 << int(k);
				str.append(a12.str().data());
				str.append("_");
				std::stringstream a19;
				a19 << int(i);
				str.append(a19.str().data());
				str.append("_");
				std::stringstream a20;
				a20 << int(j);
				str.append(a20.str().data());


				str.append(".tif");
				list_of_index.push_back(str);

			}//for (int k = x_block_b; k <= x_block_e; k++)

		}//for (int j = y_block_b; j <=y_block_e; j++)
	}//for (int i = z_block_b; i <= z_block_e; i++)
	return   list_of_index;



}



bool TDataset::readtifftobuffer_direct(const char filename[], unsigned char *block_in)
{
	if (block_in == NULL)
		return false;
	uint32 *slice = new uint32[int(BLOCK_SIZE*BLOCK_SIZE)];


	TIFFSetWarningHandler(0);
	TIFF *tif = TIFFOpen(filename, "r");
	if (tif == 0) {
		printf(" open TIFF file %s failed\n", filename);
		return false;
	}

	for (int i_block_temp = 0; i_block_temp<int(DEPTH_SIZE); ++i_block_temp) {
		// assert bits, channels, photometric & width, height
		TIFFReadRGBAImageOriented(tif, int(BLOCK_SIZE), int(BLOCK_SIZE), slice, ORIENTATION_TOPLEFT);


		//#pragma omp parallel for
		for (int j_block_voxel = 0; j_block_voxel<int(BLOCK_SIZE*BLOCK_SIZE); j_block_voxel++)
		{
			block_in[i_block_temp*int(BLOCK_SIZE*BLOCK_SIZE) + j_block_voxel] = TIFFGetR(slice[j_block_voxel]);
		}
		TIFFReadDirectory(tif);
	}

	TIFFClose(tif);
	delete[] slice;
	return true;

}


bool TDataset::select_ROI_file_with_level_xyz_8bit_2(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level)
{

	const char *path;

	vector<string> list_of_index = get_file_list_path(xbegin, xend, ybegin, yend, zbegin, zend, level);
	//	std::cout<<list_of_index.size()<<std::endl;
	for (int i = 0; i < list_of_index.size(); i++)
	{
		//std::cout<<list_of_index.at(i)<<std::endl;
	}

	uint8 **buffer_temp = NULL;
	buffer_temp = new uint8 *[list_of_index.size()];



#pragma omp parallel  for
	for (int i = 0; i < list_of_index.size(); i++)
	{

		buffer_temp[i] = new uint8[int(BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE)];
		for (int j = 0; j<BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE; j++)
		{
			buffer_temp[i][j] = 0;
		}
		readtifftobuffer_direct(list_of_index.at(i).c_str(), buffer_temp[i]);

	}



	int x_index_b = xbegin;   //零时变量
	int x_index_e = xend;
	int y_index_b = ybegin;
	int y_index_e = yend;
	int z_index_b = zbegin;
	int z_index_e = zend;




	int x_temp_b;
	int x_temp_e;
	int y_temp_b;
	int y_temp_e;
	int z_temp_b;
	int z_temp_e;


	int z_shift;   //z方向相对于block小块原点的偏移像素
	int y_shift;  //y方向相对于block小块原点的偏移像素
	int x_shift;  //x方向相对于block小块原点的偏移像素



				  //所选区域的三个方向大小
	int width_ROI = x_index_e - x_index_b + 1;
	int height_ROI = y_index_e - y_index_b + 1;
	int depth_ROI = z_index_e - z_index_b + 1;
	
	this->ROI_size[0] = width_ROI;
	this->ROI_size[1] = height_ROI;
	this->ROI_size[2] = depth_ROI;

	this->origin_ROI_x = xbegin;   //设置ROI区域的相对整个数据集原点的偏移
	this->origin_ROI_y = ybegin;
	this->origin_ROI_z = zbegin;


	this->rez_ROI_x = (2 ^ level)*rez_x;  //设置ROI区域的分辨率
	this->rez_ROI_y = (2 ^ level)*rez_y;
	this->rez_ROI_z = (2 ^ level)*rez_z;

	uint8 *buffer = new uint8[width_ROI*height_ROI*depth_ROI];  //申请总的内存开销


	int x_block_b = floor(x_index_b / BLOCK_SIZE);    //分别计算出每个方向起始的block编号
	int x_block_e = floor(x_index_e / BLOCK_SIZE);
	int y_block_b = floor(y_index_b / BLOCK_SIZE);
	int y_block_e = floor(y_index_e / BLOCK_SIZE);
	int z_block_b = floor(z_index_b / double(DEPTH_SIZE));
	int z_block_e = floor(z_index_e / double(DEPTH_SIZE));



	x_shift = x_index_b%int(BLOCK_SIZE);  //计算出起始点相对于第一个block块原点的偏移
	y_shift = y_index_b%int(BLOCK_SIZE);
	z_shift = z_index_b%int(DEPTH_SIZE);

	if (x_shift<0)
	{
		x_shift = x_shift + int(BLOCK_SIZE);
	}
	if (y_shift<0)
	{
		y_shift = y_shift + int(BLOCK_SIZE);
	}
	if (z_shift<0)
	{
		z_shift = z_shift + int(BLOCK_SIZE);
	}



	for (int i = z_block_b; i <= z_block_e; i++)
	{
		for (int j = y_block_b; j <= y_block_e; j++)
		{
			for (int k = x_block_b; k <= x_block_e; k++)
			{


				int index_of_buffer_temp = (i - z_block_b)*(y_block_e - y_block_b + 1)*(x_block_e - x_block_b + 1) + (j - y_block_b)*(x_block_e - x_block_b + 1) + (k - x_block_b);
				//计算x每个block块内部要读取的范围
				if (k == x_block_b&&k != x_block_e)              //k是起始的block不是终止的block,
				{
					x_temp_b = x_index_b%int(BLOCK_SIZE);
					if (x_temp_b<0)
					{
						x_temp_b = x_temp_b + int(BLOCK_SIZE);
					}

					x_temp_e = int(BLOCK_SIZE) - 1;
				}
				if (k != x_block_b&&k != x_block_e)             //k不是起始的block也不是终止的block
				{
					x_temp_b = 0;
					x_temp_e = int(BLOCK_SIZE) - 1;
				}
				if (k != x_block_b&&k == x_block_e)     //k不是起始的block，是终止的block
				{
					x_temp_b = 0;
					x_temp_e = x_index_e%int(BLOCK_SIZE);
					if (x_temp_e<0)
					{
						x_temp_e = x_temp_e + int(BLOCK_SIZE);
					}
				}
				if (k == x_block_b&&k == x_block_e)    //k是起始的block，也是终止的block
				{
					x_temp_b = x_index_b%int(BLOCK_SIZE);
					if (x_temp_b<0)
					{
						x_temp_b = x_temp_b + int(BLOCK_SIZE);
					}
					x_temp_e = x_index_e%int(BLOCK_SIZE);
					if (x_temp_e<0)
					{
						x_temp_e = x_temp_e + int(BLOCK_SIZE);
					}
				}

				//计算y方向每个block块内部要读取的范围

				if (j == y_block_b&&j != y_block_e)
				{
					y_temp_b = y_index_b%int(BLOCK_SIZE);
					if (y_temp_b<0)
					{
						y_temp_b = y_temp_b + int(BLOCK_SIZE);
					}
					y_temp_e = int(BLOCK_SIZE) - 1;
				}
				if (j != y_block_b&&j != y_block_e)
				{
					y_temp_b = 0;
					y_temp_e = int(BLOCK_SIZE) - 1;
				}
				if (j != y_block_b&&j == y_block_e)
				{
					y_temp_b = 0;
					y_temp_e = y_index_e%int(BLOCK_SIZE);
					if (y_temp_e<0)
					{
						y_temp_e = y_temp_e + int(BLOCK_SIZE);
					}
				}
				if (j == y_block_b&&j == y_block_e)
				{
					y_temp_b = y_index_b%int(BLOCK_SIZE);
					if (y_temp_b<0)
					{
						y_temp_b = y_temp_b + int(BLOCK_SIZE);
					}
					y_temp_e = y_index_e%int(BLOCK_SIZE);
					if (y_temp_e<0)
					{
						y_temp_e = y_temp_e + int(BLOCK_SIZE);
					}
				}

				//计算z方向每个block块内部要读取的范围

				if (i == z_block_b&&i != z_block_e)
				{
					z_temp_b = z_index_b%int(DEPTH_SIZE);
					if (z_temp_b<0)
					{
						z_temp_b = z_temp_b + int(BLOCK_SIZE);
					}
					z_temp_e = int(DEPTH_SIZE) - 1;
				}
				if (i != z_block_b&&i != z_block_e)
				{
					z_temp_b = 0;
					z_temp_e = int(DEPTH_SIZE) - 1;
				}
				if (i != z_block_b&&i == z_block_e)
				{
					z_temp_b = 0;
					z_temp_e = z_index_e%int(DEPTH_SIZE);
					if (z_temp_e<0)
					{
						z_temp_e = z_temp_e + int(BLOCK_SIZE);
					}
				}
				if (i == z_block_b&&i == z_block_e)
				{
					z_temp_b = z_index_b%int(DEPTH_SIZE);
					if (z_temp_b<0)
					{
						z_temp_b = z_temp_b + int(BLOCK_SIZE);
					}
					z_temp_e = z_index_e%int(DEPTH_SIZE);
					if (z_temp_e<0)
					{
						z_temp_e = z_temp_e + int(BLOCK_SIZE);
					}
				}

				//循环将block中要读取的像素填充在buffer中

				for (uint16 i_read_voxel_z = z_temp_b; i_read_voxel_z <= z_temp_e; i_read_voxel_z++)
				{
					for (uint32 j_read_voxel_y = y_temp_b; j_read_voxel_y <= y_temp_e; j_read_voxel_y++)
					{
						for (uint32 k_read_voxel_x = x_temp_b; k_read_voxel_x <= x_temp_e; k_read_voxel_x++)
						{


							buffer[((i - z_block_b)*int(DEPTH_SIZE) + (i_read_voxel_z - z_shift))*width_ROI*height_ROI + ((j - y_block_b)*int(BLOCK_SIZE) + (j_read_voxel_y - y_shift))*width_ROI + (k - x_block_b)*int(BLOCK_SIZE) + k_read_voxel_x - x_shift] = buffer_temp[index_of_buffer_temp][i_read_voxel_z*int(BLOCK_SIZE)*int(BLOCK_SIZE) + j_read_voxel_y*int(BLOCK_SIZE) + k_read_voxel_x];
						}
					}
				}

			}//for (int k = x_block_b; k <= x_block_e; k++)

		}//for (int j = y_block_b; j <=y_block_e; j++)
	}//for (int i = z_block_b; i <= z_block_e; i++)


#pragma omp parallel  for
	for (int i = 0; i < list_of_index.size(); i++)
	{

		delete[] buffer_temp[i];

	}


	this->ROIdata = buffer;


	return true;


}








int * TDataset::get_original_crood(string file_name)
{
	int *xyz_crood = new int[3];
	string file_temp = file_name;
	int first_dot = file_temp.rfind('.', string::npos);
	int first_ = file_temp.rfind('_', string::npos);
	int secend_ = file_temp.rfind('_', first_ - 1);
	int first_virgule = file_temp.rfind('/', string::npos);

	//std::cout<<first_dot<<std::endl;
	//std::cout<<first_<<std::endl;
	//std::cout<<secend_<<std::endl;
	//	std::cout<<first_virgule<<std::endl;

	string x_index_string = file_temp.substr(first_virgule + 1, secend_ - first_virgule - 1);
	string y_index_string = file_temp.substr(secend_ + 1, first_ - secend_ - 1);
	string z_index_string = file_temp.substr(first_ + 1, first_dot - first_ - 1);

	//	std::cout<<x_index_string<<std::endl;
	//	std::cout<<y_index_string<<std::endl;
	//	std::cout<<z_index_string<<std::endl;

	std::stringstream ssx;
	std::stringstream ssy;
	std::stringstream ssz;

	ssx << x_index_string;
	ssy << y_index_string;
	ssz << z_index_string;


	ssx >> xyz_crood[0];
	ssy >> xyz_crood[1];
	ssz >> xyz_crood[2];

	xyz_crood[0] = xyz_crood[0] * int(BLOCK_SIZE);
	xyz_crood[1] = xyz_crood[1] * int(BLOCK_SIZE);
	xyz_crood[2] = xyz_crood[2] * int(BLOCK_SIZE);


	return xyz_crood;

}



bool TDataset::readtifftobuffer2D(string path_for_read, uint8 *slice_in, int slice_index)
{
	if (slice_in == NULL)
		return false;
	const char *path;
	path = path_for_read.c_str();
	TIFFSetWarningHandler(0);
	TIFF *tif = TIFFOpen(path, "r");
	if (tif == 0) {
		printf(" open TIFF file %s failed\n", path);
		return false;
	}
	TIFFSetDirectory(tif, slice_index);

	uint32 row;
	for (row = 0; row <BLOCK_SIZE; row++)
	{
		//需要注意，这里内存偏移必须写为(row-ys)*width
		TIFFReadScanline(tif, (slice_in + int(row*BLOCK_SIZE)), row);
	}
	TIFFClose(tif);


	return true;



}
//x_focus y_focus为原始像素位置




void TDataset::write8bitImage(const char *path, unsigned char* buffer, uint32 width, uint32 height)
{
	TIFF *tif = TIFFOpen(path, "w");
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1); //每个像素点应有几个通道
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8); //每个通道的位宽，这里为8位图片
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_LEFTTOP);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK); //设定图片显示时，灰度值越小的像素越接近黑色
																	//	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, width));
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 1);

	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);




	unsigned char* curRow = new unsigned char[width];
	for (uint32 row = 0; row < height; row++)
	{
		memcpy(curRow, (buffer + row*width), width);    // check the index here, and figure out why not using h*linebytes
		if (TIFFWriteScanline(tif, curRow, row, 0) < 0)
			break;
	}
	delete[]curRow;
	TIFFClose(tif);
}

bool TDataset::load3Dtiff(QString path)
{
	

//	VolumeReader *voumereader = new VolumeReader(path);  

//	connect(voumereader, SIGNAL(emit_finish_read(ROI *)), this, SLOT(update_ROI(ROI *)));
//	connect(voumereader, SIGNAL(emit_finish_read2(unsigned char *)), this, SLOT(update_pROI(unsigned char *)), Qt::QueuedConnection);
//	voumereader->start();   //线程开始

	
	return true;
}

/*非线程读取三维tiff*/
/*
bool TDataset::load3Dtiff(const char *path)
{
	if(ROIdata)
		delete[] ROIdata;
	TIFFSetWarningHandler(0);
	TIFF *tif = TIFFOpen(path, "r");
	if (tif == 0) {
		printf(" open TIFF file %s failed\n", path);
		return false;
	}
	
	//uint16 photo=1, channels=1, bits=8;
	//TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photo); // PHOTOMETRIC_MINISWHITE=0, PHOTOMETRIC_MINISBLACK=1
	//TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &channels);
	//TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
	//if (photo > PHOTOMETRIC_MINISBLACK || channels != 1 || bits != 8) {
//	printf("[Volume::Read] TIFF image has an unsupported bits, channels or photometric\n");
//	TIFFClose(tif);
//	return false;
//	}
	
	size_t width = 0, height = 0;
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
	size_t depth = TIFFNumberOfDirectories(tif);
	uint8 *buffer = new uint8[width*height*depth];
	uint32 *slice = new uint32[width*height];
	sz0 = width;
	sz1 = height;
	sz2 = depth;
	for (uint16 i = 0; i<depth; ++i) {
		// assert bits, channels, photometric & width, height
		TIFFReadRGBAImageOriented(tif, width, height, slice, ORIENTATION_TOPLEFT);
		for (uint32 j = 0; j<width*height; j++)
		{
			buffer[i*width*height + j] = TIFFGetR(slice[j]);
		}
		TIFFReadDirectory(tif);
	}
	delete[] slice;
	TIFFClose(tif);
	ROIdata = buffer;
	if (ROIdata != NULL)
	{
		return true;
	}
	return false;

}

*/


void TDataset::update_ROI(ROI * roi)
{
	sz0 = roi->xROI;
	sz1 = roi->yROI;
	sz2 = roi->zROI;
	ROIdata = roi->pROI;
	qDebug("  updata file OK ");
	std::cout << sz0;
	roi_list.append(roi);

	emit emit_ask_actor(roi);
}


void TDataset::update_mostd(QStringList  list)
{

	//QStringList t = list;
	id = list.at(0).toInt();
	img_type = list.at(1).toInt();
	sz0 = list.at(2).toInt();
	sz1 = list.at(3).toInt();
	sz2 = list.at(4).toInt();

	int sizeoflist = list.size();


	rez_x = list.at(sizeoflist - 4).toDouble();
	rez_y = list.at(sizeoflist - 3).toDouble();
	rez_z = list.at(sizeoflist-2).toDouble();
	level_size = list.at(5).toInt();

		
}


QList<QStringList> TDataset::get_block_path_all_bit(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level)
{
	QList<QStringList> list_of_block_path;

	int x_index_b = xbegin;
	int x_index_e = xend;
	int y_index_b = ybegin;
	int y_index_e = yend;
	int z_index_b = zbegin;
	int z_index_e = zend;

	int x_temp_b;
	int x_temp_e;
	int y_temp_b;
	int y_temp_e;
	int z_temp_b;
	int z_temp_e;

	int z_shift;
	int y_shift;
	int x_shift;


	for (int i = 1; i<level; i++)
	{
		x_index_b /= 2;
		x_index_e /= 2;
		y_index_b /= 2;
		y_index_e /= 2;
		z_index_b /= 2;
		z_index_e /= 2;
	}
	int x_block_b = x_index_b / int(BLOCK_SIZE);
	int x_block_e = x_index_e / int(BLOCK_SIZE);
	int y_block_b = y_index_b / int(BLOCK_SIZE);
	int y_block_e = y_index_e / int(BLOCK_SIZE);
	int z_block_b = z_index_b / int(DEPTH_SIZE);
	int z_block_e = z_index_e / int(DEPTH_SIZE);

	QString str;
	int * temp_temp_be;
	char temppp[6];
	str.clear();

	int index_of_xy;
	for (int index_bit = 8; index_bit > 0; index_bit--)
	{
		QStringList list_of_temp;
		for (int i = y_block_b; i <= y_block_e; i++)
		{
			for (int j = z_block_b; j <= z_block_e; j++)
			{
				for (int k = x_block_b; k <= x_block_e; k++)
				{
					str = URL_path;
					str.append("/level");
					str.append(QString::number(level, 10));

					str.append("_data/z");
					str.append(QString::number(j, 10));

					str.append("/y");
					str.append(QString::number(i, 10));

					str.append("/x");
					str.append(QString::number(k, 10));

					str.append("/");

					str.append(QString::number(index_bit, 10));
					str.append(".tif");
					list_of_temp.append(str);
				}
			}
		}

		list_of_block_path.append(list_of_temp);
	}
	return   list_of_block_path;
}


void TDataset::access_ROI(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level)
{
	
	//VolumeReader *voumereader = new VolumeReader();


}