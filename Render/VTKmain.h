/**
base of vtk widget
VTKmain.h
Purpose: managr vtk widget
last Change: documented
@author behnam sabaghi
@version 1.0 10/7/1396
*/

#pragma once
#include "qobject.h"
#include <QVTKWidget.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
//#include <vtkDICOMImageReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include "TDatStyle.h"
//#include <vtkImageData.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkSmartPointer.h>
//#include <vtkInteractorStyleTrackballCamera.h>
//#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolumeRayCastCompositeFunction.h>
//#include <vtkLODProp3D.h>
#include <vtkProperty.h>

#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include <vtkVolumeRayCastMIPFunction.h>
#include <vtkImageImport.h>
#include <vtkTransform.h>
#include <vtkCamera.h>
#include <vtkTexture.h>
#include <vtkCellArray.h>
#include <QMouseEvent>
#include <vtkResliceImageViewer.h>
#include <vtkImagePlaneWidget.h>
#include <vtkCellPicker.h>
#include "./TDatypes/TDataset.h"
#include "./IOThreader/Volmer2Actor.h"
#include <vector>
#include <memory>
#include <vtkCallbackCommand.h>
#include <vtkBoxWidget2.h>
#include <vtkImageActor.h>
#include <vtkTextProperty.h>
#include <vtkActor2D.h>
#include <vtkTextMapper.h>
#include <vtkBoxRepresentation.h>
#include <vtkInteractorStyleTrackballCamera.h>

class VTKmain : public QVTKWidget {
   
	Q_OBJECT
public:
		 
	VTKmain(QWidget *parent = nullptr);
	~VTKmain();
	void SetInputVolume(TDataset *tdataset, int);
	void ReadMemoryImage(TDataset *tdataset);
	void SetVolumeRenderData(vtkImageImport*);
	int step;
	void RemoveAllActor();
	void RemoveSliceActor();
	// vtk renderer
	ROI *current_roi;
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkRenderer> slice_renderer;
	vtkSmartPointer<vtkRenderer> old_slice_renderer;
	vtkSmartPointer<vtkRenderWindowInteractor> irenderer;
	vtkSmartPointer<vtkCamera> camera2d;
	vtkSmartPointer<vtkCamera> camera3d;
	vtkSmartPointer<vtkCamera> camera_slice;
	

	vtkSmartPointer<vtkPolyDataMapper> mapper;
	vtkSmartPointer<vtkActor> actor;
	bool first_actor;
	bool is2D_or_3D;
	bool is_slice;
	bool first_slice;
	int mode_state;

	int current_plane;
	vtkRenderer* pRenderer;
	Volume2actor * volume2actor;
	vtkRenderWindow * window3d;
	vtkImageImport *importer;


	vtkImageActor* actor_slice;
	vtkImageActor* old_actor_slice;


	vtkSmartPointer<vtkAxesActor> axes;
	vtkOrientationMarkerWidget *marker;
	vtkVolume *oldVolumeMapper;
	vtkActor *boundingBoxActor;
	vtkSmartPointer<vtkTransform> transformForVolume;

//	vtkSmartPointer<vtkResliceImageViewer> riw[3];
//	vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];
	vtkSmartPointer<vtkImagePlaneWidget> planeWidget;

	vtkSmartPointer<vtkTextProperty> sliceTextProp;
	vtkSmartPointer<vtkTextMapper> sliceTextMapper;




	vtkSmartPointer<vtkActor2D> sliceTextActor;
	vtkSmartPointer<vtkActor2D> oldsliceTextActor;


	vtkCallbackCommand *boxCallBack;
	vtkSmartPointer<vtkBoxWidget2> boxWidget;
	static void on_boxWidget_transformed(vtkObject *object, unsigned long event, void *clientdata, void *calldata);//响应配准结果窗口re

	void update_projection_position(bool a);
	void update_slice_position(bool a);

	void slice_plane(int a);
	void projection_plane(int a);
	void view3D_plane(int a);

	void pick_projection();
	void pick_slice();
 
	public slots:
	void ROIrendering(ROI *roi);
	void  update_actor(vtkVolume* volume);
	void update_actor2(vtkActor* outline);
	void fly_mode(int state);
	void projetction_mode();
	void mode_3D();
	void set_plane(int a);
	void slice_mode();
	void test();
	void Update_Brightness(int low, int high);
};

class StatusMessage3D
{
public:
	static std::string Format(string str, int slice, int maxSlice)
	{
		std::stringstream tmp;
		tmp << "Slice Number " << str << slice << "/" << maxSlice<<"       ";
		return tmp.str();
	}
};
