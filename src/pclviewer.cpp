#include "pclviewer.h"
#include "ui_pclviewer.h"
#include <QMessageBox>
#include <QToolBar>

using namespace pcl;

PCLViewer::PCLViewer(QWidget *parent) :
QMainWindow(parent)
//ui(new Ui::PCLViewer)
{
        ui.setupUi(this);
        this->setWindowTitle("PCL viewer");
        this->setGeometry(250, 400, 960, 680);
        // Setup the cloud pointer
        cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
        cloud2.reset(new pcl::PointCloud<pcl::PointXYZ>);
        rad.reset(new pcl::PointCloud<pcl::PointXYZ>);
        // Set up the QVTK window
        viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
        viewer->addPointCloud(cloud, "cloud");

        ui.qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
        viewer->setupInteractor(ui.qvtkWidget->GetInteractor(), ui.qvtkWidget->GetRenderWindow());
        ui.qvtkWidget->update();



        /***************************Qt connect��һ��д��***************************************/
        //void onOpen();
        //QAction *Openfile1 = new QAction(tr("&Openfile"), this);
        //QToolBar *toolbar = addToolBar(tr("&File"));
        //Openfile1->setIcon(QIcon("./Resources/icon_file.png"));
        //toolbar->addAction(Openfile1);
        ////connect(ui->Openfile, SIGNAL(clicked()), this, SLOT(onOpen()));
        //connect(Openfile1, &QAction::triggered, this, &PCLViewer::onOpen);

        //QAction *filtered1 = new QAction(tr("&filtered"), this);
        //filtered1->setIcon(QIcon("./Resources/icon_filter.png"));
        //toolbar->addAction(filtered1);
        //
        //void pointcloud_filter();
        ////connect(ui->filtered, SIGNAL(clicked()), this, SLOT(pointcloud_filter()));
        //
        //  connect(filtered1, &QAction::triggered, this, &PCLViewer::pointcloud_filter);

        /***************************Qt connect �ڶ���д��************************************/
        connect(ui.Openfile, SIGNAL(clicked()), this, SLOT(onOpen()));

        connect(ui.filtered, SIGNAL(clicked()), this, SLOT(pointcloud_filter()));

        //connect(ui.boss_height, SIGNAL(clicked()), this, SLOT(get_boss_size()));
}


void
PCLViewer::onOpen()
{

        QString fileName = QFileDialog::getOpenFileName(this,
                QString("Open PointCloud"), ".",
                QString("Open PCD files(*.pcd)"));

        if (!fileName.isEmpty())
        {
                std::string file_name = fileName.toStdString();
                pcl::io::loadPCDFile(file_name, *cloud);
                viewer->updatePointCloud(cloud, "cloud");
                ui.qvtkWidget->update();
        }

}

void
PCLViewer::pointcloud_filter()
{
        //QMessageBox::warning(ui->centralwidget, "aaa", "aaa");
        //pcl::PointCloud<pcl::PointXYZ>::Ptr  cloud2(new pcl::PointCloud<pcl::PointXYZ>);

                //pcl::io::loadPCDFile("3d_data.pcd", *cloud);
                pcl::RadiusOutlierRemoval<PointXYZ> radiusor;
                radiusor.setInputCloud(cloud);
                radiusor.setRadiusSearch(0.1);
                radiusor.setMinNeighborsInRadius(45);
                radiusor.filter(*cloud2);

                std::cout << cloud->size() << endl;
                std::cout << cloud2->size() << endl;
                //viewer->removeAllPointClouds();

                viewer->resetCamera();
                //viewer->addPointCloud(rad, "cloud");
                viewer->updatePointCloud(cloud2, "cloud");
                ui.qvtkWidget->update();

        /*pcl::VoxelGrid<PointT> sor;
        sor.setInputCloud(cloud);
        sor.setLeafSize(0.5f, 0.5f, 0.5f);
        sor.filter(*cloud2);*/

        //pcl::io::savePCDFileASCII("20190108.pcd", *cloud2);
        //std::cout << "save pcd complete" << endl;

}

//void PCLViewer::get_boss_size()
//{
//
//
//
//}

PCLViewer::~PCLViewer()
{
        delete &ui;
}
