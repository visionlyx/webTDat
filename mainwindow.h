#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QWidget>
#include <QtNetwork>   
#include "./Render/VTKmain.h"
#include "./TDatypes/TDataset.h"
#include "./TDatypes/ROI.h"
#include"./dock/datamanagerdock.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
     void openTDataset();
	 void access_ROI(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level,int state);
	 void access_ROI_fly(int half_bit_state);
	

private:
    Ui::MainWindow *ui;
	QHBoxLayout *horizintalLayout;
	QFrame *frame;
	VTKmain *vtkMain;
	TDataset *tdataset;
	ROI *roi;
	DataManagerDock * datamangerdock;
	VolumeReader* volumereader;


signals:
	void emit_finish_read_mostd(QStringList list);//分割完成后进行的操作，将结果加入GUI
};

#endif // MAINWINDOW_H
