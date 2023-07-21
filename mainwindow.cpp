#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    maxLabelHeight = 0;
    maxLabelWidth = 0;
    connect(ui->gradient10, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
    connect(ui->gradient17, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
    connect(ui->gradient23, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
    connect(ui->gradient38, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
    ui->label->setAutoFillBackground(true);
    QPalette palette = ui->label->palette();
    palette.setColor(QPalette::Window, QColor::fromRgb(0, 0, 0));
    palette.setColor(QPalette::WindowText, QColor::fromRgb(255, 255, 255));
    ui->label->setPalette(palette);
}

MainWindow::~MainWindow()
{
    disconnect(ui->gradient10, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
    disconnect(ui->gradient17, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
    disconnect(ui->gradient23, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
    disconnect(ui->gradient38, &QRadioButton::clicked, this, &MainWindow::radioButtonClicked);
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if (maxLabelHeight == 0 || maxLabelWidth == 0)
    {
        maxLabelHeight = ui->label->height();
        maxLabelWidth = ui->label->width();

    }

    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "/home", "Images (*.png *.bmp *.jpg)");

    if (!fileName.isEmpty())
    {
        if (image.load(fileName))
        {
            image = image.convertToFormat(QImage::Format_Grayscale8);
            asciinize(image);
        }
        else
        {
            QMessageBox::critical(this, "Ошибка!", "Не удалось загрузить изображение");
        }
    }
}

void MainWindow::radioButtonClicked()
{
    if (!image.isNull())
        asciinize(image);
}

void MainWindow::asciinize(QImage &image)
{
    QFontMetrics fm(ui->label->font());
    image = image.scaled(maxLabelWidth / fm.horizontalAdvance('.'), maxLabelHeight / fm.height(),
                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setFixedHeight(image.height() * fm.height());
    ui->label->setFixedWidth(image.width() * fm.horizontalAdvance('.') * 2);
    QString gradient;
    if (ui->gradient10->isChecked())
        gradient = " .:-*+=#%@";
    if (ui->gradient17->isChecked())
        gradient = " `.',^<+cxeOD#0&@";
    if (ui->gradient23->isChecked())
        gradient = " `.':,\"~<+v7cxePORD#0&@";
    if (ui->gradient38->isChecked())
        gradient = " `.:,\"-^\\~*<;>+rv7zx=5ShH6dR#N8B%$Q&g@";
    QString screen;
    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            float color = static_cast<int>(image.pixelColor(x, y).valueF() * gradient.size());
            if (color == gradient.size()) --color;
            screen.append(gradient[color]);
            screen.append(gradient[color]);
        }
        screen.append('\n');
    }
    ui->label->setText(screen);
}
