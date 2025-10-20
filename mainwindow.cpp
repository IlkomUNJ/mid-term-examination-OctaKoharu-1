#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Create a central widget to hold the layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *subLayout = new QHBoxLayout();

    // Create and add the drawing canvas to the top of the layout
    m_canvas = new DrawingCanvas(this);
    mainLayout->addWidget(m_canvas);

    // Create buttons
    m_clearButton = new QPushButton("Clear Canvas", this);
    m_detectButton = new QPushButton("Detect Segment", this);
    m_drawButton = new QPushButton("Draw Lines", this);
    m_scanButton = new QPushButton("Scan Window", this);

    // Add buttons to layout
    mainLayout->addLayout(subLayout, 1);
    subLayout->addWidget(m_drawButton, 0);
    subLayout->addWidget(m_detectButton, 0);
    subLayout->addWidget(m_clearButton, 0);
    subLayout->addWidget(m_scanButton, 0);

    // Connect the button's clicked signal to the DrawingCanvas slots
    connect(m_clearButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::clearPoints);
    connect(m_drawButton,  &QPushButton::clicked, m_canvas, &DrawingCanvas::paintLines);
    connect(m_scanButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::scanWindowsOnly);
    connect(m_detectButton, &QPushButton::clicked, m_canvas, &DrawingCanvas::segmentDetection);

    // Set the layout for the central widget
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setWindowTitle("Drawing Canvas");
    setFixedSize(450, 450);
}

MainWindow::~MainWindow() = default;
