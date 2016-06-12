#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
class QStringList;
#include <QTimer>
#include <QTextDocument>
class TextPrinter;




namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    QStringList formaArgumentos();
    void configuracion();
    QTimer *timerVisualizaEstado;
    int cuentaProgressBar;
    bool incremento;
    QProcess *procesoComando;
    TextPrinter *textprinter_;


private slots:
    /// genera los argumentos para el comando splat
    void slot_generaComando();
    void slot_ejecutaComando();
    void slot_datosOK();
    void slot_ObtieneDatosTx();
    void slot_ObtieneDatosRx();
    void slot_EscribeDatosTx();
    void slot_EscribeDatosRx();
    void slot_EscribeConfiguracion();
    void slot_dialogoDirectorio();
    void slot_dialogoArchivo();
    void slot_comandoConvert();
    void slot_ActualizaProgressBar();
    void slot_erp();
    void slot_habilitaTab();
    void slot_dBmtomW();
    void slot_mWtodBm();
    void catchOutput();
    void imprimirReporte();
    void slotHabilita();
    void slot_acercaDe();
};

#endif // MAINWINDOW_H
