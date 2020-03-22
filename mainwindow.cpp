#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	
    ui->setupUi(this);
	datamangerdock = new DataManagerDock(this);
	this->addDockWidget(Qt::LeftDockWidgetArea, datamangerdock, Qt::Vertical);
	this->setWindowTitle(tr("webTDat Viewer"));
	this->resize(1440, 900);
	vtkMain = new VTKmain(this);
	//vtkMain->setMinimumSize(1024, 768);
	
	horizintalLayout = new QHBoxLayout();
	horizintalLayout->addWidget(vtkMain);

//	ui->centralLayout->addWidget(vtkMain);

	frame = new QFrame(this);
	frame->setLayout(horizintalLayout);

//	frame->hide();

	ui->centralLayout->addWidget(frame);
	ui->centralWidget->setLayout(ui->centralLayout);
	//ui->horizontalLayoutWidget->(vtkMain);
	//setCentralWidget(ui->centeralWidget);

    connect(ui->action_open,SIGNAL(triggered()),this,SLOT(openTDataset()));


	connect(datamangerdock, SIGNAL(emit_access_roi(int, int, int, int, int, int, int,int)), this, SLOT(access_ROI(int, int, int, int, int, int, int,int)));
	connect(datamangerdock, SIGNAL(emit_fly_mode(int)), vtkMain, SLOT(fly_mode(int)));

	connect(datamangerdock, SIGNAL(emit_access_roi_fly(int )), this, SLOT(access_ROI_fly(int )));


	connect(datamangerdock, SIGNAL(emit_projection_mode()), vtkMain, SLOT(projetction_mode()));
	connect(datamangerdock, SIGNAL(emit_3D_mode()), vtkMain, SLOT(mode_3D()));
	connect(datamangerdock, SIGNAL(emit_slice_mode()), vtkMain, SLOT(slice_mode()));
	connect(datamangerdock, SIGNAL(emit_swich_plane(int)), vtkMain, SLOT(set_plane(int)));


	connect(datamangerdock, SIGNAL(emit_test()), vtkMain, SLOT(test()));


	connect(datamangerdock, SIGNAL(emit_update_brightness(int, int)), vtkMain, SLOT(Update_Brightness(int,int)));


	
	
		
	roi = new ROI();
	tdataset = new TDataset();
	//volumereader = new VolumeReader();

}


MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::openTDataset()
{
	connect(this, SIGNAL(emit_finish_read_mostd(QStringList)), tdataset, SLOT(update_mostd(QStringList)));
	connect(this, SIGNAL(emit_finish_read_mostd(QStringList)), datamangerdock, SLOT(update_mostd(QStringList)));






	


//	const QString URLSTR = "http://61.183.137.214:7070/TDat_test/TEST2.mostd";
	const QString URLSTR = "G:/data/TDat_test/TEST2.mostd";

	QUrl url(URLSTR);
	QNetworkAccessManager manager;
	QEventLoop loop;
	QTextCodec *codec;
	QNetworkReply *reply;
	QStringList list;

	reply = manager.get(QNetworkRequest(url));
	//请求结束并下载完成后，退出子事件循环    
	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	//开启子事件循环    
	loop.exec();

	QString codeContent = reply->readAll();

	//将获取到的网页源码写入文件
	//一定要注意编码问题，否则很容易出现乱码的
//	codec = QTextCodec::codecForHtml(codeContent.toAscii());
//	codeContent = codec->toUnicode(codeContent.toAscii());
	//out.setCodec(codec);
//	out << codeContent << endl;
//	file.close();
	list = codeContent.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

	std::cout << list.at(2).toInt() << std::endl;

	emit emit_finish_read_mostd(list);

	//qDebug() << codeContent;

//	QString s = QString::number(1, 10);
//	QString path = "G:/data/data/bit/bit_" + s + ".tif";

//	VolumeReader *voumereader = new VolumeReader(path);

//	connect(voumereader, SIGNAL(emit_finish_read(ROI *)), vtkMain, SLOT(ROIrendering(ROI *)));
	//	connect(voumereader, SIGNAL(emit_finish_read2(unsigned char *)), this, SLOT(update_pROI(unsigned char *)), Qt::QueuedConnection);
//	voumereader->start();   //线程开始



}
*/


void MainWindow::openTDataset()
{
	connect(this, SIGNAL(emit_finish_read_mostd(QStringList)), tdataset, SLOT(update_mostd(QStringList)));
	connect(this, SIGNAL(emit_finish_read_mostd(QStringList)), datamangerdock, SLOT(update_mostd(QStringList)));
	QStringList list;
	
	QFile file("G:/data/test_tiff_data_tdat/TEST2.mostd");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!file.atEnd())
		{
			QByteArray line = file.readLine();
			QString str(line);
			list.append(str);
		//	displayString << str;
		}
		file.close();

	}





	emit emit_finish_read_mostd(list);

	//qDebug() << codeContent;

	//	QString s = QString::number(1, 10);
	//	QString path = "G:/data/data/bit/bit_" + s + ".tif";

	//	VolumeReader *voumereader = new VolumeReader(path);

	//	connect(voumereader, SIGNAL(emit_finish_read(ROI *)), vtkMain, SLOT(ROIrendering(ROI *)));
	//	connect(voumereader, SIGNAL(emit_finish_read2(unsigned char *)), this, SLOT(update_pROI(unsigned char *)), Qt::QueuedConnection);
	//	voumereader->start();   //线程开始



}




void MainWindow::access_ROI(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level,int state)
{
	VolumeReader *voumereader = new VolumeReader();
	connect(voumereader, SIGNAL(emit_finish_read(ROI *)), vtkMain, SLOT(ROIrendering(ROI *)));
	connect(voumereader, SIGNAL(emit_finish_read(ROI *)), datamangerdock, SLOT(updata_ROI_info(ROI *)));





	double xres = tdataset->rez_x;
	double yres = tdataset->rez_y;
	double zres = tdataset->rez_z;
	voumereader->set_xyz_res(xres, yres, zres);
	voumereader->access_ROI(xbegin, xend, ybegin, yend, zbegin, zend, level, state);
	
	
	voumereader->start();

}

void MainWindow::access_ROI_fly(int half_bit_state)
{
	double * bonund2 = new double[6];
	bonund2 = vtkMain->boxWidget->GetRepresentation()->GetBounds();
	std::cout << "half bit" << half_bit_state << std::endl;
	std::cout << "update2" << bonund2[0] << " " << bonund2[1] << " " << bonund2[2] << " " << bonund2[3] << " " << bonund2[4] << " " << bonund2[5] << std::endl;

	VolumeReader *voumereader = new VolumeReader();
	connect(voumereader, SIGNAL(emit_finish_read(ROI *)), vtkMain, SLOT(ROIrendering(ROI *)));
	connect(voumereader, SIGNAL(emit_finish_read(ROI *)), datamangerdock, SLOT(updata_ROI_info(ROI *)));
	connect(this, SIGNAL(emit_update_dock_range(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level)), datamangerdock, SLOT(update_dock_range(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level)));
	double xres = tdataset->rez_x;
	double yres = tdataset->rez_y;
	double zres = tdataset->rez_z;
	int xyzsize[6];
	xyzsize[0] = int(bonund2[0]);
	xyzsize[1] = int(bonund2[1]);
	xyzsize[2] = int(bonund2[2]);
	xyzsize[3] = int(bonund2[3]);
	xyzsize[4] = int(bonund2[4]);
	xyzsize[5] = int(bonund2[5]);

	size_t  xs = bonund2[1] - bonund2[0] + 1;
	size_t  ys = bonund2[3] - bonund2[2] + 1;
	size_t  zs = bonund2[5] - bonund2[4] + 1;
	
	size_t memory_size = xs*ys*zs / 1024 / 1024;
	int level = 1;

	while (memory_size >= 10)
	{
		memory_size /= 8;
		level++;
	}



	voumereader->set_xyz_res(xres, yres, zres);
	voumereader->access_ROI(xyzsize[0], xyzsize[1], xyzsize[2], xyzsize[3], xyzsize[4], xyzsize[5], level, half_bit_state);
	voumereader->start();

}
