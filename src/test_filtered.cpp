#include "pclviewer.h"
#include "ui_pclviewer.h"
#include <QMessageBox>
#include <QToolBar>
#include <QFont>

using namespace std;
using namespace pcl;
 static int num = 0;

 static float  a0 =0,b0=0, c0=0,d0 =0;
 struct Plane
 {
	 float a;
	 float b;
	 float c;
	 float d;
 };

 struct Plane  plane1;
 struct Plane plane2;

PCLViewer::PCLViewer(QWidget *parent) :
QMainWindow(parent)
//ui(new Ui::PCLViewer)
{
	ui.setupUi(this);
	this->setWindowTitle("PCL viewer");
	this->setGeometry(300, 250,1190, 725);

	QWidget *mainWidget (new QWidget);
	mainWidget->setObjectName("mainWidget");
	mainWidget->setStyleSheet("#mainWidget{background-image:url(:/pclviewer/Resources/1.png)}");

	// Setup the cloud pointer
	cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
	cloud2.reset(new pcl::PointCloud<pcl::PointXYZ>);
	cloud3.reset(new pcl::PointCloud<pcl::PointXYZ>);
	cloud_out.reset(new pcl::PointCloud<pcl::PointXYZ>);
	boss_plane.reset(new pcl::PointCloud<pcl::PointXYZ>);
	clicked_cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
	temp_cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
	// Set up the QVTK window 
	viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
	viewer->setBackgroundColor(0.0392,0.4, 0.5921); //RGB要归一化为(0,1)之间
	viewer->addPointCloud(cloud, "cloud");

	ui.qvtkWidget->SetRenderWindow(viewer->getRenderWindow());
	viewer->setupInteractor(ui.qvtkWidget->GetInteractor(), ui.qvtkWidget->GetRenderWindow());
	ui.qvtkWidget->update(); 

	viewer->registerAreaPickingCallback(&PCLViewer::Area_select_callback, *this);
	
	/***************************Qt connect第一种写法***************************************/
#if  0
	void onOpen();
	QAction *Openfile1 = new QAction(tr("&Openfile"), this);
	QToolBar *toolbar = addToolBar(tr("&File"));
	Openfile1->setIcon(QIcon("./Resources/icon_file.png"));
	toolbar->addAction(Openfile1);
	//connect(ui->Openfile, SIGNAL(clicked()), this, SLOT(onOpen()));
	connect(Openfile1, &QAction::triggered, this, &PCLViewer::onOpen);

	QAction *filtered1 = new QAction(tr("&filtered"), this);
	filtered1->setIcon(QIcon("./Resources/icon_filter.png")); 
	toolbar->addAction(filtered1);
	
	void pointcloud_filter();
	//connect(ui->filtered, SIGNAL(clicked()), this, SLOT(pointcloud_filter()));
	 
	  connect(filtered1, &QAction::triggered, this, &PCLViewer::pointcloud_filter);
  
#endif

	/***************************Qt connect 第二种写法************************************/
	connect(ui.Openfile, SIGNAL(clicked()), this, SLOT(onOpen()));
	
	connect(ui.filtered, SIGNAL(clicked()), this, SLOT(pointcloud_filter()));

	connect(ui.boss_height, SIGNAL(clicked()), this, SLOT(get_boss_size()));

	connect(ui.extract, SIGNAL(clicked()), this, SLOT(extract_baseplane()));
	
	connect(ui.toolButton, SIGNAL(clicked()), this, SLOT(fitPlane_tool()));
	connect(ui.boundry, SIGNAL(clicked()), this, SLOT(feature_boundry()));
	
	text = new QTextEdit(this);
	text->setGeometry(890, 390,260, 310);
	QFont sansFont("Helvetica [Cronyx]", 14);
	text->setFont(sansFont);
	text->setTextBackgroundColor(QColor(255,255,255));
	text->setTextColor(QColor(0, 0, 0));
	text->setFontPointSize(14);
	text->setText(QString("*****Output Information****"));
	text->scrollBarWidgets(Qt::AlignBottom);
	text->setReadOnly(true);
	//connect(ui.boss_height, SIGNAL(clicked()), this, SLOT(update_text()));
	//QString str = QString::number(, 'f', 6);
	//text->setText(str);

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
		viewer->resetCamera();
		
		QString num = QString::number(cloud->size());
		QString str = QString("Oringinal point_clouds : ") + num;
		text->append(str);
		cout << "press the 'X' and then use mouse to select area interested ...." << endl;
		ui.qvtkWidget->update();

	}

}

void
PCLViewer::pointcloud_filter()
{        
	
		pcl::RadiusOutlierRemoval<PointXYZ> radiusor;
		radiusor.setInputCloud(cloud);
		radiusor.setRadiusSearch(0.11);
		radiusor.setMinNeighborsInRadius(39);
		radiusor.filter(*cloud2);
		
		//QString str = QString("Oringin point_clouds :  1%").arg(cloud->size());

		QString num= QString::number(cloud2->size());
		QString str = QString("point_clouds Filtered  : ") + num;
			text->append(str);
		
		viewer->resetCamera();
		//viewer->addPointCloud(rad, "cloud");
		viewer->updatePointCloud(cloud2, "cloud");
		//viewer->registerAreaPickingCallback(&PCLViewer::Area_select_callback, *this);
		ui.qvtkWidget->update();

}

void
PCLViewer::get_boss_size()
{

	std::vector<int> inliers;
	pcl::SampleConsensusModelPlane<pcl::PointXYZ>::Ptr model_p(new pcl::SampleConsensusModelPlane<pcl::PointXYZ>(cloud2));
	pcl::RandomSampleConsensus<pcl::PointXYZ>ransac(model_p);
	ransac.setDistanceThreshold(0.01);
	ransac.computeModel();
	ransac.getInliers(inliers);

	Eigen::VectorXf coef;
	ransac.getModelCoefficients(coef);
	Eigen::Vector4f centroid;
	pcl::copyPointCloud<pcl::PointXYZ>(*cloud2, inliers, *cloud3);
	pcl::compute3DCentroid(*cloud3, centroid);
	Eigen::Vector3d normal(coef[0], coef[1], coef[2]);
	Eigen::Vector3d z(0, 0, 1);

	if (normal(2) < 0)
	{
		normal = -1 * normal;
	}
	normal = normal / normal.norm();
	Eigen::Vector3d n = z.cross(normal);
	n = n / n.norm();//单位法向量
	float angle = acos(z.dot(normal));
	Eigen::AngleAxisd rotate_vector(-angle, n);
	Eigen::Matrix3d rotate_matrix = Eigen::Matrix3d::Identity();
	rotate_matrix = rotate_vector.toRotationMatrix();
	Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
	transform(0, 0) = rotate_matrix(0, 0);
	transform(0, 1) = rotate_matrix(0, 1);
	transform(0, 2) = rotate_matrix(0, 2);
	transform(1, 0) = rotate_matrix(1, 0);
	transform(1, 1) = rotate_matrix(1, 1);
	transform(1, 2) = rotate_matrix(1, 2);
	transform(2, 0) = rotate_matrix(2, 0);
	transform(2, 1) = rotate_matrix(2, 1);
	transform(2, 2) = rotate_matrix(2, 2);

	//先将原点平移至平面中心，再做旋转
	Eigen::Matrix4d transform1 = Eigen::Matrix4d::Identity();
	transform1(0, 3) = -centroid(0);
	transform1(1, 3) = -centroid(1);
	transform1(2, 3) = -centroid(2);
	pcl::transformPointCloud(*cloud2, *cloud_out, transform1);
	pcl::transformPointCloud(*cloud_out, *cloud_out, transform);
	//pcl::io::savePCDFile("transformed_cloud.pcd", *cloud_out);
	//过滤不同高度的点云物体

	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(cloud_out);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(-0.5, 5.5);
	pass.filter(*cloud_out);

	//pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(cloud_out);
	pass.setFilterFieldName("z");
	pass.setFilterLimits(2.05, 2.15);
	pass.filter(*boss_plane);
	std::cout << boss_plane->size() << endl;

	viewer->setBackgroundColor(0.1, 0.05, 0.1);
	viewer->updatePointCloud(cloud_out, "cloud");
	viewer->resetCamera();

	Eigen::Vector4f centroid_boss;//柱中心
	pcl::compute3DCentroid(*boss_plane, centroid_boss);
	float size_boss = centroid_boss[2];
	std::cout << "柱高：" << size_boss << endl;
	float d = size_boss;
	QString str = QString("Height of Boss is %1").arg(d);
		text->append(str);

		ui.qvtkWidget->update();
}

void
PCLViewer::extract_baseplane()
{
	//connect(ui.extract, SIGNAL(clicked()), this, SLOT(get_boss_size()));
	viewer->setBackgroundColor(0.0392, 0.4, 0.5921);
	viewer->updatePointCloud(cloud3, "cloud");
	viewer->resetCamera();
	text->append(QString("Extracting the base plane ...."));
	ui.qvtkWidget->update();

}

void
PCLViewer::Area_select_callback(const pcl::visualization::AreaPickingEvent  &event, void* args){
	
	if (clicked_cloud->size() != 0)
	{
		clicked_cloud->clear();
	}
	std::vector<int> indices;
	if (event.getPointsIndices(indices) == -1)
		return;
	
	for (int i = 0; i < indices.size(); i++)
	{
		clicked_cloud->points.push_back(cloud->points.at(indices[i]));
	}

	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>  green(clicked_cloud, 0, 255, 0);
	
	std::stringstream ss;
	std::string  cloudName;
	ss << num++;
	ss >> cloudName;
	cloudName += "_cloud";
	cout << cloudName << endl;
    viewer->removePointCloud("cloudName");
	
	viewer->addPointCloud(clicked_cloud, green, cloudName);
    
	ui.qvtkWidget->update();
}

void 
PCLViewer::fitPlane_tool(){

	std::vector<int> inliers;
	pcl::SampleConsensusModelPlane<pcl::PointXYZ>::Ptr model_p(new pcl::SampleConsensusModelPlane<pcl::PointXYZ>(clicked_cloud));
	pcl::RandomSampleConsensus<pcl::PointXYZ>ransac(model_p);
	ransac.setDistanceThreshold(0.01);
	ransac.computeModel();
	ransac.getInliers(inliers);

	Eigen::VectorXf coef;
	ransac.getModelCoefficients(coef);
	float a, b, c, d;
	a = coef[0]; b = coef[1]; c = coef[2]; d = coef[3];
	//QString str = QString("Fit Plane with ransac ...   and  the Selected  Plane : (%1) x +( %2) y +( %3) z+ (%4)  = 0").arg(a).arg(b).arg(c).arg(d);
	

	/*struct Plane  plane1;
	struct Plane plane2;*/
	if (num<2)
	{	
		
		plane1.a = a; plane1.b = b; plane1.c = c; plane1.d = d;
		QString str1= QString("Fit Plane with ransac ...   and  the Selected  Plane1 : (%1) x +( %2) y +( %3) z+ (%4)  = 0").arg(plane1.a).arg(plane1.b).arg(plane1.c).arg(plane1.d);
		text->append(str1);
	}
	else{
		plane2.a = a; plane2.b = b; plane2.c = c; plane2.d = d;
		QString str2 = QString("Fit Plane with ransac ...   and  the Selected  Plane2 : (%1) x +( %2) y +( %3) z+ (%4)  = 0").arg(plane2.a).arg(plane2.b).arg(plane2.c).arg(plane2.d);
		text->append(str2);
		computeDistance();
	}
	
	ui.qvtkWidget->update();
}

float
PCLViewer::computeDistance(){
	//只需要将两个平面的向量，旋转至平行即可
	//double radian_angle = 0;

		Eigen::Vector3d v1(plane1.a, plane1.b, plane1.c);
		Eigen::Vector3d v2(plane2.a, plane2.b, plane2.c);
    double    radian_angle = atan2(v1.cross(v2).norm(), v1.transpose() * v2);
	   if (v1.cross(v2).z()<0)
	   {
		   radian_angle = 2 * M_PI - radian_angle;
	   }
	   // return  angle =  radian_angle * 180  / M_PI;
     QString str = QString("the angle between two planes： %1").arg(radian_angle);
	 text->append(str);

#if 0
   v1 = v1 / v1.norm();
	   v2 = v2 / v2.norm();
	Eigen::Vector3d n =v1.cross(v2); //旋转轴
	n = n / n.norm();
	Eigen::AngleAxisd rotate_vector(-radian_angle, n);
	Eigen::Matrix3d rotate_matrix = Eigen::Matrix3d::Identity();
	rotate_matrix = rotate_vector.toRotationMatrix();
	Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
	transform(0, 0) = rotate_matrix(0, 0);
	transform(0, 1) = rotate_matrix(0, 1);
	transform(0, 2) = rotate_matrix(0, 2);
	transform(1, 0) = rotate_matrix(1, 0);
	transform(1, 1) = rotate_matrix(1, 1);
	transform(1, 2) = rotate_matrix(1, 2);
	transform(2, 0) = rotate_matrix(2, 0);
	transform(2, 1) = rotate_matrix(2, 1);
	transform(2, 2) = rotate_matrix(2, 2);
	pcl::transformPointCloud(*clicked_cloud, *temp_cloud, transform);
#endif
	
	if (radian_angle < (M_PI / 10))
	{  
		std::cout << "radian_angle " << radian_angle << endl;
		float  aa = plane1.a *plane1.a + plane1.b *plane1.b + plane1.c *plane1.c;
		float  bb = plane2.a *plane2.a + plane2.b *plane2.b + plane2.c *plane2.c;
		float dis1 = ((plane1.d - plane2.d)) / sqrt(aa);
		float dis2 = (plane1.d - plane2.d) / sqrt(bb);
		QString str = QString("the distance between two planes： %1  ").arg(dis1);
		text->append(str);
	}
	else
	{
		std::cout<< "the angle is  more than  standard";
	}
	return radian_angle * 180 / M_PI;
}

void
PCLViewer::feature_boundry(){
	
	//提取边界
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	pcl::PointCloud<pcl::Boundary> boundaries;
	pcl::BoundaryEstimation<pcl::PointXYZ, pcl::Normal, pcl::Boundary> est;
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());

	pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
	kdtree.setInputCloud(clicked_cloud);
	int k = 2;
	float everagedistance = 0;
	for (int i = 0; i < clicked_cloud->size() / 2; i++)
	{

		std::vector<int> nnh;
		std::vector<float> squaredistance;

		kdtree.nearestKSearch(clicked_cloud->points[i], k, nnh, squaredistance);

		everagedistance += sqrt(squaredistance[1]);

	}
	everagedistance = everagedistance / (clicked_cloud->size() / 2);
	cout << "everage distance is : " << everagedistance << endl;

	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normEst;  //其中pcl::PointXYZ表示输入类型数据，pcl::Normal表示输出类型,且pcl::Normal前三项是法向，最后一项是曲率
	normEst.setInputCloud(clicked_cloud);
	normEst.setSearchMethod(tree);
	// normEst.setRadiusSearch(2);  //法向估计的半径
	normEst.setKSearch(40);  //法向估计的点数
	normEst.compute(*normals);
	cout << "normal size is " << normals->size() << endl;

	//normal_est.setViewPoint(0,0,0); //这个应该会使法向一致
	est.setInputCloud(clicked_cloud);
	est.setInputNormals(normals);/*M_PI_2 */
	est.setAngleThreshold(M_PI_2);   ///在这里 由于构造函数已经对其进行了初始化 为Π/2 ，必须这样 使用 M_PI/2  M_PI_2  
	est.setSearchMethod(tree);
	est.setKSearch(30);  //一般这里的数值越高，最终边界识别的精度越好 20
	//  est.setRadiusSearch(everagedistance);  //搜索半径
	est.compute(boundaries);

	pcl::PointCloud<pcl::PointXYZ>::Ptr boundPoints(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ> noBoundPoints;
	int countBoundaries = 0;
	for (int i = 0; i < clicked_cloud->size(); i++){
		uint8_t x = (boundaries.points[i].boundary_point);
		int a = static_cast<int>(x); //该函数的功能是强制类型转换
		if (a == 1)
		{
			//  boundPoints.push_back(cloud->points[i]);
			(*boundPoints).push_back(clicked_cloud->points[i]);
			countBoundaries++;
		}
		else
			noBoundPoints.push_back(clicked_cloud->points[i]);

	}
	std::cout << "boudary size is：" << countBoundaries << std::endl;

	//save  the boundry pointcloud
#if 0
	if (num < 2)
	{
		pcl::io::savePCDFileASCII("F:/Projects/pclviewer/pclviewer/3D_data/clicked_cloud1_boundry.ply", *boundPoints);
	}
	pcl::io::savePCDFileASCII("F:/Projects/pclviewer/pclviewer/3D_data/clicked_cloud2_boundry.ply", *boundPoints);
#endif
	
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> red(boundPoints, 255, 0, 0);
	std::string boundry;
	viewer->addPointCloud(boundPoints,red ,boundry);

	ui.qvtkWidget->update();

}

void 
PCLViewer::circle_radius(){




}

PCLViewer::~PCLViewer()
{
	delete &ui;
}