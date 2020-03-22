#ifndef DATAMANAGERDOCK_H
#define DATAMANAGERDOCK_H

#include <QDockWidget>
#include "../IOThreader/Volmereader.h"
namespace Ui {
class DataManagerDock;
}

class DataManagerDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit DataManagerDock(QWidget *parent = 0);
    ~DataManagerDock();

private:
    Ui::DataManagerDock *ui;
	void slidercontrol();
	int half_bit;

public slots:
	void update_mostd(QStringList list);
	void access_ROI();
	void fly_mode(int state);
	void half_bit_mode(int state);
	void access_ROI_fly();
	void updata_ROI_info(ROI *);
	void update_dock_range(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level);
	void projection_2D(bool a);
	void mode_3D(bool a);
	void swich_plane(int a);
	void slice_mode(bool a );
	void test(bool a );
	void update_bright_low(int low);
	void update_bright_high(int high);
	void update_thikness(int num);




signals:
	void emit_access_roi(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level, int half_bit_state);//分割完成后进行的操作，将结果加入GUI
	void emit_fly_mode(int fly_state);//分割完成后进行的操作，将结果加入GUI
	void emit_access_roi_fly(int half_bit_state);
	void emit_projection_mode();
	void emit_3D_mode();
	void emit_swich_plane(int plane);
	void emit_slice_mode();
	void emit_test();
	void emit_update_brightness(int low, int high);
	void emit_update_thikness(int num);


};



#endif // DATAMANAGERDOCK_H
