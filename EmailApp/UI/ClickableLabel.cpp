#include "ClickableLabel.h"


ClickableLabel::ClickableLabel( const QString& text, QWidget * parent ) :
    QLabel(parent)

  {
      this->setText(text);
  }


  void ClickableLabel::mousePressEvent ( QMouseEvent * event )

  {
      emit clicked();
  }
