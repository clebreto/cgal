#include <CGAL/Three/Polyhedron_demo_plugin_interface.h>

#include "Scene_cad_item.h"

#include "Scene_c3t3_cad_item.h"
#include "ui_CAD_mesher_protect_initialization_dialog.h"

#include "C3t3_cad_type.h"

//dtk
#include <dtkCore>
#include <dtkContinuousGeometry>
#include <dtkContinuousGeometryUtils>
#include <dtkBRep>
#include <dtkBRepReader>
#include <dtkNurbsSurface>
#include <dtkClippedNurbsSurface>
#include <dtkClippedTrim>
#include <dtkClippedTrimLoop>
#include <dtkRationalBezierCurve>
#include <dtkTrim>
#include <dtkTopoTrim>

//dtk-nurbs-probing
#include <dtkSeamProtectionGraph>

#include <QObject>
#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QApplication>
#include <QtPlugin>
#include <QInputDialog>
#include <QStringList>

// declare the CGAL function
CGAL::Three::Scene_item* cgal_code_cad_protect_intialization(QWidget* parent,
                                              dtkBRep*,
                                              const double sizing);

// To avoid verbose function and named parameters call
using namespace CGAL::parameters;

using namespace CGAL::Three;
class Polyhedron_demo_CAD_initialization_plugin :
    public QObject,
    protected Polyhedron_demo_plugin_interface
{
  Q_OBJECT
  Q_INTERFACES(CGAL::Three::Polyhedron_demo_plugin_interface)
  Q_PLUGIN_METADATA(IID "com.geometryfactory.PolyhedronDemo.PluginInterface/1.0")

public:
  void init(QMainWindow* mainWindow, CGAL::Three::Scene_interface* scene_interface, Messages_interface*) {
    this->scene = scene_interface;
    this->mw = mainWindow;
    actionProtectInitialization = new QAction(tr("Initialization by protection"), mw);
    actionRandomInitialization = new QAction(tr("Initialization by random shooting"), mw);
    actionSamplingInitialization = new QAction(tr("Initialization by sampling the surfaces"), mw);
    actionProtectInitialization->setProperty("subMenuName", "Mesh initialization");
    actionRandomInitialization->setProperty("subMenuName", "Mesh initialization");
    actionSamplingInitialization->setProperty("subMenuName", "Mesh initialization");
    if(actionProtectInitialization) {
      connect(actionProtectInitialization, SIGNAL(triggered()),
              this, SLOT(protectInitialization()));
    }
    if(actionRandomInitialization != nullptr) {
      connect(actionRandomInitialization, SIGNAL(triggered()),
              this, SLOT(randomInitialization()));
    }
    if(actionSamplingInitialization != nullptr) {
        connect(actionSamplingInitialization, SIGNAL(triggered()),
                this, SLOT(samplingInitialization()));
    }
  }

  bool applicable(QAction*) const {
    return qobject_cast<Scene_cad_item*>(scene->item(scene->selectionIndices().first()));
  }

  QList<QAction*> actions() const {
      return QList<QAction*>() << actionProtectInitialization << actionRandomInitialization << actionSamplingInitialization;
  }
public Q_SLOTS:
    void protectInitialization();
    void randomInitialization();
    void samplingInitialization();

private:
    QAction *actionProtectInitialization;
    QAction *actionRandomInitialization;
    QAction *actionSamplingInitialization;
    Scene_interface *scene;
    QMainWindow *mw;
}; // end class Polyhedron_demo_remeshing_plugin

void Polyhedron_demo_CAD_initialization_plugin::protectInitialization()
{
  Scene_cad_item* cad_item = qobject_cast<Scene_cad_item*>(scene->item(scene->selectionIndices().first()));
  if(!cad_item) {return;}

  dtkBRep* brep = cad_item->brep();
  if(!brep) return;

  QDialog dialog(mw);
  Ui::CADMesherProtectInitializationDialog ui;
  ui.setupUi(&dialog);
  connect(ui.buttonBox, SIGNAL(accepted()),
          &dialog, SLOT(accept()));
  connect(ui.buttonBox, SIGNAL(rejected()),
          &dialog, SLOT(reject()));
  double diag = scene->len_diagonal();

  ui.sizeSpinBox->setDecimals(4);
  ui.sizeSpinBox->setRange(diag * 10e-6, // min
                           diag); // max
  ui.sizeSpinBox->setValue(diag * 0.05); // default value

  int i = dialog.exec();
  if(i == QDialog::Rejected)
    return;

  const double edge_sizing = ui.sizeSpinBox->value();
  Mesh_domain_with_features* cgal_brep_mesh_domain_with_features = new Mesh_domain_with_features(*brep);
  ///////////////////////////////////////////////////////////////////
  //    Recovers the features
  ///////////////////////////////////////////////////////////////////
  dtkSeamProtectionGraph protection_graph(*brep, true);

  C3t3 p_c3t3;
  Tr& tr = p_c3t3.triangulation();
  const CGAL::Bbox_3& cgal_bbox = cgal_brep_mesh_domain_with_features->bbox();
  std::list< Weighted_point > w_points;
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmax(), cgal_bbox.ymin(), cgal_bbox.zmin()), 1.));
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmin(), cgal_bbox.ymin(), cgal_bbox.zmin()), 1.));
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmin(), cgal_bbox.ymin(), cgal_bbox.zmin()), 1.));
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmin(), cgal_bbox.ymax(), cgal_bbox.zmin()), 1.));
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmax(), cgal_bbox.ymax(), cgal_bbox.zmin()), 1.));
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmin(), cgal_bbox.ymin(), cgal_bbox.zmax()), 1.));
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmax(), cgal_bbox.ymin(), cgal_bbox.zmax()), 1.));
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmin(), cgal_bbox.ymax(), cgal_bbox.zmax()), 1.));
  w_points.push_back(Weighted_point(Point_3(cgal_bbox.xmax(), cgal_bbox.ymax(), cgal_bbox.zmax()), 1.));
  for(auto& w_point : w_points) {
      Vertex_handle vi = tr.insert(w_point);
      CGAL_assertion(vi != Vertex_handle());
      p_c3t3.set_dimension(vi, 0);
      p_c3t3.set_index(vi, 0);
  }

  std::unordered_map< const dtkTopoTrim *, std::size_t > tts_map;
  std::size_t index = 1;
  std::size_t curr_index = 0;
  for(auto& p_sphere : protection_graph.m_protection_spheres) {
      auto find_tt = protection_graph.m_map.find(p_sphere->m_bezier_curve);
      if(find_tt == protection_graph.m_map.end()) { dtkFatal() << "Mistmatching between bezier curve pointers on spheres and in map";}
      auto find = tts_map.find(find_tt->second);
      if( find == tts_map.end()) {
          curr_index = index;
          tts_map.insert(std::make_pair(find_tt->second, index));
          ++index;
      } else {
          curr_index = find->second;
      }
      Weighted_point pi(Point_3(p_sphere->center()[0], p_sphere->center()[1], p_sphere->center()[2]),
                        p_sphere->radius() * p_sphere->radius());
      Vertex_handle v = tr.insert(pi);
      // `v` could be null if `pi` is hidden by other vertices of `tr`.
      CGAL_assertion(v != Vertex_handle());
      p_c3t3.set_dimension(v, 1); // by construction, points are on surface
      p_c3t3.set_index(v, curr_index);//TODO replace by curve index
  }

  // //Output
  Scene_c3t3_cad_item *c3t3_cad_item = new Scene_c3t3_cad_item(p_c3t3, *cgal_brep_mesh_domain_with_features);
  if(!c3t3_cad_item)
  {
    qDebug()<<"c3t3 CAD item not created";
    return;
  }
  c3t3_cad_item->setName(QString("%1 (c3t3)").arg(cad_item->name()));
  scene->addItem(c3t3_cad_item);
}

void Polyhedron_demo_CAD_initialization_plugin::randomInitialization(){

}

void Polyhedron_demo_CAD_initialization_plugin::samplingInitialization(){

}

#include "CAD_initialization_plugin.moc"
