/*
 * Projekt ICP
 * Autori: Adam Ševčík - xsevci64, Martin Hiner - xhiner00
*/

#include "myscene.h"
#include "mainwindow.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

MyScene::MyScene(QObject *parent) :
    QGraphicsScene(parent)
{

}


void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    for (auto* item : items(event->scenePos())) {
        if (auto line = dynamic_cast<QGraphicsLineItem*>(item); line){
            auto pos = event->screenPos();
            qDebug() << pos;
            if(pos == QPointF(100,255)){
            line->setPen(QPen({Qt::red},3));
            qDebug() << event->scenePos();
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}
