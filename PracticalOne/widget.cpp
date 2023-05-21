#include "widget.h"
#include "ui_widget.h"
#include "QPushButton"
#include "QBoxLayout"
#include "QMessageBox"
#include "QDialogButtonBox"
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QCheckBox>
#include "QInputDialog"
#include <QButtonGroup>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QFileInfo>




QDateTime Widget::transforToDateTime(int day, int month, int year, int hh, int mm, int ss ){
    //Hay que ver lo de AM o PM
    // Crear una instancia de QDate con los valores de día, mes y año
    QDate date(year, month, day);

    // Crear una instancia de QTime con los valores de hora, minuto y segundo
    QTime time(hh,mm, ss);

    // Combinar la fecha y la hora para crear un QDateTime
    QDateTime dateTime(date, time);

    // Ajustar el desplazamiento horario (offset) según el valor de sHorario
    //dateTime = dateTime.addSecs(sHorario * 3600);  // Convertir horas a segundos

    return dateTime;

}
void Widget::interfazResposive(){



    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);



    // Ejemplo de uso de QBoxLayout para organizar elementos en una disposición horizontal
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(ui->buttonMenu);
    buttonLayout->addWidget(ui->buttonAtras);
    buttonLayout->addWidget(ui->mesNow);
    buttonLayout->addWidget(ui->labelMonth);
    buttonLayout->addWidget(ui->spinBoxAnio);
    buttonLayout->addWidget(ui->labelYear);
    buttonLayout->addWidget(ui->buttonNextMonth);
    mainLayout->addLayout(buttonLayout);




    QHBoxLayout* btnTableLayout = new QHBoxLayout;
    QVBoxLayout* buttonLayout2 = new QVBoxLayout;
    buttonLayout2->addWidget(ui->crearEvento);
    buttonLayout2->addSpacing(50);
    buttonLayout2->addWidget(ui->ButtonDeleteEvent);
    buttonLayout2->addStretch();
    buttonLayout2->addSpacing(20);
    buttonLayout2->addWidget(ui->buttonExit);
    buttonLayout2->addStretch();
    btnTableLayout->addLayout(buttonLayout2);


    // Configurar tamaño mínimo y máximo para el QTableWidget
    QHeaderView* header = ui->tablecalendar->horizontalHeader();
    header->setMinimumSectionSize(60);
    header->setMaximumSectionSize(100);
    QHeaderView* headerV = ui->tablecalendar->verticalHeader();
    headerV->setMinimumSectionSize(90);
    headerV->setMaximumSectionSize(110);
    header->setSectionResizeMode(QHeaderView::Stretch);
    // Agregar el QTableWidget al QHBoxLayout
    btnTableLayout->addWidget(ui->tablecalendar);


    // Establecer la política de tamaño del QHBoxLayout
    btnTableLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    mainLayout->addLayout(btnTableLayout);
    // Establece políticas de tamaño para los elementos
    ui->buttonAtras->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->buttonNextMonth->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mesNow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->spinBoxAnio->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->crearEvento->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->tablecalendar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    // Establece propiedades de tamaño mínimo y máximo para los elementos
    ui->buttonNextMonth->setMinimumSize(100, 50);
    ui->buttonNextMonth->setMaximumSize(200, 100);
    ui->buttonAtras->setMinimumSize(100, 50);
    ui->buttonAtras->setMaximumSize(200, 100);
    ui->mesNow->setMaximumSize(200,50);
    ui->crearEvento->setMinimumSize(50,70);
    ui->crearEvento->setMaximumSize(150,100);
    ui->tablecalendar->setMinimumWidth(580);
    ui->tablecalendar->setMaximumWidth(2500);
    ui->tablecalendar->setMinimumHeight(800);
    ui->tablecalendar->setMaximumHeight(900);




}
void Widget::updateCalendar(int year,int month )
{


    // Obtener el número de días en el mes y el día de la semana en que comienza
    QDate date(year, month, 1);
    int numDays = date.daysInMonth();
    int dayBegin = date.dayOfWeek() - 1;

    // Actualizar la tabla del calendario con el número correcto de días
    int row = 0; // Comenzar a agregar días a la primera fila
    int col = dayBegin;

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < ui->tablecalendar->columnCount(); ++j) {
            QTableWidgetItem *item = ui->tablecalendar->item(i, j);
            if (item) {
                delete item;
            }
            item = new QTableWidgetItem;
            ui->tablecalendar->setItem(i, j, item);
        }
    }

    for (int day = 1; day <= numDays; ++day) {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString::number(day));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tablecalendar->setItem(row, col, item);
        col = (col + 1) % 7; // Cambiar a la siguiente columna después de agregar un día
        if (col == 0) {
            ++row; // Cambiar a la siguiente fila después de agregar una semana completa
        }
    }

    // Rellenar las celdas vacías al final del mes con el primer día del mes siguiente
    if (col != 0) {
        QDate nextMonth = date.addMonths(1);
        int daysToAdd = 7 - col;
        for (int i = 0; i < daysToAdd; ++i) {
            QTableWidgetItem *item = new QTableWidgetItem;
            ui->tablecalendar->setItem(row, col, item);
            col = (col + 1) % 7;
            if (col == 0) {
                ++row;
            }
        }
        dayBegin = col;
        date = nextMonth;
        numDays = date.daysInMonth();
    }

    // Rellenar las celdas vacías al principio del mes con el último día del mes anterior
    if (dayBegin != 0) {
        QDate prevMonth = date.addMonths(-1);
        int prevNumDays = prevMonth.daysInMonth();
        for (int i = dayBegin - 1; i >= 0; --i) {
            QTableWidgetItem *item = new QTableWidgetItem;
            ui->tablecalendar->setItem(0, i, item);
        }
    }
}


class dateNow{
    public:
    dateNow(){
        QDate currentDate = QDate::currentDate();
        day=currentDate.day();
        month=currentDate.month();
        year=currentDate.year();
    }
    ~dateNow(){

    }

    int getDay(){
        return day;
    }

    int getYear(){
        return year;
    }

    int getMonth(){
        return month;
    }
    private:
    int day, month, year;

};
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{



    dateNow dateNow1;
    ui->setupUi(this);
    m_exists=QFileInfo("data.sqlite").exists();
    m_db=QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("data.sqlite");

     m_db.open();


    //populateDatabase("Primer", QDateTime::fromString("2023-10-17 07:59", "yyyy-MM-dd hh:mm"), QDateTime::fromString("2023-10-17 07:59", "yyyy-MM-dd hh:mm"));

    m_model=new QSqlTableModel(this);
    m_model->setTable("Evento");
    m_model->select();
    ui->TableBase->setModel(m_model);

    ui->ButtonDeleteEvent->setText("Eliminar\nEvento");
    ui->crearEvento->setText("Agregar\nEvento");
    ui->spinBoxAnio->setRange(1,999999999);
    ui->spinBoxAnio->setValue(dateNow1.getYear());


    interfazResposive();

    const QString month[] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre",
                             "Noviembre","Diciembre"};

    for (int i = 0; i < (sizeof(month)/sizeof(month[0])); i++) {
        ui->mesNow->addItem(month[i]);
    }


    ui->mesNow->setCurrentIndex((dateNow1.getMonth())-1);
    ui->mesNow->setStyleSheet("background-color: white; QComboBox::drop-down { background-color: red; }");


   ui->tablecalendar->setRowCount(6);
   ui->tablecalendar->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tablecalendar->setColumnCount(7);

   ui->tablecalendar->setStyleSheet("QTableView {border: none;}");
   ui->tablecalendar->verticalHeader()->setVisible(false);
   QStringList labels {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"};
   ui->tablecalendar->setHorizontalHeaderLabels(labels);
   ui->tablecalendar->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   ui->tablecalendar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



   int numColumns = ui->tablecalendar->columnCount();
   for (int i = 0; i < numColumns; i++) {
        ui->tablecalendar->setColumnWidth(i, 85);
   }

  // Establecer una altura de 30 píxeles para todas las filas
   for (int i = 0; i < ui->tablecalendar->rowCount(); i++) {
        ui->tablecalendar->setRowHeight(i, 60);
   }

   // Obtener el mes y el año seleccionados
   int monthAhora = ui->mesNow->currentIndex() + 1; // El índice del mes comienza desde 0
   int yearAhora = ui->spinBoxAnio->value();
   updateCalendar(yearAhora,monthAhora);
   connect(ui->spinBoxAnio, SIGNAL(valueChanged(int)), this, SLOT(updateCalendar()));
   connect(ui->mesNow, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCalendar()));



    //Darle tamaño al primer frame de der a izq

   ui->frame->setFixedHeight(1000);
   ui->frame->setFixedWidth(140);
}

Widget::~Widget()
{
    delete ui;
}


//Interfaz y boton de agregar eventos

void Widget::on_crearEvento_clicked()
{


    dateNow dateNow1;
    QDialog dialog(this);
    dialog.setWindowTitle("Crear Evento");
    QVBoxLayout layout(&dialog);

    QHBoxLayout nameEventLayout;

    // Campo del nombre del evento
    QLineEdit evento(&dialog);
    QFont font("Arial", 12, QFont::Bold); // Fuente Arial, tamaño 12, negrita
    QLabel nomb("Nombre del evento", &dialog);
    nomb.setFont(font); // Aplicar la fuente en negrita al QLabel
    nameEventLayout.addWidget(&nomb);
    nameEventLayout.addWidget(&evento);

    layout.addLayout(&nameEventLayout);



    QHBoxLayout dateBeginEventLayout;
    // Campo de año
    QSpinBox yearSpinBox(&dialog);
    yearSpinBox.setRange(1, 99999);
    yearSpinBox.setValue(QDate::currentDate().year());
    QLabel yearLabel("Año:", &dialog);
        dateBeginEventLayout.addWidget(&yearLabel);
    dateBeginEventLayout.addWidget(&yearSpinBox);
    int year = yearSpinBox.value();



    // Campo de mes
    QDate DateMonth = QDate::currentDate();
    int monthNow = DateMonth.month();
    const QStringList monthList = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                                   "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    QComboBox monthComboBox(&dialog);
    monthComboBox.addItems(monthList);
    QLabel monthLabel("Mes:", &dialog);
    monthComboBox.setCurrentIndex(monthNow-1);
    dateBeginEventLayout.addWidget(&monthLabel);
    dateBeginEventLayout.addWidget(&monthComboBox);
    int month = monthComboBox.currentIndex();


    // Campo de día
    QSpinBox daySpinBox(&dialog);
    daySpinBox.setRange(1,31);
    QLabel dayLabel("Día:", &dialog);
        dateBeginEventLayout.addWidget(&dayLabel);
    dateBeginEventLayout.addWidget(&daySpinBox);
    layout.addLayout(&dateBeginEventLayout);

    // Conexión de señales
    QObject::connect(&yearSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int year) {
        int month = monthComboBox.currentIndex() + 1;
        int maxDays = QDate(year, month, 1).daysInMonth();
        daySpinBox.setRange(1, maxDays);
    });
    QObject::connect(&monthComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](int index) {
        int year = yearSpinBox.value();
        int month = index + 1;
        int maxDays = QDate(year, month, 1).daysInMonth();
        daySpinBox.setRange(1, maxDays);
    });


    // Campo de hora
    QHBoxLayout hourLayout;
    QSpinBox hourSpinBox(&dialog);
    hourSpinBox.setRange(0, 12);
    hourSpinBox.setValue(QTime::currentTime().hour());
    QLabel hourLabel("Hora:", &dialog);
    hourLayout.addWidget(&hourLabel);
    hourLayout.addWidget(&hourSpinBox);
    hourLabel.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hourSpinBox.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Campo de minutos
    QSpinBox minuteSpinBox(&dialog);
    minuteSpinBox.setRange(0, 59);
    minuteSpinBox.setValue(QTime::currentTime().minute());
    QLabel minuteLabel("Minutos:", &dialog);
    hourLayout.addWidget(&minuteLabel);
    hourLayout.addWidget(&minuteSpinBox);

    QCheckBox *checkAm = new QCheckBox(nullptr);
    checkAm->setText("AM");
    hourLayout.addWidget(checkAm);
    checkAm->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QCheckBox *checkPm = new QCheckBox(nullptr);
    checkPm->setText("PM");
    hourLayout.addWidget(checkPm);
    checkPm->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QButtonGroup *buttonGroup = new QButtonGroup(nullptr);
    buttonGroup->addButton(checkAm);
    buttonGroup->addButton(checkPm);
    buttonGroup->setExclusive(true);

    layout.addLayout(&hourLayout);

    // Campo de detalles del evento
    QLineEdit asunto(&dialog);
    QLabel tex("Detalles del evento", &dialog);
    tex.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    asunto.setMinimumHeight(40); // Ajusta la altura mínima del QLineEdit
    layout.addWidget(&tex);
    layout.addWidget(&asunto);





    // Botones de aceptar y cancelar
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    QPushButton* acceptButton = buttonBox.button(QDialogButtonBox::Ok);
    acceptButton->setText("MiAceptar");




    layout.addWidget(&buttonBox);


    // Conectar los botones aceptar y cancelar
    QObject::connect(acceptButton, &QPushButton::clicked, [&]() {
        int day = daySpinBox.value();
        int month = monthComboBox.currentIndex() + 1;
        int year = yearSpinBox.value();
        QString nombre = evento.text();
        QString detalles = asunto.text();
        //populateDatabase();
        //insertEvento(nombre, day, month, year, detalles); // Insertar el evento en la base de datos
        dialog.accept(); // Cerrar el diálogo después de insertar el evento
    });
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Muestra el diálogo y espera a que el usuario seleccione "Aceptar" o "Cancelar"
    if (dialog.exec() == QDialog::Accepted) {
        int day = daySpinBox.value();
        int month = monthComboBox.currentIndex() + 1; // Los índices comienzan en 0
        int year = yearSpinBox.value();
        int hour = hourSpinBox.value();
        QString nombreEvento = evento.text();

        QDateTime d1=transforToDateTime(day, month, year, hour, 45, 23);
        QDateTime d2=transforToDateTime(day+1, month+2, year, hour, 45, 23);



        populateDatabase(nombreEvento, d1, d2);
    }
}


void Widget::on_buttonExit_clicked()
{
    if (QMessageBox::question(this, "Pregunta", "¿Seguro desea salir de la aplicación?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        close();
    } else {
        QMessageBox::information(this, "Mensaje", "Continúa en la aplicación");
    }
}


void Widget::on_mesNow_activated(int index)
{
    // Obtener el valor actual del spinBoxAnio
    int yearSelect = ui->spinBoxAnio->value();

    // Actualizar el comboBoxMes
    ui->mesNow->setCurrentIndex(index);

    // Actualizar el calendario con los valores actuales de ambos widgets
    updateCalendar(yearSelect, index+1);
}

void Widget::on_spinBoxAnio_valueChanged(int arg1)
{
    // Obtener el valor actual del comboBoxMes
    int monthSelect = ui->mesNow->currentIndex() + 1;


    // Actualizar el calendario con los valores actuales de ambos widgets
    updateCalendar(arg1, monthSelect);
}


void Widget::on_buttonAtras_clicked()
{

    int yearSelect = ui->spinBoxAnio->value();
    int monthSelect = ui->mesNow->currentIndex() ;
    if(monthSelect>1){
        monthSelect--;
        ui->mesNow->setCurrentIndex(monthSelect - 1);
        updateCalendar(yearSelect, monthSelect);
    }

}


void Widget::on_buttonNextMonth_clicked()
{
    int yearSelect = ui->spinBoxAnio->value();
    int monthSelect = ui->mesNow->currentIndex()+1;
    if(monthSelect<12){
        monthSelect++;
        ui->mesNow->setCurrentIndex(monthSelect-1);
        updateCalendar(yearSelect, monthSelect);
    }
}

void Widget::populateDatabase(QString nombreEvento,QDateTime fechaTimeBegin, QDateTime fechaTimeEnd){
    if(m_exists==false){
        QString sql="create table if not exists Evento(id integer primary key autoincrement, nombreEvento text, fechaTimeBegin DATETIME, fechaTimeEnd DATETIME)";
        QSqlQuery query;
        query.prepare(sql);
        query.exec();

    }

    insertPerson(nombreEvento, fechaTimeBegin , fechaTimeEnd);
}

void Widget::insertPerson(const QString nombreEvento, QDateTime fechaTimeBegin, QDateTime fechaTimeEnd)const{
    QString sql="insert into Evento(nombreEvento, fechaTimeBegin, fechaTimeEnd) values(?,?,?)";
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(nombreEvento);
    query.addBindValue(fechaTimeBegin);
    query.addBindValue(fechaTimeEnd);
    query.exec();
}




