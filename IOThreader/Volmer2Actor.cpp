#include "Volmer2Actor.h"

void Volume2actor::run()
{
	qDebug(" begin render  ");
//	ReadMemoryImage();


	int newX = roi2actor->xROI;
	int newY = roi2actor->yROI;
	int newZ = roi2actor->zROI;
	unsigned char * tex = roi2actor->pROI;
	vtkImageImport *importer;
	importer = vtkImageImport::New();
	importer->SetWholeExtent(0, newX - 1, 0, newY - 1, 0, newZ - 1);

	importer->SetDataExtentToWholeExtent();
	importer->SetDataScalarTypeToUnsignedChar();
	importer->SetNumberOfScalarComponents(1);
	//importer->SetImportVoidPointer(tex);
	importer->CopyImportVoidPointer(tex, newX * newY * newZ);
	importer->SetDataSpacing(roi2actor->res_xROI, roi2actor->res_yROI, roi2actor->res_zROI); //分辨率
	importer->SetDataOrigin(roi2actor->origin_x, roi2actor->origin_y, roi2actor->origin_z);
	importer->Update();
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

	//if (!transformForVolume) transformForVolume = vtkSmartPointer<vtkTransform>::New();
//	transformForVolume->Identity();

//	transformForVolume->Translate(600, 888, 8888);

	/* Part 8*/
	volume = vtkVolume::New();
	volume->SetMapper(hrVolmapper);
	volume->SetProperty(volproperty);
//	volume->SetUserTransform(transformForVolume);



	vtkOutlineFilter *outlineData = vtkOutlineFilter::New();
	//outlineData->set
	//pRenderer->AddActor(volume);
	//volume->get
	outlineData->SetInputConnection(importer->GetOutputPort());
	vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());
	outline = vtkActor::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(255, 0, 0);

	
	emit_finish_actor(volume);
	emit_finish_actor2(outline);

}

void Volume2actor::ReadMemoryImage()
{
	int newX = roi2actor->xROI;
	int newY = roi2actor->yROI;
	int newZ = roi2actor->zROI;
	unsigned char * tex = roi2actor->pROI;
	vtkImageImport *importer;
	importer = vtkImageImport::New();
	importer->SetWholeExtent(0, newX - 1, 0, newY - 1, 0, newZ - 1);
	importer->SetDataExtentToWholeExtent();
	importer->SetDataScalarTypeToUnsignedChar();
	importer->SetNumberOfScalarComponents(1);
	//importer->SetImportVoidPointer(tex);
	importer->CopyImportVoidPointer(tex, newX * newY * newZ);
	importer->SetDataSpacing(roi2actor->res_xROI, roi2actor->res_yROI, roi2actor->res_zROI); //分辨率
	importer->Update();
	SetVolumeRenderData(importer);
	//	delete[] tex;
////	pRenderer->SetBackground(0, 0, 0);//设置背景颜色
	//pRenderer->GetRenderWindow()->Render();
////	pRenderer->ResetCamera();
}

void Volume2actor::SetVolumeRenderData(vtkImageImport *importer)
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
	volume = vtkVolume::New();
	volume->SetMapper(hrVolmapper);
	volume->SetProperty(volproperty);
	volume->SetUserTransform(transformForVolume);
	/* Part9 if at low frame per second, use high resolution.*/

	//vtkLODProp3D *atlasLOD = vtkLODProp3D::New();
	//atlasLOD->AddLOD(hrVolmapper, volproperty, 0.0);
	//atlasLOD->AddLOD(lrVolmapper, volproperty, 0.0);
	//-- set renderer
	//pRenderer->RemoveAllProps();
////	RemoveAllActor();
	//pRenderer->AddActor(atlasLOD);
	//oldVolumeMapper = atlasLOD;
////	pRenderer->AddActor(volume);
	//oldVolumeMapper = volume;

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
////	pRenderer->AddActor(boundingBoxActor);

	// last settings
	//	pRenderer->ResetCamera();
}

