#ifndef GBUTTON
#define GBUTTON

#include "ekgsignal.h"
class GButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(GColor bcolor READ getColor WRITE setColor)
public:
    enum GColor
    {
        Blue = 1,
        Green,
        Red,
        Orenge,
        Disable
    };

    enum Status
    {
        normal = 1,
        enter,
        push
    };
    // class constructor
    GButton(QWidget * parent = 0);
    GButton(const QString & text, QWidget * parent = 0);
    GButton(const GColor,const QString & text, QWidget * parent = 0);
     //class destructor
    ~GButton();

     GButton::GColor getColor();
     void setColor(GColor color);
public slots:
     void changeAlpha();

signals:
     void click();

protected:
     void paintEvent(QPaintEvent * pe);

     void enterEvent(QEvent * e);
     void leaveEvent(QEvent * e);

     void mousePressEvent(QMouseEvent * e);
     void mouseReleaseEvent(QMouseEvent * e);

private:
     void initialize(const GColor = Blue);// create and set the basic setting that simillar on all constructor
     void fade(QImage &,QImage &,QPainter &);
     void setStatus(Status);
     GButton::Status getStatus() const;

     //! Variable
     qreal currentAlpha;
     QTimer *timer;
     QString bText; // Button text
     QImage bImage[3]; // Button image
     GColor bcolor; // Button color
     Status bStatus; // Button status
     QImage pImage , nImage; // Define Previous image and next image for fading effect
};

#endif
