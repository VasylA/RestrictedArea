#include "gview.h"

#include <QWheelEvent>
#include <QScrollBar>
#ifdef Q_OS_MAC
#include <QGLWidget>
#endif

const qreal GView::MAX_ZOOM_PER = 300.0;
const qreal GView::MIN_ZOOM_PER = 30.0;
const qreal GView::DEF_ZOOM_PER = 100.0;
/* Taking proportional steps */
const qreal GView::STEP_REMOTE = 10.0; /* Step for zoom if current_zoom < DEF_ZOOM_PER */
const qreal GView::STEP_APPROX         /* Step for zoom if current_zoom > DEF_ZOOM_PER */
    = STEP_REMOTE * (MAX_ZOOM_PER - DEF_ZOOM_PER) / (DEF_ZOOM_PER - MIN_ZOOM_PER);

GView::GView(QWidget *parent, bool useOGLbyOS) :
    QGraphicsView(parent),
    _isZoomEnabled(false),
    _zoomPer(DEF_ZOOM_PER),
    _cachedZoomScale(-1.0)
{
    init(useOGLbyOS);
}

GView::GView(QGraphicsScene *scene, QWidget *parent, bool useOGLbyOS) :
    QGraphicsView( scene, parent),
    _isZoomEnabled(false),
    _zoomPer(DEF_ZOOM_PER)
{
    init(useOGLbyOS);
}

void GView::fitInWindow()
{
    QRectF itemsRect = scene()->itemsBoundingRect();
    QRectF viewRect = rect();

    qreal hRatio = (viewRect.width() - verticalScrollBar()->width() - 5) / itemsRect.width();
    qreal vRatio = (viewRect.height() - horizontalScrollBar()->height() - 5) / itemsRect.height();

    centerOn(itemsRect.center());
    setZoomAbsolute(hRatio < vRatio ? hRatio : vRatio, false);
}

void GView::resetZoom()
{
    centerOn(scene()->itemsBoundingRect().center());
    setZoomPercentage(DEF_ZOOM_PER);
}

void GView::setZoomPercentage(qreal per, bool centerOnCursor)
{
    setZoomAbsolute(qMax(minZoomPer(), qMin(per, maxZoomPer())) / 100.0, centerOnCursor);
}

void GView::init(bool useOGLbyOS)
{
    QFont font = QFont ("Tahoma", -1);
    font.setPixelSize(11);
#if defined(Q_OS_MAC)
    if(useOGLbyOS)
        setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
#else
    Q_UNUSED(useOGLbyOS);
#endif
}

void GView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    emit sizeChanged(geometry());
}

bool GView::viewportEvent(QEvent *event)
{
    if ( event->type() == QEvent::Paint )
    {
        QRect viewport_rect(0, 0, this->viewport()->width(), this->viewport()->height());
        QRectF ViewAREA = this->mapToScene(viewport_rect).boundingRect();
        emit viewportChange(ViewAREA);
    }
    return QGraphicsView::viewportEvent(event);
}

void GView::wheelEvent(QWheelEvent *event)
{
    if (isInteractive() && event->orientation() == Qt::Vertical
        && event->modifiers() == Qt::ControlModifier)
    {
        bool isIncrease = event->delta() > 0;
        qreal step = relativeStep(isIncrease);

        setZoomPercentage(isIncrease ? _zoomPer + step : _zoomPer - step, true);
    }
    else
        QGraphicsView::wheelEvent(event);
}

void GView::setZoomAbsolute(qreal scale, bool centerOnCursor)
{
    if (!isZoomEnabled() || qFuzzyCompare(_cachedZoomScale, scale))
        return;

    QMatrix matrix;
    matrix.scale(scale, scale);
    setTransformationAnchor(centerOnCursor ? AnchorUnderMouse : AnchorViewCenter);
    setMatrix(matrix);

    _zoomPer = scale * 100.0;
    _cachedZoomScale = scale;

    emit zoomChanged(_zoomPer);
}

qreal GView::relativeStep(bool isIncrease)
{
    if (qFuzzyCompare(_zoomPer, DEF_ZOOM_PER))
        return isIncrease ? STEP_APPROX : STEP_REMOTE;

    qreal step = _zoomPer > DEF_ZOOM_PER ? STEP_APPROX : STEP_REMOTE;
    qreal nextZoomPer = isIncrease ? _zoomPer + step : _zoomPer - step;

    if ((nextZoomPer > DEF_ZOOM_PER) != (_zoomPer > DEF_ZOOM_PER)
        && !qFuzzyCompare(nextZoomPer, DEF_ZOOM_PER))
    {
        return isIncrease ? DEF_ZOOM_PER - _zoomPer : _zoomPer - DEF_ZOOM_PER;
    }

    return step;
}
