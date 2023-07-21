#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "/home", "Images (*.png *.bmp *.jpg)");
    if (!fileName.isEmpty())
    {
        QImage image;
        if (image.load(fileName))
        {
            image = image.convertToFormat(QImage::Format_Grayscale8);
            ui->label->setPixmap(QPixmap::fromImage(image));
            asciinize(image);
        }
        else
        {
            QMessageBox::critical(this, "Ошибка!", "Не удалось загрузить изображение");
        }
    }
}

void MainWindow::asciinize(QImage &image)
{
    QFontMetrics fm(ui->label->font());
    image = image.scaled(ui->label->width() / fm.horizontalAdvance('.'), ui->label->height() / fm.height(),
                         Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label->setFixedHeight(image.height() * fm.height());
    ui->label->setFixedWidth(image.width() * fm.horizontalAdvance('.') * 2);
    QString gradient = " `.',^<+cxeOD#0&@";
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

