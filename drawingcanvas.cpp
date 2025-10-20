#include "drawingcanvas.h"
#include <QDebug>

void printPattern(const QVector<uint8_t>& data, int N) {
    QString patternString = "\n";
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            patternString += QString::number(data[r * N + c]) + " ";
        }
        patternString += "\n";
    }
    qDebug().noquote() << patternString;
}


DrawingCanvas::DrawingCanvas(QWidget *parent) {
    // Set a minimum size for the canvas
    setMinimumSize(this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
    // Set a solid background color
    setStyleSheet("background-color: white; border: 1px solid gray;");

    QVector<uint8_t> verticalSolid1 = {1, 1, 0, 1, 1, 0, 1, 1, 0};
    QVector<uint8_t> verticalSolid2 = {0, 1, 1, 0, 1, 1, 0, 1, 1};
    QVector<uint8_t> verticalSolid3 = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    QVector<uint8_t> horizontalSolid1 = {1, 1, 1, 1, 1, 1, 0, 0, 0};
    QVector<uint8_t> horizontalSolid2 = {0, 0, 0, 1, 1, 1, 1, 1, 1};
    QVector<uint8_t> horizontalSolid3 = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    QVector<uint8_t> diagonal_main = {1, 0, 0, 0, 1, 0, 0, 0, 1};
    QVector<uint8_t> diagonal_anti = {0, 0, 1, 0, 1, 0, 1, 0, 0};


    m_idealPatterns.push_back(verticalSolid1);
    m_idealPatterns.push_back(verticalSolid2);
    m_idealPatterns.push_back(horizontalSolid1);
    m_idealPatterns.push_back(horizontalSolid2);
    m_idealPatterns.push_back(verticalSolid3);
    m_idealPatterns.push_back(diagonal_main);
    m_idealPatterns.push_back(diagonal_anti);
}


void DrawingCanvas::clearPoints() {
    m_points.clear();
    detectedRegions.clear();
    scannedWindows.clear();
    update();
}

void DrawingCanvas::paintLines() {
    isPaintLinesClicked = true;
    update();
}

void DrawingCanvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set up the pen and brush for drawing the points
    QPen pen(Qt::blue, 5);
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::blue));

    // Draw a small circle at each stored point
    for (const QPoint &point : std::as_const(m_points)) {
        painter.drawEllipse(point, 3, 3);
    }

    if (isPaintLinesClicked) {
        pen.setColor(Qt::red);
        pen.setWidth(4);  // 4-pixel wide line
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        for (int i = 0; i < m_points.size() - 1; i += 2) {
            // cout << m_points[i].x() << endl;
            painter.drawLine(m_points[i], m_points[i + 1]);
        }
        isPaintLinesClicked = false;

        //return painter pen to blue
        pen.setColor(Qt::blue);
        painter.setPen(pen);
    }



    painter.setPen(QPen(QColor(128, 0, 128), 1));
    painter.setBrush(QColor(128, 0, 128, 100));
    for (const QRect &r : detectedRegions) {
        painter.drawRect(r);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    // Add the mouse click position to our vector of points
    m_points.append(event->pos());
    // Trigger a repaint
    update();
}


QVector<uint8_t> DrawingCanvas::samplePatch(const QImage &img, int x, int y, int N, int threshold) {
    QVector<uint8_t> out;
    out.reserve(N * N);

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int px = x + c;
            int py = y + r;
            if (px < 0 || py < 0 || px >= img.width() || py >= img.height()) {
                out.push_back(0);
            } else {
                QColor col = QColor::fromRgba(img.pixel(px, py));
                int gray = qGray(col.rgb());

                out.push_back(gray < threshold ? 1 : 0);
            }
        }
    }
    return out;
}

QVector<DrawingCanvas::WindowPatch> DrawingCanvas::scanAllWindows(const QImage &img, int N) {
    QVector<WindowPatch> result;

    for (int y = 0; y <= img.height() - N; ++y) {
        for (int x = 0; x <= img.width() - N; ++x) {
            auto data = samplePatch(img, x, y, N);

            bool nonEmpty = std::any_of(data.begin(), data.end(), [](uint8_t v) { return v != 0; });
            if (nonEmpty) {
                WindowPatch p;
                p.topLeft = QPoint(x, y);
                p.size = N;
                p.data = std::move(data);
                result.push_back(p);
            }
        }
    }
    return result;
}


void DrawingCanvas::scanWindowsOnly() {
    QPixmap pixmap = this->grab();
    QImage image = pixmap.toImage();
    const int N = WINDOW_SIZE;

    qDebug() << "Scanning all" << N << "x" << N << "windows...";
    qDebug() << "image width:" << image.width();
    qDebug() << "image height:" << image.height();

    scannedWindows = scanAllWindows(image, N);
    qDebug() << "Total non-empty windows found:" << scannedWindows.size();

    qDebug() << "Dumping first 100 non-empty windows for analysis:";
    for (int i = 0; i < qMin(scannedWindows.size(), 100); ++i) {
        qDebug() << "Window" << i << "at" << scannedWindows[i].topLeft;
        printPattern(scannedWindows[i].data, N);
    }
}


void DrawingCanvas::segmentDetection() {
    if (scannedWindows.isEmpty()) {
        qDebug() << "No scanned data yet. Please click 'Scan Window' first!";
        return;
    }

    detectedRegions.clear();
    int N = WINDOW_SIZE;
    int matches = 0;

    for (const auto& window : scannedWindows) {
        bool isMatch = false;

        for (const auto& idealPattern : m_idealPatterns) {
            if (window.data == idealPattern) {
                isMatch = true;
                break;
            }
        }

        if (isMatch) {

            detectedRegions.push_back(QRect(window.topLeft, QSize(N, N)));
            matches++;
        }
    }

    qDebug() << "Detect Segment: found and showing" << matches << "matching regions.";
    update();
}
