#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "dirent.h"
#include <stdio.h>

int LastSlash(std::string path)
{
    int last_pos;
    int i,len;
    char s;
    char char_slash;

    last_pos = 0;
    i = 0;
    len = path.length();


    char slash = 0x5c;
    char slash2 = 0x2f;

    // "/"
    while ( i <= len)
    {
        s = path[i];

        if (s == slash)
        {
            last_pos = i;
        }

        if (s == slash2)
        {
            last_pos = i;
        }

        i++;
    }

    return last_pos;
}

std::string GetDirName(std::string Dir)
{
    int Last;

    Last = LastSlash(Dir);

    Dir = Dir.substr(0,Last-1);

    return Dir;
}

bool CheckOpenDir(std::string Path )
{
    DIR * dir = opendir( Path.c_str() );

    if (dir == 0x00)
    {
        return false;
    }
    else
    {
        return true;
    }
}


void MainWindow::LoadFromDir(QTableView * qTableView,
                             std::string Path )
{
    QString title = QString(Path.c_str());

    DIR * dir = opendir( Path.c_str() );

    struct stat buf;

    if (dir == 0x00)
    {
        //exit(0);
        return;
    }

    this->ui->centralWidget->setWindowTitle(title);

    QStandardItemModel * model4 = new QStandardItemModel(0,5);
    model4->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("Имя"));
    model4->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("Тип"));
    model4->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("Размер"));
    model4->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("Путь"));
    model4->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("Права"));

    QString str;
    std::string str2;
    char buffer[50];
    int i = 0;
    int sz = 0;

    if ( dir )
    {
        struct dirent * ent;

        ent = readdir( dir );

        while (ent != NULL)
        {
            if (ent != NULL)
            {
            //List->insert(QString(ent->d_name));
            str = QString("ssss");//ent->d_name);
            //str = "ssss";
            //str2 = std::string(ent->d_name);
            sprintf(buffer,"%s",ent->d_name);
            //strcpy(buffer,ent->d_name);
            str2 = buffer;

            str = QString(str2.c_str());
            //str.

            //List->append( str );
            //List << ent->d_name;

            QStandardItem * si = new QStandardItem(str);
            model4->setItem(i,0,si);

            //if (ent->d_type == 8 )
            //{
                //sprintf(buffer,"%s","Файл");
            //}
            if  (ent->d_type == 4 )
            {
                sprintf(buffer,"%s","Папка");
            }
            else
            {
                //sprintf(buffer,"[%d]",ent->d_type);
                sprintf(buffer,"%s","Файл");
            }
            str2 = buffer;
            str = QString(str2.c_str());
            QStandardItem * si2 = new QStandardItem(str);
            model4->setItem(i,1,si2);

            sz = ent->d_reclen;

            if (sz < 1000)
            {
                sprintf(buffer,"%d байт",sz);
            }
            else
            {
                sz = sz / 1000;
                sprintf(buffer,"%d Кбайт",sz);
            }
            str2 = buffer;
            str = QString(str2.c_str());
            QStandardItem * si3 = new QStandardItem(str);
            model4->setItem(i,2,si3);

            str = QString(Path.c_str());
            QStandardItem * si4 = new QStandardItem(str);
            model4->setItem(i,3,si4);

            sprintf(buffer,"%s%s",Path.c_str(),ent->d_name);
            str2 = buffer;
            QString str4 = QString(str2.c_str());

            stat(str2.c_str(),&buf);

                        //if ((buf.st_mode & S_IFMT) == S_IFREG)
            QString Str3 = "----";

            if (S_ISREG(buf.st_mode))
            {
                 sprintf(buffer,"%ld",buf.st_mode );
                 Str3 = buffer;
            }
            else
            {
                Str3 = "none";
                switch (buf.st_mode & S_IFMT)
                {
                    case S_IFBLK:Str3 ="S_IFBLK";break;
                    case S_IFCHR:Str3 ="S_IFCHR";break;
                    case S_IFDIR:Str3 ="S_IFDIR";break;
                    case S_IFIFO:Str3 ="S_IFIFO";break;
                    case S_IFLNK:Str3 ="S_IFLNK";break;
                    case S_IFREG:Str3 ="S_IFREG";break;
                    case S_IFSOCK:Str3 ="S_IFSOCK";break;

                    default:
                    {
                       Str3 ="default";break;
                    }
                }


            }

            str = QString(Str3);
            QStandardItem * si5 = new QStandardItem(Str3);
            model4->setItem(i,4,si5);

            i++;

            }

            ent = readdir(dir);
        }
    }
    else
    {
    }

    qTableView->setModel(model4);
    qTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    qTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    qTableView->setSortingEnabled(false);
}

void MainWindow::DblClick2()
{
    int row = qTableView2->currentIndex().row();
    int column = qTableView2->currentIndex().column();

    QModelIndex mi = qTableView2->currentIndex();

    QString Str = "";

    Str = qTableView2->model()->data(qTableView2->currentIndex(),0).toString();

    QItemSelection is = qTableView2->selectionModel()->selection();

    std::string str2 = Str.toLocal8Bit().constData();

    if(Str.compare("..") == 0 )
    {
        QString temp = QString(currentPath2.c_str());

        std::string pathTo = GetDirName(currentPath2);

        if (pathTo == "")
        {
            pathTo = "//";
        }

        if (CheckOpenDir(pathTo) == true)
        {
            currentPath2 = pathTo;
            LoadFromDir(qTableView2,pathTo);
        }
        else
        {
        }
    }
    else if (( Str.compare("Папка") != 0) & Str.compare("Файла")!=0)
    {
        std::string pathTo;

        char cr = currentPath2[currentPath2.length()-1];

        char fcr = currentPath2[0];

        char buffer[255];

        // 0x2f /
        if (cr == 0x2f)
        {
            sprintf(buffer,"%s",currentPath2.c_str());
            pathTo = buffer;
            sprintf(buffer,"%s",Str.toStdString().c_str());
            pathTo = pathTo + buffer;

            //pathTo = "//usr";

            QString str3 = QString(pathTo.c_str());

            str3 = str3 + "dddd";
        }
        else if (cr == 0x5c )
        {
            pathTo = currentPath2 + str2;
        }
        else
        {
            pathTo = currentPath2 + "//" + str2;
        }

        if (CheckOpenDir(pathTo) == true)
        {
            currentPath2 = pathTo;
            LoadFromDir(qTableView2,pathTo);
        }
        else
        {
        }
    }
}

void MainWindow::DblClick()
{
    //QString str = "hello";
    //QMessageBox msgbox;
    //msgbox.setText(str);
    //msgbox.exec();

    int row = qTableView->currentIndex().row();
    int column = qTableView->currentIndex().column();

    QModelIndex mi = qTableView->currentIndex();

    QString Str = "";

    Str = qTableView->model()->data(qTableView->currentIndex(),0).toString();

    QItemSelection is = qTableView->selectionModel()->selection();

    std::string str2 = Str.toLocal8Bit().constData();

    if(Str.compare("..") == 0 )
    {
        QString temp = QString(currentPath.c_str());

        std::string pathTo = GetDirName(currentPath);

        if (pathTo == "")
        {
            pathTo = "//";
        }

        if (CheckOpenDir(pathTo) == true)
        {
            currentPath = pathTo;
            LoadFromDir(qTableView,pathTo);
        }
        else
        {
        }
    }
    else if (( Str.compare("Папка") != 0) & Str.compare("Файла")!=0)
    {
        std::string pathTo;

        char cr = currentPath[currentPath.length()-1];

        char fcr = currentPath[0];

        char buffer[255];

        // 0x2f /
        if (cr == 0x2f)
        {
            sprintf(buffer,"%s",currentPath.c_str());
            pathTo = buffer;
            sprintf(buffer,"%s",Str.toStdString().c_str());
            pathTo = pathTo + buffer;

            //pathTo = "//usr";

            QString str3 = QString(pathTo.c_str());

            str3 = str3 + "dddd";
        }
        else if (cr == 0x5c )
        {
            pathTo = currentPath + str2;
        }
        else
        {
            pathTo = currentPath + "//" + str2;
        }

        if (CheckOpenDir(pathTo) == true)
        {
            currentPath = pathTo;
            LoadFromDir(qTableView,pathTo);
        }
        else
        {
        }
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myLayout = new QVBoxLayout();

    listview1 = new QListView();
    qTableView = new QTableView();

    Model = new QStringListModel();

    List << "Item1";
    List << "Item1";
    List << "Item1";
    List << "Item1";

    currentPath = "//usr";
    currentPath = "//dev";//vcsa5//";
    currentPath = "//";
    //run";


    LoadFromDir(qTableView,currentPath);

    Model->setStringList(List);

    listview1->setModel(Model);

    //this->myLayout->addWidget(listview1,2,0,2,4);

    connect(qTableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(DblClick()));
    this->myLayout->addWidget(qTableView);//,2,0,2,4);

    mainLayout = new QGridLayout;
    mainLayout->addLayout(myLayout,0,1);

    rightLayout = new QVBoxLayout;

    qTableView2 = new QTableView;
    currentPath2 = "//";

    //LoadFromDir(qTableView2,currentPath2);

    //connect(qTableView2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(DblClick2()));
    //rightLayout->addWidget(qTableView2);//,2,0,2,4);
    //mainLayout->addLayout(rightLayout,0,2);

    this->centralWidget()->setLayout( mainLayout );

}

MainWindow::~MainWindow()
{
    delete ui;
}
