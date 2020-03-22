#ifndef VOLUMER2ACTOR_H
#define	VOLUMER2ACTOR_H
#include <QThread>
#include <iostream>
#include <tiffio.h>
#include "./TDatypes/TDataset.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
//#include <vtkDICOMImageReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>

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
#include "./TDatypes/TDataset.h"
#include "./TDatypes/ROI.h"
#include <vector>
#include <vtkSmartVolumeMapper.h>
#include <vtkAppendPolyData.h>
#include <vtkCubeSource.h>
#include "vtkSmartPointer.h"
#include <memory>
#include <vtkOutlineFilter.h>

class Volume2actor :public QThread
{
	Q_OBJECT
private:
	vtkVolume* volume;
	vtkActor *outline;
	ROI * roi2actor;
	vtkSmartPointer<vtkTransform> transformForVolume;
	vtkActor *boundingBoxActor;
public:
	//most_SWC * result_soma_swc;//分割完成后的swc
	Volume2actor(ROI * roi)
	{
		
		roi2actor = roi;
	}
	~Volume2actor(){  }
	void ReadMemoryImage();
	void SetVolumeRenderData(vtkImageImport*);
	void RemoveAllActor();

protected:
	void run();//开始分割

signals:
	void emit_finish_actor(vtkVolume*);
	void emit_finish_actor2(vtkActor*); //分割完成后进行的操作，将结果加入GUI
	//	void emit_finish_read2(unsigned char *pROI);//分割完成后进行的操作，将结果加入GUI的的一些信号操作

};



#endif