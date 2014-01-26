#include "c3.h"

const QString defaultSaveFileName = "/home/vlad/LAL_2013/Qt/camera_3/1.jpg";

c3::c3(QWidget *parent)
    : QMainWindow(parent)
{
    SaveDefault = true;
    saveFileName = defaultSaveFileName;

    SetCamera();
    ConstructGUI();

    this->resize(600,500);
    this->move(400,100);

    connect(imageCapture,SIGNAL(imageAvailable(int,QVideoFrame)),this,SLOT(onBufferAvailable(int,QVideoFrame)));
}

c3::~c3()
{
    delete camera;
    delete viewfinder;
}


bool c3::GetSaveDefaultStatus()
{
    return SaveDefault;
}

void c3::ChangeSaveDefaultStatus()
{
    SaveDefault = !SaveDefault;
}

void c3::SetCamera()
{
    QByteArray cameraDevice = QCamera::availableDevices()[0];
    camera = new QCamera(cameraDevice);
    viewfinder = new QCameraViewfinder(this);
    imageCapture = new QCameraImageCapture(camera);
    imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);

    viewfinder->show();
    camera->setViewfinder(viewfinder);

    camera->setCaptureMode(QCamera::CaptureStillImage);
    camera->start();
}

void c3::ConstructGUI()
{
    widget = new QWidget(this);
    this->setCentralWidget(widget);

    MainLayout = new QVBoxLayout;
    MainLayout->addWidget(viewfinder);

    mnFile = new QMenu("&File");
    saveAction = new QAction("Save image",mnFile);
    mnFile->addAction(saveAction);
    menuBar()->addMenu(mnFile);

    mnOptions = new QMenu("&Options");
    sdAction = new QAction("Save to default location",mnOptions);
    sdAction->setCheckable(true);
    sdAction->setChecked(true);
    connect(sdAction,SIGNAL(changed()),this,SLOT(ChangeSaveDefaultStatus()));
    mnOptions->addAction(sdAction);
    menuBar()->addMenu(mnOptions);

    closeButton = new QPushButton(this);
    closeButton->setText(tr("Close"));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
    MainLayout->addWidget(closeButton);

    imageCaptureButton = new QPushButton(this);
    imageCaptureButton->setText(tr("Capture Image"));
    connect(imageCaptureButton,SIGNAL(clicked()),this,SLOT(ImageCapture()));
    MainLayout->addWidget(imageCaptureButton);

    widget->setLayout(MainLayout);
}


//void c3::ImageCapture()
//{
//    camera->searchAndLock();

//    if (SaveDefault == false) {
//        saveFileName = QFileDialog::getSaveFileName(this, "Save file");
//    } else {
//        saveFileName = defaultSaveFileName;
//    }

//    imageCapture->capture(saveFileName);
//    camera->unlock();
//}

void c3::ImageCapture()
{
    camera->searchAndLock();
    imageCapture->capture(); /* emited here imageAvailable(int, QVideoFrame) signal */
}

void c3::onBufferAvailable(int id, const QVideoFrame& pFrame)
{
    this->camera->unlock();
    this->camera->stop();

    QVideoFrame lvFrame = pFrame;
    if (!lvFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        return;
    }
    QImage lvImage;
    lvImage.loadFromData((const uchar*)lvFrame.bits(), lvFrame.mappedBytes(), (const char*)"JPEG");
    lvFrame.unmap();

/* here you can process lvImage before saving */
//    lvImage.invertPixels(QImage::InvertRgb);

    lvImage.save(defaultSaveFileName, "JPEG");
    this->camera->start();
}
