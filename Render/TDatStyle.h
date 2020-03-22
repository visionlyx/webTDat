#ifndef TDATSTYLE_H
#define	TDATSTYLE_H
#include <vtkAutoInit.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCommand.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include "VTKmain.h"
class VTKmain;
class TDatInteractorStyleTrackballCamera : public vtkInteractorStyleTrackballCamera
{
public:
	static TDatInteractorStyleTrackballCamera* New();
	virtual ~TDatInteractorStyleTrackballCamera();
	VTKmain *vtkmian;

protected:
	TDatInteractorStyleTrackballCamera();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnMouseMove();


private:
	
};




#endif 