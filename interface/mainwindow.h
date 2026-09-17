#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QStackedWidget>
#include <memory>
#include "data/data.h"
#include "interface/BusMap/busmap.h"
#include "interface/Manager/manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    //~MainWindow(){}

private slots:
    void addIntersection() {setEditMode();busmap->addIntersection();}
    void addBusStop() {setEditMode();busmap->addBusStop();}
    void addRoad() {setEditMode();busmap->addRoad();}
    void addRoute();
    void deleteIntersection() {setEditMode();busmap->deleteIntersection();}
    void deleteRoad() {setEditMode();busmap->deleteRoad();}
    void divideRoadwithInt() {setEditMode();busmap->divideRoadwithIntersection();}
    void divideRoadwithStop() {setEditMode();busmap->divideRoadwithBusStop();}
    void displayHelp();
    void newSituation();
    void openSituation();
    void saveSituation();
    void saveAsSituation();


private:
    Data* data;
    QStackedWidget* stacked_widget;
    Manager* manager;
    BusMap* busmap;
    QScrollArea* scroll_area;
    QWidget* scroll_widget;
    QWidget* stacked_page;
    QVBoxLayout* stacked_layout;
    QVBoxLayout* scroll_layout;
    enum class ViewMode {sim,edit,man};
    ViewMode view_mode {ViewMode::sim};

      void createMenus();
      void createActions();
      void setSimMode();
      void setEditMode();
      void setManMode();
      void changeTime();

        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *viewMenu;
        QMenu *helpMenu;
            QMenu *addMenu;
            QMenu *deleteMenu;
            QMenu *divideMenu;

        QAction* newAct;
        QAction* openAct;
        QAction* saveAct;
        QAction* saveAsAct;
        QAction* exitAct;
            QAction* addIntersectionAct;
            QAction* addBusStopAct;
            QAction* addRoadAct;
            QAction* addRouteAct;
            QAction* deleteIntersectionAct;
            QAction* deleteRoadAct;
            QAction* divideRoadwithIntAct;
            QAction* divideRoadwithStopAct;
        QAction* helpAct;
        QAction* changeModetoSimAct;
        QAction* changeModetoEditAct;
        QAction* changeModetoManAct;
        QAction* changeTimeAct;

        void closeEvent(QCloseEvent *event)  override;// show prompt when user wants to close app
        int askForSave();
        void setMainWindow();


};
#endif // MAINWINDOW_H
