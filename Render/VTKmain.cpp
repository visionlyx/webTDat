/**
base of vtk widget
VTKmain.cpp
Purpose: managr vtk widget
last Change: documented
@author behnam sabaghi
@version 1.0 10/7/1396
*/
#include "VTKmain.h"
#include <vtkAutoInit.h>  
#include <vtkImageShrink3D.h>
#include <vtkCubeSource.h>
#include <QMessageBox>
#include <vtkSmartVolumeMapper.h>
#include <vtkAppendPolyData.h>
#include <vtkTIFFReader.h>
#include "vtkSmartPointer.h"
#include <vtkAutoInit.h>
#include <vtkInteractorStyleUnicam.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingContextOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2)


#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

void printxyz(double x, double x1, double y, double y1, double z, double z1)
{
	std::cout << "out" << x << " " << x1 << " " << y << " " << y1 << " " << z << " " << z1 << std::endl;
}


void VTKmain::on_boxWidget_transformed(vtkObject *object, unsigned long event, void *clientdata, void *calldata)
{
	VTKmain *self = reinterpret_cast<VTKmain*>(clientdata);
	double * bonund2 = new double[6];
	bonund2 = self->boxWidget->GetRepresentation()->GetBounds();
	std::cout << "update" << bonund2[0] << " " << bonund2[1] << " " << bonund2[2] << " " << bonund2[3] << " " << bonund2[4] << " " << bonund2[5] << std::endl;

	if (bonund2[0] < 0)
	{
		bonund2[0] = 0;
		self->boxWidget->GetRepresentation()->PlaceWidget(bonund2);
	}
	if (bonund2[2] < 0)
	{
		bonund2[2] = 0;
		self->boxWidget->GetRepresentation()->PlaceWidget(bonund2);
	}
	if (bonund2[4] < 0)
	{
		bonund2[4] = 0;
		self->boxWidget->GetRepresentation()->PlaceWidget(bonund2);
	}


}


VTKmain::VTKmain(QWidget *parent) : QVTKWidget(parent)
{
	
	pRenderer = vtkRenderer::New();
	camera3d = vtkCamera::New();
	//	camera3d->SetViewUp(0, 1, 0);
	//camera3d->SetViewUp(0, 1, -1);
	//xy view
	camera3d->SetPosition(0,0,-1);
	camera3d->SetViewUp(0, -1, 0);
	//xz view
	//camera3d->SetPosition(0,-1,0);
	//camera3d->SetViewUp(0,0, 1);
	//yz view
	//camera3d->SetPosition(1, 0,0);
	//camera3d->SetViewUp(0, 0, 1);

	camera3d->ComputeViewPlaneNormal();
	camera2d = vtkCamera::New();
	//	camera2d->SetViewUp(0, 0,0);
	pRenderer->SetActiveCamera(camera3d);
	GetRenderWindow()->AddRenderer(pRenderer);
	pRenderer->GetActiveCamera()->ParallelProjectionOn();
	
	irenderer = GetRenderWindow()->GetInteractor();

	vtkSmartPointer<TDatInteractorStyleTrackballCamera> style =
		vtkSmartPointer<TDatInteractorStyleTrackballCamera>::New();
	//renderWindowInteractor->SetInteractorStyle(style);
	style->AutoAdjustCameraClippingRangeOn();
	style->vtkmian = this;
	irenderer->SetInteractorStyle(style);

	step = 0;
	current_plane = 0;

	window3d = GetRenderWindow();
	importer = vtkImageImport::New();
	/*
	char* fileName = "C:/Users/liyuxin/Desktop/test/test.tif";	
	vtkSmartPointer<vtkTIFFReader> reader = vtkSmartPointer<vtkTIFFReader>::New();
	reader->SetFileName(fileName);
	reader->SetDataSpacing(1.0, 1.0, 5.0); //分辨率
	reader->Update();
	int bound[6];
	reader->GetDataExtent(bound);
	double dataspace[3];
	reader->GetDataSpacing(dataspace);
	printxyz(bound[0], bound[1], bound[2], bound[3], bound[4], bound[5]);
	printxyz(dataspace[0], dataspace[1], dataspace[2], 0, 0, 0);
	vtkSmartVolumeMapper *hrVolmapper = vtkSmartVolumeMapper::New();
	hrVolmapper->SetBlendModeToMaximumIntensity();
	vtkColorTransferFunction *colorTransfer = vtkColorTransferFunction::New();
	colorTransfer->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	colorTransfer->AddRGBPoint(255.0, 1.0, 1.0, 1.0);
	vtkPiecewiseFunction *opacFunction = vtkPiecewiseFunction::New();
	opacFunction->AddPoint(0.0, 0.99);
	opacFunction->AddPoint(255.0, 1.0);
	opacFunction->ClampingOff();//
	vtkVolumeProperty *volproperty = vtkVolumeProperty::New();
	volproperty->SetColor(colorTransfer);
	volproperty->SetScalarOpacity(opacFunction);
	volproperty->SetScalarOpacityUnitDistance(0.2);
	volproperty->SetInterpolationType(2);
	hrVolmapper->SetInputData(reader->GetOutput());
	if (!transformForVolume) transformForVolume = vtkSmartPointer<vtkTransform>::New();
	transformForVolume->Identity();
	transformForVolume->Translate(0, 0, 0);
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(hrVolmapper);
	volume->SetProperty(volproperty);
	volume->SetUserTransform(transformForVolume);
	pRenderer->AddActor(volume);
	oldVolumeMapper = volume;
	camera2d->ParallelProjectionOn();
	double *c = oldVolumeMapper->GetCenter();
	camera2d->SetFocalPoint(c[0], c[1], c[2]);
	camera2d->SetPosition(c[0], c[1], bound[4] + 100);
	camera2d->SetClippingRange(100-1, 100);
	camera2d->ComputeViewPlaneNormal();
	printxyz(c[0], c[1], c[2], 0, 0, 0);
	pRenderer->SetActiveCamera(camera2d);

	*/


	


	planeWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
	planeWidget->SetInteractor(irenderer);
	//	planeWidget[i]->SetPicker(picker);
	planeWidget->SetDefaultRenderer(pRenderer);
	planeWidget->Off();//将widget显示出来

	
	



	boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();

	boxWidget->SetInteractor(irenderer);
	boxWidget->GetRepresentation()->SetPlaceFactor(1); // Default is 0.5
	boxWidget->RotationEnabledOff();
	boxWidget->ScalingEnabledOff();
	// boxWidget->TranslationEnabledOff();
	boxWidget->MoveFacesEnabledOn();

	double * bonund = new double[6];
	bonund[0] = 0;
	bonund[1] = 200;
	bonund[2] = 0;
	bonund[3] = 200;
	bonund[4] = 0;
	bonund[5] = 200;

	boxWidget->GetRepresentation()->PlaceWidget(bonund);

	boxCallBack = vtkCallbackCommand::New();
	this->boxCallBack->SetCallback(VTKmain::on_boxWidget_transformed);
	this->boxCallBack->SetClientData(this);

	boxWidget->AddObserver(vtkCommand::InteractionEvent, this->boxCallBack);

	boxWidget->Off();




	//-- Set OrientationMarker like amira
	axes =	vtkSmartPointer<vtkAxesActor>::New();

	marker = vtkOrientationMarkerWidget::New();
	marker->SetOutlineColor(1, 1, 1);
	marker->SetOrientationMarker(axes);
	//widget->SetInteractor( renderWindowInteractor );
	marker->SetInteractor(GetRenderWindow()->GetInteractor());
	//widget->SetDefaultRenderer(pRenderer);
	marker->SetViewport(0.0, 0.0, 0.1, 0.1);
	marker->SetEnabled(1);
	marker->InteractiveOn();
	pRenderer->SetBackground(0, 0, 0);//设置背景颜色


	oldVolumeMapper = NULL;
//	marker = NULL;
	boundingBoxActor = NULL;
	pRenderer->ResetCamera();
	first_actor = true;
	is2D_or_3D = false;
	is_slice = false;
	mode_state = 0;

}

VTKmain::~VTKmain() 
{
	pRenderer->Delete();
	pRenderer = NULL;
	if (marker) marker->Delete();
	if (oldVolumeMapper) oldVolumeMapper->Delete();
	if (boundingBoxActor) boundingBoxActor->Delete();
}

void VTKmain::SetInputVolume(TDataset *tdataset, int)
{
	;
}




void VTKmain::ROIrendering(ROI *roi)
{
	current_roi = roi;
	volume2actor = new Volume2actor(roi);
	connect(volume2actor, SIGNAL(emit_finish_actor(vtkVolume*)), this, SLOT(update_actor(vtkVolume*)));
	connect(volume2actor, SIGNAL(emit_finish_actor2(vtkActor*)), this, SLOT(update_actor2(vtkActor*)));
	volume2actor->start();

}


void VTKmain::ReadMemoryImage(TDataset *tdataset)
{
	int newX = tdataset->sz0;
	int newY = tdataset->sz1;
	int newZ = tdataset->sz2;
	unsigned char * tex = tdataset->ROIdata;
	vtkImageImport *importer;
	importer = vtkImageImport::New();
	importer->SetWholeExtent(0, newX - 1, 0, newY - 1, 0, newZ - 1);
	importer->SetDataExtentToWholeExtent();
	importer->SetDataScalarTypeToUnsignedChar();
	importer->SetNumberOfScalarComponents(1);
	//importer->SetImportVoidPointer(tex);
	importer->CopyImportVoidPointer(tex, newX * newY * newZ);
	importer->SetDataSpacing(1, 1, 1); //分辨率
	importer->Update();
	SetVolumeRenderData(importer);
//	delete[] tex;
	pRenderer->SetBackground(0, 0, 0);//设置背景颜色
	//pRenderer->GetRenderWindow()->Render();
	pRenderer->ResetCamera();
}

void VTKmain::SetVolumeRenderData(vtkImageImport *importer)
{
	vtkSmartVolumeMapper *hrVolmapper = vtkSmartVolumeMapper::New();
	//hrVolmapper->SetRequestedRenderMode(vtkSmartVolumeMapper::GPURenderMode);
	hrVolmapper->SetBlendModeToMaximumIntensity();
	//vtkVolumeRayCastMIPFunction*mipRaycastFunction =
	//        vtkVolumeRayCastMIPFunction::New();
	//lrVolmapper->SetVolumeRayCastFunction(mipRaycastFunction);
	//hrVolmapper->SetVolumeRayCastFunction(mipRaycastFunction);

	/* Part4 color trans function, only gray*/
	vtkColorTransferFunction *colorTransfer = vtkColorTransferFunction::New();
	colorTransfer->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
	//colorTransfer->AddRGBPoint(5.0,0.3,0.3,0.3);
	//colorTransfer->AddRGBPoint(25.0,0.5,0.5,0.5);
	//colorTransfer->AddRGBPoint(125.0,0.7,0.7,0.7);
	colorTransfer->AddRGBPoint(255.0, 1.0, 1.0, 1.0);
	/* Part5 opac Function*/
	vtkPiecewiseFunction *opacFunction = vtkPiecewiseFunction::New();
	opacFunction->AddPoint(0.0, 0.99);
	opacFunction->AddPoint(255.0, 1.0);
	opacFunction->ClampingOff();//
	/* Part6 set properties*/
	vtkVolumeProperty *volproperty = vtkVolumeProperty::New();
	volproperty->SetColor(colorTransfer);
	volproperty->SetScalarOpacity(opacFunction);

	volproperty->SetScalarOpacityUnitDistance(0.2);

	//volproperty->ShadeOn();
	//volproperty->SetAmbient(0.1);
	//volproperty->SetDiffuse(0.9);
	//volproperty->SetSpecular(0.2);
	//volproperty->SetSpecularPower(10);
	//volproperty->SetInterpolationTypeToNearest();
	volproperty->SetInterpolationType(2);
	/* Part7 set high resolution mapper and low resolution mapper*/
	hrVolmapper->SetInputData(importer->GetOutput());
	//lrVolmapper->SetInput(shrinkFilter->GetOutput());

	if (!transformForVolume) transformForVolume = vtkSmartPointer<vtkTransform>::New();
	transformForVolume->Identity();
	//transformForVolume->Translate(xOffset_, yOffset_ , zOffset_ );
	transformForVolume->Translate(0, 0, 0);

	/* Part 8*/
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(hrVolmapper);
	volume->SetProperty(volproperty);
	volume->SetUserTransform(transformForVolume);
	/* Part9 if at low frame per second, use high resolution.*/

	//vtkLODProp3D *atlasLOD = vtkLODProp3D::New();
	//atlasLOD->AddLOD(hrVolmapper, volproperty, 0.0);
	//atlasLOD->AddLOD(lrVolmapper, volproperty, 0.0);
	//-- set renderer
	//pRenderer->RemoveAllProps();
	RemoveAllActor();
	//pRenderer->AddActor(atlasLOD);
	//oldVolumeMapper = atlasLOD;
	pRenderer->AddActor(volume);
	oldVolumeMapper = volume;

	vtkSmartPointer<vtkCubeSource> cubeData = vtkSmartPointer<vtkCubeSource>::New();
	int bound[6];
	importer->GetDataExtent(bound);
	double dataspace[3];
	importer->GetDataSpacing(dataspace);
	cubeData->SetBounds(0, bound[1] * dataspace[0],
		0, bound[3] * dataspace[1],
		0, bound[5] * dataspace[2]);
	vtkSmartPointer<vtkPolyDataMapper> cubeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cubeMapper->SetInputData(cubeData->GetOutput());
	boundingBoxActor = vtkActor::New();
	boundingBoxActor->SetMapper(cubeMapper);
	boundingBoxActor->GetProperty()->SetRepresentationToWireframe();
	boundingBoxActor->GetProperty()->SetColor(1.0, 1.0, 0.0);
	boundingBoxActor->GetProperty()->SetLineWidth(1.0);
	pRenderer->AddActor(boundingBoxActor);

	// last settings
//	pRenderer->ResetCamera();
}

void VTKmain::RemoveAllActor()
{
	if (oldVolumeMapper){
		pRenderer->RemoveVolume(oldVolumeMapper);
		oldVolumeMapper->Delete();
		oldVolumeMapper = NULL;
	}
	if (boundingBoxActor){
		pRenderer->RemoveActor(boundingBoxActor);
		boundingBoxActor->Delete();
		boundingBoxActor = NULL;
	}
}


void VTKmain::RemoveSliceActor()
{
	if (old_actor_slice){
		slice_renderer->RemoveVolume(old_actor_slice);
		old_actor_slice->Delete();
		old_actor_slice = NULL;
	}
	if (oldsliceTextActor){
		slice_renderer->RemoveActor(oldsliceTextActor);
		oldsliceTextActor->Delete();
		oldsliceTextActor = NULL;
	}
}



void VTKmain::update_actor(vtkVolume* volume)
{
	qDebug(" 11111~~~~~~~~~~~");
		RemoveAllActor();
		pRenderer->SetBackground(0, 0, 0);//设置背景颜色
		//pRenderer->GetRenderWindow()->Render();
		oldVolumeMapper = volume;
		
		pRenderer->AddActor(volume);
	//	pRenderer->UpdateLightsGeometryToFollowCamera();
		if (first_actor)
		{
			pRenderer->ResetCamera();
			first_actor = false;
		}

			
			
		pRenderer->GetRenderWindow()->Render();
		double *bound = new double[6];
		bound = volume->GetBounds();
		std::cout << "volume:   "<<bound[0] << " " << " " << bound[1] << " " << bound[2] << " " << bound[3] << " " << bound[4] << " " << bound[5] << " " << std::endl;


		//qDebug(bound[]);
	//	pRenderer
	//	pRenderer->GetRenderWindow()->Render();
		//qDebug(" 22222！~~~~~~~~~~~");
}

void VTKmain::update_actor2(vtkActor* outline)
{
	pRenderer->AddActor(outline);
	double *bound = new double[6];
	bound = outline->GetBounds();
	boundingBoxActor = outline;

		std::cout << "outline:   " << bound[0] << " " << " " << bound[1] << " " << bound[2] << " " << bound[3] << " " << bound[4] << " " << bound[5] << " " << std::endl;

	pRenderer->GetRenderWindow()->Render();
}


void VTKmain::fly_mode(int state)
{
	if (state == Qt::Checked) // "选中"
	{
		boxWidget->On();
		pRenderer->GetRenderWindow()->Render();
		irenderer->Render();
	}
	if (state == Qt::Unchecked) // "选中"
	{
		boxWidget->Off();
		pRenderer->GetRenderWindow()->Render();
		irenderer->Render();
	}
	
	


}


void VTKmain::projetction_mode()
{
	mode_state = 1;
	is2D_or_3D = true;
	marker->Off();
	if (is_slice)
	{
		is_slice = false;
		GetRenderWindow()->RemoveRenderer(slice_renderer);
		GetRenderWindow()->AddRenderer(pRenderer);
		planeWidget->Off();
		pRenderer->ResetCamera();
	}

	//marker->Off();
	pRenderer->RemoveActor(boundingBoxActor);
	set_plane(-2);

}

void VTKmain::mode_3D()
{
	mode_state = 0;
	is2D_or_3D = false;
	
//	marker->On();
	if (is_slice)
	{
		is_slice = false;
		GetRenderWindow()->RemoveRenderer(slice_renderer);
		GetRenderWindow()->AddRenderer(pRenderer);
		planeWidget->Off();
		pRenderer->ResetCamera();
	}
	pRenderer->SetActiveCamera(camera3d);
//	marker->SetInteractor(GetRenderWindow()->GetInteractor());
	marker->On();
	update();
}


void VTKmain::test()
{
//	importer->Update();
	if (current_roi->pROI)
	{
		importer->CopyImportVoidPointer(current_roi->pROI, current_roi->xROI * current_roi->yROI * current_roi->zROI);
		importer->Update();
	}
	
}


void VTKmain::slice_mode()
{
	mode_state = 2;
	is_slice = true;
	marker->Off();

	first_slice = true;

	slice_renderer = vtkRenderer::New();
	old_slice_renderer = slice_renderer;

	GetRenderWindow()->AddRenderer(slice_renderer);
	
		
	old_actor_slice = NULL;
	oldsliceTextActor = NULL;

	
	importer->SetWholeExtent(0, current_roi->xROI - 1, 0, current_roi->yROI - 1, 0, current_roi->zROI - 1);
	importer->SetDataExtentToWholeExtent();
	importer->SetDataScalarTypeToUnsignedChar();
	importer->SetNumberOfScalarComponents(1);
	//importer->SetImportVoidPointer(tex);
	importer->CopyImportVoidPointer(current_roi->pROI, current_roi->xROI * current_roi->yROI * current_roi->zROI);
	importer->SetDataSpacing(current_roi->res_xROI, current_roi->res_yROI, current_roi->res_zROI); //分辨率
	importer->SetDataOrigin(current_roi->origin_x, current_roi->origin_y, current_roi->origin_z);
	importer->Update();

	slice_plane(-2);
}

void VTKmain::update_slice_position(bool a)
{
	int imageDims[3];
	imageDims[0] = current_roi->xROI;
	imageDims[1] = current_roi->yROI;
	imageDims[2] = current_roi->zROI;
	int old_plane = planeWidget->GetSliceIndex();
	int update_plane=old_plane;
	std::string msg;

	switch (current_plane)
	{
	case -2:
		if (a)
		{
			if (update_plane <  imageDims[2])
			{
				update_plane++;
				planeWidget->SetSliceIndex(update_plane);//设置显示的切面位置
			}
		}
		else
		{
			if (update_plane >  0)
			{
				update_plane--;
				planeWidget->SetSliceIndex(update_plane);//设置显示的切面位置
			}
		}


		msg = StatusMessage3D::Format("XY:", update_plane, imageDims[2]);

		sliceTextMapper->SetInput(msg.c_str());

		break;
	case -3:

		if (a)
		{
			if (update_plane <  imageDims[1])
			{
				update_plane++;
				planeWidget->SetSliceIndex(update_plane);//设置显示的切面位置
			}
		}
		else
		{
			if (update_plane >  0)
			{
				update_plane--;
				planeWidget->SetSliceIndex(update_plane);//设置显示的切面位置
			}
		}


		msg = StatusMessage3D::Format("XZ:", update_plane, imageDims[1]);

		sliceTextMapper->SetInput(msg.c_str());


		break;
	case -4:
		if (a)
		{
			if (update_plane <  imageDims[0])
			{
				update_plane++;
				planeWidget->SetSliceIndex(update_plane);//设置显示的切面位置
			}
		}
		else
		{
			if (update_plane >  0)
			{
				update_plane--;
				planeWidget->SetSliceIndex(update_plane);//设置显示的切面位置
			}
		}


		msg = StatusMessage3D::Format("YZ:", update_plane, imageDims[0]);

		sliceTextMapper->SetInput(msg.c_str());


		break;
	default:
		break;
	}


	camera_slice->Modified();
	update();




	;
}



void VTKmain::update_projection_position(bool a)
{
	
	double *bound = new double[6];
	bound = oldVolumeMapper->GetBounds();
	double *oldclip = new double[2];
	oldclip = camera2d->GetClippingRange();
	int clip_begin = oldclip[0];
	int clip_end = oldclip[1];

	switch (current_plane)
	{
	case -2:
		if (a)
		{
			if (clip_end <  1000 + bound[5] - bound[4])
			{
				clip_begin += 1 * current_roi->res_zROI;
				clip_end += 1 * current_roi->res_zROI;
				camera2d->SetClippingRange(clip_begin, clip_end);
			}


			std::cout << "range:   " << clip_begin << " " << " " << clip_end << std::endl;

		}
		else
		{
			if (clip_begin >  1000)
			{
				clip_begin -= 1 * current_roi->res_zROI;
				clip_end -= 1 * current_roi->res_zROI;
				camera2d->SetClippingRange(clip_begin, clip_end);
			}
			std::cout << "range:   " << clip_begin << " " << " " << clip_end << std::endl;
		}
		break;
	case -3:

		if (a)
		{
			if (clip_end <  1000 + bound[3] - bound[2])
			{
				clip_begin += 1 * current_roi->res_yROI;
				clip_end += 1 * current_roi->res_yROI;
				camera2d->SetClippingRange(clip_begin, clip_end);
			}


			std::cout << "y range:   " << clip_begin << " " << " " << clip_end << std::endl;

		}
		else
		{
			if (clip_begin >  1000)
			{
				clip_begin -= 1 * current_roi->res_yROI;
				clip_end -= 1 * current_roi->res_yROI;
				camera2d->SetClippingRange(clip_begin, clip_end);
			}
			std::cout << "y range:   " << clip_begin << " " << " " << clip_end << std::endl;
		}


		break;
	case -4:
		if (a)
		{
			if (clip_end <  1000 + bound[1] - bound[0])
			{
				clip_begin += 1 * current_roi->res_xROI;
				clip_end += 1 * current_roi->res_xROI;
				camera2d->SetClippingRange(clip_begin, clip_end);
			}


			std::cout << "x range:   " << clip_begin << " " << " " << clip_end << std::endl;

		}
		else
		{
			if (clip_begin >  1000)
			{
				clip_begin -= 1 * current_roi->res_xROI;
				clip_end -= 1 * current_roi->res_xROI;
				camera2d->SetClippingRange(clip_begin, clip_end);
			}
			std::cout << "x range:   " << clip_begin << " " << " " << clip_end << std::endl;
		}


		break;
	default:
		break;
	}


	





	camera2d->Modified();
	update();



}

void VTKmain::slice_plane(int a)
{


	mode_state = 2;
	is_slice = true;
	marker->Off();

	first_slice = true;
	
	slice_renderer = vtkRenderer::New();


	GetRenderWindow()->AddRenderer(slice_renderer);
	GetRenderWindow()->RemoveRenderer(old_slice_renderer);
	old_slice_renderer->Delete();
	old_slice_renderer = slice_renderer;


	current_plane = a;

	camera_slice = vtkCamera::New();
	camera_slice->ParallelProjectionOn();

//	camera_slice->ComputeViewPlaneNormal();

	//slice_renderer->SetActiveCamera(camera_slice);
	
	int imageDims[3];
	imageDims[0] = current_roi->xROI;
	imageDims[1] = current_roi->yROI;
	imageDims[2] = current_roi->zROI;
	// int

	std::string msg;


	planeWidget->SetInteractor(irenderer);
	//	planeWidget[i]->SetPicker(picker);
	planeWidget->RestrictPlaneToVolumeOn();
	//planeWidget[i]->SetLeftButtonAutoModifier(1);//修改左键
	planeWidget->SetLeftButtonAction(1);//修改左键
	planeWidget->SetInputConnection(importer->GetOutputPort());
	planeWidget->TextureInterpolateOff();
	//	planeWidget[i]->SetInput

	//xz view
	
	//yz view

	//xy view
//	camera3d->SetPosition(0, 0, -1);
	//camera3d->SetViewUp(0, -1, 0);
	//xz view
	//camera3d->SetPosition(0,-1,0);
	//camera3d->SetViewUp(0,0, 1);
	//yz view
	//camera3d->SetPosition(1, 0,0);
	//camera3d->SetViewUp(0, 0, 1);



	switch (a)
	{
	case -2:
	
		planeWidget->SetPlaneOrientation(2);
		planeWidget->SetSliceIndex(imageDims[2] / 2);//设置显示的切面位置
		msg = StatusMessage3D::Format("XY:", imageDims[2] / 2, imageDims[2]);
//		camera_slice->SetPosition(current_roi->xROI*current_roi->res_xROI, current_roi->yROI*current_roi->res_yROI, imageDims[2] / 2);
		camera_slice->SetPosition(0, 0, -1);
		camera_slice->SetViewUp(0, -1, 0);
		
		break;


	case -3:

		planeWidget->SetPlaneOrientation(1);
		planeWidget->SetSliceIndex(imageDims[1] / 2);//设置显示的切面位置
		msg = StatusMessage3D::Format("XZ:", imageDims[1] / 2, imageDims[1]);
	//	camera_slice->SetPosition(current_roi->xROI*current_roi->res_xROI, imageDims[1] / 2, current_roi->zROI*current_roi->res_zROI);
		camera_slice->SetPosition(0, 0, -1);
		camera_slice->SetViewUp(1, 0, 0);
	//	camera_slice->SetPosition(0,-1,0);
	//	camera_slice->SetViewUp(0,0, 1);
		break;
	case -4:
	
		planeWidget->SetPlaneOrientation(0);
		planeWidget->SetSliceIndex(imageDims[0] / 2);//设置显示的切面位置
		msg = StatusMessage3D::Format("YZ:", imageDims[0] / 2, imageDims[0]);
		//camera_slice->SetPosition(imageDims[0] / 2,current_roi->yROI*current_roi->res_yROI,  current_roi->zROI*current_roi->res_zROI);
		camera_slice->SetPosition(0, 0, 1);
		camera_slice->SetViewUp(0, 1, 0);
	//	camera_slice->SetPosition(1, 0,0);
	//	camera_slice->SetViewUp(0, 0, 1);
		break;

	default:
		break;
	}

	planeWidget->On();//将widget显示出来
	planeWidget->InteractionOn();//Enable/disable mouse interaction so the widget remains on display


	sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
	sliceTextProp->SetFontFamilyToCourier();
	sliceTextProp->SetFontSize(18);//修改字体的大小
	sliceTextProp->SetVerticalJustificationToBottom();
	sliceTextProp->SetJustificationToLeft();

	sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
	sliceTextMapper->SetInput(msg.c_str());
	sliceTextMapper->SetTextProperty(sliceTextProp);

	sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
	sliceTextActor->SetMapper(sliceTextMapper);
		sliceTextActor->SetPosition(100, 10);
	//slice_renderer->GetRenderWindow()->gets
	actor_slice = vtkImageActor::New();
	actor_slice->SetInputData(planeWidget->GetResliceOutput());
	old_actor_slice = actor_slice;
	slice_renderer->AddActor(actor_slice);
	slice_renderer->AddActor2D(sliceTextActor);
	oldsliceTextActor = sliceTextActor;
	double *center = actor_slice->GetCenter();


	camera_slice->ComputeViewPlaneNormal();
	
	slice_renderer->SetActiveCamera(camera_slice);
	//if (first_slice)
	{
		slice_renderer->ResetCamera();
		first_slice = false;

	}
	
	
	//marker->On();

//	slice_renderer->GetRenderWindow()->Render();


	update();


}
void VTKmain::projection_plane(int a)
{
	double *bound = new double[6];
	bound = oldVolumeMapper->GetBounds();
	camera2d = vtkCamera::New();
	camera2d->ParallelProjectionOn();
	double *center = oldVolumeMapper->GetCenter();
	printxyz(center[0], center[1], center[2], 0, 0, 0);
	printxyz(bound[0], bound[1], bound[2], bound[3], bound[4], bound[5]);
	current_plane = a;

	switch (a)
	{
	case -2:
		camera2d->SetFocalPoint(center[0], center[1], 1000 + 5 * current_roi->res_zROI);
		camera2d->SetPosition(center[0], center[1], bound[4] - 1000);
		camera2d->SetClippingRange(1000, 1000 + 10 * current_roi->res_zROI);
		camera2d->SetParallelScale((bound[1] - bound[0]) / 2);
		camera2d->ComputeViewPlaneNormal();
		camera2d->SetViewUp(0, -1, 0);

		break;


	case -3:
		camera2d->SetFocalPoint(center[0], 1000 + 5 * current_roi->res_yROI, center[2]);
		camera2d->SetPosition(center[0], bound[2] - 1000, center[2]);
		camera2d->SetClippingRange(1000, 1000 + 10 * current_roi->res_zROI);
		camera2d->SetParallelScale((bound[1] - bound[0]) / 2);
		camera2d->ComputeViewPlaneNormal();
		camera2d->SetViewUp(0, 0, 1);

		break;
	case -4:
		camera2d->SetFocalPoint(1000 + 5 * current_roi->res_xROI, center[1], center[2]);
		camera2d->SetPosition(bound[1] + 1000, center[1], center[2]);

		camera2d->SetClippingRange(1000, 1000 + 10 * current_roi->res_zROI);
		camera2d->SetParallelScale((bound[3] - bound[2]) / 2);
		camera2d->ComputeViewPlaneNormal();
		camera2d->SetViewUp(0, 0, 1);


		break;

	default:
		break;
	}
	pRenderer->SetActiveCamera(camera2d);
	update();
}
void VTKmain::view3D_plane(int a)
{

}


void VTKmain::pick_projection()
{
	int X, Y;
	X = GetRenderWindow()->GetInteractor()->GetEventPosition()[0];
	Y = GetRenderWindow()->GetInteractor()->GetEventPosition()[1];


	pRenderer->SetDisplayPoint(X, Y, 0);
	pRenderer->DisplayToWorld();
	double worldPoint[4];
	pRenderer->GetWorldPoint(worldPoint);
	

	std::cout << "Screen Coordinate: " << X << "," << Y << "\n";
	std::cout << "Screen Coordinate: " << worldPoint[0] << "," << worldPoint[1] << "," << worldPoint[2] << "," << worldPoint[3] << "\n";
}


void VTKmain::pick_slice()
{
	int X, Y;
	X = GetRenderWindow()->GetInteractor()->GetEventPosition()[0];
	Y = GetRenderWindow()->GetInteractor()->GetEventPosition()[1];

	slice_renderer->SetDisplayPoint(X, Y, 0);
	slice_renderer->DisplayToWorld();
	double worldPoint[4];
	slice_renderer->GetWorldPoint(worldPoint);

	std::cout << "Screen Coordinate: " << X << "," << Y << "\n";
	std::cout << "Screen Coordinate: " << worldPoint[0] << "," << worldPoint[1] << "," << worldPoint[2] << "," << worldPoint[3] << "\n";
}


void VTKmain::set_plane(int a)
{
	if (mode_state == 0)
	{
		view3D_plane(a);
	}
	if (mode_state ==1)
	{
		projection_plane(a);
	}
	if (mode_state == 2)
	{
		slice_plane(a);
	}

}


void VTKmain::Update_Brightness(int low, int high)
{
	//2015-8-13
	if (oldVolumeMapper){
		vtkVolumeProperty *oldProperty;
		oldProperty = oldVolumeMapper->GetProperty();
		VTK_CREATE(vtkPiecewiseFunction, newOpacFunction);
		VTK_CREATE(vtkColorTransferFunction, colorTransfer);

		newOpacFunction->AddPoint(low, 0.99);
		newOpacFunction->AddPoint(high, 1.0);
		newOpacFunction->ClampingOn();
		colorTransfer->AddRGBPoint(low, 0.0, 0.0, 0.0);
		colorTransfer->AddRGBPoint(high, 1.0, 1.0, 1.0);

		oldProperty->SetScalarOpacity(newOpacFunction);
		oldProperty->SetColor(colorTransfer);
	}
	this->update();
}


