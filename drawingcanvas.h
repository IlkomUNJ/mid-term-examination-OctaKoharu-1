#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QPainter>
#include <QMouseEvent>
#include <iostream>
#include <iomanip>
#include <QPixmap>
#include <QColor>
#include <algorithm>
#include "CustomMatrix.h"

using namespace std;

class DrawingCanvas : public QWidget
{
    Q_OBJECT
private:
    const int WINDOW_WIDTH = 600;
    const int WINDOW_HEIGHT = 400;
    const int WINDOW_SIZE = 3;
public:
    explicit DrawingCanvas(QWidget *parent = nullptr);

    // Slot to clear all points from the canvas
    void clearPoints();
    void paintLines();
    void segmentDetection();
    void scanWindowsOnly();

protected:
    // Overridden method to handle painting on the widget
    void paintEvent(QPaintEvent *event) override;

    // Overridden method to handle mouse clicks
    void mousePressEvent(QMouseEvent *event) override;

private:
    // A vector to store all the points drawn by the user
    QVector<QPoint> m_points;
    bool isPaintLinesClicked = false;

    QVector<QVector<uint8_t>> m_idealPatterns;

    struct WindowPatch {
        QPoint topLeft;
        int size;
        QVector<uint8_t> data;
    };

    QVector<QRect> detectedRegions;
    QVector<WindowPatch> scannedWindows;
    QVector<uint8_t> samplePatch(const QImage &img, int x, int y, int N, int threshold = 127);
    QVector<WindowPatch> scanAllWindows(const QImage &img, int N);
};

#endif // DRAWINGCANVAS_H
