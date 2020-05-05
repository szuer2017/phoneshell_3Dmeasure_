

#include <iostream>

// Qt
#include <QMainWindow>
#include <qfiledialog.h>
#include <qtextedit.h>

// Point Cloud Library
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/common/transforms.h>  
#include <pcl/features/normal_3d.h>
#include <pcl/features/boundary.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/filters/passthrough.h>
#include <pcl/common/centroid.h>
#include <pcl/common/common.h> 
#include <pcl/console/parse.h>

// Visualization Toolkit (VTK)
#include <vtkRenderWindow.h>
#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);


#include <ui_pclviewer.h>

typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

namespace Ui
{
	class PCLViewer;

}

class PCLViewer : public QMainWindow
{
	Q_OBJECT

public:
	explicit PCLViewer(QWidget *parent = 0);
	~PCLViewer();

private:

	PointCloudT::Ptr cloud;
	PointCloudT::Ptr cloud2;
	PointCloudT::Ptr cloud3;
	PointCloudT::Ptr cloud_out;
	PointCloudT::Ptr boss_plane;
	PointCloudT::Ptr  clicked_cloud;
	PointCloudT::Ptr  temp_cloud;
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
//	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer1;

	public slots:
	
	void
		onOpen();

	void
		pointcloud_filter();
	void
		get_boss_size();
	void
		extract_baseplane();
	void
		Area_select_callback(const pcl::visualization::AreaPickingEvent  &event, void* args);
	void
		fitPlane_tool();

	float
		computeDistance();

	void
		feature_boundry();
	void
		circle_radius();

protected:

	//boost::mutex cloud_mutex;
	
private:
	Ui::PCLViewer ui;
	QTextEdit *text;

	
};

