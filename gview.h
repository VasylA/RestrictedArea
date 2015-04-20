#ifndef GVIEW_H
#define GVIEW_H
#include <QGraphicsView>

class GView : public QGraphicsView
{
    Q_OBJECT

    static const qreal MAX_ZOOM_PER;
    static const qreal MIN_ZOOM_PER;
    static const qreal DEF_ZOOM_PER;

    static const qreal STEP_REMOTE;
    static const qreal STEP_APPROX;

public:
    GView(QWidget *parent = 0, bool useOGLbyOS = true);
    GView(QGraphicsScene *scene, QWidget *parent = 0, bool useOGLbyOS = true);

    void setZoomEnabled(bool isEnabled) { _isZoomEnabled = isEnabled; }

    qreal maxZoomPer() const { return MAX_ZOOM_PER; }
    qreal minZoomPer() const { return MIN_ZOOM_PER; }
    qreal currentZoomPer() const { return _zoomPer; }
    bool isZoomEnabled() const { return _isZoomEnabled; }

public slots:
    void fitInWindow();
    void resetZoom();
    void setZoomPercentage(qreal per, bool centerOnCursor = false);

protected:
    void init(bool useOGLbyOS);

    void resizeEvent  (QResizeEvent *event);
    bool viewportEvent(QEvent *event);
    void wheelEvent(QWheelEvent* event);

    void setZoomAbsolute(qreal scale, bool centerOnCursor);
    qreal relativeStep(bool isIncrease);

signals:
    void sizeChanged(QRect newSize);
    void viewportChange(QRectF ViewAREA);

    void zoomChanged(qreal per);

private:
    bool _isZoomEnabled;
    qreal _zoomPer;
    qreal _cachedZoomScale;
};

#endif // GVIEW_H
