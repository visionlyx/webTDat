#include "datamanagerdock.h"
#include "ui_datamanagerdock.h"

DataManagerDock::DataManagerDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DataManagerDock)
{
    ui->setupUi(this);
	half_bit = 0;
	ui->bit_load_progress->setValue(0);

	

	connect(ui->access_button, SIGNAL(clicked()), this, SLOT(access_ROI()));
	connect(ui->access2_button, SIGNAL(clicked()), this, SLOT(access_ROI_fly()));

	connect(ui->fly_mode_check, SIGNAL(stateChanged(int)), this, SLOT(fly_mode(int)));
	connect(ui->half_bit_check, SIGNAL(stateChanged(int)), this, SLOT(half_bit_mode(int)));

	connect(ui->projection_radio, SIGNAL(clicked(bool)), this, SLOT(projection_2D(bool)));
	connect(ui->view3D_radio, SIGNAL(clicked(bool)), this, SLOT(mode_3D(bool)));
	connect(ui->slice_radio, SIGNAL(clicked(bool)), this, SLOT(slice_mode(bool)));

	connect(ui->save_button, SIGNAL(clicked(bool)), this, SLOT(test(bool))); 
	connect(ui->min_bright_slider, SIGNAL(valueChanged(int)), this, SLOT(update_bright_low(int)));
	connect(ui->max_bright_slider, SIGNAL(valueChanged(int)), this, SLOT(update_bright_high(int)));

	connect(ui->thickness_slider, SIGNAL(valueChanged(int)), this, SLOT(update_thikness(int)));
	//update_thikness



	//connect(ui->fly_mode_check, SIGNAL(stateChanged(int)), this, SLOT(fly_mode(int)));


	ui->access_button->setEnabled(false);
	ui->access2_button->hide();


	QButtonGroup *buttonGround = new QButtonGroup();    
	buttonGround->addButton(ui->xy_button);
	buttonGround->addButton(ui->xz_button);
	buttonGround->addButton(ui->yz_button);


	buttonGround->setExclusive(true);
	connect(buttonGround, SIGNAL(buttonClicked(int)), this, SLOT(swich_plane(int)));

	ui->min_bright->setText(QString::number(0, 10));
	ui->max_bright->setText(QString::number(255, 10));
	ui->min_bright_slider->setMaximum(255);
	ui->min_bright_slider->setMinimum(0);
	ui->max_bright_slider->setMaximum(255);
	ui->max_bright_slider->setMinimum(0);
	ui->max_bright_slider->setValue(255);

	ui->thickness->setText(QString::number(10, 10));



}

DataManagerDock::~DataManagerDock()
{
    delete ui;
}

void DataManagerDock::slidercontrol()
{
	
	
}

void DataManagerDock::update_mostd(QStringList list)
{
	int sz0 = list.at(2).toInt();
	int sz1 = list.at(3).toInt();
	int sz2 = list.at(4).toInt();
	int level_all = list.at(5).toInt();

	int sizeoflist = list.size();

	ui->TDat_size_label->setText(list.at(2) + " x" + list.at(3) + " x" + list.at(4) + " pix");
	ui->TDat_voxel_label->setText(list.at(sizeoflist - 4) + " x" + list.at(sizeoflist - 4) + " x" + list.at(sizeoflist - 4) + " um");
	ui->TDat_level_label->setText(list.at(5));
	
	ui->xmin->setRange(0, sz0 - 1);
	ui->ymin->setRange(0, sz1 - 1);
	ui->zmin->setRange(0, sz2 - 1);
	ui->xmax->setRange(0, sz0 - 1);
	ui->ymax->setRange(0, sz1 - 1);
	ui->zmax->setRange(0, sz2 - 1);

	ui->TDat_level->setRange(1, level_all);

	ui->access_button->setEnabled(true);
}


void DataManagerDock::access_ROI()
{
	int xmin_value = ui->xmin->value();
	int xmax_value = ui->xmax->value();
	int ymin_value = ui->ymin->value();
	int ymax_value = ui->ymax->value();
	int zmin_value = ui->zmin->value();
	int zmax_value = ui->zmax->value();
	int level_value = ui->TDat_level->value();
	emit_access_roi(xmin_value, xmax_value, ymin_value, ymax_value, zmin_value, zmax_value, level_value, half_bit);
}

void DataManagerDock::access_ROI_fly()
{
	emit_access_roi_fly(half_bit);
}


void DataManagerDock::projection_2D(bool a)
{
	std::cout << "projcetion mode" << std::endl;
	emit_projection_mode();
}

void DataManagerDock::mode_3D(bool a)
{
	emit_3D_mode();
}

void DataManagerDock::slice_mode(bool a)
{
	emit_slice_mode();
}


void DataManagerDock::test(bool a)
{
	emit_test();
}

void DataManagerDock::fly_mode(int state)
{
	int fly_state = state;
	emit_fly_mode(fly_state);
	if (state == Qt::Checked) // "选中"
	{
		ui->access_button->hide();
		ui->access2_button->setVisible(true);

		ui->xmin->setEnabled(false);
		ui->xmax->setEnabled(false);
		ui->ymin->setEnabled(false);
		ui->ymax->setEnabled(false);
		ui->zmin->setEnabled(false);
		ui->zmax->setEnabled(false);
		ui->TDat_level->setEnabled(false);



	}
	if (state == Qt::Unchecked) // "选中"
	{
		ui->access2_button->hide();
		ui->access_button->setVisible(true);
		ui->xmin->setEnabled(true);
		ui->xmax->setEnabled(true);
		ui->ymin->setEnabled(true);
		ui->ymax->setEnabled(true);
		ui->zmin->setEnabled(true);
		ui->zmax->setEnabled(true);
		ui->TDat_level->setEnabled(true);
	}
//	emit emit_fly_mode(state);
}
void DataManagerDock::half_bit_mode(int state)
{
	
	if (state == Qt::Checked) // "选中"
	{
		half_bit = 1;
	}
	if (state == Qt::Unchecked) // "选中"
	{
		half_bit = 0;
	}
}


void DataManagerDock::updata_ROI_info(ROI * roi)
{
	
	ui->ROI_size_lable->setText(QString::number(roi->xROI) + " x " + QString::number(roi->yROI) + " x " + QString::number(roi->zROI) + " pix");
	ui->ROI_offset_lable->setText("x: " + QString::number(roi->origin_x) + " y: " + QString::number(roi->origin_y) + " z: " + QString::number(roi->origin_z));
	ui->ROI_voxel_size_lable->setText(QString::number(roi->res_xROI, 'f', 3) + " x " + QString::number(roi->res_yROI, 'f', 3) + " x " + QString::number(roi->res_zROI, 'f', 3) + " um");
	ui->bit_load_progress->setMinimum(0);
	ui->bit_load_progress->setMaximum(roi->bit_all_read);
	ui->bit_load_progress->setValue(roi->progress);

	
}

void DataManagerDock::update_dock_range(int xbegin, int xend, int ybegin, int yend, int zbegin, int zend, int level)
{
	ui->xmin->setValue(xbegin);
	ui->xmax->setValue(xend);
	ui->ymin->setValue(ybegin);
	ui->ymax->setValue(yend);
	ui->zmin->setValue(zbegin);
	ui->zmax->setValue(zend);
	

	ui->TDat_level->setValue(level);

}

void DataManagerDock::swich_plane(int a)
{
	std::cout << a << std::endl;
	emit_swich_plane(a);
	//emit_swich_plane(a);
}


void DataManagerDock::update_bright_low(int low)
{
	int high = ui->max_bright_slider->value();
	std::cout<<"bright" << low << "  " << high << std::endl;
	emit_update_brightness(low, high);
}
void DataManagerDock::update_bright_high(int high)
{
	int low = ui->min_bright_slider->value();
	emit_update_brightness(low, high);
		std::cout << "bright" << low << "  " << high << std::endl;
}

void DataManagerDock::update_thikness(int num)
{
	emit_update_thikness(num);
}