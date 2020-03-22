#include "Volmereader.h"


bool VolumeReader::write_buffer(const char filename[], unsigned char * buffer, int x_size, int y_size, int z_size)
{


	TIFFSetWarningHandler(0);
	TIFF* out = TIFFOpen(filename, "w");
	if (out)
	{
		size_t nWidth = x_size;
		size_t nHeight = y_size;
		size_t nTotalFrame = z_size;
		//size_t wh = nWidth * nHeight;
		//   uint8 *temp = new uint8[x_size * y_size];
		int N_size = 0;
		size_t nCur = 0;
		//UChar den = (sizeof(T) == 1) ? 1 : 4;
		do{
			TIFFSetField(out, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
			TIFFSetField(out, TIFFTAG_PAGENUMBER, nTotalFrame);
			TIFFSetField(out, TIFFTAG_IMAGEWIDTH, (uint32)nWidth);
			TIFFSetField(out, TIFFTAG_IMAGELENGTH, (uint32)nHeight);
			//TIFFSetField(out, TIFFTAG_RESOLUTIONUNIT, 2);
			/*TIFFSetField(out, TIFFTAG_YRESOLUTION, 196.0f);
			TIFFSetField(out, TIFFTAG_XRESOLUTION, 204.0f);*/
			TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
			// 设置一个样本所占内存的大小.
			TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
			TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 1);
			TIFFSetField(out, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
			TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
			TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
			TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, nHeight);


			TIFFWriteEncodedStrip(out, 0, &buffer[N_size], nWidth * nHeight);
			//TIFFWriteRawStrip (out, 0, temp, nWidth * nHeight);
			++nCur;
			N_size = N_size + nWidth*nHeight;
		} while (TIFFWriteDirectory(out) && nCur < nTotalFrame);
		//  delete[] temp;
		TIFFClose(out);
		return true;
	}
	else
	{
		printf("cant create tiff!\n");
		return false;
	}


}


void VolumeReader::run()
{
			
	uint8 **buffer_temp = NULL;
	buffer_temp = new uint8 *[path.at(0).size()];

	//uint8 * buffer_bit = new uint8[int(BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE / 8)];

	int width_ROI = x_index_e - x_index_b + 1;
	int height_ROI = y_index_e - y_index_b + 1;
	int depth_ROI = z_index_e - z_index_b + 1;

	uint8 *buffer = new uint8[width_ROI*height_ROI*depth_ROI];  //申请总的内存开销


	for (int j = 0; j < width_ROI*height_ROI*depth_ROI; j++)
	{
		buffer[j] = 0;
	}
	

	std::cout << path.size() << std::endl;
	std::cout << path.at(0).size() << std::endl;
	
	for (int i = 0; i < path.at(0).size(); i++)
	{

		buffer_temp[i] = new uint8[int(BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE)];
		for (int j = 0; j < BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE; j++)
		{
			buffer_temp[i][j] = 0;
		}
	}
	int bit_depth = 0;
	if (half_bit_state)
	{
		bit_depth = path.size() / 2;
	}
	else
	{
		bit_depth = path.size();
	}
	for (int i = 0; i <bit_depth; i++)
	{


		#pragma omp parallel num_threads(4)
		for (int index = 0; index < path.at(0).size(); index++)
		{
			uint8 * buffer_bit = new uint8[int(BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE / 8)];
			cv::VideoCapture capture;
			cv::Mat image;
			std::string path_str = path.at(i).at(index).toStdString();
			capture.open(path_str);
			if (!capture.isOpened())
			{
				//cerr << "Failed to open the image sequence!\n" << endl;
				std::cout << "failed" << std::endl;


			}
			capture >> image;
			
			for (int j = 0; j < BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE / 8; j++)

			{

				buffer_bit[j] = image.data[j*3];
			}

	



			for (size_t j = 0; j < BLOCK_SIZE*BLOCK_SIZE*DEPTH_SIZE/8; ++j)
			{
				buffer_temp[index][j * 8] |= ((buffer_bit[j] & 128) >> i);
				buffer_temp[index][j * 8 + 1] |= (((buffer_bit[j] & 64) << 1) >> i);
				buffer_temp[index][j * 8 + 2] |= (((buffer_bit[j] & 32) << 2) >> i);
				buffer_temp[index][j * 8 + 3] |= (((buffer_bit[j] & 16) << 3) >> i);
				buffer_temp[index][j * 8 + 4] |= (((buffer_bit[j] & 8) << 4) >> i);
				buffer_temp[index][j * 8 + 5] |= (((buffer_bit[j] & 4) << 5) >> i);
				buffer_temp[index][j * 8 + 6] |= (((buffer_bit[j] & 2) << 6) >> i);
				buffer_temp[index][j * 8 + 7] |= (((buffer_bit[j] & 1) << 7) >> i);
			}

		//	const char* path2 = "G:/data/7test.tif";
		//	write_buffer(path2, buffer_temp[index],128,128,128);

		}

		


			int x_temp_b;
			int x_temp_e;
			int y_temp_b;
			int y_temp_e;
			int z_temp_b;
			int z_temp_e;

			
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


		//	const char* path2 = "G:/data/buffertest.tif";
	//		write_buffer(path2, buffer, roi_x, roi_y, roi_z);

			roi->pROI = buffer;
			roi->xROI = roi_x;
			roi->yROI = roi_y;
			roi->zROI = roi_z;
			roi->res_xROI = res_x;
			roi->res_yROI = res_y;
			roi->res_zROI = res_z;
			roi->bit_all_read = bit_depth;
			roi->progress = i+1;
			



			emit_finish_read(roi);//发出信号，计算完成
		

	
	}

	
}


void VolumeReader::set_xyz_res(double rex, double rey, double rez)
{
	res_x = rex;
	res_y = rey;
	res_z = rez;
}

void VolumeReader::access_ROI(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int levelt,int ishalfbit)
{



	x_index_b = xbegin;
	y_index_b = ybegin;
	z_index_b = zbegin;
	x_index_e = xend;
	y_index_e = yend;
	z_index_e = zend;
//	URL = URL_path;
	level = levelt;

	get_block_path_all_bit();


	roi->origin_x = xbegin;
	roi->origin_y = ybegin;
	roi->origin_z = zbegin;
	half_bit_state = ishalfbit;

	for (int i = 1; i<level; i++)
	{
		res_x *= 2;
		res_y *= 2;
		res_z *= 2;
	}

}

void VolumeReader::get_block_path_all_bit()
{
	int x_temp_b;
	int x_temp_e;
	int y_temp_b;
	int y_temp_e;
	int z_temp_b;
	int z_temp_e;

	for (int i = 1; i<level; i++)
	{
		x_index_b /= 2;
		x_index_e /= 2;
		y_index_b /= 2;
		y_index_e /= 2;
		z_index_b /= 2;
		z_index_e /= 2;
	}
	x_block_b = x_index_b / int(BLOCK_SIZE);
	x_block_e = x_index_e / int(BLOCK_SIZE);
	y_block_b = y_index_b / int(BLOCK_SIZE);
	y_block_e = y_index_e / int(BLOCK_SIZE);
	z_block_b = z_index_b / int(DEPTH_SIZE);
	z_block_e = z_index_e / int(DEPTH_SIZE);


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

	roi_x = x_index_e - x_index_b + 1;
	roi_y = y_index_e - y_index_b + 1;
	roi_z = z_index_e - z_index_b + 1;

	QString str;
	int * temp_temp_be;
	char temppp[6];
	str.clear();

	int index_of_xy;
	for (int index_bit = 8; index_bit > 0; index_bit--)
	{
		QStringList list_of_temp;
		for (int i = z_block_b; i <= z_block_e; i++)
		{
			for (int j = y_block_b; j <= y_block_e; j++)
			{
				for (int k = x_block_b; k <= x_block_e; k++)
				{
					str = URL;
					str.append("/level");
					str.append(QString::number(level, 10));

					str.append("_data/z");
					str.append(QString::number(i, 10));

					str.append("/y");
					str.append(QString::number(j, 10));

					str.append("/x");
					str.append(QString::number(k, 10));

					str.append("/");

					str.append(QString::number(index_bit, 10));
					str.append(".tif");
					list_of_temp.append(str);
				}
			}
		}

		path.append(list_of_temp);
	}
}