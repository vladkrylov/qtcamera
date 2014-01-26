#ifndef C3_H
#define C3_H

#include <QtWidgets>
#include <QtGui>
#include <QtGui/QPushButton>
#include <QLayout>

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>


class c3 : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(bool SaveDefault READ GetSaveDefaultStatus)

public:
    c3(QWidget *parent = 0);
    ~c3();
private:
    // Options
    bool SaveDefault;

    // Members
    QString saveFileName;
    QCamera* camera;
    QCameraViewfinder* viewfinder;
    QCameraImageCapture* imageCapture;

    // GUI
    QWidget* widget;
    QVBoxLayout* MainLayout;
    QMenu* mnFile;
        QAction* saveAction;
    QMenu* mnOptions;
        QAction* sdAction;
    QPushButton* closeButton;
    QPushButton* imageCaptureButton;

    // Methods
    bool GetSaveDefaultStatus();
    void SetCamera();
    void ConstructGUI();

private slots:
    void ImageCapture();
    void ChangeSaveDefaultStatus();
    void onBufferAvailable(int id, const QVideoFrame& pFrame);
};

#endif // C3_H
