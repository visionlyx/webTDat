#include "TDatStyle.h"


TDatInteractorStyleTrackballCamera::TDatInteractorStyleTrackballCamera()
{

}

TDatInteractorStyleTrackballCamera *TDatInteractorStyleTrackballCamera::New()
{
	return new TDatInteractorStyleTrackballCamera();
}

TDatInteractorStyleTrackballCamera::~TDatInteractorStyleTrackballCamera()
{
}


void TDatInteractorStyleTrackballCamera::OnLeftButtonDown()
{
	if (vtkmian->mode_state == 1){
		std::cout << "2D mode , no style" << std::endl;
		vtkmian->pick_projection();
	}
	if (vtkmian->mode_state == 0)
	{
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}
	if (vtkmian->mode_state == 2)
	{
		std::cout << "slice mode , ++" << std::endl;

		vtkmian->pick_slice();

	
	}

}




void TDatInteractorStyleTrackballCamera::OnLeftButtonUp()
{

	vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}


void TDatInteractorStyleTrackballCamera::OnRightButtonDown()
{
	vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}

void TDatInteractorStyleTrackballCamera::OnRightButtonUp()
{
	vtkInteractorStyleTrackballCamera::OnRightButtonUp();
}

void TDatInteractorStyleTrackballCamera::OnMiddleButtonDown()
{
	vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
	
}

void TDatInteractorStyleTrackballCamera::OnMiddleButtonUp()
{
	vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
	
	
}

void TDatInteractorStyleTrackballCamera::OnMouseWheelForward()
{

	if (vtkmian->mode_state==1){
		std::cout << "projection mode , ++" << std::endl;
		vtkmian->update_projection_position(true);
	//	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	}
	if (vtkmian->mode_state == 0)
	{
		vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	}
	if (vtkmian->mode_state == 2)
	{
		std::cout << "slice mode , ++" << std::endl;
		vtkmian->update_slice_position(true);
	}

}

void TDatInteractorStyleTrackballCamera::OnMouseWheelBackward()
{
	if (vtkmian->mode_state == 1){
		std::cout << "projection mode , --" << std::endl;
		vtkmian->update_projection_position(false);
		//	vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
	}
	if (vtkmian->mode_state == 0)
	{
		vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
	}
	if (vtkmian->mode_state == 2)
	{
		std::cout << "slice mode , --" << std::endl;
		vtkmian->update_slice_position(false);
	}

}

void TDatInteractorStyleTrackballCamera::OnMouseMove()
{

	vtkInteractorStyleTrackballCamera::OnMouseMove();// if (!glbox->Is2DView())  
}



