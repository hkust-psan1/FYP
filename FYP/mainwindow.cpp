#include "mainwindow.h"
#include "setting.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
    qRegisterMetaType<Vec3>("Vec3");
	ui.setupUi(this);
	QSize size = ui.pixmapLabel->size();
    renderManager = new RenderManager(size.width() - 2, size.height() - 2, 4);

    lastPos = NULL;
    imageRendered = false;
    
    parser = new Parser;
}

MainWindow::~MainWindow()
{

}

void MainWindow::load_scene()
{
	QString filename = QFileDialog::getOpenFileName(this, "Load Scene",  QDir::currentPath(), "Wavefront Object File(*.obj);;All files(*.*)");
	if (!filename.isNull()) 
	{
        renderManager->setScene(parser->parseFile(filename.toStdString()));
		// renderManager->setScene(parser->parseScene(filename.toStdString().c_str()));
	}
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About"), tr("HKUST 2014 FYP"));
}

void MainWindow::render()
{
	if (!renderManager->sceneLoaded()) {
        return;
	}
    
	Setting::HBV = ui.HBVcheckBox->isChecked();

	rendererThread = new QThread;

	renderManager->moveToThread(rendererThread);
	connect(rendererThread, SIGNAL(started()), renderManager, SLOT(render()));
	connect(renderManager, SIGNAL(updateScreen()), this, SLOT(updateScreen()));
	// connect(rendererThread, SIGNAL(terminate()), this, SLOT(threadTerminated()));
	
    frameReady = false;
    imageRendered = true;
    
	rendererThread->start();
}

void MainWindow::threadTerminated() {
    std::cout << "thread terminated" << std::endl;
    render();
}

void MainWindow::updateScreen() {
	pixmap = QPixmap::fromImage(*renderManager->getFrontBuffer());
	ui.pixmapLabel->setPixmap(pixmap);
    
    frameReady = true;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (imageRendered) {
        renderManager->clearTasks();
        if (lastPos == NULL) {
            lastPos = new QPoint(event->pos());
        }
        
        Camera* cam = renderManager->getCamera();
        
        int xDiff = lastPos->x() - event->x();
        int yDiff = event->y() - lastPos->y();
        
        Vec3 eye = cam->getEyePos();
        Vec3 ctr = cam->getCenterPos();
        
        float newX = (eye.x - ctr.x) * cos(xDiff / 30.0) - (eye.z - ctr.z) * sin(xDiff / 30.0);
        float newY = (eye.z - ctr.z) * cos(xDiff / 30.0) + (eye.x - ctr.x) * sin(xDiff / 30.0);
        
        cam->setEyePos(Vec3(newX, cam->getEyePos().y, newY) + cam->getCenterPos());
        cam->update();
        
        renderManager->stopRendering();
        renderManager->render();
        
        lastPos = new QPoint(event->pos());
        lastX = event->x();
        lastY = event->y();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (imageRendered) {
        if (event->button() != Qt::RightButton)
            return;
        // tracer->traceRay(event->x() - 85, event->y() - 35);

    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (imageRendered) {
        lastPos = NULL;
    }
}

void MainWindow::wheelEvent(QWheelEvent *event) {
    if (frameReady) {
        renderManager->clearTasks();
        
        Camera* cam = renderManager->getCamera();
        Vec3 view = cam->getEyePos() - cam->getCenterPos();
        
        cam->setEyePos(cam->getCenterPos() + view * (1 - event->delta() * 0.001));
        cam->update();
        
        renderManager->stopRendering();
        renderManager->render();
    }
}
