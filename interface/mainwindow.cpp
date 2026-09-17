#include "interface/mainwindow.h"
#include "interface/changetimedialog.h"
#include "qboxlayout.h"
#include "qdialogbuttonbox.h"
#include "qmessagebox.h"
#include <QMenuBar>
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/icons/Icon.png"));

    stacked_widget = new QStackedWidget;

    stacked_layout = new QVBoxLayout();
    stacked_page = new QWidget();
    scroll_area = new QScrollArea (stacked_page);
    scroll_widget = new QWidget (scroll_area);
    scroll_layout = new QVBoxLayout(scroll_widget);

    data = new Data;
    busmap = new BusMap(data,this);
    manager = new Manager(data,this);

    createActions();
    createMenus();

    setMainWindow();
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("P&lik"), this);
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct); 

    addMenu = new QMenu(tr("Dodaj..."),this);
    addMenu->addAction(addBusStopAct);
    addMenu->addAction(addIntersectionAct);
    addMenu->addAction(addRoadAct);
    addMenu->addAction(addRouteAct);

    deleteMenu = new QMenu(tr("Usuń..."),this);
    deleteMenu->addAction(deleteIntersectionAct);
    deleteMenu->addAction(deleteRoadAct);

    divideMenu = new QMenu(tr("Podziel drogę ..."),this);
    divideMenu->addAction(divideRoadwithIntAct);
    divideMenu->addAction(divideRoadwithStopAct);

    editMenu = new QMenu(tr("E&dycja"), this);
    editMenu->addMenu(addMenu);
    editMenu->addMenu(deleteMenu);
    editMenu->addMenu(divideMenu);

    viewMenu = new QMenu(tr("W&idok"), this);
    viewMenu->addAction(changeModetoSimAct);
    viewMenu->addAction(changeModetoEditAct);
    viewMenu->addAction(changeModetoManAct);
    viewMenu->addSeparator();
    viewMenu->addAction(changeTimeAct);


    helpMenu = new QMenu(tr("P&omoc"), this);
    helpMenu->addAction(helpAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("Nowy"), this);
        newAct->setShortcuts(QKeySequence::New);
        connect(newAct, &QAction::triggered, this, &MainWindow::newSituation);
    openAct = new QAction(tr("Otwórz"), this);
        openAct->setShortcuts(QKeySequence::Open);
        connect(openAct, &QAction::triggered, this, &MainWindow::openSituation);
    saveAct = new QAction(tr("Zapisz"), this);
        saveAct->setShortcuts(QKeySequence::Save);
        connect(saveAct, &QAction::triggered, this, &MainWindow::saveSituation);
    saveAsAct = new QAction(tr("Zapisz jako"), this);
        saveAsAct->setShortcuts(QKeySequence::SaveAs);
        connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsSituation);
    exitAct = new QAction(tr("Wyjdź"), this);
        exitAct->setShortcuts(QKeySequence::Quit);
        connect(exitAct, &QAction::triggered, this, &MainWindow::close);
    changeModetoSimAct = new QAction(tr("Ekran główny"), this);
        connect(changeModetoSimAct, &QAction::triggered, this, &MainWindow::setSimMode);
    changeModetoEditAct = new QAction(tr("Ekran edycji"), this);
        connect(changeModetoEditAct, &QAction::triggered, this, &MainWindow::setEditMode);
    changeModetoManAct = new QAction(tr("Ekran zarządzania"), this);
        connect(changeModetoManAct, &QAction::triggered, this, &MainWindow::setManMode);
    addBusStopAct = new QAction(tr("Przystanek"), this);
      connect(addBusStopAct, &QAction::triggered, this, &MainWindow::addBusStop);
    addIntersectionAct = new QAction(tr("Skrzyżowanie"), this);
      connect(addIntersectionAct, &QAction::triggered, this, &MainWindow::addIntersection);
    addRoadAct = new QAction(tr("Drogę"), this);
      connect(addRoadAct, &QAction::triggered, this, &MainWindow::addRoad);
    addRouteAct = new QAction(tr("Wariant (trasę)"),this);
      connect(addRouteAct,&QAction::triggered,this,&MainWindow::addRoute);
    divideRoadwithIntAct = new QAction(tr("Dodaj skrzyżowanie"), this);
      connect(divideRoadwithIntAct, &QAction::triggered, this, &MainWindow::divideRoadwithInt);
    divideRoadwithStopAct = new QAction(tr("Dodaj przystanek"), this);
        connect(divideRoadwithStopAct, &QAction::triggered, this, &MainWindow::divideRoadwithStop);
    deleteIntersectionAct = new QAction(tr("Przystanek/Skrzyżowanie"), this);
      connect(deleteIntersectionAct, &QAction::triggered, this, &MainWindow::deleteIntersection);
    deleteRoadAct = new QAction(tr("Drogę"), this);
      connect(deleteRoadAct, &QAction::triggered, this, &MainWindow::deleteRoad);
    changeTimeAct = new QAction(tr("Zmień datę i czas symulacji"), this);
        connect(changeTimeAct, &QAction::triggered, this, &MainWindow::changeTime);
    helpAct = new QAction(tr("Informacje"), this);
        connect(helpAct, &QAction::triggered, this, &MainWindow::displayHelp);

}

void MainWindow::setSimMode()
{
      busmap->setSimMode();
      switch(view_mode)
      {
      case ViewMode::sim:
      case ViewMode::edit:
          break;
      case ViewMode::man:
          stacked_widget->setCurrentIndex(0);
          break;
      }
      view_mode = ViewMode::sim;
}

void MainWindow::setEditMode()
{
      busmap->setEditMode();
      switch(view_mode)
      {
      case ViewMode::sim:
      case ViewMode::edit:
          break;
      case ViewMode::man:
          stacked_widget->setCurrentIndex(0);
          break;
      }
      view_mode = ViewMode::edit;
}

void MainWindow::setManMode()
{
      switch(view_mode)
      {
      case ViewMode::sim:
      case ViewMode::edit:
          stacked_widget->setCurrentIndex(1);
          break;
      case ViewMode::man:
          break;
      }
      view_mode = ViewMode::man;
}

void MainWindow::changeTime()
{
      ChangeTimeDialog dial (busmap->getDate(),busmap->getTime(),busmap->getTimeSpeed(),data);
      auto res = dial.exec();
      if(res)
      {
          busmap->setTime(dial.getTime());
          busmap->setTimeSpeed(dial.getSpeed());
          busmap->setDate(dial.getDate(),dial.getDayType());
      }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    auto res = askForSave();
    if(res == QMessageBox::Yes)
        data->writeData();
    event->accept();
}

int MainWindow::askForSave()
{
    QMessageBox mb (QMessageBox::Question,"Zapis","Czy chcesz zapisać sytuację?",QMessageBox::Yes | QMessageBox::No);
    return mb.exec();
}

void MainWindow::setMainWindow()
{
    stacked_widget->addWidget(stacked_page);
    stacked_widget->addWidget(manager);

    scroll_layout->addWidget(busmap);
    scroll_widget->setLayout(scroll_layout);
    scroll_widget->resize(busmap->getMapHSize(),busmap->getMapVSize());
    scroll_area->setWidget(scroll_widget);
    stacked_page->setLayout(stacked_layout);
    stacked_layout->addWidget(scroll_area);

    this->setCentralWidget(stacked_widget);
}

void MainWindow::addRoute()
{
      QDialog dial;
      QVBoxLayout layout;
      QLabel label("Wybierz linię:");
      QComboBox combo_box;
      QDialogButtonBox button_box (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
      std::map<std::string,std::string> line_names;

      connect(&button_box, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
      connect(&button_box, &QDialogButtonBox::rejected, &dial, &QDialog::reject);
      for(const auto& el : *(data->getLines()))
      {
          auto name = el.second->getName();
          combo_box.addItem(QString::fromStdString(name));
          line_names[name] = el.second->getID();
      }
      layout.addWidget(&label);
      layout.addWidget(&combo_box);
      layout.addWidget(&button_box);
      dial.setLayout(&layout);
      dial.exec();
      if(dial.result())
      {
          auto line = combo_box.currentText().toStdString();
          setEditMode();
          busmap->addRoute(line_names[line]);
      }
}

void MainWindow::displayHelp()
{
      QMessageBox msg_box(QMessageBox::Information,"Informacje","BusBoss v1.6\nCopyright Paweł Schmidt 2023\nKontakt z deweloperem: ps300319@student.polsl.pl",QMessageBox::Ok);
          msg_box.exec();
}

void MainWindow::newSituation()
{
    if(askForSave()==QMessageBox::Ok)
        data->writeData();
    delete data;
    delete busmap;
    delete manager;
    QString dir = QFileDialog::getOpenFileName(nullptr, "Wybierz plik przedstawiający mapę",
                                                    QDir::currentPath(), tr("Images (*.jpg)"));
    if(!dir.size())
        return;
    QString s;
    bool found = false;
    do
    {
        s = QFileDialog::getSaveFileName(this,"Stwórz folder zapisu",QDir::currentPath() + "/profiles",tr("Folder (*)"));
        for (auto const& dir_entry : std::filesystem::directory_iterator{(QDir::currentPath() + "/profiles").toStdString()})
        {
            if(dir_entry.path() == s.toStdString())
                found = true;
        }
    }
    while(found);

    std::filesystem::create_directory(std::filesystem::path(s.toStdString()));
    std::filesystem::copy_file(std::filesystem::path(dir.toStdString()),std::filesystem::path(s.toStdString()+"\\map.jpg"));

    data = new Data(s);
    busmap = new BusMap(data,this);
    manager = new Manager(data,this);
    setMainWindow();
}

void MainWindow::openSituation()
{
    if(askForSave()==QMessageBox::Ok)
          data->writeData();
    delete data;
    delete busmap;
    delete manager;
    QString dir = QFileDialog::getExistingDirectory(nullptr, "Otwórz",
                                                 QDir::currentPath() + "/profiles",
                                                 QFileDialog::ShowDirsOnly |
                                                     QFileDialog::DontResolveSymlinks);
    data = new Data(dir);
    busmap = new BusMap(data,this);
    manager = new Manager(data,this);
    setMainWindow();
}

void MainWindow::saveSituation()
{
    data->writeData();
}

void MainWindow::saveAsSituation()
{
    auto s = QFileDialog::getSaveFileName(this,"Zapisz jako",QDir::currentPath() + "/profiles",tr("Folder (*)"));
    if(s.size())
    {
        std::filesystem::create_directory(std::filesystem::path(s.toStdString()));
        data->writeData(s);
    }
}


