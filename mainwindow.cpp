#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QDirModel>
#include <QProcess>
#include <QDebug>
#include <QFileDialog>
#include <math.h>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QMessageBox>
#include "textprinter.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);    

    textprinter_ = new TextPrinter(this);
    textprinter_->setHeaderSize(10);
    textprinter_->setFooterSize(10);
    textprinter_->setDateFormat("MMMM dd yyyy");

    timerVisualizaEstado = new QTimer(this);
    connect(timerVisualizaEstado, SIGNAL(timeout()), this, SLOT(slot_ActualizaProgressBar()));
    cuentaProgressBar=0;
    incremento=true;

    ui->pBexDirTrabajo->setCheckable(true);
    ui->pBexDirSdf->setCheckable(true);
    ui->pBexDirBinario->setCheckable(true);
    ui->pBTx1->setCheckable(true);
    ui->pBRx1->setCheckable(true);
    ui->pb_localizacionCiudades->setCheckable(true);
    //ui->labelAltAntena->setHidden(true);

    ui->labelCoberturaRegional->setMaximumSize((ui->tab_coberturaRegional->size()));

    configuracion();    
    connect(ui->TxName1, SIGNAL(textChanged ( const QString &)), this, SLOT(slot_generaComando()));
    connect(ui->RxName1, SIGNAL(textChanged ( const QString &)), this, SLOT(slot_generaComando()));
    connect(ui->checkBoxMeters, SIGNAL(stateChanged(int)), this, SLOT(slot_generaComando()));
    connect(ui->checkBoxPerfil, SIGNAL(stateChanged(int)), this, SLOT(slot_generaComando()));
    connect(ui->checkBoxElevacion, SIGNAL(stateChanged(int)), this, SLOT(slot_generaComando()));
    connect(ui->checkBoxAltura, SIGNAL(stateChanged(int)), this, SLOT(slot_generaComando()));
    connect(ui->checkBoxNormalizada, SIGNAL(stateChanged(int)), this, SLOT(slot_generaComando()));

    connect(ui->checkBoxPerdidasTrayectoria, SIGNAL(stateChanged(int)), this, SLOT(slot_generaComando()));
    connect(ui->checkBoxMapaTopografico, SIGNAL(stateChanged(int)), this, SLOT(slot_generaComando()));
    connect(ui->checkBoxKml, SIGNAL(stateChanged(int)), this, SLOT(slot_generaComando()));
    connect(ui->rb_los, SIGNAL(clicked(bool)), this, SLOT(slot_generaComando()));
    connect(ui->lineEditAltAntena, SIGNAL(textChanged ( const QString &)), this, SLOT(slot_generaComando()));
    connect(ui->le_erp, SIGNAL(textChanged ( const QString &)), this, SLOT(slot_generaComando()));
    connect(ui->pBCalcular, SIGNAL(clicked()), this, SLOT(slot_ejecutaComando()));
    connect(ui->TxName1, SIGNAL(textChanged(QString)),this,SLOT(slot_ObtieneDatosTx()));
    connect(ui->RxName1, SIGNAL(textChanged(QString)),this,SLOT(slot_ObtieneDatosRx()));
    connect(ui->pushButtonGuardarConfiguracion,SIGNAL(clicked()),this,SLOT(slot_EscribeConfiguracion()));
    connect(ui->pBexDirTrabajo,SIGNAL(clicked()),this,SLOT(slot_dialogoDirectorio()));
    connect(ui->pBexDirSdf,SIGNAL(clicked()),this,SLOT(slot_dialogoDirectorio()));
    connect(ui->pBexDirBinario ,SIGNAL(clicked()),this,SLOT(slot_dialogoDirectorio()));
    connect(ui->pBTx1,SIGNAL(clicked()),this,SLOT(slot_dialogoArchivo()));
    connect(ui->pBRx1,SIGNAL(clicked()),this,SLOT(slot_dialogoArchivo()));
    connect(ui->pb_localizacionCiudades,SIGNAL(clicked()),this, SLOT(slot_dialogoArchivo()));
    connect(ui->le_potenciaTx, SIGNAL(textChanged(QString)),this,SLOT(slot_erp()));
    connect(ui->le_gananciadBi, SIGNAL(textChanged(QString)),this,SLOT(slot_erp()));
    connect(ui->le_lossTxAntenna, SIGNAL(textChanged(QString)),this,SLOT(slot_erp()));
    connect(ui->rb_PuntoPunto, SIGNAL(clicked(bool)),this,SLOT(slot_habilitaTab()));
    connect(ui->rb_prediccion, SIGNAL(clicked(bool)),this,SLOT(slot_habilitaTab()));
    connect(ui->rb_PuntoPunto, SIGNAL(clicked(bool)),this,SLOT(slot_generaComando()));
    connect(ui->rb_prediccion, SIGNAL(clicked(bool)),this,SLOT(slot_generaComando()));

    connect(ui->le_potenciaTxdBm, SIGNAL(textChanged(QString)),this, SLOT(slot_dBmtomW()));
    connect(ui->le_potenciaTx, SIGNAL(textChanged(QString)),this, SLOT(slot_mWtodBm()));
    connect(ui->rb_contornodBm, SIGNAL(clicked(bool)),this,SLOT(slot_generaComando()));
    connect(ui->rb_contornodBuV, SIGNAL(clicked(bool)),this,SLOT(slot_generaComando()));
    connect(ui->rb_los, SIGNAL(clicked(bool)),this,SLOT(slot_generaComando()));
    connect(ui->rb_perdidaTrayectoria, SIGNAL(clicked(bool)),this,SLOT(slot_generaComando()));
    connect(ui->le_umbraldB, SIGNAL(textChanged ( const QString &)), this, SLOT(slot_generaComando()));
    connect(ui->le_rango, SIGNAL(textChanged ( const QString &)), this, SLOT(slot_generaComando()));
    connect(ui->le_localizacionCiudades,SIGNAL(textChanged(QString)),this, SLOT(slot_generaComando()));
    connect(ui->le_clutter,SIGNAL(textChanged(QString)),this, SLOT(slot_generaComando()));


    connect(ui->actionImprimir,SIGNAL(triggered()),SLOT(imprimirReporte()));
    connect(ui->actionAcercaDe,SIGNAL(triggered()),SLOT(slot_acercaDe()));

    connect(ui->checkBoxNormalizada, SIGNAL(toggled(bool)),this, SLOT(slotHabilita()));
    connect(ui->rb_prediccion, SIGNAL(toggled(bool)), this, SLOT(slotHabilita()));
    connect(ui->checkBoxPerfil, SIGNAL(toggled(bool)), this, SLOT(slotHabilita()));
    connect(ui->checkBoxElevacion, SIGNAL(toggled(bool)), this, SLOT(slotHabilita()));
    connect(ui->checkBoxAltura, SIGNAL(toggled(bool)), this, SLOT(slotHabilita()));
    connect(ui->checkBoxPerdidasTrayectoria, SIGNAL(toggled(bool)), this, SLOT(slotHabilita()));
    connect(ui->checkBoxMapaTopografico, SIGNAL(toggled(bool)), this, SLOT(slotHabilita()));



    slotHabilita();

    ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_mapaTopografico));

    ui->tabWidget_2->setTabEnabled(2,false);


    procesoComando = new QProcess;
    connect(procesoComando,SIGNAL(finished(int)),this,SLOT(slot_comandoConvert()));
    connect(procesoComando, SIGNAL(readyReadStandardOutput()),this, SLOT(catchOutput()));
    //connect(procesoComando,SIGNAL(finished(int)),this,SLOT(slot_datosOK()));
    connect(ui->actionSalir,SIGNAL(triggered()),SLOT(close()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_generaComando()
{
    QString comando;
    comando = formaArgumentos().join(" ");
    ui->textCommand->setText("splat "+comando);

}

QStringList MainWindow::formaArgumentos()
{
    QString Tx=ui->TxName1->text();
    QString Rx=ui->RxName1->text();
    if(Tx.contains(" ")) Tx.replace(QString(" "),QString("\\ "));
    if(Rx.contains(" ")) Rx.replace(QString(" "),QString("\\ "));
    QStringList argumentos;
    argumentos<<"-t"<<Tx;
    argumentos<<"-f"<<ui->lineEditFrecuencia->text();
    argumentos<<"-d"<<ui->lineEditRutaSdf->text();
    argumentos<<"-fz"<<ui->lineEditFresnel->text();
    if(ui->cb_localizacionCiudades->isChecked()) argumentos<<"-s"<<ui->le_localizacionCiudades->text();
    if(ui->checkBoxMeters->isChecked()) argumentos<<"-metric";
    if(!ui->rb_perdidaTrayectoria->isChecked()) argumentos<< "-erp"<<ui->le_erp->text();
    QString stringmapaCobertura= "mapa_cobertura_"+ui->TxName1->text()+".ppm";
    argumentos<<"-gc"<<ui->le_clutter->text();

    if(ui->rb_prediccion->isChecked())
    {
        //calculo cobertura linea de vista
        if(ui->rb_los->isChecked())
        {
            argumentos<<"-c"<<ui->lineEditAltAntena->text();
        }
        //calculo cobertura potencia de señal en dBm
        else if(ui->rb_contornodBm->isChecked())
        {
            argumentos<<"-L"<<ui->lineEditAltAntena->text()<<"-dbm";
        }
        //calculo cobertura intensidad de campo
        else if (ui->rb_contornodBuV->isChecked())
        {
            argumentos<<"-L"<<ui->lineEditAltAntena->text();
        }
        //calculo perdida por trayectoria
        else if(ui->rb_perdidaTrayectoria->isChecked())
        {
            argumentos<<"-L"<<ui->lineEditAltAntena->text()<<"-erp 0";
        }
        argumentos<<"-o"<<stringmapaCobertura;
        argumentos<<"-db"<<ui->le_umbraldB->text();
        argumentos<<"-R"<<ui->le_rango->text();
        if(ui->checkBoxKml->isChecked()) argumentos <<"-kml";
    }
    else
    {
        argumentos<<"-r"<<Rx;
        if(ui->checkBoxPerfil->isChecked()) argumentos<<"-p"<<"perfil_"+Tx+"_"+Rx;
        if(ui->checkBoxElevacion->isChecked()) argumentos<<"-e"<<"elevacion_"+Tx+"_"+Rx;
        if(ui->checkBoxAltura->isChecked()) argumentos<<"-h"<<"altura_"+Tx+"_"+Rx;
        if(ui->checkBoxNormalizada->isChecked()) argumentos<<"-H"<<"alturaNormalizada_"+Tx+"_"+Rx;
        if(ui->checkBoxPerdidasTrayectoria->isChecked()) argumentos<<"-l"<<"perdida_"+Tx+"_"+Rx;
        if(ui->checkBoxMapaTopografico->isChecked()) argumentos<<"-o"<<"topografico_"+Tx+"_"+Rx;
        if(ui->checkBoxKml->isChecked()) argumentos<<"-kml";
    }




    return argumentos;
}

    void MainWindow::slot_ejecutaComando()
    {

        qDebug("ejecutando splat");
        timerVisualizaEstado->start(1000);
        cuentaProgressBar=0;
        incremento=true;
        slot_EscribeDatosTx();
        slot_EscribeDatosRx();
        ui->labelEstado->setText("Trabajando...");
        //QString comando("/usr/bin/splat");
        QString comando(ui->le_rutaBinario->text()+"/splat");
        /////////     QProcess *procesoComando = new QProcess;
        /////////     connect(procesoComando,SIGNAL(finished(int)),this,SLOT(slot_comandoConvert()));
        //connect(procesoComando,SIGNAL(finished(int)),this,SLOT(slot_datosOK()));
        procesoComando->setWorkingDirectory(ui->lineEditRutaTrabajo->text());
        procesoComando->start(comando, formaArgumentos());

    }

void MainWindow::slot_datosOK()
{    
    //qDebug()<<"checando..."<<ui->lineEditRutaTrabajo->text()+"/"+"alturaNormalizada_"+ui->TxName1->text()+"_"+ui->RxName1->text()+".png";
    QString datosTxRx = ui->TxName1->text()+"_"+ui->RxName1->text();
    ui->labelAlturaNormalizada->setPixmap(QPixmap(ui->lineEditRutaTrabajo->text()+"/"+"alturaNormalizada_"+datosTxRx+".png"));
    ui->labelEstado->setText("Altura Normalizada OK...");    
    ui->labelCoberturaRegional->setPixmap(QPixmap(ui->lineEditRutaTrabajo->text()+"/"+"mapa_cobertura_"+ui->TxName1->text()+".jpg"));
    ui->label_perfil->setPixmap(QPixmap(ui->lineEditRutaTrabajo->text()+"/"+"perfil_"+datosTxRx+".png"));
    ui->label_perdidaTrayectoria->setPixmap(QPixmap(ui->lineEditRutaTrabajo->text()+"/"+"perdida_"+datosTxRx+".png"));
    ui->label_elevacion->setPixmap(QPixmap(ui->lineEditRutaTrabajo->text()+"/"+"elevacion_"+datosTxRx+".png"));
    ui->label_altura->setPixmap(QPixmap(ui->lineEditRutaTrabajo->text()+"/"+"altura_"+datosTxRx+".png"));

    //"mapa_cobertura_"+ui->TxName1+".ppm"
    qDebug()<<"listillo"<<QDir::currentPath();
    ui->plainTextEditSiteReport->clear();
    int numero=0;
    numero+=1;
    qDebug()<<QString::number(numero);
    QFile file(ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+"-site_report.txt");
    numero+=1;
    qDebug()<<QString::number(numero);
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
     numero+=1;
     qDebug()<<QString::number(numero);
     while (!file.atEnd()) {
         numero+=1;
         qDebug()<<QString::number(numero);
         QByteArray line = file.readLine();
         //qDebug()<<"contiene: plainTextEditSiteReport"<<line;
         if(!line.isEmpty())
             ui->plainTextEditSiteReport->insertPlainText(line);
     }
     numero+=1;
     qDebug()<<QString::number(numero);
     numero+=1;
     file.close();
     qDebug()<<QString::number(numero);
     ui->labelEstado->setText("Reporte del sitio OK...");

     ui->plainTextEditAnalisisTxRx->clear();     
     //file.setFileName(ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+"-to-"+ui->RxName1->text()+".txt");
     file.setFileName(ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+"-to-"+ui->RxName1->text()+".txt");
     qDebug()<<"-x "<<ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+"-to-"+ui->RxName1->text()+".txt";
     if (file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
        //return;
        qDebug("creando arch....");
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            if(!line.isEmpty())
                ui->plainTextEditAnalisisTxRx->insertPlainText(line);
            }
        file.close();
     }
     ui->labelEstado->setText("Analisis Tx-RX OK...");
     ui->labelEstado->setText("Terminado...");
     timerVisualizaEstado->stop();
     ui->progressBar->setValue(100);
}

void MainWindow::configuracion()
{    
    QDir directorioConfig;
    if(directorioConfig.exists(QDir::homePath()+"/.qsplat"))
    {
        qDebug("exiiiiste");
        QFile file(QDir::homePath()+"/.qsplat/config");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;

     QTextStream in(&file);
     while (!in.atEnd()) {
         QString line = in.readLine();
         //process_line(line);
         qDebug()<<"->"<<line;
         if(line.startsWith("workDirectoryAbsolutePath"))
         {             
             ui->lineEditRutaTrabajo->setText(line.section("=",1,-1));
             qDebug()<<"seccion-->"<<line.section("=",1,-1);

         }
         if(line.startsWith("sdfDirectory"))
         {
             ui->lineEditRutaSdf->setText(line.section("=",1,-1));
             qDebug()<<"seccion-->"<<line.section("=",1,-1);
         }
         if(line.startsWith("pathBin"))
         {
             ui->le_rutaBinario->setText(line.section("=",1,-1));
             qDebug()<<"seccion-->"<<line.section("=",1,-1);
         }

     }

    }
    else
    {
        directorioConfig.mkdir(QDir::homePath()+"/.qsplat");
        qDebug()<<"creando directorio: "<<QDir::homePath()<<"/.qsplat";
    }
}

void MainWindow::slot_ObtieneDatosTx()
{
    //QFile file(ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+"-site_report.txt");

    qDebug()<<"leyendo archivo:"<<ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+".qth";
    QFile file(ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+".qth");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
     qDebug("entrando a leer");
     int cuenta=0;
     while (!file.atEnd()) {
         QByteArray line = file.readLine();
         //process_line(line);
         if(cuenta==1) ui->TxLat1->setText(line);
         if(cuenta==2) ui->TxLong1->setText(line);
         if(cuenta==3) ui->TxLenght1->setText(line);
         cuenta++;
     }
     file.close();
}

void MainWindow::slot_ObtieneDatosRx()
{
    //QFile file(ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+"-site_report.txt");
    QFile file(ui->lineEditRutaTrabajo->text()+"/"+ui->RxName1->text()+".qth");
     if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
     int cuenta=0;
     while (!file.atEnd()) {
         QByteArray line = file.readLine();
         //process_line(line);
         if(cuenta==1) ui->RxLat1->setText(line);
         if(cuenta==2) ui->RxLong1->setText(line);
         if(cuenta==3) ui->RxLenght1->setText(line);
         cuenta++;
     }
     file.close();
}

void MainWindow::slot_EscribeDatosTx()
{    
    qDebug()<<"Escribiendo datos Tx."<<endl;
    QFile file(ui->lineEditRutaTrabajo->text()+"/"+ui->TxName1->text()+".qth");
    qDebug()<<"Escribiendo datos Tx..";
    //if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    //{
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;     
         qDebug()<<"Escribiendo datos Tx...";
         QTextStream out(&file);
         out << ui->TxName1->text().trimmed()<<"\n"
             << ui->TxLat1->text().trimmed()<<"\n"
             << ui->TxLong1->text().trimmed()<<"\n"
             << ui->TxLenght1->text().trimmed();
         return;
     //}
     file.close();
     qDebug()<<"Escibiendo datos Tx....";
}

void MainWindow::slot_EscribeDatosRx()
{
    QFile file(ui->lineEditRutaTrabajo->text()+"/"+ui->RxName1->text()+".qth");
         if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
         QTextStream out(&file);
         out << ui->RxName1->text().trimmed()<<"\n"
             << ui->RxLat1->text().trimmed()<<"\n"
             << ui->RxLong1->text().trimmed()<<"\n"
             << ui->RxLenght1->text();
         return;     
     file.close();
}

void MainWindow::slot_EscribeConfiguracion()
{
    QDir directorioConfig;
    if(directorioConfig.exists(QDir::homePath()+"/.qsplat"))
    {
        qDebug("exiiiiste .qsplat");
    }
    else
    {
        directorioConfig.mkdir(QDir::homePath()+"/.qsplat");
        qDebug()<<"creando directorio: "<<QDir::homePath()<<"/.qsplat";        
    }
    QFile file(QDir::homePath()+"/.qsplat/config");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "workDirectoryAbsolutePath="<< ui->lineEditRutaTrabajo->text()<<endl
        << "sdfDirectory="<< ui->lineEditRutaSdf->text()<<endl
        << "pathBin="<< ui->le_rutaBinario->text();
    return;
    file.close();

}

void MainWindow::slot_dialogoDirectorio()
{
    qDebug("filedialogo Directorio");    

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 ui->lineEditRutaTrabajo->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

    if(ui->pBexDirTrabajo->isChecked())
    {
        ui->lineEditRutaTrabajo->setText(dir);
        ui->pBexDirTrabajo->setChecked(false);
    }
    if(ui->pBexDirSdf->isChecked())
    {
        ui->lineEditRutaSdf->setText(dir);
        ui->pBexDirSdf->setChecked(false);
    }
    if(ui->pBexDirBinario->isChecked())
    {
        ui->le_rutaBinario->setText(dir);
        ui->pBexDirBinario->setChecked(false);
    }

}

void MainWindow::slot_dialogoArchivo()
{
    qDebug("filedialogo Archivo");

    QString fileName;
    if(ui->pBTx1->isChecked() || ui->pBRx1->isChecked() || ui->pb_localizacionCiudades->isChecked())
        fileName = QFileDialog::getOpenFileName(this, tr("Open Archive"), ui->lineEditRutaTrabajo->text(),"Archivos (*.qth *.dat)");
    
    if(ui->pBTx1->isChecked())
    {
        QString archivoQth=fileName.section("/",-1);
        ui->TxName1->setText(archivoQth.section(".",0,0));
        ui->pBTx1->setChecked(false);
    }
    if(ui->pBRx1->isChecked())
    {
        QString archivoQth=fileName.section("/",-1);
        ui->RxName1->setText(archivoQth.section(".",0,0));
        ui->pBRx1->setChecked(false);
    }
    if(ui->pb_localizacionCiudades->isChecked())
    {
        //QString archivo=fileName.section("/",-1);
        QString archivo = fileName;
        //ui->le_localizacionCiudades->setText(archivo.section(".",0,0));
        ui->le_localizacionCiudades->setText(archivo);
        ui->pb_localizacionCiudades->setChecked(false);
    }


}

void MainWindow::slot_comandoConvert()
{
    qDebug("ejecutando convert 1");
    QString comando("/usr/bin/convert");
    QProcess *procesoComando = new QProcess;
    connect(procesoComando,SIGNAL(finished(int)),this,SLOT(slot_datosOK()));

    procesoComando->setWorkingDirectory(ui->lineEditRutaTrabajo->text());
    //procesoComando->start(comando, formaArgumentos());

    QStringList argumentosConvert;
    argumentosConvert<< ui->lineEditRutaTrabajo->text()+"/"+"mapa_cobertura_"+ui->TxName1->text()+".ppm"
                     << "-trim"<<"+repage"
                     <<ui->lineEditRutaTrabajo->text()+"/"+"mapa_cobertura_"+ui->TxName1->text()+".jpg";
    qDebug()<< ui->lineEditRutaTrabajo->text()+"/"+"mapa_cobertura_"+ui->TxName1->text()+".ppm"
            << "-trim"<<"+repage"
            <<ui->lineEditRutaTrabajo->text()+"/"+"mapa_cobertura_"+ui->TxName1->text()+".jpg";


    procesoComando->start(comando, argumentosConvert);    


}

void MainWindow::slot_ActualizaProgressBar()
{
    if(incremento)
    {        
        ui->progressBar->setInvertedAppearance(false);        
        if(cuentaProgressBar>=100)
        {
            cuentaProgressBar=0;
            incremento=false;
            ui->progressBar->setInvertedAppearance(true);
        }
    }
    else
    {
        ui->progressBar->setInvertedAppearance(true);
        if(cuentaProgressBar>=100)
        {
            cuentaProgressBar=0;
            incremento=true;
            ui->progressBar->setInvertedAppearance(false);
        }
    }    

    cuentaProgressBar+=10;

    ui->progressBar->setValue(cuentaProgressBar);
}

void MainWindow::slot_erp()
{
    QString a = ui->le_lossTxAntenna->text();
    QString b= ui->le_gananciadBi->text();
    QString c= ui->le_potenciaTx->text();
    double perdida = pow(10.0, a.toDouble()/10.0);
    double ganancia = pow(10.0, b.toDouble()/10.0);
    double eirp = c.toDouble()*perdida*ganancia;
    double erp = eirp / pow(10,2.14/10.0);
    ui->le_erp->setText(QString::number(erp));
    ui->le_eirp->setText(QString::number(eirp));
}

void MainWindow::slot_habilitaTab()
{
    if(ui->rb_PuntoPunto->isChecked())
    {        
        ui->tabWidget_2->setTabEnabled(1,true);
        ui->tabWidget_2->setTabEnabled(2,false);
        ui->frame_rx->setEnabled(true);

    }
    else
    {
        ui->tabWidget_2->setTabEnabled(1,false);
        ui->tabWidget_2->setTabEnabled(2,true);
        ui->frame_rx->setEnabled(false);
    }
}

void MainWindow::slot_dBmtomW()
{
    //ui->le_potenciaTx();
    //ui->le_potenciaTxdBm();
    //myObject->disconnect(myReceiver);

    qDebug("dBmtomW");
    ui->le_potenciaTx->disconnect(this);
    QString a = ui->le_potenciaTxdBm->text();
    double pmW = pow(10.0,a.toDouble()/10.0);
    double pWatts = pmW/1000.0;
    ui->le_potenciaTx->setText(QString::number(pWatts));    
    connect(ui->le_potenciaTx, SIGNAL(textChanged(QString)),this, SLOT(slot_mWtodBm()));
}

void MainWindow::slot_mWtodBm()
{
    //ui->le_potenciaTx();
    //ui->le_potenciaTxdBm();
    qDebug("mWtodBm");
    ui->le_potenciaTxdBm->disconnect(this);
    QString a = ui->le_potenciaTx->text();
    double dBm = 10.0*log10(a.toDouble()*1000.0);
    ui->le_potenciaTxdBm->setText(QString::number(dBm));
    connect(ui->le_potenciaTxdBm, SIGNAL(textChanged(QString)),this, SLOT(slot_dBmtomW()));
}

void MainWindow::catchOutput()
{
    while(procesoComando->canReadLine())
    {
        QByteArray buffer(procesoComando->readLine());
        ui->log->append(QString(buffer));
    }
}

void MainWindow::imprimirReporte()
{    

    textprinter_ = new TextPrinter(this);
    textprinter_->setHeaderSize(10);
    textprinter_->setFooterSize(10);
    textprinter_->setDateFormat("MMMM dd yyyy");

    QString headertext =
        "<table width=\"100%\">"
        "  <tr>"
        "    <td align=\"left\"><strong>%1</strong></td>"
        "    <td align=\"right\"><strong>&date;</strong></td>"
        "  </tr>"
        "</table>";

    //QString footertext = "<p align=\"right\"><strong>%1 &page;</strong></p>";
    QString footertext = "<table width=\"100%\">"
                         "  <tr>"
                         "    <td align=\"left\"><strong>%1</strong></td>"
                         "    <td align=\"right\"><strong>Pag. &page;</strong></td>"
                         "  </tr>"
                         "</table>";

    QString html;
    QString alturaNormalizada;
    QString datosTxRx = ui->TxName1->text()+"_"+ui->RxName1->text();

    html += ui->plainTextEditAnalisisTxRx->toPlainText();
    html += ui->plainTextEditSiteReport->toPlainText();
    html = Qt::convertFromPlainText(html);

    html += "<img src=";
    html += ui->lineEditRutaTrabajo->text()+"/"+"alturaNormalizada_"+datosTxRx+".png";
    html += ">";

    html += "<img src=";
    html += ui->lineEditRutaTrabajo->text()+"/"+"perfil_"+datosTxRx+".png";
    html += ">";

    html += "<img src=";
    html += ui->lineEditRutaTrabajo->text()+"/"+"perdida_"+datosTxRx+".png";
    html += ">";

    html += "<img src=";
    html += ui->lineEditRutaTrabajo->text()+"/"+"elevacion_"+datosTxRx+".png";
    html += ">";

    html += "<img src=";
    html += ui->lineEditRutaTrabajo->text()+"/"+"altura_"+datosTxRx+".png";
    html += ">";



    QTextDocument *textDocument = new QTextDocument(this);
    textDocument->setHtml(html);
    textprinter_->setHeaderText(headertext.arg("QRfSplat"));
    textprinter_->setFooterText(footertext.arg("Datos de enlace de "+ui->TxName1->text()+" a "+ui->RxName1->text()));
    textprinter_->preview(textDocument, "Reporte QRfSplat");

}

void MainWindow::slotHabilita()
{
    if(ui->checkBoxNormalizada->isChecked())
    {
        ui->tabWidget->addTab(ui->tab_altNormalizada,QString("Alt. Normalizada"));
    }
    else
    {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_altNormalizada));
    }

    if(ui->rb_prediccion->isChecked())
    {
        ui->tabWidget->addTab(ui->tab_coberturaRegional,QString("Cobertura regional"));
    }
    else
    {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_coberturaRegional));
    }

    if(ui->checkBoxPerfil->isChecked())
    {
        ui->tabWidget->addTab(ui->tab_perfil,QString("Perfil"));
    }
    else
    {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_perfil));
    }

    if(ui->checkBoxElevacion->isChecked())
    {
        ui->tabWidget->addTab(ui->tab_elevacion,QString("Elevacion"));
    }
    else
    {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_elevacion));
    }

    if(ui->checkBoxAltura->isChecked())
    {
        ui->tabWidget->addTab(ui->tab_altura,QString("Altura"));
    }
    else
    {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_altura));
    }

    if(ui->checkBoxPerdidasTrayectoria->isChecked())
    {
        ui->tabWidget->addTab(ui->tab_perdidaTrayectoria,QString("Perdida por trayectoria"));
    }
    else
    {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_perdidaTrayectoria));
    }

    if(ui->checkBoxMapaTopografico->isChecked())
    {
        ui->tabWidget->addTab(ui->tab_mapaTopografico,QString("Mapa topografico"));
    }
    else
    {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->tab_mapaTopografico));
    }


}





void MainWindow::slot_acercaDe()
{
   QMessageBox::about(this, tr("Acerca de QRfSplat"),
            tr("<b>QRfSplat 1.0</b> <br/> Un sencillo front-end para "
               "<a href=\"http://www.qsl.net/kd2bd/splat.html\">Splat!</a><br/>"
               "Derechos reservados (c) 2010-2016 <br/> Luis Enrique Meneses Wong<br/>"
               "<a href=\"https://github.com/dasnetz/qrfsplat\">https://github.com/dasnetz/qrfsplat</a>"));
}
