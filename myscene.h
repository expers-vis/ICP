#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = nullptr);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:

};

#endif // MYSCENE_H
